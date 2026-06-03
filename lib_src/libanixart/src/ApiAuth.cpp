#include <anixart/ApiAuth.hpp>
#include <anixart/ApiErrors.hpp>
#include <anixart/CachingJson.hpp>
#include <netsess/JsonTools.hpp>

namespace anixart {
#ifdef LIBANIXART_AUTH_PRESENTED
    ApiAuth::ApiAuth(const ApiSession& session) : _session(session) {}

    ApiAuthPending::UPtr ApiAuth::sign_up(const std::string& login, const std::string& email, const std::string& password) const {
        json::CachingJsonObject response = _session.api_request(requests::auth::sign_up(login, email, password));
        assert_status_code<SignUpError>(response);
        auto hash = response.get<std::string>("hash");
        auto timestamp = response.get<TimestampPoint>("codeTimestampExpires");
        return std::make_unique<ApiAuthPending>(_session, login, email, requests::auth::AuthKeys::password, password, hash, timestamp);
    }

    ApiAuthPending::UPtr ApiAuth::sign_up_google(const std::string& google_id_token) const {
        /* Anixart google sign up not implemented */
        throw ApiError();
    }

    ApiAuthPending::UPtr ApiAuth::sign_up_vk(const std::string& vk_access_token) const {
        /* Anixart vk sign up not implemented */
        throw ApiError();
    }

    ApiRestorePending::UPtr ApiAuth::restore(const std::string& email_or_username, const std::string& new_password) const {
        json::CachingJsonObject response = _session.api_request(requests::auth::restore(email_or_username));
        assert_status_code<RestoreError>(response);
        auto hash = response.get<std::string>("hash");
        auto timestamp = response.get<TimestampPoint>("codeTimestampExpires");
        return std::make_unique<ApiRestorePending>(_session, email_or_username, new_password, hash, timestamp);
    }

    std::pair<Profile::Ptr, ProfileToken> ApiAuth::sign_in(const std::string& username, const std::string& password) const {
        json::CachingJsonObject response = _session.api_request(requests::auth::sign_in(username, password));
        assert_status_code<SignInError>(response);
        Profile::Ptr profile = response.get<Profile::Ptr>("profile");
        ProfileToken token = response.get<ProfileToken>("profileToken");
        return std::make_pair(profile, token);
    }

    std::pair<Profile::Ptr, ProfileToken> ApiAuth::sign_in_google(const std::string& google_id_token) const {
        /* Anixart google sign in not implemented */
        throw ApiError();
    }

    std::pair<Profile::Ptr, ProfileToken> ApiAuth::sign_in_vk(const std::string& vk_access_token) const {
        /* Anixart vk sign in not implemented */
        throw ApiError();
    }

    ApiAuthPending::ApiAuthPending(
        const ApiSession& session,
        const std::string& login,
        const std::string& email,
        std::string_view auth_key,
        const std::string& auth_value,
        const std::string& hash,
        const TimestampPoint timestamp
    ) :
        _session(session),
        _login(login),
        _email(email),
        _auth_key(auth_key),
        _auth_value(auth_value),
        _hash(hash),
        _timestamp(timestamp)
    {
    }

    void ApiAuthPending::resend() {
        json::CachingJsonObject response = _session.api_request(requests::auth::resend(_login, _email, _hash, _auth_key, _auth_value));
        assert_status_code<ResendError>(response);
        _timestamp = response.get<TimestampPoint>("timestampExpires");
    }

    std::pair<Profile::Ptr, ProfileToken> ApiAuthPending::verify(const std::string& email_code) const {
        json::CachingJsonObject response = _session.api_request(requests::auth::verify(_login, _email, _hash, email_code, _auth_key, _auth_value));
        assert_status_code<VerifyError>(response);
        Profile::Ptr profile = response.get<Profile::Ptr>("profile");
        ProfileToken token = response.get<ProfileToken>("profileToken");
        return std::make_pair(profile, token);
    }

    bool ApiAuthPending::is_expired() const {
        return _timestamp < std::chrono::system_clock::now();
    }

    ApiRestorePending::ApiRestorePending(
        const ApiSession& session,
        const std::string email_or_login,
        const std::string& password,
        const std::string& hash,
        const TimestampPoint timestamp
    ) :
        _session(session),
        _email_or_login(email_or_login),
        _password(password),
        _hash(hash),
        _timestamp(timestamp)
    {
    }

    void ApiRestorePending::resend() {
        json::CachingJsonObject resp = _session.api_request(requests::auth::restore_resend(_email_or_login, _password, _hash));
        assert_status_code<RestoreResendError>(resp);
        _timestamp = resp.get<TimestampPoint>("timestampExpires");
    }

    std::pair<Profile::Ptr, ProfileToken> ApiRestorePending::verify(const std::string& email_code) const {
        json::CachingJsonObject response = _session.api_request(requests::auth::restore_verify(_email_or_login, _password, _hash, email_code));
        assert_status_code<RestoreVerifyError>(response);
        Profile::Ptr profile = response.get<Profile::Ptr>("profile");
        ProfileToken token = response.get<ProfileToken>("profileToken");
        return std::make_pair(profile, token);
    }

    bool ApiRestorePending::is_expired() const {
        return _timestamp < std::chrono::system_clock::now();

    }
#endif
}