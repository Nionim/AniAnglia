#include <anixart/ApiRequests.hpp>
#include <netsess/StringTools.hpp>

namespace anixart::requests {
    using network::StringTools;

    namespace auth {
        ApiPostRequest firebase(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "auth/firebase",
                .data = StringTools::sformat("token=%s", token),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest restore(const std::string& email_or_login) {
            return ApiPostRequest{
                .sub_url = "auth/restore",
                .data = StringTools::sformat("data=%s", email_or_login),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest restore_resend(const std::string& email_or_login, const std::string& password, const std::string& hash) {
            return ApiPostRequest{
                .sub_url = "auth/restore/resend",
                .data = StringTools::sformat("data=%s&password=%s&hash=%s", email_or_login, password, hash),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest restore_verify(const std::string& email_or_login, const std::string& password, const std::string& hash, const std::string& code) {
            return ApiPostRequest{
                .sub_url = "auth/restore/verify",
                .data = StringTools::sformat("data=%s&password=%s&hash=%s&code=%s", email_or_login, password, hash, code),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_in(const std::string& login, const std::string& password) {
            return ApiPostRequest{
                .sub_url = "auth/signIn",
                .data = StringTools::sformat("login=%s&password=%s", login, password),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_in_google(const std::string& google_id_token) {
            return ApiPostRequest{
                .sub_url = "auth/google",
                .data = StringTools::sformat("googleIdToken=%s", google_id_token),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_in_vk(const std::string& vk_access_token) {
            return ApiPostRequest{
                .sub_url = "auth/vk",
                .data = StringTools::sformat("vkAccessToken=%s", vk_access_token),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_up(const std::string& login, const std::string& email, const std::string& password) {
            return ApiPostRequest{
                .sub_url = "auth/signUp",
                .data = StringTools::sformat("login=%s&email=%s&password=%s", login, email, password),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_up_google(const std::string& login, const std::string& email, const std::string& google_id_token) {
            return ApiPostRequest{
                .sub_url = "auth/google",
                .data = StringTools::sformat("login=%s&email=%s&googleIdToken=%s", login, email, google_id_token),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest sign_up_vk(const std::string& login, const std::string& email, const std::string& vk_access_token) {
            return ApiPostRequest{
                .sub_url = "auth/vk",
                .data = StringTools::sformat("login=%s&email=%s&vkAccessToken=%s", login, email, vk_access_token),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest resend(const std::string& login, const std::string& email, const std::string& hash, std::string_view auth_key, const std::string& auth_value) {
            return ApiPostRequest{
                .sub_url = "auth/resend",
                .data = StringTools::sformat(
                    "login=%s&email=%s&%s=%s&hash=%s",
                    login,
                    email,
                    auth_key,
                    auth_value,
                    hash
                ),
                .type = "application/x-www-form-urlencoded"
            };
        }

        ApiPostRequest verify(const std::string& login, const std::string& email, const std::string& hash, const std::string& code, std::string_view auth_key, const std::string& auth_value) {
            return ApiPostRequest{
                .sub_url = "auth/verify",
                .data = StringTools::sformat(
                    "login=%s&email=%s&%s=%s&hash=%s&code=%s",
                    login,
                    email,
                    auth_key,
                    auth_value,
                    hash,
                    code
                ),
                .type = "application/x-www-form-urlencoded"
            };
        }
    };
};