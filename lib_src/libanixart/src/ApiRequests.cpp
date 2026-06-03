#include <anixart/ApiRequests.hpp>
#include <netsess/StringTools.hpp>

namespace anixart::requests {
    constexpr std::string_view base_url = "https://api-s.anixsekai.com/";
    constexpr std::string_view base_url_alt = "https://api-alt.anixart.app/";
    // https://editor.anixsekai.com/?v=[10-10000]
    constexpr std::string_view editor_url = "https://editor.anixsekai.com/";

    using network::StringTools;
    using network::UrlParameters;

    namespace auth {
        // Definition in ApiRequetsAuth.cpp
    };
    namespace collection {
        ApiGetRequest collection(const int64_t collection_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/%lld", collection_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest collections(const int32_t page, const int32_t previous_page, const int32_t where, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/all/%ld", page),
                .params = {
                    { "previous_page", std::to_string(previous_page) },
                    { "where", std::to_string(where) },
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest profile_collections(const int64_t profile_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/all/profile/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest release_collections(const int64_t release_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/all/release/%lld/%ld", release_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest releases(const int64_t collection_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/%lld/releases/%ld", collection_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest report(const int64_t collection_id, const CollectionReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/report/%lld", collection_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
    };
    namespace collection::comment {
        ApiPostRequest add(const int64_t collection_id, const CommentAddRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/comment/add/%lld", collection_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest comment(const int64_t comment_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/comment/%lld", comment_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest comments(const int64_t collection_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/comment/all/%lld/%ld", collection_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int64_t comment_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/comment/delete/%lld", comment_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest edit(const int64_t comment_id, const CommentEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/comment/edit/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest process(const int64_t comment_id, const CommentProcessRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/comment/process/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest profile_comments(const int64_t profile_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/comment/all/profile/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest replies(const int64_t comment_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/comment/replies/%lld/%ld", comment_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiPostRequest report(const int64_t comment_id, const DeprecatedReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collection/comment/report/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest vote(const int64_t comment_id, const int32_t vote, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collection/comment/vote/%lld/%ld", comment_id, vote),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace collection::favorite {
        ApiGetRequest add(const int64_t collection_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionFavorite/add/%lld", collection_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int64_t collection_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionFavorite/delete/%lld", collection_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest favorites(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionFavorite/all/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace collection::my {
        ApiPostRequest create(const CreateEditCollectionRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "collectionMy/create",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest remove(const int64_t collection_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionMy/delete/%lld", collection_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest edit(const int64_t collection_id, const CreateEditCollectionRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("collectionMy/edit/%lld", collection_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostMultipartRequest edit_image(const int64_t collection_id, const network::MultipartPart& image, const std::string& token) {
            return ApiPostMultipartRequest{
                .sub_url = StringTools::snformat("collectionMy/editImage/%lld", collection_id),
                .params = {
                    { "token", token }
                },
                .forms = {
                    image,
                    new network::MultipartContentPart("name", "image", "text/plain")
                }
            };
        }
        ApiGetRequest release_add(const int64_t collection_id, const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionMy/release/add/%lld", collection_id),
                .params = {
                    { "release_id", std::to_string(release_id) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest releases(const int64_t collection_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("collectionMy/%lld/releases", collection_id),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace profile {
        ApiGetRequest change_login_history(const int64_t profile_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/login/history/all/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest process(const int64_t profile_id, const ProfileProcessRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("profile/process/%lld", profile_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest profile(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest social(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/social/%ld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace profile::blockList {
        ApiGetRequest add_to_block_list(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/blocklist/add/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest block_list(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/social/all/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove_from_block_list(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/blocklist/remove/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace profile::friends {
        ApiGetRequest friends(const int64_t profile_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/all/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest recommendations(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/friend/recommendations",
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest request_hide(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/request/hide/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest request_remove(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/request/remove/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest request_send(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/request/send/%lld", profile_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest requests_in(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/requests/in/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest requests_in_last(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/friend/requests/in/last",
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest requests_out(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/friend/requests/out/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest requests_out_last(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/friend/requests/out/last",
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace profile::list {
        ApiGetRequest add(const int32_t status, const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/list/add/%ld/%lld", status, release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int32_t status, const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/list/delete/%ld/%lld", status, release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest profile_list(const int32_t status, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/list/all/%ld/%ld", status, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest profile_list_by_profile(const int64_t profile_id, const int32_t status, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/list/all/%lld/%ld/%ld", profile_id, status, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
    };
    namespace profile::preferences {
        ApiPostMultipartRequest avatar_edit(const network::MultipartPart& image, const std::string& token) {
            return ApiPostMultipartRequest{
                .sub_url = "profile/preference/avatar/edit",
                .params = {
                    { "token", token }
                },
                .forms = {
                    image,
                    new network::MultipartContentPart("name", "image", "text/plain")
                }
            };
        }
        ApiGetRequest change_email(const std::string& current_password, const std::string& current_email, const std::string& new_email, const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/email/change",
                .params = {
                    { "current_password", current_password },
                    { "current", current_email },
                    { "new", new_email },
                    { "token", token }
                }
            };
        }
        ApiGetRequest change_email_confirm(const std::string& current_password, const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/email/change/confirm",
                .params = {
                    { "current", current_password },
                    { "token", token }
                },
            };
        }
        ApiPostRequest change_login(const std::string& login, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/login/change",
                .params = {
                    { "login", login },
                    { "token", token }
                }
            };
        }
        ApiPostRequest change_login_info(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/login/info",
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest change_password(const std::string& current_password, const std::string& new_password, const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/password/change",
                .params = {
                    { "current", current_password },
                    { "new", new_password },
                    { "token", token }
                },
            };
        }
        ApiPostRequest google_bind(const std::string& google_id_token, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/google/bind",
                .params = {
                    { "token", token }
                },
                .data = StringTools::sformat("idToken=%s", google_id_token),
                .type = "application/x-www-form-urlencoded"
            };
        }
        ApiPostRequest google_unbind(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/google/unbind",
                .params = {
                    { "token", token }
                },
                .data = "{}",
                .type = "application/json"
            };
        }
        ApiGetRequest my(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/my",
                .params = {
                    { "token", token }
                },
            };
        }
        ApiPostRequest privacy_counts_edit(const PrivacyEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/privacy/counts/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest privacy_friend_request_edit(const PrivacyEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/privacy/friendRequests/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest privacy_social_edit(const PrivacyEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/privacy/social/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest privacy_stats_edit(const PrivacyEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/privacy/stats/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest social(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/social",
                .params = {
                    { "token", token }
                },
            };
        }
        ApiPostRequest social_pages_edit(const SocialPagesEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/social/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest status_remove(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/preference/status/delete",
                .params = {
                    { "token", token }
                },
            };
        }
        ApiPostRequest status_edit(const StatusEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/status/edit",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest vk_bind(const std::string& vk_access_token, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/vk/bind",
                .params = {
                    { "token", token }
                },
                .data = StringTools::sformat("accessToken=%s", vk_access_token),
                .type = "application/x-www-form-urlencoded"
            };
        }
        ApiPostRequest vk_unbind(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "profile/preference/vk/unbind",
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace profile::releaseVote {
        ApiGetRequest all_release_unvoted(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/vote/release/unvoted/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest all_release_voted(const int64_t profile_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("profile/vote/release/voted/%lld/%ld", profile_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest last_release_unvoted(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "profile/vote/release/unvoted/last",
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace commentVotes {

    };
    namespace config {

    };
    namespace directLink {
        ApiPostRequest links(const DirectLinkRequest& request) {
            return ApiPostRequest{
                .sub_url = "video/parse",
                .data = request.serialize(),
                .type = "application/json"
            };
        }
    };
    namespace discover {
        ApiPostRequest comments_week() {
            return ApiPostRequest{
                .sub_url = "discover/comments",
                .data = "",
                .type = "application/json"
            };
        }
        ApiPostRequest discussing(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "discover/discussing",
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest intresting() {
            return ApiPostRequest{
                .sub_url = "discover/interesting"
            };
        }
        ApiPostRequest recommendations(const int32_t page, const int32_t previous_page, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("discover/recommendations/%ld", page),
                .params = {
                    { "previous_page", std::to_string(previous_page) },
                    { "token", token }
                }
            };
        }
        ApiPostRequest watching(const int32_t page, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("discover/watching/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace episode {
        ApiGetRequest episode_target(int64_t release_id, int64_t source_id, int32_t position) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("episode/target/%lld/%lld/%ld", release_id, source_id, position),
            };
        }
        ApiGetRequest episodes(const int64_t release_id, const int64_t type_id, const int64_t source_id, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("episode/%lld/%lld/%lld", release_id, type_id, source_id),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiPostRequest report(const int64_t release_id, const int64_t source_id, const int32_t position, const EpisodeReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("episode/report/%lld/%lld/%ld", release_id, source_id, position),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest sources(const int64_t release_id, const int64_t type_id) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("episode/%lld/%lld", release_id, type_id),
            };
        }
        ApiGetRequest types(const int64_t release_id) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("episode/%lld", release_id),
            };
        }
        ApiPostRequest unwatch(const int64_t release_id, const int64_t source_id, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("episode/unwatch/%lld/%lld", release_id, source_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest unwatch(const int64_t release_id, const int64_t source_id, const int32_t position, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("episode/unwatch/%lld/%lld/%ld", release_id, source_id, position),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest updates(const int64_t release_id, const int32_t page) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("episode/updates/%lld/%ld", release_id, page),
            };
        }
        ApiPostRequest watch(const int64_t release_id, const int64_t source_id, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("episode/watch/%lld/%lld", release_id, source_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest watch(const int64_t release_id, const int64_t source_id, const int32_t position, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("episode/watch/%lld/%lld/%ld", release_id, source_id, position),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace imporing {

    };
    namespace exporting {

    };
    namespace favorite {
        extern ApiGetRequest add(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("favorite/add/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        extern ApiGetRequest remove(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("favorite/delete/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        extern ApiGetRequest favorites(const int32_t page, const int32_t sort, int32_t filter_announce, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("favorite/all/%ld", page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "filter_announce", std::to_string(filter_announce) },
                    { "token", token }
                }
            };
        }
    };
    namespace filter {
        ApiPostRequest filter(const int32_t page, const FilterRequest& request, const bool extended_mode, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("filter/%ld", page),
                .params = {
                    { "extended_mode", extended_mode ? "true" : "false" },
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
    };
    namespace history {
        ApiGetRequest add(const int64_t release_id, const int64_t source_id, const int32_t position, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("history/add/%lld/%lld/%ld", release_id, source_id, position),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("history/delete/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest history(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("history/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace release {
        ApiGetRequest delete_vote(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/vote/delete/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest random(const bool extended_mode, const std::string& token) {
            return ApiGetRequest{
                .sub_url = "release/random",
                .params = {
                    { "token", token },
                    { "extended_mode", extended_mode ? "true" : "false" }
                }
            };
        }
        /* random from collection */
        ApiGetRequest random_collection(const int64_t release_id, const bool extended_mode, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/collection/%lld/random", release_id),
                .params = {
                    { "token", token },
                    { "extended_mode", extended_mode ? "true" : "false" }
                }
            };
        }
        ApiGetRequest random_favorite(const bool extended_mode, const std::string& token) {
            return ApiGetRequest{
                .sub_url = "release/random/favorite",
                .params = {
                    { "token", token },
                    { "extended_mode", extended_mode ? "true" : "false" }
                }
            };
        }
        ApiGetRequest random_profile_list(const int64_t profile_id, const int32_t status, const bool extended_mode, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/random/profile/list/%lld/%ld", profile_id, status),
                .params = {
                    { "token", token },
                    { "extended_mode", extended_mode ? "true" : "false" }
                }
            };
        }
        ApiGetRequest release(const int64_t release_id, const bool extended_mode, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/%lld", release_id),
                .params = {
                    { "token", token },
                    { "extended_mode", extended_mode ? "true" : "false" }
                }
            };
        }
        ApiPostRequest report(const int64_t release_id, const ReleaseReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/report/%lld", release_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest vote(const int64_t release_id, const int32_t vote, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/vote/add/%lld/%ld", release_id, vote),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace release::related {
        ApiGetRequest related(const int64_t related_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("related/%lld/%ld", related_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
    }
    namespace release::comment {
        ApiPostRequest add(const int64_t release_id, const CommentAddRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/comment/add/%lld", release_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest comment(const int64_t comment_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/comment/%lld", comment_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest comments(const int64_t release_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/comment/all/%lld/%ld", release_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int64_t comment_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/comment/delete/%lld", comment_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest edit(const int64_t comment_id, const CommentEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/comment/edit/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest process(const int64_t comment_id, const CommentProcessRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/comment/process/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest profile_comments(const int64_t profile_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/comment/all/profile/%lld/%ld", profile_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiPostRequest replies(const int64_t comment_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/comment/replies/%lld/%ld", comment_id, page),
                .params = {
                    { "sort", std::to_string(sort) },
                    { "token", token }
                }
            };
        }
        ApiPostRequest report(const int64_t comment_id, const DeprecatedReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("release/comment/report/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest vote(const int64_t comment_id, const int32_t vote, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/comment/vote/%lld/%ld", comment_id, vote),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace release::streamingPlatform {
        ApiGetRequest release_streaming_platform(const int64_t release_id) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("release/streaming/platform/%lld", release_id)
            };
        }
    };
    namespace release::video {
        ApiGetRequest categories() {
            return ApiGetRequest{
                .sub_url = "video/release/categories"
            };
        }
        ApiGetRequest category(const int64_t release_id, const int64_t category_id, const int32_t page) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("video/release/%lld/category/%lld/%ld", release_id, category_id, page)
            };
        }
        ApiGetRequest main(const int64_t release_id) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("video/release/%lld", release_id)
            };
        }
        ApiGetRequest profile_video(const int64_t profile_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("video/profile/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest video(const int64_t release_id, const int32_t page) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("video/release/%lld/%ld", release_id, page)
            };
        }
    };
    namespace release::video::appeal {
        ApiPostRequest add(const ReleaseVideoAppealRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = "video/appeal/add",
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest appeals(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("video/appeal/profile/%lld", page),
                .params = {
                    { "token", token }
                },
            };
        }
        ApiGetRequest appeals(const std::string& token) {
            return ApiGetRequest{
                .sub_url = "video/appeal/profile/last",
                .params = {
                    { "token", token }
                },
            };
        }
        ApiPostRequest remove(const int64_t appeal_id, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("video/appeal/delete/%lld", appeal_id),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace release::video::favorite {
        ApiGetRequest add(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("releaseVideoFavorite/add/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest remove(const int64_t release_id, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("releaseVideoFavorite/delete/%lld", release_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest favorites(const int64_t profile_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("releaseVideoFavorite/all/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace search {
        ApiPostRequest collection_search(const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/collections/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest favorite_collection_search(const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/favoriteCollections/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest favorites_search(const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/favorites/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest history_search(const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/history/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest profile_collection_search(const int64_t profile_id, const int64_t release_id, const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/profileCollections/%lld/%ld", profile_id, page),
                .params = {
                    { "token", token },
                    { "release_id", std::to_string(release_id) }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest profile_list_search(const int32_t status, const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/profile/list/%ld/%ld", status, page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest profile_search(const int32_t page, const SearchRequest& search_request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/profiles/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest release_search(const int32_t page, const SearchRequest& search_request, const std::string& api_version, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("search/releases/%ld", page),
                .params = {
                    { "token", token }
                },
                .headers = {
                    StringTools::sformat("API-Version: %s", api_version)
                },
                .data = search_request.serialize(),
                .type = "application/json"
            };
        }
    };
    namespace type {
        ApiGetRequest types(const int64_t release_id) {
            return ApiGetRequest{
                .sub_url = StringTools::snformat("type/%lld", release_id)
            };
        }
        ApiGetRequest types(const std::string& token) {
            return ApiGetRequest{
                 .sub_url = "type/all",
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
    namespace article {
        // ...
    };
    namespace article {
        ApiGetRequest article(const int64_t article_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/%lld", article_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest articles(const int32_t page, const ArticlesFilterRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/all/%ld", page),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest create(const int64_t channel_id, const ArticleCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/create/%lld", channel_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest remove(const int64_t article_id, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/delete/%lld", article_id),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest edit(const int64_t article_id, const ArticleCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/edit/%lld", article_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest latest_article(const std::string& token) {
            return ApiPostRequest{
                .sub_url = "article/latest",
                .params = {
                    { "token", token }
                }
            };
        }
        ApiPostRequest latest_articles(const int32_t page, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/latest/all/%ld", page),
                .params = {
                    { "token", token }
                }
            };
        }
        ApiGetRequest reposts(const int64_t article_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/reposts/%lld/%ld", article_id, page),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest vote(const int64_t article_id, const int32_t vote, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/vote/%lld/%ld", article_id, vote),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest votes(const int64_t article_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/votes/%lld/%ld", article_id, page),
                .params = {
                    { "token", token },
                    { "sort", std::to_string(sort) },
                }
            };
        }
    };
    namespace article::comment {
        ApiPostRequest add(const int64_t article_id, const CommentAddRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/add/%lld", article_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest comment(const int64_t article_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/%lld", article_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest comments(const int64_t article_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/all/%lld/%ld", article_id, page),
                 .params = {
                    { "token", token },
                    { "sort", std::to_string(sort) },
                 }
            };
        }
        ApiGetRequest comment_popular(const int64_t article_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/all/%lld/popular", article_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest remove(const int64_t comment_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/delete/%lld", comment_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest edit(const int64_t comment_id, const CommentEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/edit/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest process(const int64_t comment_id, const CommentProcessRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/process/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest profile_comments(const int64_t profile_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/all/profile/%lld/%ld", profile_id, vote),
                 .params = {
                    { "token", token },
                    { "sort", std::to_string(sort) }
                 }
            };
        }
        ApiPostRequest replies(const int64_t comment_id, const int32_t page, const int32_t sort, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/replies/%lld/%ld", comment_id, page),
                .params = {
                    { "token", token },
                    { "sort", std::to_string(sort) }
                }
            };
        }
        ApiPostRequest report(const int64_t comment_id, const DeprecatedReportRequest& request, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/report/%lld", comment_id),
                .params = {
                    { "token", token }
                },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest vote(const int64_t comment_id, const int32_t vote, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/comment/vote/%lld/%ld", comment_id, vote),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest votes(const int64_t comment_id, const int32_t page, int32_t sort, const std::string& token) {
            return ApiPostRequest{
                .sub_url = StringTools::snformat("article/comment/votes/%lld/%ld", comment_id, page),
                .params = {
                    { "token", token }
                }
            };
        }
    };
    namespace article::suggestion {
        ApiGetRequest article_suggestion(const int64_t article_suggestion_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("article/suggestion/%lld", article_suggestion_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest article_suggestions(const int32_t page, const ArticleSuggestionsFilterRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("article/suggestion/all/%ld", page),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest create(const int64_t channel_id, const ArticleSuggestionCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("article/suggestion/create/%lld", channel_id, vote),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest remove(const int64_t channel_id, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("article/suggestion/delete/%lld", channel_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest edit(const int64_t article_suggestion_id, const ArticleSuggestionCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("article/suggestion/edit/%lld", article_suggestion_id),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest publish(const int64_t article_suggestion_id, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("article/suggestion/publish/%lld", article_suggestion_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
    namespace channel {
        ApiPostMultipartRequest avatar_upload(const int64_t channel_id, const network::MultipartPart& image, const std::string& token) {
            return ApiPostMultipartRequest{
                .sub_url = StringTools::snformat("channel/avatar/upload/%lld", channel_id),
                .params = {
                    { "token", token }
                },
                .forms = {
                    image
                }
            };
        }
        ApiGetRequest block(const int64_t channel_id, const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/%lld/block/%lld", channel_id, profile_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest block_manage(const int64_t channel_id, const ChannelBlockManageRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/%lld/block/manage", channel_id),
                 .params = {
                    { "token", token }
                 },
                 .data = request.serialize(),
                 .type = "application/json"
            };
        }
        ApiGetRequest blocks(const int64_t channel_id, const int32_t page, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/%lld/block/all/%ld", channel_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest blog(const int64_t profile_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/blog/%lld", profile_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest channel(const int64_t channel_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/%lld", channel_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest channels(const int32_t page, const requests::ChannelsFilterRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/all/%ld", page),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostMultipartRequest cover_upload(const int64_t channel_id, const network::MultipartPart& image, const std::string& token) {
            return ApiPostMultipartRequest{
                .sub_url = StringTools::snformat("channel/cover/upload/%lld", channel_id),
                .params = {
                    { "token", token }
                },
                .forms = {
                    image
                }
            };
        }
        ApiPostRequest create(const ChannelCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = "channel/create",
                 .params = {
                    { "token", token }
                 },
                 .data = request.serialize(),
                 .type = "application/json"
            };
        }
        ApiPostRequest create_blog(const std::string& token) {
            return ApiPostRequest{
                 .sub_url = "channel/blog/create",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest edit(const int64_t channel_id, const ChannelCreateEditRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/edit/%lld", channel_id),
                 .params = {
                    { "token", token }
                 },
                 .data = request.serialize(),
                 .type = "application/json"
            };
        }
        ApiGetRequest editor_available(const int64_t channel_id, const bool is_suggestion, const bool is_edit_mode, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/%lld/editor/available", channel_id),
                 .params = {
                    { "token", token },
                    { "is_suggestion", is_suggestion ? "true" : "false" },
                    { "is_edit_mode", is_edit_mode ? "true" : "false" }
                 }
            };
        }
        ApiPostRequest permission_manage(const int64_t channel_id, const ChannelPermissionManageRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/%lld/permission/manage", channel_id),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest permissions(const int64_t channel_id, const int32_t page, const ChannelPermissionsFilterRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/%lld/permission/all/%ld", channel_id, page),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest recomendations(const int32_t page, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/recommendations/%ld", page),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest subscribe(const int64_t channel_id, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/subscribe/%lld", channel_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest subscribers(const int64_t channel_id, const int32_t page, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/%lld/subscriber/all/%ld", channel_id, page),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest subscription_count(const std::string& token) {
            return ApiGetRequest{
                 .sub_url = "channel/subscription/count",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest subscriptions(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("channel/subscription/all/%ld", page),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest unsubscribe(const int64_t channel_id, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("channel/unsubscribe/%lld", channel_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
    }
    namespace achievements {
        ApiGetRequest get_achievement(const int64_t achievement_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("achievement/get/%lld", achievement_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
    namespace profile::badge {
        ApiGetRequest all(const int32_t page, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("profile/preference/badge/all/%ld", page),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest edit(const int64_t badge_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("profile/preference/badge/edit/%lld", badge_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest remove(const std::string& token) {
            return ApiGetRequest{
                 .sub_url = "profile/preference/badge/remove",
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
    namespace feed {
        ApiPostRequest feed(const int32_t page, const ArticlesFilterRequest& request, const std::string& token) {
            return ApiPostRequest{
                 .sub_url = StringTools::snformat("feed/my/all/%ld", page),
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
    };
    namespace report {
        ApiPostRequest article(const ArticleReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/article",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest article_comment(const ArticleCommentReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/comment/article",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest article_comments_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/comment/article/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest article_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/article/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest channel(const ChannelReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/channel",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest channel_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/channel/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest collection(const CollectionReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/collection",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest collection_comment(const CollectionCommentReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/comment/collection",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest collection_comments_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/comment/collection/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest collection_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/collection/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest episode(const EpisodeReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/episode",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest episode_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/episode/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest profile(const ProfileReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/profile",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest profile_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/profile/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiPostRequest release(const ReleaseReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/release",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiPostRequest release_comment(const ReleaseCommentReportRequest& request, const std::string token) {
            return ApiPostRequest{
                 .sub_url = "report/comment/release",
                 .params = {
                    { "token", token }
                 },
                .data = request.serialize(),
                .type = "application/json"
            };
        }
        ApiGetRequest release_comments_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/comment/release/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
        ApiGetRequest release_reasons(const std::string token) {
            return ApiGetRequest{
                 .sub_url = "report/release/reasons",
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
    namespace schedule {
        ApiGetRequest schedule() {
            return ApiGetRequest{
                 .sub_url = "schedule"
            };
        }
    };
    namespace profile::role_list {
        ApiGetRequest all(const int32_t page, const int64_t role_id, const std::string& token) {
            return ApiGetRequest{
                 .sub_url = StringTools::snformat("role/all/%ld/%lld", page, role_id),
                 .params = {
                    { "token", token }
                 }
            };
        }
    };
};
