#include <anixart/ApiTypes.hpp>
#include <anixart/CachingJson.hpp>
#include <anixart/Random.hpp>
#include <netsess/JsonTools.hpp>
#include <netsess/StringTools.hpp>

namespace anixart {
	using namespace json;
	using network::json::ParseJson;
	using network::json::InlineJson;
	using network::StringTools;
	using random::gen_random_string;

	static std::string to_string(const MediaFile::Ptr media_file) {
		return media_file->serialize();
	}

	struct ArticleBlockVisitor : boost::static_visitor<std::string> {
		template<typename T>
		std::string operator()(T&& val) const {
			return val->serialize();
		}
	};

	static std::string to_string(const ArticlePayload::BlockVariant& block_variant) {
		return boost::apply_visitor(ArticleBlockVisitor(), block_variant);
	}

	Badge::Badge(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name")),
		type(object.get<Type>("type")),
		date(object.get<TimestampPoint>("date")),
		badge_url(object.get<std::string>("badge_url"))
	{}

	Badge Badge::from_inner(json::CachingJsonObject & object) {
		return Badge(object, ctx_from_inner);
	}

	Badge::Badge(CachingJsonObject& object, ctx_from_inner_type) :
		id(object.get<int64_t>("badge_id")),
		name(object.get<std::string>("badge_name")),
		type(object.get<Type>("badge_type")),
		date(),
		badge_url(object.get<std::string>("badge_url"))
	{}

	ProfileToken::ProfileToken(const int64_t id, const std::string& token) :
		id(id),
		token(token)
	{}

	ProfileToken::ProfileToken(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		token(object.get<std::string>("token"))
	{}

	ProfileWatchDynamic::ProfileWatchDynamic(json::CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		day(object.get<std::chrono::day>("day")),
		watched_count(object.get<int32_t>("count")),
		date(object.get<TimestampPoint>("timestamp"))
	{}

	Role::Role(json::CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name")),
		color(object.get<std::string>("color"))
	{}

	Profile::Profile(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		username(object.get<std::string>("login")),
		avatar_url(object.get<std::string>("avatar")),
		status(object.get_if<std::string>("status", ParseJson::not_null)),
		telegram_page(object.get<std::string>("tg_page")),
		vk_page(object.get<std::string>("vk_page")),
		instagram_page(object.get<std::string>("inst_page")),
		discord_page(object.get<std::string>("discord_page")),
		tt_page(object.get<std::string>("tt_page")),
		last_activity_time(object.get<TimestampPoint>("last_activity_time")),
		register_date(object.get<TimestampPoint>("register_date")),

		badge(Badge::from_inner(object)),

		is_banned(object.get<bool>("is_banned")),
		is_perm_banned(object.get<bool>("is_perm_banned")),
		ban_expires(object.get<TimestampPoint>("ban_expires")),
		ban_reason(object.get_if<std::string>("ban_reason", ParseJson::not_null)),

		privilege_level(object.get<PrivilegeLevel>("privilege_level")),
		watched_count(object.get<int32_t>("completed_count")),
		dropped_count(object.get<int32_t>("dropped_count")),
		watched_time(object.get<int64_t>("watched_time")),
		watching_count(object.get<int32_t>("watching_count")),
		plan_count(object.get<int32_t>("plan_count")),
		hold_on_count(object.get<int32_t>("hold_on_count")),
		favorite_count(object.get<int32_t>("favorite_count")),
		video_count(object.get<int32_t>("video_count")),
		watched_episode_count(object.get<int32_t>("watched_episode_count")),
		comment_count(object.get<int32_t>("comment_count")),
		collection_count(object.get<int32_t>("collection_count")),
		rating_score(object.get<int32_t>("rating_score")),
		friend_status(parse_friend_status(object)),
		friend_count(object.get<int32_t>("friend_count")),
		votes(object.get<CachingJsonArray>("votes").to_vector<Release::Ptr>()),
		history(object.get<CachingJsonArray>("history").to_vector<Release::Ptr>()),
		watch_dynamics(object.get<CachingJsonArray>("watch_dynamics").to_vector<ProfileWatchDynamic::Ptr>()),
		roles(object.get<CachingJsonArray>("roles").to_vector<Role::Ptr>()),
		collections_preview(object.get<CachingJsonArray>("collections_preview").to_vector<Collection::Ptr>()),
		comments_preview(object.get<CachingJsonArray>("comments_preview").to_vector<Comment::Ptr>()),
		release_comments_preview(object.get<CachingJsonArray>("release_comments_preview").to_vector<Comment::Ptr>()),
		release_videos_preview(object.get<CachingJsonArray>("release_videos_preview").to_vector<ReleaseVideo::Ptr>()),

		is_blocked(object.get<bool>("is_blocked")),
		is_me_blocked(object.get<bool>("is_me_blocked")),
		block_added_date(object.get_if<TimestampPoint>("block_added_date", ParseJson::not_null)),

		is_sponsor(object.get<bool>("is_sponsor")),
		sponsorship_expires(object.get<TimestampPoint>("sponsorshipExpires")),

		is_online(object.get<bool>("is_online")),
		is_verified(object.get<bool>("is_verified")),
		is_social(object.get_if<bool>("is_social", ParseJson::exists)),
		is_social_hidden(object.get<bool>("is_social_hidden")),
		is_stats_hidden(object.get<bool>("is_stats_hidden")),
		is_counts_hidden(object.get<bool>("is_counts_hidden")),
		is_comment_notifications_enabled(object.get<bool>("is_comment_notifications_enabled")),
		is_release_type_notifications_enabled(object.get<bool>("is_release_type_notifications_enabled")),
		is_related_release_notifications_enabled(object.get<bool>("is_related_release_notifications_enabled")),
		is_report_process_notifications_enabled(object.get<bool>("is_report_process_notifications_enabled")),
		is_my_collection_comment_notifications_enabled(object.get<bool>("is_my_collection_comment_notifications_enabled")),
		is_my_article_comment_notifications_enabled(object.get<bool>("is_my_article_comment_notifications_enabled")),
		is_episode_notifications_enabled(object.get<bool>("is_episode_notifications_enabled")),
		is_first_episode_notification_enabled(object.get<bool>("is_first_episode_notification_enabled")),
		is_friend_requests_disallowed(object.get<bool>("is_friend_requests_disallowed")),
		is_login_changed(object.get_if<bool>("is_login_changed", ParseJson::exists)),
		is_vk_bound(object.get<bool>("is_vk_bound")),
		is_google_bound(object.get<bool>("is_vk_bound"))
	{}

	int32_t Profile::parse_friend_status(CachingJsonObject& object) {
		auto friend_status_value = object.get<CachingJsonValue>("friend_status");
		if (friend_status_value.is_null()) {
			return -1;
		}
		return static_cast<int32_t>(friend_status_value.as_int64());
	}

	Profile::FriendStatus Profile::get_friend_status_to(ProfileID other_id) const {
		if (friend_status == -1) {
			return FriendStatus::NotFriends;
		}
		if (friend_status == 2) {
			return FriendStatus::Friends;
		}
		if ((friend_status == 0 && other_id < id) || (friend_status == 1 && other_id >= id)) {
			return FriendStatus::SendedRequest;
		}
		return (friend_status == 1 && other_id < id) || (friend_status == 0 && other_id >= id) ? FriendStatus::RecievedRequest : FriendStatus::NotFriends;
	}

	ReleaseRelated::ReleaseRelated(json::CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name")),
		name_ru(object.get<std::string>("nameRu")),
		description(object.get_if<std::string>("description", ParseJson::not_null)),
		image_url(object.get<std::string>("image")),
		image_urls(object.get_if<CachingJsonArray>("images", ParseJson::not_null).to_vector<std::string>()),
		release_count(object.get<int64_t>("int64_t"))
	{}

	ReleaseVideoBanner::ReleaseVideoBanner(json::CachingJsonObject& object) :
		action_id(object.get<Action>("action_id")),
		name(object.get<std::string>("name")),
		image_url(object.get<std::string>("image")),
		value(object.get<std::string>("value")),
		is_new(object.get<bool>("is_new"))
	{}

	Comment::Comment(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		parent_comment_id(object.get_if<int64_t>("parent_comment_id", ParseJson::not_null)),
		message(object.get<std::string>("message")),
		vote(object.get<int32_t>("vote")),
		vote_count(object.get<int32_t>("vote_count")),
		reply_count(object.get<int64_t>("reply_count")),
		date(object.get<TimestampPoint>("timestamp")),
		author(object.get<Profile::Ptr>("profile")),
		release(object.get_if<Release::Ptr>("release", ParseJson::exists)),
		collection(object.get_if<Collection::Ptr>("collection", ParseJson::exists)),
		is_deleted(object.get<bool>("is_deleted")),
		is_edited(object.get<bool>("is_edited")),
		is_spoiler(object.get<bool>("is_spoiler"))
	{}

	Release::Release(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		title_original(object.get_if<std::string>("title_original", ParseJson::not_null)),
		title_ru(object.get_if<std::string>("title_ru", ParseJson::not_null)),
		title_alt(object.get_if<std::string>("title_alt", ParseJson::not_null)),
		description(object.get_if<std::string>("description", ParseJson::not_null)),
		author(object.get_if<std::string>("author", ParseJson::not_null)),
		director(object.get_if<std::string>("director", ParseJson::not_null)),
		studio(object.get_if<std::string>("studio", ParseJson::not_null)),
		image_url(object.get_if<std::string>("image", ParseJson::not_null)),
		country(object.get_if<std::string>("country", ParseJson::not_null)),
		translators(object.get_if<std::string>("translators", ParseJson::not_null)),
		year(object.get_if<std::string>("year", ParseJson::not_null)),
		genres(object.get_if<std::string>("genres", ParseJson::not_null)),
		rating(object.get<int32_t>("rating")),
		grade(object.get<double>("grade")),
		status(object.get<CachingJsonObject>("status").get<Status>("id")),
		category(object.get<CachingJsonObject>("category").get<Category>("id")),
		season(object.get<Season>("season")),
		release_date(object.get_if<std::string>("release_date", ParseJson::not_null)),
		creation_date(object.get_if<TimestampPoint>("creation_date", ParseJson::not_null)),
		last_update_date(object.get_if<TimestampPoint>("last_update_date", ParseJson::not_null)),
		screenshot_image_urls(object.get<CachingJsonArray>("screenshot_images").to_vector<std::string>()),
		related_releases(object.get<CachingJsonArray>("related_releases").to_vector<Release::Ptr>()),
		related(object.get<ReleaseRelated::Ptr>("related")),
		video_banners(object.get<CachingJsonArray>("video_banners").to_vector<ReleaseVideoBanner::Ptr>()),
		comments(object.get<CachingJsonArray>("comments").to_vector<Comment::Ptr>()),

		age_rating(object.get<AgeRating>("age_rating")),
		duration(object.get<std::chrono::minutes>("duration")),
		broadcast(object.get<int32_t>("broadcast")),
		aired_on_date(object.get<TimestampPoint>("aired_on_date")),
		profile_release_type_notification_preference_count(object.get_if<int32_t>("profile_release_type_notification_preference_count", ParseJson::exists)),
		// something here,
		vote1_count(object.get_if<int32_t>("vote_1_count", ParseJson::exists)),
		vote2_count(object.get_if<int32_t>("vote_2_count", ParseJson::exists)),
		vote3_count(object.get_if<int32_t>("vote_3_count", ParseJson::exists)),
		vote4_count(object.get_if<int32_t>("vote_4_count", ParseJson::exists)),
		vote5_count(object.get_if<int32_t>("vote_5_count", ParseJson::exists)),
		vote_count(object.get<int32_t>("vote_count")),
		your_vote(object.get_if<int32_t>("your_vote", ParseJson::exists_not_null)),
		voted_date(object.get_if<TimestampPoint>("voted_at", ParseJson::exists)),
		my_vote(object.get_if<int32_t>("my_vote", ParseJson::exists_not_null)),

		episodes_total(object.get_if<int32_t>("episodes_total", ParseJson::not_null)),
		collection_count(object.get_if<int64_t>("collection_count", ParseJson::exists)),
		favorite_count(object.get<int32_t>("favorites_count")),
		comment_count(object.get_if<int64_t>("comment_count", ParseJson::exists)),
		comment_per_day_count(object.get_if<int32_t>("comment_per_day_count", ParseJson::exists)),
		watched_count(object.get_if<int32_t>("completed_count", ParseJson::exists)),
		dropped_count(object.get_if<int32_t>("dropped_count", ParseJson::exists)),
		hold_on_count(object.get_if<int32_t>("hold_on_count", ParseJson::exists)),
		plan_count(object.get_if<int32_t>("plan_count", ParseJson::exists)),
		watching_count(object.get_if<int32_t>("watching_count", ParseJson::exists)),

		episode_last_update(object.get_if<EpisodeUpdate::Ptr>("episode_last_update", ParseJson::not_null)),
		episodes_released(object.get_if<int32_t>("episodes_released", ParseJson::not_null)),

		last_set_completed_date(object.get_if<TimestampPoint>("lastSetCompletedDate", ParseJson::exists)),
		last_set_dropped_date(object.get_if<TimestampPoint>("lastSetDroppedDate", ParseJson::exists)),
		last_set_favorite_date(object.get_if<TimestampPoint>("lastSetFavoriteDate", ParseJson::exists)),
		last_set_hold_on_date(object.get_if<TimestampPoint>("lastSetHoldOnDate", ParseJson::exists)),
		last_set_plan_date(object.get_if<TimestampPoint>("lastSetPlanDate", ParseJson::exists)),
		last_set_viewed_date(object.get_if<TimestampPoint>("lastSetViewedDate", ParseJson::exists)),
		last_set_watching_date(object.get_if<TimestampPoint>("lastSetWatchingDate", ParseJson::exists)),
		last_view_date(object.get_if<TimestampPoint>("last_view_timestamp", ParseJson::exists)),
		last_view_episode(object.get<Episode::Ptr>("last_view_episode")),

		note(object.get_if<std::string>("note", ParseJson::exists_not_null)),
		profile_list_status(object.get_if<Profile::ListStatus>("profile_list_status", ParseJson::exists_not_null)),

		is_adult(object.get<bool>("is_adult")),
		is_deleted(object.get<bool>("is_deleted")),
		is_favorite(object.get<bool>("is_favorite")),
		is_viewed(object.get<bool>("is_viewed")),
		is_play_disabled(object.get<bool>("is_play_disabled")),
		is_release_type_notifications_enabled(object.get_if<bool>("is_release_type_notifications_enabled", ParseJson::exists)),
		is_third_party_platforms_disabled(object.get_if<bool>("is_tpp_disabled", ParseJson::exists)),
		is_view_blocked(object.get_if<bool>("is_view_blocked", ParseJson::exists)),
		can_torlook_search(object.get_if<bool>("can_torlook_search", ParseJson::exists)),
		can_video_appeal(object.get_if<bool>("can_video_appeal", ParseJson::exists))
	{}

	EpisodeUpdate::EpisodeUpdate(CachingJsonObject& object) :
		last_episode_source_update_id(object.get<int64_t>("last_episode_source_update_id")),
		last_episode_type_update_id(object.get<int64_t>("last_episode_type_update_id")),
		last_episode_update_date(object.get<TimestampPoint>("last_episode_update_date")),
		last_episode_update_name(object.get_if<std::string>("last_episode_update_name", ParseJson::not_null)),
		last_episode_source_update_name(object.get<std::string>("last_episode_source_update_name")),
		last_episode_type_update_name(object.get<std::string>("lastEpisodeTypeUpdateName"))
	{}

	EpisodeSource::EpisodeSource(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name")),
		episodes_count(object.get<int64_t>("episodes_count"))
	{}

	EpisodeType::EpisodeType(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		view_count(object.get<int64_t>("view_count")),
		episodes_count(object.get<int64_t>("episodes_count")),
		name(object.get<std::string>("name")),
		workers(object.get_if<std::string>("workers", ParseJson::not_null))
	{}

	Episode::Episode(CachingJsonObject& object) :
		id(object.get_if<int64_t>("id", ParseJson::exists)),
		name(object.get_if<std::string>("name", ParseJson::not_null)),
		url(object.get<std::string>("url")),
		release_id(object.get_if<int64_t>("release_id", ParseJson::exists)),
		source_id(object.get_if<int64_t>("source_id", ParseJson::exists)),
		playback_position(object.get_if<int64_t>("playback_position", ParseJson::exists)),
		position(object.get<int32_t>("position")),

		is_watched(object.get<bool>("is_watched")),
		is_filler(object.get<bool>("is_filler"))
	{}

	ReleaseVideoCategory::ReleaseVideoCategory() :
		id(-1),
		name()
	{}

	ReleaseVideoCategory::ReleaseVideoCategory(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name"))
	{}

	ReleaseVideoHosting::ReleaseVideoHosting(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("name")),
		icon_url(object.get_if<std::string>("icon", ParseJson::not_null))
	{}

	ReleaseVideo::ReleaseVideo(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		title(object.get<std::string>("title")),
		image_url(object.get<std::string>("image")),
		url(object.get<std::string>("url")),
		player_url(object.get<std::string>("player_url")),
		date(object.get<TimestampPoint>("timestamp")),
		category(object.get_if<ReleaseVideoCategory::Ptr>("category", ParseJson::not_null)),
		hosting(object.get_if<ReleaseVideoHosting::Ptr>("hosting", ParseJson::not_null)),
		profile(object.get<Profile::Ptr>("profile")),
		release(object.get<Release::Ptr>("release")),

		favorite_count(object.get<int32_t>("favorites_count")),

		is_favorite(object.get<bool>("is_favorite"))
	{}

	ReleaseVideoBlock::ReleaseVideoBlock(CachingJsonObject& object) :
		category(object.get_if<ReleaseVideoCategory::Ptr>("category", ParseJson::not_null))
	{
		object.get<CachingJsonArray>("videos").assign_to(videos);
	}

	ReleaseStreamingPlatform::ReleaseStreamingPlatform(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		name(object.get<std::string>("id")),
		icon_url(object.get_if<std::string>("icon", ParseJson::not_null)),
		url(object.get<std::string>("id"))
	{}

	ReleaseVideos::ReleaseVideos(CachingJsonObject& object) :
		release(object.get_if<Release::Ptr>("release", ParseJson::not_null)),
		blocks(object.get<CachingJsonArray>("blocks").to_vector<ReleaseVideoBlock::Ptr>()),
		last_videos(object.get<CachingJsonArray>("last_videos").to_vector<ReleaseVideo::Ptr>()),
		streaming_platforms(object.get<CachingJsonArray>("streaming_platforms").to_vector<ReleaseStreamingPlatform::Ptr>())
	{}

	Collection::Collection(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		title(object.get<std::string>("title")),
		description(object.get<std::string>("description")),
		creator(object.get<Profile::Ptr>("creator")),
		image_url(object.get<std::string>("image")),
		last_update_date(object.get<TimestampPoint>("last_update_date")),
		creation_date(object.get<TimestampPoint>("creation_date")),
		releases(object.get<CachingJsonArray>("releases").to_vector<Release::Ptr>()),

		comment_count(object.get<int64_t>("comment_count")),
		favorite_count(object.get<int32_t>("favorites_count")),

		is_favorite(object.get<bool>("is_favorite")),
		is_private(object.get<bool>("is_private"))
	{}

	Interesting::Interesting(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		type(object.get<Type>("type")),
		title(object.get<std::string>("title")),
		description(object.get<std::string>("description")),
		image_url(object.get<std::string>("image")),
		action(object.get<std::string>("action")),
		is_hidden(object.get<bool>("is_hidden"))
	{}

	LoginChange::LoginChange(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		new_login(object.get<std::string>("newLogin")),
		date(object.get<TimestampPoint>("timestamp"))
	{}

	ProfileSocial::ProfileSocial(CachingJsonObject& object) :
		vk_page(object.get<std::string>("vk_page")),
		telegram_page(object.get<std::string>("tg_page")),
		instagram_page(object.get<std::string>("inst_page")),
		tiktok_page(object.get<std::string>("tt_page")),
		discord_page(object.get<std::string>("discord_page"))
	{}

	ProfilePreferenceStatus::ProfilePreferenceStatus(CachingJsonObject& object) :
		change_avatar_ban_expires(object.get<TimestampPoint>("ban_change_avatar_expires")),
		change_login_ban_expires(object.get<TimestampPoint>("ban_change_login_expires")),
		is_change_avatar_banned(object.get<bool>("is_change_avatar_banned")),
		is_change_login_banned(object.get<bool>("is_change_login_banned")),
		is_google_bound(object.get<bool>("is_google_bound")),
		is_vk_bound(object.get<bool>("is_vk_bound")),
		is_login_changed(object.get<bool>("is_login_changed")),
		privacy_activity(object.get<Profile::ActivityPermission>("privacy_counts")),
		privacy_friend_requests(object.get<Profile::FriendRequestPermission>("privacy_friend_requests")),
		privacy_social(object.get<Profile::SocialPermission>("privacy_social")),
		privacy_stats(object.get<Profile::StatsPermission>("privacy_stats")),
		avatar_url(object.get<std::string>("avatar")),
		status(object.get<std::string>("status")),
		vk_page(object.get<std::string>("vk_page")),
		tg_page(object.get<std::string>("tg_page"))
	{}

	LoginChangeInfo::LoginChangeInfo(CachingJsonObject& object) :
		is_change_available(object.get<bool>("is_change_available")),
		last_change_date(object.get<TimestampPoint>("last_change_at")),
		next_change_available_date(object.get<TimestampPoint>("next_change_available_at")),
		login(object.get<std::string>("login")),
		avatar_url(object.get<std::string>("avatar"))
	{}

	CollectionGetInfo::CollectionGetInfo(CachingJsonObject& object) :
		collection(object.get<Collection::Ptr>("collection")),
		watched_count(object.get<int32_t>("watched_count")),
		dropped_count(object.get<int32_t>("dropped_count")),
		hold_on_count(object.get<int32_t>("hold_on_count")),
		plan_count(object.get<int32_t>("plan_count")),
		watching_count(object.get<int32_t>("watching_count"))
	{}

	ArticleBlock::ArticleBlock(std::string_view id, std::string_view name) :
		id(id),
		name(name)
	{}

	ArticleBlock::ArticleBlock(CachingJsonObject& object) :
		id(object.get<std::string>("id")),
		name(object.get<std::string>("name"))
	{}

	std::string ArticleBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		//InlineJson::append(json, "data", std::nullopt);
		InlineJson::append(json, "data", *this, [](const ArticleBlock& block) {
			return "{}";
		});
		InlineJson::close_object(json);
		return json;
	}

	std::string ArticleBlock::get_random_uuid() {
		return gen_random_string(11, random::ascii);
	}

	ArticleDelimiterBlock::ArticleDelimiterBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleDelimiterBlock::ArticleDelimiterBlock(CachingJsonObject& object) :
		ArticleBlock(object)
	{}

	std::string ArticleDelimiterBlock::serialize() const {
		return ArticleBlock::serialize();
	}

	ArticleEmbedBlock::ArticleEmbedBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleEmbedBlock::ArticleEmbedBlock(CachingJsonObject& object) :
		ArticleEmbedBlock(object.get<std::string>("id"), object.get<CachingJsonObject>("data"))
	{}

	ArticleEmbedBlock::ArticleEmbedBlock(std::string_view id, json::CachingJsonObject data_object) :
		ArticleBlock(id, name),
		title(data_object.get<std::string>("title")),
		description(data_object.get<std::string>("description")),
		embed_url(data_object.get<std::string>("embed")),
		hash(data_object.get<std::string>("hash")),
		image_url(data_object.get<std::string>("image")),
		service(data_object.get<std::string>("service")),
		site_name(data_object.get<std::string>("site_name")),
		url(data_object.get<std::string>("url")),

		height(data_object.get<int32_t>("height")),
		width(data_object.get<int32_t>("width"))
	{}

	std::string ArticleEmbedBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleEmbedBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "title", block.title);
			InlineJson::append(json, "description", block.description);
			InlineJson::append(json, "embed", block.embed_url);
			InlineJson::append(json, "hash", block.hash);
			InlineJson::append(json, "image", block.image_url);
			InlineJson::append(json, "service", block.service);
			InlineJson::append(json, "site_name", block.site_name);
			InlineJson::append(json, "url", block.url);
			InlineJson::append(json, "height", block.height);
			InlineJson::append(json, "width", block.width);
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleHeaderBlock::ArticleHeaderBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleHeaderBlock::ArticleHeaderBlock(CachingJsonObject& object) :
		ArticleHeaderBlock(object, object.get<CachingJsonObject>("data"))
	{}

	std::string ArticleHeaderBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleHeaderBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "text", block.text);
			InlineJson::append(json, "text_length", block.text.length());
			InlineJson::append(json, "level", block.level);
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleHeaderBlock::ArticleHeaderBlock(CachingJsonObject& object, CachingJsonObject data_object) :
		ArticleBlock(object),
		text(data_object.get<std::string>("text")),
		text_length(data_object.get<int32_t>("text_length")),
		is_expand_available(data_object.get<bool>("is_expand_available")),
		level(data_object.get<int32_t>("level"))
	{}

	ArticleListBlock::ArticleListBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleListBlock::ArticleListBlock(CachingJsonObject& object) :
		ArticleListBlock(object, object.get<CachingJsonObject>("data"))
	{}

	std::string ArticleListBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleListBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "items", block.items);
			InlineJson::append(json, "item_count", block.items.size());
			InlineJson::append(json, "style", block.serialize_style());
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleListBlock::ArticleListBlock(CachingJsonObject& object, CachingJsonObject data_object) :
		ArticleBlock(object),
		items(data_object.get<CachingJsonArray>("items").to_vector<std::string>()),
		item_count(data_object.get<int32_t>("item_count")),
		style(parse_style(data_object.get<std::string>("style"))),
		is_expand_available(data_object.get<bool>("is_expand_available"))
	{}

	ArticleListBlock::Style ArticleListBlock::parse_style(const std::string& str) {
		if (str == style_unordered) {
			return Style::Unordered;
		}
		else if (str == style_ordered) {
			return Style::Ordered;
		}
		return Style::None;
	}

	std::string_view ArticleListBlock::serialize_style() const {
		switch(style) {
		case Style::Unordered:
			return style_unordered;
		case Style::Ordered:
			return style_ordered;
		default:
			return "none";
		}
	}

	MediaFile::MediaFile() :
		width(0),
		height(0)
	{}

	MediaFile::MediaFile(CachingJsonObject& object) :
		uuid(object.get<std::string>("id")),
		hash(object.get<std::string>("hash")),
		url(object.get<std::string>("url")),

		height(object.get<int32_t>("height")),
		width(object.get<int32_t>("width"))
	{}

	std::string MediaFile::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", uuid);
		InlineJson::append(json, "hash", hash);
		InlineJson::append(json, "url", url);
		InlineJson::append(json, "height", height);
		InlineJson::append(json, "width", width);
		InlineJson::close_object(json);
		return json;
	}

	ArticleMediaBlock::ArticleMediaBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleMediaBlock::ArticleMediaBlock(CachingJsonObject& object) :
		ArticleMediaBlock(object, object.get<CachingJsonObject>("data"))
	{}

	std::string ArticleMediaBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleMediaBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "items", block.items);
			InlineJson::append(json, "item_count", block.items.size());
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleMediaBlock::ArticleMediaBlock(CachingJsonObject& object, CachingJsonObject data_object) :
		ArticleBlock(object),
		items(data_object.get<CachingJsonArray>("items").to_vector<MediaFile::Ptr>()),
		item_count(data_object.get<int32_t>("item_count"))
	{}

	ArticleParagraphBlock::ArticleParagraphBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleParagraphBlock::ArticleParagraphBlock(CachingJsonObject& object) :
		ArticleParagraphBlock(object, object.get<CachingJsonObject>("data"))
	{}

	std::string ArticleParagraphBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleParagraphBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "text", block.text);
			InlineJson::append(json, "text_length", block.text.length());
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleParagraphBlock::ArticleParagraphBlock(CachingJsonObject& object, CachingJsonObject data_object) :
		ArticleBlock(object),
		text(data_object.get<std::string>("text")),
		text_length(data_object.get<int32_t>("text_length")),
		is_expand_available(data_object.get<int32_t>("is_expand_available"))
	{}

	ArticleQuoteBlock::ArticleQuoteBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleQuoteBlock::ArticleQuoteBlock(CachingJsonObject& object) :
		ArticleQuoteBlock(object, object.get<CachingJsonObject>("data"))
	{}

	std::string ArticleQuoteBlock::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "id", id);
		InlineJson::append(json, "name", name);
		InlineJson::append(json, "type", name);
		InlineJson::append(json, "data", *this, [](const ArticleQuoteBlock& block) {
			std::string json;
			InlineJson::open_object(json);
			InlineJson::append(json, "alignment", block.serialize_alignment());
			InlineJson::append(json, "caption", block.caption);
			InlineJson::append(json, "text", block.text);
			InlineJson::append(json, "caption_length", block.caption.length());
			InlineJson::append(json, "text_length", block.text.length());
			InlineJson::close_object(json);
			return json;
		});
		InlineJson::close_object(json);
		return json;
	}

	ArticleQuoteBlock::ArticleQuoteBlock(CachingJsonObject& object, CachingJsonObject data_object) :
		ArticleBlock(object),
		alignment(parse_alignment(data_object.get<std::string>("alignment"))),
		caption(data_object.get<std::string>("caption")),
		text(data_object.get<std::string>("text")),
		caption_length(data_object.get<int32_t>("caption_length")),
		text_length(data_object.get<int32_t>("text_length"))
	{}

	ArticleQuoteBlock::Alignment ArticleQuoteBlock::parse_alignment(const std::string& str) {
		if (str == alignment_left) {
			return Alignment::Left;
		}
		else if (str == alignment_center) {
			return Alignment::Center;
		}
		return Alignment::None;
	}

	std::string_view ArticleQuoteBlock::serialize_alignment() const {
		switch(alignment) {
		case Alignment::Left:
			return alignment_left;
		case Alignment::Center:
			return alignment_center;
		default:
			return "none";
		}
	}

	ArticleUnsupportedBlock::ArticleUnsupportedBlock(std::string_view id) :
		ArticleBlock(id, name)
	{}

	ArticleUnsupportedBlock::ArticleUnsupportedBlock(json::CachingJsonObject& object) :
		ArticleBlock(object)
	{}

	std::string ArticleUnsupportedBlock::serialize() const {
		return ArticleBlock::serialize();
	}

	ArticlePayload::ArticlePayload() :
		version(last_version)
	{}

	ArticlePayload::ArticlePayload(json::CachingJsonObject& object) :
		blocks(parse_blocks(object.get<CachingJsonArray>("blocks"))),
		block_count(object.get<int32_t>("block_count")),

		date(object.get<TimestampPoint>("time")),
		version(object.get<std::string>("version")),

		is_collapse_available(object.get<bool>("is_collapse_available")),
		is_expand_available(object.get<bool>("is_expand_available"))
	{}

	std::string ArticlePayload::serialize() const {
		std::string json;
		InlineJson::open_object(json);
		InlineJson::append(json, "blocks", blocks);
		InlineJson::append(json, "block_count", blocks.size());
		InlineJson::append(json, "time", date);
		InlineJson::append(json, "version", version);
		InlineJson::close_object(json);
		return json;
	}

	std::vector<ArticlePayload::BlockVariant> ArticlePayload::parse_blocks(CachingJsonArray array) {
		std::vector<BlockVariant> blocks;
		for (size_t i = 0; i < array.size(); ++i) {
			blocks.emplace_back(parse_block(array[i].as_object()));
		}
		return blocks;
	}

	ArticlePayload::BlockVariant ArticlePayload::parse_block(CachingJsonObject object) {
		std::string name = object.get<std::string>("name");
		if (name == ArticleDelimiterBlock::name) {
			return std::make_shared<ArticleDelimiterBlock>(object);
		}
		if (name == ArticleEmbedBlock::name) {
			return std::make_shared<ArticleEmbedBlock>(object);
		}
		if (name == ArticleHeaderBlock::name) {
			return std::make_shared<ArticleHeaderBlock>(object);
		}
		if (name == ArticleListBlock::name) {
			return std::make_shared<ArticleListBlock>(object);
		}
		if (name == ArticleMediaBlock::name) {
			return std::make_shared<ArticleMediaBlock>(object);
		}
		if (name == ArticleParagraphBlock::name) {
			return std::make_shared<ArticleParagraphBlock>(object);
		}
		if (name == ArticleQuoteBlock::name) {
			return std::make_shared<ArticleQuoteBlock>(object);
		}
		return std::make_shared<ArticleUnsupportedBlock>(object);
	}

	Channel::Channel(CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		title(object.get<std::string>("title")),
		description(object.get<std::string>("description")),
		avatar_url(object.get<std::string>("avatar_url")),
		cover_url(object.get<std::string>("cover_url")),
		blog_profile_id(object.get<int64_t>("blog_profile_id")),

		permission(object.get<Permission>("permission")),

		article_count(object.get<int32_t>("article_count")),
		creation_date(object.get<TimestampPoint>("creation_date")),
		last_update_date(object.get<TimestampPoint>("last_update_date")),
		subscriber_count(object.get<int32_t>("subscriber_count")),

		badge_id(object.get<int64_t>("badge_id")),
		badge_name(object.get<std::string>("badge_name")),
		badge_type(object.get<std::string>("badge_type")),
		badge_url(object.get<std::string>("badge_url")),

		block_expire_date(object.get<TimestampPoint>("block_expire_date")),
		block_reason(object.get<std::string>("block_reason")),

		is_deleted(object.get<bool>("is_deleted")),
		is_blocked(object.get<bool>("is_blocked")),
		is_perm_banned(object.get<bool>("is_perm_banned")),
		is_article_suggestion_enabled(object.get<bool>("is_article_suggestion_enabled")),
		is_verified(object.get<bool>("is_verified")),
		is_subscribed(object.get<bool>("is_subscribed")),
		is_commenting_enabled(object.get<bool>("is_commenting_enabled")),
		is_blog(object.get<bool>("isBlog")),

		is_administrator_or_higher(object.get<bool>("is_administrator_or_higher")),
		is_creator(object.get<bool>("is_creator"))
	{}

	Article::Article(json::CachingJsonObject& object) :
		id(object.get<int64_t>("id")),
		creation_date(object.get<TimestampPoint>("creation_date")),
		last_update_date(object.get<TimestampPoint>("last_update_date")),
		payload(object.get<ArticlePayload::Ptr>("payload")),

		author(object.get<Profile::Ptr>("author")),
		channel(object.get<Channel::Ptr>("channel")),

		vote_count(object.get<int32_t>("vote_count")),
		vote(object.get<int32_t>("vote")),

		contains_repost_article(object.get<bool>("contains_repost_article")),
		repost_count(object.get<int64_t>("repost_count")),
		repost_article(object.get<Article::Ptr>("repost_article")),

		is_deleted(object.get<bool>("is_deleted"))
	{}

	ChannelProfile::ChannelProfile(CachingJsonObject& object) :
		profile_id(object.get<int64_t>("id")),
		username(object.get<std::string>("login")),
		avatar_url(object.get<std::string>("avatar")),
		privilege_level(object.get<Profile::PrivilegeLevel>("privilege_level")),

		badge(Badge::from_inner(object)),

		ban_reason(object.get<std::string>("ban_reason")),
		ban_expires_date(object.get<TimestampPoint>("ban_expires_date")),

		is_banned(object.get<bool>("is_banned")),
		is_sponsor(object.get<bool>("is_sponsor")),
		is_verified(object.get<bool>("is_verified")),

		channel_id(object.get<int64_t>("channel_id")),
		permission(object.get<Channel::Permission>("permission")),
		permission_creation_date(object.get<TimestampPoint>("permission_creation_date")),

		block_reason(object.get<std::string>("block_reason")),
		block_expire_date(object.get<TimestampPoint>("block_expire_date")),

		is_blocked(object.get<bool>("is_blocked")),
		is_perm_blocked(object.get<bool>("is_perm_blocked"))
	{}
};