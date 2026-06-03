#include <anixart/ApiProfiles.hpp>
#include <netsess/JsonTools.hpp>

namespace anixart {
	using requests::PrivacyEditRequest;
	using requests::SocialPagesEditRequest;
	using requests::StatusEditRequest;
	using namespace json;

	ApiProfiles::ApiProfiles(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	std::pair<Profile::Ptr, bool> ApiProfiles::get_profile(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::profile(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_pair(resp.get_if<Profile::Ptr>("profile", network::json::ParseJson::not_null), resp.get<bool>("is_my_profile"));
	}

	FriendsPages::UPtr ApiProfiles::get_friends(const ProfileID profile_id, const int32_t start_page) const {
		return std::make_unique<FriendsPages>(_session, _token, start_page, profile_id);
	}

	LoginChangeHistoryPages::UPtr ApiProfiles::get_profile_login_history(const ProfileID profile_id, const int32_t start_page) const {
		return std::make_unique<LoginChangeHistoryPages>(_session, _token, start_page, profile_id);
	}

	ProfileSocial::Ptr ApiProfiles::get_profile_social(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::social(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfileSocial>(resp);
	}

	ProfileReleaseCommentsPages::UPtr ApiProfiles::get_profile_release_comments(const ProfileID profile_id, const int32_t start_page, const Comment::Sort sort) const {
		return std::make_unique<ProfileReleaseCommentsPages>(_session, _token, start_page, profile_id, sort);
	}

	ProfileCollectionCommentsPages::UPtr ApiProfiles::get_profile_collection_comments(const ProfileID profile_id, const int32_t start_page, const Comment::Sort sort) const {
		return std::make_unique<ProfileCollectionCommentsPages>(_session, _token, start_page, profile_id, sort);
	}

	BlockListPages::UPtr ApiProfiles::block_list(const int32_t start_page) const {
		return std::make_unique<BlockListPages>(_session, _token, start_page);
	}

	void ApiProfiles::remove_from_block_list(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::blockList::remove_from_block_list(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::add_to_block_list(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::blockList::remove_from_block_list(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
	}

	std::vector<Profile::Ptr> ApiProfiles::friend_recomendations() const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::recommendations(_token));
		assert_status_code<GenericProfileError>(resp);
		return resp.get<CachingJsonArray>("content").to_vector<Profile::Ptr>();
	}

	void ApiProfiles::hide_friend_request(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::request_hide(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::remove_friend_request(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::request_remove(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::send_friend_request(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::request_send(static_cast<int64_t>(profile_id), _token));
		assert_status_code<GenericProfileError>(resp);
	}

	FriendRequestsInPages::UPtr ApiProfiles::friend_requests_in(const int32_t start_page) const {
		return std::make_unique<FriendRequestsInPages>(_session, _token, start_page);
	}

	std::vector<Profile::Ptr> ApiProfiles::friend_requests_in_last() const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::requests_in_last(_token));
		assert_status_code<GenericProfileError>(resp);
		return resp.get<CachingJsonArray>("content").to_vector<Profile::Ptr>();
	}

	FriendRequestsOutPages::UPtr ApiProfiles::friend_requests_out(const int32_t start_page) const {
		return std::make_unique<FriendRequestsOutPages>(_session, _token, start_page);
	}

	std::vector<Profile::Ptr> ApiProfiles::friend_requests_out_last() const {
		CachingJsonObject resp = _session.api_request(requests::profile::friends::requests_out_last(_token));
		assert_status_code<GenericProfileError>(resp);
		return resp.get<CachingJsonArray>("content").to_vector<Profile::Ptr>();
	}

	ProfilePreferenceStatus::Ptr ApiProfiles::edit_avatar(const std::filesystem::path& image_path) const {
		// TODO: create custom MultiPart which sets 'filename' field to jpg. This will allow uploading any image file extensions
		network::MultipartPart image_multipart(new network::MultipartFilePart("image", image_path.string(), "image/*"));
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::avatar_edit(image_multipart, _token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfilePreferenceStatus>(resp);
	}

	void ApiProfiles::change_email(const std::string& current_password, const std::string& current_email, const std::string& new_email) const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::change_email(current_password, current_email, new_email, _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::confirm_change_email(const std::string& current_password) const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::change_email_confirm(current_password, _token));
		assert_status_code<GenericProfileError>(resp);
	}
	std::string ApiProfiles::change_password(const std::string& current_password, const std::string& new_password) const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::change_password(current_password, new_password, _token));
		assert_status_code<ChangePasswordError>(resp);
		return resp.get_if<std::string>("token", network::json::ParseJson::not_null);
	}

	void ApiProfiles::change_login(const std::string& new_login) const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::change_login(new_login, _token));
		assert_status_code<ChangeLoginError>(resp);
	}

	LoginChangeInfo::Ptr ApiProfiles::login_change_info() const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::change_login_info(_token));
		assert_status_code<ChangeLoginError>(resp);
		return std::make_shared<LoginChangeInfo>(resp);
	}

	void ApiProfiles::bind_google(const std::string& google_id_token) const {
		// not implemented
		throw ApiError();
	}

	void ApiProfiles::unbind_google() const {
		// not implemented
		throw ApiError();
	}

	void ApiProfiles::bind_vk(const std::string& vk_access_token) const {
		// not implemented
		throw ApiError();
	}

	void ApiProfiles::unbind_vk() const {
		// not implemented
		throw ApiError();
	}

	ProfilePreferenceStatus::Ptr ApiProfiles::my_preferences() const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::my(_token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfilePreferenceStatus>(resp);
	}

	void ApiProfiles::edit_privacy_activity(const Profile::ActivityPermission permission) const {
		PrivacyEditRequest privacy_request;
		privacy_request.permission = static_cast<int32_t>(permission);
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::privacy_counts_edit(privacy_request, _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::edit_privacy_friend_requests(const Profile::FriendRequestPermission permission) const {
		PrivacyEditRequest privacy_request;
		privacy_request.permission = static_cast<int32_t>(permission);
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::privacy_friend_request_edit(privacy_request, _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::edit_privacy_social(const Profile::SocialPermission permission) const {
		PrivacyEditRequest edit_request;
		edit_request.permission = static_cast<int32_t>(permission);
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::privacy_social_edit(edit_request, _token));
		assert_status_code<GenericProfileError>(resp);
	}

	void ApiProfiles::edit_privacy_stats(const Profile::StatsPermission permission) const {
		PrivacyEditRequest privacy_request;
		privacy_request.permission = static_cast<int32_t>(permission);
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::privacy_stats_edit(privacy_request, _token));
		assert_status_code<GenericProfileError>(resp);
	}

	ProfileSocial::Ptr ApiProfiles::my_social() const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::social(_token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfileSocial>(resp);
	}

	ProfilePreferenceStatus::Ptr ApiProfiles::edit_social(const ProfileSocial::Ptr& new_social) const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::social_pages_edit(SocialPagesEditRequest(*new_social), _token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfilePreferenceStatus>(resp);
	}

	ProfilePreferenceStatus::Ptr ApiProfiles::remove_status() const {
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::status_remove(_token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfilePreferenceStatus>(resp);
	}

	ProfilePreferenceStatus::Ptr ApiProfiles::edit_status(const std::string& new_status) const {
		StatusEditRequest edit_request;
		edit_request.status = new_status;
		CachingJsonObject resp = _session.api_request(requests::profile::preferences::status_edit(edit_request, _token));
		assert_status_code<GenericProfileError>(resp);
		return std::make_shared<ProfilePreferenceStatus>(resp);
	}
};

