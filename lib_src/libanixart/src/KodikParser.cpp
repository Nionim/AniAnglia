#include <anixart/KodikParser.hpp>
#include <ext/base64.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <cctype> 

#include "BaseUrls.h"

// if set this to positive value, then KodikParser doesn't try to get it
constexpr int32_t UNIVERSAL_CAESAR_OFFSET = -1;
static_assert(UNIVERSAL_CAESAR_OFFSET < 0 || (0 <= UNIVERSAL_CAESAR_OFFSET && UNIVERSAL_CAESAR_OFFSET <= 26));

static constexpr size_t cstrlen(const char* const str) {
	return std::char_traits<char>::length(str);
}

static void extend_url_protocol(std::string& url) {
	if (!url.starts_with("http")) {
		url.insert(0, "https:");
	}
}

namespace anixart::parsers {
	constexpr std::string_view LINKS_URL_S = anixart::urls::LINKS_URL_S;
	constexpr std::string_view KODIK_HOST = anixart::urls::KODIK_HOST;
	constexpr std::string_view KODIK_FTOR_URL = anixart::urls::KODIK_FTOR_URL;
	constexpr std::string_view KODIK_INFO_URL = anixart::urls::KODIK_INFO_URL;

	KodikParser::KodikParser() : _caesar_offset(UNIVERSAL_CAESAR_OFFSET) {
		_session.set_default_headers(get_default_headers());
	}

	bool KodikParser::valid_host(const std::string& host) const {
    	using namespace de_anixart::de_kodik::domains;
		
		if (std::find(kodik_exact.begin(), kodik_exact.end(), host) != kodik_exact.end())
        	return true;

		return std::any_of(kodik_partial.begin(), kodik_partial.end(),
        	[&](const std::string& p) { return host.find(p) != std::string::npos; });
	}
	
	std::string_view KodikParser::get_name() const {
		return "KodikParser";
	}

	std::unordered_map<std::string, std::string> KodikParser::extract_info_fallback(const std::string& url, const std::vector<ParserParameter>& params) {
		static const boost::regex ID_HASH_RE(R"(vInfo.hash ?= ?(?:"|')(\w+)(?:"|'))");
		static const boost::regex URL_TYPE_ID_RE(R"(/(seria|video)/(\d+))");
		boost::smatch hash_match, type_id_match;
		if (!boost::regex_search(url, type_id_match, URL_TYPE_ID_RE)) {
			// cannot find serial id in url
			return {};
		}
		std::string serial_url = url.substr(0, url.find('?')); // no url parameters
		std::string serial_resp = _session.get_request(serial_url);
		if (!boost::regex_search(serial_resp, hash_match, ID_HASH_RE)) {
			// cannot find hash. error
			return {};
		}
		std::string serial_pdata;
		serial_pdata.reserve(cstrlen("type=&hash=&id=") + type_id_match[1].length() + hash_match[1].length() + type_id_match[2].length() + 1);
		serial_pdata.append("type=");
		serial_pdata.append(type_id_match[1]);
		serial_pdata.append("&hash=");
		serial_pdata.append(hash_match[1]);
		serial_pdata.append("&id=");
		serial_pdata.append(type_id_match[2]);

		network::JsonObject links_json = network::parse_json(_session.post_request(std::string(KODIK_FTOR_URL), serial_pdata, "application/x-www-form-urlencoded"));
		if (!links_json.contains("links")) {
			// no links. :C
			return {};
		}
		if (_caesar_offset < 0) {
			// offset not initialized. Initialize it
			// TODO: maybe invalidate it after some time
			_caesar_offset = get_caesar_offset(serial_resp);
			if (_caesar_offset < 0) {
				// failed to initialize caesar offset. error
				return {};
			}
		}

		std::unordered_map<std::string, std::string> out;
		network::JsonObject& links = links_json["links"].as_object();
		for (auto& [res, enc_url_arr] : links) {
			network::JsonObject& enc_url_obj = enc_url_arr.as_array()[0].as_object();
			std::string url = boost::json::value_to<std::string>(enc_url_obj["src"]);
			if (!url.ends_with("manifest.m3u8")) {
				// base64 encoded url
				url = decode_url(url);
			}
			out[res] = url;
			extend_url_protocol(out[res]);
		}
		return out;
	}

	std::unordered_map<std::string, std::string> KodikParser::extract_info(const std::string& url, const std::vector<ParserParameter>& params) {
		process_params(params);
		return extract_info_fallback(url, params);
	}
	int32_t KodikParser::get_caesar_offset(const std::string& serial_response) const {
		static const boost::regex SEASON_SCRIPT_RE(R"(/assets/js/app\.(?:season|player_single)\.[a-zA-Z0-9]+\.js)");
		static const boost::regex CAESAR_OFFSET_RE(R"(String\.fromCharCode\(\(e<="Z"\?90:122\)>=\(e=e\.charCodeAt\(0\)\+(\d+)\)\?e:e-\d+\))");
		boost::smatch season_script_match, caesar_offset_match;
		if (!boost::regex_search(serial_response, season_script_match, SEASON_SCRIPT_RE)) {
			return -1;
		}
		std::string app_season_resp = _session.get_request(std::string(KODIK_INFO_URL) + season_script_match[0]);
		if (!boost::regex_search(app_season_resp, caesar_offset_match, CAESAR_OFFSET_RE)) {
			return -1;
		}
		return std::stol(caesar_offset_match[1]);
	}
	std::string KodikParser::decode_url(std::string_view encoded_url) const {
		std::string url_b64;
		const int src_endc = encoded_url.length() % 4;
		const size_t pad = src_endc == 0 ? 0ULL : (4ULL - src_endc);
		url_b64.reserve(encoded_url.length() + pad);
		for (const char& c : encoded_url) {
			if (std::isdigit(c)) {
				url_b64.push_back(c);
				continue;
			}
			url_b64.push_back(std::tolower(c) <= ('z' - _caesar_offset) ? c + _caesar_offset : c - (26 - _caesar_offset));
		}
		// add padding
		for (int i = 0; i < pad; ++i) {
			url_b64.push_back('=');
		}
		return base64::from_base64(url_b64);
	}
};
