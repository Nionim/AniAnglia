#include <anixart/ApiSession.hpp>
#include <anixart/ApiErrors.hpp>
#include <anixart/Platform.hpp>
#include <anixart/Version.hpp>
#include <anixart/Random.hpp>
#include <netsess/StringTools.hpp>

namespace anixart {
    using namespace network;
    using namespace random;

    ApiSession::ApiSession(std::string_view lang, std::string_view application, std::string_view application_version) : _is_verbose(false), _base_url(requests::base_url) {
        std::string user_agent_header = StringTools::sformat(
            "User-Agent: %s/%s (Android " PLATFORM_NAME " %s; SDK libanixart " LIBANIXART_API_VERSION "; " ARCH_NAME "; %s %s; %s)",
            application,
            application_version,
            get_platform_version(),
            get_product_name(),
            get_product_model(),
            lang
        );
        std::string sign_header = StringTools::sformat("Sign: %s", gen_random_string(192ULL, ascii));

        set_default_headers({
            user_agent_header,
            sign_header
        });
    }

    void ApiSession::set_verbose(const bool api_verbose, const bool sess_verbose) {
        _is_verbose = api_verbose;
        UrlSession::set_verbose(sess_verbose);
    }

    void ApiSession::set_base_url(const std::string& base_url) {
        _base_url = base_url;
    }

    JsonObject ApiSession::api_request(const requests::ApiPostRequest& request) const {
        try {
            std::string response = post_request(request.base_url.value_or(_base_url) + request.sub_url, request.data, request.type, request.headers, request.params);
            if (_is_verbose) {
                std::cout << "POST Request: " << request.sub_url << ".Params: " << request.params.get() << ". Data: " << request.data << std::endl;
                std::cout << response << std::endl;
            }
            return parse_json(response);
        }
        catch (const JsonError& e) {
            (e);
            throw ApiError();
        }
        catch (const UrlSessionError& e) {
            (e);
            throw ApiError();
        }
    }

    JsonObject ApiSession::api_request(const requests::ApiGetRequest& request) const {
        try {
            std::string response = get_request(request.base_url.value_or(_base_url) + request.sub_url, request.headers, request.params);
            if (_is_verbose) {
                std::cout << "GET Request: " << request.sub_url << ". Params: " << request.params.get() << std::endl;
                std::cout << response << std::endl;
            }
            return parse_json(response);
        }
        catch (const JsonError& e) {
            (e);
            throw ApiError();
        }
        catch (const UrlSessionError& e) {
            (e);
            throw ApiError();
        }
    }

    JsonObject ApiSession::api_request(const requests::ApiPostMultipartRequest& request) const {
        try {
            std::string response = post_multipart_request(request.base_url.value_or(_base_url) + request.sub_url, request.forms, request.headers, request.params);
            if (_is_verbose) {
                std::cout << "POST (multipart) Request: " << request.sub_url << ". Params: " << request.params.get() << std::endl;
                std::cout << response << std::endl;
            }
            return parse_json(response);
        }
        catch (const JsonError& e) {
            (e);
            throw ApiError();
        }
        catch (const UrlSessionError& e) {
            (e);
            throw ApiError();
        }
    }
}