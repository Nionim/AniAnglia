#include <anixart/ApiArticles.hpp>
#include <netsess/StringTools.hpp>
#include <boost/regex.hpp>

#pragma execution_character_set("utf-8")

namespace anixart {
	using namespace json;
	using namespace network;
	using namespace requests;

	static const boost::regex is_youtube_regex(R"((?:https?:\/\/)?(?:www\.)?(?:youtube\.com\/\S*(?:(?:\/embed)?\/|watch\?\S*?&?v=)|youtu\.be\/)([a-zA-Z0-9_-]{6,11})((?:[?&][A-Za-z0-9._-]+=[A-Za-z0-9._-]+)*))");
	static const boost::regex is_vk_regex(R"((?:https?:\/\/)?(?:www\.)?vk\.com\/video\?z=video([-0-9]+)_([-0-9]+).*)");
	static const boost::regex is_link_regex(R"((?:https|http):\/\/.*)");

	ApiArticles::ApiArticles(const ApiSession& session, const std::string& token) : _session(session), _token(token) {}

	Article::Ptr ApiArticles::get_article(const ArticleID article_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::article(static_cast<int64_t>(article_id), _token));
		assert_status_code<GenericArticleError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	ArticlesPages::UPtr ApiArticles::get_articles(const requests::ArticlesFilterRequest& request, const int32_t start_page) const {
		return std::make_unique<ArticlesPages>(_session, _token, start_page, request);
	}

	LatestArticlesPages::UPtr ApiArticles::get_latest_articles(const int32_t start_page) const {
		return std::make_unique<LatestArticlesPages>(_session, _token, start_page);
	}

	ArticleID ApiArticles::get_latest_article_id() const {
		CachingJsonObject resp = _session.api_request(requests::article::latest_article(_token));
		assert_status_code<GenericArticleError>(resp);
		return static_cast<ArticleID>(resp.get<int64_t>("article_id")); // TODO: test
	}

	ArticleRepostsPages::UPtr ApiArticles::get_article_reposts(const ArticleID article_id, const ArticleRepostsPages::Sort sort,const int32_t start_page) const {
		return std::make_unique<ArticleRepostsPages>(_session, _token, start_page, article_id, sort);
	}

	void ApiArticles::vote_article_comment(const CommentID comment_id, const int32_t vote) const {
		CachingJsonObject resp = _session.api_request(requests::article::comment::vote(static_cast<int64_t>(comment_id), vote, _token));
		assert_status_code<CommentVoteError>(resp);
	}

	void ApiArticles::vote_article(const ArticleID article_id, const int32_t vote) const {
		CachingJsonObject resp = _session.api_request(requests::article::vote(static_cast<int64_t>(article_id), vote, _token));
		assert_status_code<GenericArticleError>(resp);
	}

	ArticleVotesPages::UPtr ApiArticles::get_article_votes(const ArticleID article_id, const Profile::VoteFilterBy filter, const int32_t start_page) const {
		return std::make_unique<ArticleVotesPages>(_session, _token, start_page, article_id, filter);
	}

	Article::Ptr ApiArticles::create_article(const ChannelID channel_id, const requests::ArticleCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::create(static_cast<int64_t>(channel_id), request, _token));
		assert_status_code<ArticleCreateEditError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	Article::Ptr ApiArticles::edit_article(const ArticleID article_id, const requests::ArticleCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::edit(static_cast<int64_t>(article_id), request, _token));
		assert_status_code<ArticleCreateEditError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	void ApiArticles::remove_article(const ArticleID article_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::remove(static_cast<int64_t>(article_id), _token));
		assert_status_code<ArticleRemoveError>(resp);
	}

	void ApiArticles::report_article(const requests::ArticleReportRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::report::article(request, _token));
		assert_status_code<ReportError>(resp);
	}

	ArticleCommentsPages::UPtr ApiArticles::get_article_comments(const ArticleID article_id, const Comment::Sort sort, const int32_t start_page) const {
		return std::make_unique<ArticleCommentsPages>(_session, _token, start_page, article_id, sort);
	}

	ArticlePopularCommentsPages::UPtr ApiArticles::get_article_popular_comments(const ArticleID article_id) const {
		return std::make_unique<ArticlePopularCommentsPages>(_session, _token, article_id);
	}

	ArticleCommentsByProfilePages::UPtr ApiArticles::get_profile_article_comments(const ProfileID profile_id, const Comment::Sort sort, const int32_t start_page) const {
		return std::make_unique<ArticleCommentsByProfilePages>(_session, _token, start_page, profile_id, sort);
	}

	ArticleCommentRepliesPages::UPtr ApiArticles::get_article_comment_replies(const CommentID comment_id, const Comment::Sort sort, const int32_t start_page) const {
		return std::make_unique<ArticleCommentRepliesPages>(_session, _token, start_page, comment_id, sort);
	}

	Comment::Ptr ApiArticles::add_article_comments(const ArticleID article_id, const requests::CommentAddRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::comment::add(static_cast<int64_t>(article_id), request, _token));
		assert_status_code<CommentAddError>(resp);
		return resp.get<Comment::Ptr>("comment");
	}

	void ApiArticles::edit_article_comments(const CommentID comment_id, const requests::CommentEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::comment::edit(static_cast<int64_t>(comment_id), request, _token));
		assert_status_code<CommentEditError>(resp);
	}

	void ApiArticles::remove_article_comments(const CommentID comment_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::comment::remove(static_cast<int64_t>(comment_id), _token));
		assert_status_code<CommentRemoveError>(resp);
	}

	void ApiArticles::report_article_comment(const requests::ArticleCommentReportRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::report::article_comment(request, _token));
		assert_status_code<ReportError>(resp);
	}

	void ApiArticles::process_article_comment(const CommentID comment_id, const requests::CommentProcessRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::comment::process(static_cast<int64_t>(comment_id), request, _token));
		assert_status_code<GenericArticleError>(resp);
	}

	Article::Ptr ApiArticles::get_article_suggestion(const ArticleID article_suggestion_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::suggestion::article_suggestion(static_cast<int64_t>(article_suggestion_id), _token));
		assert_status_code<ArticleError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	ArticleSuggestionsPages::UPtr ApiArticles::get_article_suggestions(const requests::ArticleSuggestionsFilterRequest& request, const int32_t start_page) const {
		return std::make_unique<ArticleSuggestionsPages>(_session, _token, start_page, request);
	}

	Article::Ptr ApiArticles::create_article_suggestion(const ChannelID channel_id, const requests::ArticleSuggestionCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::suggestion::create(static_cast<int64_t>(channel_id), request, _token));
		assert_status_code<ArticleCreateEditError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	Article::Ptr ApiArticles::edit_article_suggestion(const ArticleID article_suggestion_id, const requests::ArticleSuggestionCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::article::suggestion::edit(static_cast<int64_t>(article_suggestion_id), request, _token));
		assert_status_code<ArticleCreateEditError>(resp);
		return resp.get<Article::Ptr>("article");
	}

	void ApiArticles::remove_article_suggestion(const ArticleID article_suggestion_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::suggestion::remove(static_cast<int64_t>(article_suggestion_id), _token));
		assert_status_code<ArticleSuggestionRemoveError>(resp);
	}

	ArticleID ApiArticles::publish_article_suggestion(const ArticleID article_suggestion_id) const {
		CachingJsonObject resp = _session.api_request(requests::article::suggestion::publish(static_cast<int64_t>(article_suggestion_id), _token));
		assert_status_code<ArticleSuggestionPublishError>(resp);
		return static_cast<ArticleID>(resp.get<int64_t>("article_id"));
	}

	ArticleCommentVotesPages::UPtr ApiArticles::get_article_comment_votes(const CommentID comment_id, const Profile::VoteFilterBy filter, const int32_t start_page) const {
		return std::make_unique<ArticleCommentVotesPages>(_session, _token, start_page, comment_id, filter);
	}

	Channel::Ptr ApiArticles::get_channel(const ChannelID channel_id) const {
		CachingJsonObject resp = _session.api_request(requests::channel::channel(static_cast<int64_t>(channel_id), _token));
		assert_status_code<ChannelError>(resp);
		return resp.get<Channel::Ptr>("channel");
	}

	ChannelsPages::UPtr ApiArticles::get_channels(const requests::ChannelsFilterRequest& request, const int32_t start_page) const {
		return std::make_unique<ChannelsPages>(_session, _token, start_page, request);
	}

	Channel::Ptr ApiArticles::get_blog_channel(const ProfileID profile_id) const {
		CachingJsonObject resp = _session.api_request(requests::channel::blog(static_cast<int64_t>(profile_id), _token));
		assert_status_code<ChannelError>(resp);
		return resp.get<Channel::Ptr>("channel");
	}

	Channel::Ptr ApiArticles::create_channel(const requests::ChannelCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::channel::create(request, _token));
		assert_status_code<ChannelCreateEditError>(resp);
		return resp.get<Channel::Ptr>("channel");
	}

	Channel::Ptr ApiArticles::edit_channel(const ChannelID channel_id, const requests::ChannelCreateEditRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::channel::edit(static_cast<int64_t>(channel_id), request, _token));
		assert_status_code<ChannelCreateEditError>(resp);
		return resp.get<Channel::Ptr>("channel");
	}

	Channel::Ptr ApiArticles::create_blog_channel() const {
		CachingJsonObject resp = _session.api_request(requests::channel::create_blog(_token));
		assert_status_code<BlogCreateError>(resp);
		return resp.get<Channel::Ptr>("channel");
	}

	std::string ApiArticles::upload_channel_cover(const ChannelID channel_id, const std::filesystem::path& filepath) const {
		MultipartPart part = new MultipartFilePart("image", filepath.string());
		CachingJsonObject resp = _session.api_request(requests::channel::cover_upload(static_cast<int64_t>(channel_id), part, _token));
		assert_status_code<ChannelUploadCoverAvatarError>(resp);
		return resp.get<std::string>("url");
	}

	std::string ApiArticles::upload_channel_avatar(const ChannelID channel_id, const std::filesystem::path& filepath) const {
		MultipartPart part = new MultipartFilePart("image", filepath.string());
		CachingJsonObject resp = _session.api_request(requests::channel::avatar_upload(static_cast<int64_t>(channel_id), part, _token));
		assert_status_code<ChannelUploadCoverAvatarError>(resp);
		return resp.get<std::string>("url");
	}

	ChannelSubscribersPages::UPtr ApiArticles::get_channel_subscribers(const ChannelID channel_id, const int32_t start_page) const {
		return std::make_unique<ChannelSubscribersPages>(_session, _token, start_page, channel_id);
	}

	void ApiArticles::subscribe_to_channel(const ChannelID channel_id) const {
		CachingJsonObject resp = _session.api_request(requests::channel::subscribe(static_cast<int64_t>(channel_id), _token));
		assert_status_code<ChannelSubscribeError>(resp);
	}

	void ApiArticles::unsubscribe_to_channel(const ChannelID channel_id) const {
		CachingJsonObject resp = _session.api_request(requests::channel::unsubscribe(static_cast<int64_t>(channel_id), _token));
		assert_status_code<ChannelUnsubscribeError>(resp);
	}

	SubscribtionsPages::UPtr ApiArticles::get_my_subscriptions(const int32_t start_page) const {
		return std::make_unique<SubscribtionsPages>(_session, _token, start_page);
	}

	int64_t ApiArticles::get_my_subscriptions_count() const {
		CachingJsonObject resp = _session.api_request(requests::channel::subscription_count(_token));
		assert_status_code<GenericArticleError>(resp);
		return resp.get<int64_t>("subscription_count");
	}

	ChannelRecomendationsPages::UPtr ApiArticles::get_channel_recomendations(const int32_t start_page) const {
		return std::make_unique<ChannelRecomendationsPages>(_session, _token, start_page);
	}

	ChannelPermissionsPages::UPtr ApiArticles::get_channel_permissions(const ChannelID channel_id, const requests::ChannelPermissionsFilterRequest& request, const int32_t start_page) const {
		return std::make_unique<ChannelPermissionsPages>(_session, _token, start_page, channel_id, request);
	}

	void ApiArticles::edit_channel_permissions(const ChannelID channel_id, const requests::ChannelPermissionManageRequest& request) const {
		CachingJsonObject resp = _session.api_request(requests::channel::permission_manage(static_cast<int64_t>(channel_id), request, _token));
		assert_status_code<ChannelPermissionManageError>(resp);
	}

	std::string ApiArticles::get_channel_media_token(ChannelID channel_id, bool is_suggestion, bool is_edit_mode) const {
		CachingJsonObject resp = _session.api_request(requests::channel::editor_available(static_cast<int64_t>(channel_id), is_suggestion, is_edit_mode, _token));
		assert_status_code<ArticleEditorAvailableError>(resp);
		return resp.get<std::string>("media_upload_token");
	}

	MediaFile::Ptr ApiArticles::upload_media_file(const std::filesystem::path& filepath, const std::string& media_upload_token) const {
		requests::ApiPostMultipartRequest request;
		request.base_url = requests::editor_url;
		request.sub_url = "content/upload";
		request.forms = {
			new MultipartFilePart("file", filepath.string())
		};
		request.headers = {
			make_authorization_header(media_upload_token)
		};
		request.params = {
			{ "token", _token }
		};

		CachingJsonObject resp = _session.api_request(request);
		if (resp.get<int64_t>("success") != 1) {
			throw GenericArticleError(1);
		}
		return resp.get<MediaFile::Ptr>("file");
	}

	ArticleEmbedBlock::Ptr ApiArticles::upload_embed_url(const std::string& url, const std::string& media_upload_token) const {
		std::string_view service_name = get_url_service_name(url);
		requests::ApiPostRequest request;
		request.base_url = requests::editor_url;
		request.sub_url = StringTools::sformat("embed/%s", service_name);
		request.headers = {
			make_authorization_header(media_upload_token)
		};
		request.params = {
			{ "token", _token },
			{ "url", UrlEncoded::escape(url) }
		};

		CachingJsonObject resp = _session.api_request(request);
		if (resp.get<int64_t>("success") != 1) {
			throw GenericArticleError(1);
		}

		auto block = std::make_shared<ArticleEmbedBlock>(ArticleBlock::get_random_uuid(), resp);
		block->service = service_name;
		block->url = url;
		return block;
	}

	std::string_view ApiArticles::get_url_service_name(std::string_view url) const {
		if (boost::regex_match(url.begin(), url.end(), is_youtube_regex)) {
			return "youtube";
		}
		if (boost::regex_match(url.begin(), url.end(), is_vk_regex)) {
			return "vk";
		}
		if (boost::regex_match(url.begin(), url.end(), is_link_regex)) {
			return "link";
		}
		throw ApiParseError();
	}

	std::string ApiArticles::make_authorization_header(const std::string& media_upload_token) const {
		return StringTools::sformat("Authorization: Bearer %s", media_upload_token);
	}
}