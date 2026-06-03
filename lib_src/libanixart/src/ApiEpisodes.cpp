#include <anixart/ApiEpisodes.hpp>
#include <anixart/ApiErrors.hpp>
#include <netsess/JsonTools.hpp>

namespace anixart {
	using namespace json;

	ApiEpisodes::ApiEpisodes(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	Episode::Ptr ApiEpisodes::get_episode_target(const ReleaseID release_id, const EpisodeSourceID source_id, const int32_t position) const {
		CachingJsonObject resp = _session.api_request(requests::episode::episode_target(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), position));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get_if<Episode::Ptr>("episode", network::json::ParseJson::not_null);
	}
	std::vector<Episode::Ptr> ApiEpisodes::get_release_episodes(const ReleaseID release_id, const EpisodeTypeID type_id, const EpisodeSourceID source_id, const Episode::Sort sort) const {
		CachingJsonObject resp = _session.api_request(requests::episode::episodes(static_cast<int64_t>(release_id), static_cast<int64_t>(type_id), static_cast<int64_t>(source_id), static_cast<int32_t>(sort), _token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("episodes").to_vector<Episode::Ptr>();
	}
	std::vector<EpisodeSource::Ptr> ApiEpisodes::get_release_sources(const ReleaseID release_id, const EpisodeTypeID type_id) const {
		CachingJsonObject resp = _session.api_request(requests::episode::sources(static_cast<int64_t>(release_id), static_cast<int64_t>(type_id)));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("sources").to_vector<EpisodeSource::Ptr>();
	}
	std::vector<EpisodeType::Ptr> ApiEpisodes::get_release_types(const ReleaseID release_id) const {
		CachingJsonObject resp = _session.api_request(requests::episode::types(static_cast<int64_t>(release_id)));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("types").to_vector<EpisodeType::Ptr>();
	}
	std::vector<EpisodeType::Ptr> ApiEpisodes::get_all_types() const {
		CachingJsonObject resp = _session.api_request(requests::type::types(_token));
		assert_status_code<GenericReleaseError>(resp);
		return resp.get<CachingJsonArray>("types").to_vector<EpisodeType::Ptr>();
	}
	void ApiEpisodes::add_watched_episode(const ReleaseID release_id, EpisodeSourceID source_id, const int32_t position) const {
		CachingJsonObject resp = _session.api_request(requests::episode::watch(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), position, _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiEpisodes::add_watched(const ReleaseID release_id, EpisodeSourceID source_id) const {
		CachingJsonObject resp = _session.api_request(requests::episode::watch(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiEpisodes::remove_watched_episode(const ReleaseID release_id, EpisodeSourceID source_id, const int32_t position) const {
		CachingJsonObject resp = _session.api_request(requests::episode::unwatch(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), position, _token));
		assert_status_code<GenericReleaseError>(resp);
	}
	void ApiEpisodes::remove_watched(const ReleaseID release_id, EpisodeSourceID source_id) const {
		CachingJsonObject resp = _session.api_request(requests::episode::unwatch(static_cast<int64_t>(release_id), static_cast<int64_t>(source_id), _token));
		assert_status_code<GenericReleaseError>(resp);
	}
}
