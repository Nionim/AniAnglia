#include <anixart/ApiSearch.hpp>
#include <netsess/JsonTools.hpp>

namespace anixart {
	using requests::FilterRequest;
	using requests::SearchRequest;

	ApiSearch::ApiSearch(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	FilterPages::UPtr ApiSearch::filter_search(const requests::FilterRequest& filter_request, const bool extended_mode, const int32_t start_page) const {
		return std::make_unique<FilterPages>(_session, _token, start_page, filter_request, false);
	}
	ReleaseSearchPages::UPtr ApiSearch::release_search(const SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<ReleaseSearchPages>(_session, _token, start_page, request);
	}
	ProfileSearchPages::UPtr ApiSearch::profile_search(const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<ProfileSearchPages>(_session, _token, start_page, request);
	}
	ProfileListSearchPages::UPtr ApiSearch::profile_list_search(const Profile::ListStatus list_status, const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<ProfileListSearchPages>(_session, _token, start_page, list_status, request);
	}
	ProfileCollectionSearchPages::UPtr ApiSearch::profile_collection_search(const ProfileID profile_id, const ReleaseID release_id, const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<ProfileCollectionSearchPages>(_session, _token, start_page, profile_id, release_id, request);
	}
	CollectionSearchPages::UPtr ApiSearch::collection_search(const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<CollectionSearchPages>(_session, _token, start_page, request);
	}
	FavoriteCollectionSearchPages::UPtr ApiSearch::favorite_collection_search(const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<FavoriteCollectionSearchPages>(_session, _token, start_page, request);
	}
	FavoriteSearchPages::UPtr ApiSearch::favorite_search(const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<FavoriteSearchPages>(_session, _token, start_page, request);
	}
	HistorySearchPages::UPtr ApiSearch::history_search(const requests::SearchRequest& request, const int32_t start_page) const {
		return std::make_unique<HistorySearchPages>(_session, _token, start_page, request);
	}
	CommentsWeekPages::UPtr ApiSearch::comments_week() const {
		return std::make_unique<CommentsWeekPages>(_session);
	}
	DiscussingPages::UPtr ApiSearch::discussing() const {
		return std::make_unique<DiscussingPages>(_session, _token);
	}
	InterestingPages::UPtr ApiSearch::interesting() const {
		return std::make_unique<InterestingPages>(_session);
	}
	RecomendationsPages::UPtr ApiSearch::recomendations(const int32_t start_page) const {
		return std::make_unique<RecomendationsPages>(_session, _token, start_page);
	}
	WatchingPages::UPtr ApiSearch::currently_watching(const int32_t start_page) const {
		return std::make_unique<WatchingPages>(_session, _token, start_page);
	}
}
