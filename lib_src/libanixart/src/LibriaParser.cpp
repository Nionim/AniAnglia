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

	// https://anilibria.top/api/v1/anime/releases/10243

	// https://anilibria.top/api/v1/anime/releases/ID

	//	{
	//		"episodes": {
	//			"0.. X": {
	//				hls_480: "",
	//				hls_720: "",
	//				hls_1080: "",
	//				...
	//			}
	//		}
	//	}

	std::unordered_map<std::string, std::string> LibriaParser::extract_info(const std::string& url, const std::vector<ParserParameter>& params) {
    	static const boost::regex ID_EP_RE(R"(id=(\d+)&ep=(\d+))");
    	boost::smatch id_ep_match;
    	if (!boost::regex_search(url, id_ep_match, ID_EP_RE)) 
	    	return {};
    
	    std::string release_url;
	    release_url.reserve(de_anixart::urls::RELEASE_API_URL_V1.length() + id_ep_match[1].length() + 1);
    	release_url += de_anixart::urls::RELEASE_API_URL_V1;
    	release_url += id_ep_match[1];

	    network::JsonObject resp = network::parse_json(_session.get_request(release_url));
    	network::JsonArray& episodes = ParseJson::strong_get<network::JsonArray&>(resp, "episodes");

	    int target_ordinal = std::stoi(id_ep_match[2]);

    	network::JsonObject* found_ep = nullptr;
    	for (auto& ep_val : episodes) {
        	auto& ep_obj = ep_val.as_object();
        	if (ParseJson::exists(ep_obj, "ordinal") &&
            	ParseJson::strong_get<int>(ep_obj, "ordinal") == target_ordinal) {
            	found_ep = &ep_obj;
            	break;
        	}
    	}

    	if (!found_ep) 
        	return {};

    	std::unordered_map<std::string, std::string> out;
    	if (ParseJson::exists(*found_ep, "hls_1080"))
        	out["1080"] = ParseJson::strong_get<std::string>(*found_ep, "hls_1080");
    	if (ParseJson::exists(*found_ep, "hls_720"))
        	out["720"] = ParseJson::strong_get<std::string>(*found_ep, "hls_720");
    	if (ParseJson::exists(*found_ep, "hls_480"))
        	out["480"] = ParseJson::strong_get<std::string>(*found_ep, "hls_480");

    	return out;
	}
}