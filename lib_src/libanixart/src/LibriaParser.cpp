#include <anixart/LibriaParser.hpp>
#include <netsess/JsonTools.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <string>

#include "util/BaseUrls.h"

namespace anixart::parsers {
    using network::json::ParseJson;

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
        if (!boost::regex_search(url, id_ep_match, ID_EP_RE)) return {};

        std::string release_id = id_ep_match[1];
        std::string episode_number = id_ep_match[2];

        std::string release_url;
        release_url.reserve(de_anixart::urls::RELEASE_API_URL_V1.length() + release_id.length());
        release_url += de_anixart::urls::RELEASE_API_URL_V1;
        release_url += release_id;

        network::JsonObject response = network::parse_json(_session.get_request(release_url));

        network::JsonArray& episodes = ParseJson::strong_get<network::JsonArray&>(response, "episodes");

        for (auto& episode_value : episodes) {
            auto& episode = episode_value.as_object();
            int ordinal = ParseJson::strong_get<int>(episode, "ordinal");
            if (std::to_string(ordinal) == episode_number) {
                std::unordered_map<std::string, std::string> out;

                if (ParseJson::exists(episode, "hls_1080")) 
                    out["1080"] = ParseJson::strong_get<std::string>(episode, "hls_1080");
                if (ParseJson::exists(episode, "hls_720")) 
                    out["720"] = ParseJson::strong_get<std::string>(episode, "hls_720");
                if (ParseJson::exists(episode, "hls_480")) 
                    out["480"] = ParseJson::strong_get<std::string>(episode, "hls_480");

                return out;
            }
        }

        return {};
    }
};