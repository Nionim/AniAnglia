#include <anixart/LibriaParser.hpp>
#include <netsess/JsonTools.hpp>
#include <boost/regex.hpp>
#include <algorithm>

#include "util/BaseUrls.h"

static constexpr size_t cstrlen(const char* const str) {
	return std::char_traits<char>::length(str);
}

namespace anixart::parsers {
	using network::json::ParseJson;

	const std::string_view EP_INFO_URL_S =  de_anixart::urls::EP_INFO_URL_S;

	static std::string create_ep_url(const std::string& host, const std::string& url) {
		std::string out_url;
		out_url.reserve(cstrlen("https://") + host.length() + url.length() + 1);
		out_url += "https://";
		out_url += host;
		out_url += url;
		return out_url;
	}

	LibriaParser::LibriaParser() {}

	bool LibriaParser::valid_host(const std::string& host) const {
    	using namespace de_anixart::de_libria::domains;
		
		if (std::find(libria_exact.begin(), libria_exact.end(), host) != libria_exact.end())
        	return true;

		return std::any_of(libria_partial.begin(), libria_partial.end(),
        	[&](const std::string& p) { return host.find(p) != std::string::npos; });
	}

	std::string_view LibriaParser::get_name() const {
		return "LibriaParser";
	}
	std::unordered_map<std::string, std::string> LibriaParser::extract_info(const std::string& url, const std::vector<ParserParameter>& params) {
		static const boost::regex ID_EP_RE(R"(id=(\d+)&ep=(\d+))");

		boost::smatch id_ep_match;
		if (!boost::regex_search(url, id_ep_match, ID_EP_RE)) {
			return {};
		}
		std::string ep_info_url;
		ep_info_url.reserve(cstrlen("&id=") + EP_INFO_URL_S.length() + id_ep_match[1].length() + id_ep_match[2].length() + 1);
		ep_info_url += EP_INFO_URL_S;
		ep_info_url += id_ep_match[2];
		ep_info_url += "&id=";
		ep_info_url += id_ep_match[1];
		network::JsonObject resp = network::parse_json(_session.get_request(ep_info_url));
		network::JsonObject& player = ParseJson::strong_get<network::JsonObject&>(resp, "player");
		network::JsonObject& playlist = ParseJson::strong_get<network::JsonArray&>(player, "playlist")[std::stoll(id_ep_match[2])].as_object();
		network::JsonObject& playlist_hls = ParseJson::strong_get<network::JsonObject&>(playlist, "hls");

		std::unordered_map<std::string, std::string> out;
		std::string host = ParseJson::strong_get<std::string>(player, "host");
		if (ParseJson::exists(playlist_hls, "fhd")) {
			out["1080"] = create_ep_url(host, ParseJson::strong_get<std::string>(playlist_hls, "fhd"));
		}
		if (ParseJson::exists(playlist_hls, "hd")) {
			out["720"] = create_ep_url(host, ParseJson::strong_get<std::string>(playlist_hls, "hd"));
		}
		if (ParseJson::exists(playlist_hls, "sd")) {
			out["480"] = create_ep_url(host, ParseJson::strong_get<std::string>(playlist_hls, "sd"));
		}

		return out;
	}
};