#include <anixart/ApiCollections.hpp>
#include <anixart/CachingJson.hpp>

namespace anixart {
	using namespace json;

	ApiCollections::ApiCollections(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	CollectionGetInfo::Ptr ApiCollections::get_collection(const CollectionID collection_id) {
		CachingJsonObject resp = _session.api_request(requests::collection::collection(static_cast<int64_t>(collection_id), _token));
		assert_status_code<GetCollectionError>(resp);
		return std::make_shared<CollectionGetInfo>(resp);
	}
	CollectionsPages::UPtr ApiCollections::all_collections(const Collection::Sort sort, const int32_t where, const int32_t start_page) {
		return std::make_unique<CollectionsPages>(_session, _token, start_page, where, sort);
	}
	ProfileCollectionsPages::UPtr ApiCollections::profile_collections(const ProfileID profile_id, const int32_t start_page) {
		return std::make_unique<ProfileCollectionsPages>(_session, _token, start_page, profile_id);
	}
	ReleaseCollectionsPages::UPtr ApiCollections::release_collections(const ReleaseID release_id, const Collection::Sort sort, const int32_t start_page) {
		return std::make_unique<ReleaseCollectionsPages>(_session, _token, start_page, release_id, sort);
	}
	CollectionReleasesPages::UPtr ApiCollections::collection_releases(const CollectionID collection_id, const int32_t start_page) {
		return std::make_unique<CollectionReleasesPages>(_session, _token, start_page, collection_id);
	}
	void ApiCollections::report_collection(const requests::CollectionReportRequest& request) {
		CachingJsonObject resp = _session.api_request(requests::report::collection(request, _token));
		assert_status_code<GenericCollectionError>(resp);
	}
	Comment::Ptr ApiCollections::add_collection_comment(const CollectionID collection_id, const requests::CommentAddRequest& request) {
		CachingJsonObject resp = _session.api_request(requests::collection::comment::add(static_cast<int64_t>(collection_id), request, _token));
		assert_status_code<CommentAddError>(resp);
		return resp.get<Comment::Ptr>("comment");
	}
	Comment::Ptr ApiCollections::collection_comment(const CommentID comment_id) {
		CachingJsonObject resp = _session.api_request(requests::collection::comment::comment(static_cast<int64_t>(comment_id), _token));
		assert_status_code<GenericCollectionError>(resp);
		return std::make_shared<Comment>(resp);
	}
	CollectionCommentsPages::UPtr ApiCollections::collection_comments(const CollectionID collection_id, const Comment::Sort sort, const int32_t start_page) {
		return std::make_unique<CollectionCommentsPages>(_session, _token, start_page, collection_id, sort);
	}
	void ApiCollections::remove_comment(const CommentID comment_id) {
		CachingJsonObject resp = _session.api_request(requests::collection::comment::remove(static_cast<int64_t>(comment_id), _token));
		assert_status_code<CommentRemoveError>(resp);
	}
	void ApiCollections::edit_comment(const CommentID comment_id, const requests::CommentEditRequest& request) {
		CachingJsonObject resp = _session.api_request(requests::collection::comment::edit(static_cast<int64_t>(comment_id), request, _token));
		assert_status_code<CommentEditError>(resp);
	}
	void ApiCollections::process_comment(const CommentID comment_id, const requests::CommentProcessRequest& request) {
		CachingJsonObject resp = _session.api_request(requests::collection::comment::process(static_cast<int64_t>(comment_id), request, _token));
		assert_status_code<GenericCollectionError>(resp);
	}
	CollectionCommentRepliesPages::UPtr ApiCollections::replies_to_comment(const CommentID comment_id, const Comment::Sort sort, const int32_t start_page) {
		return std::make_unique<CollectionCommentRepliesPages>(_session, _token, start_page, comment_id, sort);
	}
	void ApiCollections::report_collection_comment(const requests::CollectionCommentReportRequest& request) {
		CachingJsonObject resp = _session.api_request(requests::report::collection_comment(request, _token));
		assert_status_code<GenericCollectionError>(resp);
	}
	void ApiCollections::vote_collection_comment(const CommentID comment_id, const Comment::Sign vote) {
		assert(vote != Comment::Sign::Neutral);
		CachingJsonObject resp = _session.api_request(requests::collection::comment::vote(static_cast<int64_t>(comment_id), static_cast<int32_t>(vote), _token));
		assert_status_code<CommentVoteError>(resp);
	}
	void ApiCollections::add_collection_to_favorites(const CollectionID collection_id) {
		CachingJsonObject resp = _session.api_request(requests::collection::favorite::add(static_cast<int64_t>(collection_id), _token));
		assert_status_code<FavoriteCollectionAddError>(resp);
	}
	void ApiCollections::remove_collection_from_favorites(const CollectionID collection_id) {
		CachingJsonObject resp = _session.api_request(requests::collection::favorite::remove(static_cast<int64_t>(collection_id), _token));
		assert_status_code<FavoriteCollectionRemoveError>(resp);
	}
	FavoriteCollectionsPages::UPtr ApiCollections::my_favorite_collections(const int32_t start_page) {
		return std::make_unique<FavoriteCollectionsPages>(_session, _token, start_page);
	}
};