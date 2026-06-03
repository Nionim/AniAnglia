#include <anixart/Api.hpp>
#include <anixart/Platform.hpp>

namespace anixart {
    Api::Api(std::string_view lang, std::string_view application, std::string_view application_version) :
        _token(),
        _session(lang, application, application_version),
#ifdef LIBANIXART_AUTH_PRESENTED
        _auth(_session),
#endif
        _search(_session, _token),
        _episodes(_session, _token),
        _profiles(_session, _token),
        _releases(_session, _token),
        _collections(_session, _token),
        _articles(_session, _token)
    {
    }

    const std::string& Api::get_token() const {
        return _token;
    }

    void Api::set_token(std::string_view token) {
        _token = token;
    }

    void Api::set_verbose(const bool api_verbose, const bool sess_verbose) {
        _session.set_verbose(api_verbose, sess_verbose);
    }

    ApiSession& Api::get_session() {
        return _session;
    }

    const ApiSession& Api::get_session() const {
        return _session;
    }
#ifdef LIBANIXART_AUTH_PRESENTED
    ApiAuth& Api::auth() {
        return _auth;
    }
#endif

    ApiSearch& Api::search() {
        return _search;
    }

    ApiEpisodes& Api::episodes() {
        return _episodes;
    }

    ApiProfiles& Api::profiles() {
        return _profiles;
    }
    ApiReleases& Api::releases() {
        return _releases;
    }
    ApiCollections& Api::collections() {
        return _collections;
    }
    ApiArticles& Api::articles() {
        return _articles;
    }
};