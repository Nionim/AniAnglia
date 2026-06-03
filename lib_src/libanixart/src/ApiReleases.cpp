#include <anixart/ApiReleases.hpp>
#include <netsess/JsonTools.hpp>

namespace anixart {
	using namespace json;

	ApiReleases::ApiReleases(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	void ApiReleases::release_vote(const ReleaseID release_id, const int32_t vote) const {
		CachingJsonObject resp = _session.api_request(requests::release::vote(static_cast<int64_t>(release_id), vote, _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::delete_release_vote(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::delete_vote(static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	Release::Ptr ApiReleases::random_release(const bool extended_mode) const {
		CachingJsonObject resp = _session.api_request(requests::release::random(extended_mode, _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Release::Ptr>("release", network::json::ParseJson::not_null);
	}
	Release::Ptr ApiReleases::random_collection_release(const CollectionID collection_id, const bool extended_mode) const {
		CachingJsonObject resp = _session.api_request(requests::release::random_collection(static_cast<int64_t>(collection_id), extended_mode, _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Release::Ptr>("release", network::json::ParseJson::not_null);
	}
	Release::Ptr ApiReleases::random_favorite_release(const bool extended_mode) const {
		CachingJsonObject resp = _session.api_request(requests::release::random_favorite(extended_mode, _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Release::Ptr>("release", network::json::ParseJson::not_null);
	}
	Release::Ptr ApiReleases::random_profile_release(const ProfileID profile_id, const Release::Status status) const {
		CachingJsonObject resp = _session.api_request(requests::release::random_profile_list(static_cast<int64_t>(profile_id), static_cast<int64_t>(status), true, _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Release::Ptr>("release", network::json::ParseJson::not_null);
	}
	Release::Ptr ApiReleases::get_release(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::release(static_cast<int64_t>(release_id), false, _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Release::Ptr>("release", network::json::ParseJson::not_null);
	}
	ReleaseRelatedPages::UPtr ApiReleases::release_related(const ReleaseRelatedID related_id, const int32_t start_page) const {
		return std::make_unique<ReleaseRelatedPages>(_session, _token, start_page, related_id);
	}
	void ApiReleases::report_release(const requests::ReleaseReportRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::report::release(request, _token));
		assert_status_code<GenericReleaseError>(resp);
	}

	Comment::Ptr ApiReleases::add_release_comment(const ReleaseID release_id, const requests::CommentAddRequest& add_request) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::add(static_cast<int64_t>(release_id), add_request, _token));
		assert_status_code<CommentAddError>(resp);
		return resp.get_if<Comment::Ptr>("comment", network::json::ParseJson::not_null);
	}
	void ApiReleases::edit_release_comment(const CommentID comment_id, const requests::CommentEditRequest& edit_request) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::edit(static_cast<int64_t>(comment_id), edit_request, _token));
		assert_status_code<CommentEditError>(resp);
	}
	Comment::Ptr ApiReleases::release_comment(const CommentID comment_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::comment(static_cast<int64_t>(comment_id), _token));
		//assert_status_code<ReleaseError>(resp);
		return std::make_shared<Comment>(resp);
	}
	void ApiReleases::remove_release_comment(const CommentID comment_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::remove(static_cast<int64_t>(comment_id), _token));
		assert_status_code<CommentRemoveError>(resp);
	}
	ReleaseCommentsPages::UPtr ApiReleases::release_comments(const ReleaseID release_id, const int32_t start_page, const Comment::Sort sort) const {
		return std::make_unique<ReleaseCommentsPages>(_session, _token, start_page, release_id, sort);
	}
	void ApiReleases::process_release_comment(const CommentID comment_id, const requests::CommentProcessRequest& process_request) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::process(static_cast<int64_t>(comment_id), process_request, _token));
		assert_status_code<CommentEditError>(resp);
	}
	ReleaseCommentRepliesPages::UPtr ApiReleases::replies_to_comment(const CommentID comment_id, const int32_t start_page, const Comment::Sort sort) const {
		return std::make_unique<ReleaseCommentRepliesPages>(_session, _token, start_page, comment_id, sort);
	}
	void ApiReleases::report_release_comment(const requests::ReleaseCommentReportRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::report::release_comment(request, _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::vote_release_comment(const CommentID comment_id, const Comment::Sign sign) const {
		CachingJsonObject resp = _session.api_request(requests::release::comment::vote(static_cast<int64_t>(comment_id), static_cast<int32_t>(sign), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	std::vector<ReleaseVideoCategory::Ptr> ApiReleases::release_video_categories() const {
		CachingJsonObject resp = _session.api_request(requests::release::video::categories());
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("categories").to_vector<ReleaseVideoCategory::Ptr>();
	}
	ReleaseVideoCategoryPages::UPtr ApiReleases::release_video_category(const ReleaseID release_id, const ReleaseVideoCategoryID category_id, const int32_t start_page) const {
		return std::make_unique<ReleaseVideoCategoryPages>(_session, start_page, release_id, category_id);
	}
	std::vector<ReleaseVideoBlock::Ptr> ApiReleases::release_video_main(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::video::main(static_cast<int64_t>(release_id)));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("blocks").to_vector<ReleaseVideoBlock::Ptr>();
	}
	ReleaseVideoPages::UPtr ApiReleases::release_videos(const ReleaseID release_id, const int32_t start_page) const {
		return std::make_unique<ReleaseVideoPages>(_session, start_page, release_id);
	}
	ProfileReleaseVideoPages::UPtr ApiReleases::profile_release_videos(const ProfileID profile_id, const int32_t start_page) const {
		return std::make_unique<ProfileReleaseVideoPages>(_session, _token, start_page, profile_id);
	}
	void ApiReleases::raise_release_appeal(const requests::ReleaseVideoAppealRequest& appeal_request) const {
		CachingJsonObject resp = _session.api_request(requests::release::video::appeal::add(appeal_request, _token));
		assert_status_code<ReleaseVideoAppealError>(resp);
	}
	void ApiReleases::remove_release_appeal(const ReleaseVideoID video_appeal_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::video::appeal::remove(static_cast<int64_t>(video_appeal_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	ReleaseVideoAppealPages::UPtr ApiReleases::my_appeals(const int32_t start_page) const {
		return std::make_unique<ReleaseVideoAppealPages>(_session, _token, start_page);
	}
	std::vector<ReleaseVideo::Ptr> ApiReleases::my_last_appeals() {
		/* This is a Pageable Request, but with one page */
		CachingJsonObject resp = _session.api_request(requests::release::video::appeal::appeals(_token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("content").to_vector<ReleaseVideo::Ptr>();
	}
	void ApiReleases::add_release_to_favorites(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::favorite::add(static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::remove_release_from_favorites(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::favorite::remove(static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	ProfileFavoriteReleasesPages::UPtr ApiReleases::profile_favorites(const ProfileID profile_id, const Profile::ListSort sort, const int32_t filter_announce, const int32_t start_page) const {
		return std::make_unique<ProfileFavoriteReleasesPages>(_session, _token, start_page, profile_id, sort, filter_announce);
	}
	void ApiReleases::add_release_video_to_favorites(const ReleaseVideoID release_video_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::video::favorite::add(static_cast<int64_t>(release_video_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::remove_release_video_from_favorites(const ReleaseVideoID release_video_id) const {
		CachingJsonObject resp = _session.api_request(requests::release::video::favorite::remove(static_cast<int64_t>(release_video_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	ProfileReleaseVideoFavoritesPages::UPtr ApiReleases::profile_favorite_videos(const ProfileID profile_id, const int32_t start_page) const {
		return std::make_unique<ProfileReleaseVideoFavoritesPages>(_session, _token, start_page, profile_id);
	}
	void ApiReleases::add_to_history(const ReleaseID release_id, const EpisodeSourceID source_id, const int32_t position) const {
		CachingJsonObject resp = _session.api_request(requests::history::add(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), position, _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::remove_release_from_history(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::history::remove(static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	HistoryPages::UPtr ApiReleases::get_history(const int32_t start_page) const {
		return std::make_unique<HistoryPages>(_session, _token, start_page);
	}
	void ApiReleases::add_release_to_profile_list(const ReleaseID release_id, const Profile::ListStatus status) const {
		CachingJsonObject resp = _session.api_request(requests::profile::list::add(static_cast<int32_t>(status), static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiReleases::remove_release_from_profile_list(const ReleaseID release_id, const Profile::ListStatus status) const {
		CachingJsonObject resp = _session.api_request(requests::profile::list::remove(static_cast<int32_t>(status), static_cast<int64_t>(release_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	ProfileListPages::UPtr ApiReleases::my_profile_list(const Profile::ListStatus status, const Profile::ListSort sort, const int32_t start_page) const {
		return std::make_unique<ProfileListPages>(_session, _token, start_page, status, sort);
	}
	ProfileListByProfilePages::UPtr ApiReleases::profile_list(const ProfileID profile_id, const Profile::ListStatus status, const Profile::ListSort sort, const int32_t start_page) const {
		return std::make_unique<ProfileListByProfilePages>(_session, _token, start_page, profile_id, status, sort);
	}
	ProfileVotedReleasesPages::UPtr ApiReleases::profile_voted_releases(const ProfileID profile_id, const Release::ByVoteSort sort, const int32_t start_page) {
		return std::make_unique<ProfileVotedReleasesPages>(_session, _token, start_page, profile_id, sort);
	}
	AllReleaseUnvotedPages::UPtr ApiReleases::all_unvotes_releases(const int32_t start_page) {
		return std::make_unique<AllReleaseUnvotedPages>(_session, _token, start_page);
	}
};