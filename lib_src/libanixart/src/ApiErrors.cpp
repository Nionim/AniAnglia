#include <anixart/ApiErrors.hpp>

#define DECLARE_ERROR_MESSAGE(klass, message)   \
    const char* klass::what() const noexcept {  \
        return message;                         \
    }

#define DECLARE_TEMPLATE_ERROR_MESSAGE(klass, message)   \
    template<>                                  \
    const char* klass::what() const noexcept {  \
        return message;                         \
    }

namespace anixart {
    DECLARE_ERROR_MESSAGE(ApiError, "libanixart api error")
    ApiRequestError::ApiRequestError(int64_t code) : code(code) {}
    int64_t ApiRequestError::get_code() const noexcept {
        return static_cast<int64_t>(code);
    }
    DECLARE_ERROR_MESSAGE(ApiRequestError, "libanixart request error")
    DECLARE_ERROR_MESSAGE(ApiParseError, "libanixart parse error")

    ApiAuthRequestError::ApiAuthRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiAuthRequestError, "libanixart auth error")
    ApiBookmarksRequestError::ApiBookmarksRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiBookmarksRequestError, "libanixart bookmarks request error")
    ApiCollectionRequestError::ApiCollectionRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiCollectionRequestError, "libanixart collection request error")
    ApiProfileRequestError::ApiProfileRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiProfileRequestError, "libanixart profile request error")
    ApiReleaseRequestError::ApiReleaseRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiReleaseRequestError, "libanixart release request error")
    ApiReportRequestError::ApiReportRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiReportRequestError, "libanixart report request error")
    ApiArticleRequestError::ApiArticleRequestError(int64_t code) : ApiRequestError(code) {}
    DECLARE_ERROR_MESSAGE(ApiArticleRequestError, "libanixart report article error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericAuthError, "libanixart generic auth error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericBookmarksError, "libanixart generic bookmarks error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericCollectionError, "libanixart generic collection error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericProfileError, "libanixart generic profile error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericReleaseError, "libanixart generic release error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericRequestError, "libanixart generic request error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GenericArticleError, "libanixart generic article error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(SignUpError, "libanixart sign up error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ResendError, "libanixart email resend error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(VerifyError, "libanixart email verify error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(SignInError, "libanixart sign in error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(RestoreError, "libanixart restore error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(RestoreResendError, "libanixart restore email resend error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(RestoreVerifyError, "libanixart restore email verify error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GoogleAuthError, "libanixart google auth error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(VkAuthError, "libanixart vk auth error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(BookmarksImportError, "libanixart bookmarks import error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(BookmarksImportStatusError, "libanixart bookmarks import status error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(BookmarksExportError, "libanixart bookmarks export error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(GetCollectionError, "libanixart collection get error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(CreateEditCollectionError, "libanixart create/edit colletion error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(RemoveCollectionError, "libanixart Remove colletion error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(EditImageCollection, "libanixart edit image colletion error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(FavoriteCollectionAddError, "libanixart favorite collection add error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(FavoriteCollectionRemoveError, "libanixart favorite collection Remove error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ReleaseAddCollectionError, "libanixart release add collection error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ChangeEmailConfirmError, "libanixart chenge email confirm error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChangeEmailError, "libanixart change email error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChangeLoginInfoError, "libanixart change login info error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChangeLoginError, "libanixart change login error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChangePasswordError, "libanixart change password error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GoogleBindError, "libanixart google bind error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(GoogleUnbindError, "libanixart google unbind error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(RemoveFriendRequestError, "libanixart remove friend request error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(SendFriendRequestError, "libanixart send friend request error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(SocialEditError, "libanixart social edit error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(VkBindError, "libanixart vk bind error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(VkUnbindError, "libanixart vk unbind error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ReleaseVideosError, "libanixart release videos error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ReleaseVideoAppealError, "libanixart release video appeal error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleError, "libanixart article error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleCreateEditError, "libanixart article create/edit error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleRemoveError, "libanixart article remove error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleEditorAvailableError, "libanixart article editor available error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleSuggestionRemoveError, "libanixart article suggestion remove error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ArticleSuggestionPublishError, "libanixart article suggestion publish error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelError, "libanixart channel error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(BlogCreateError, "libanixart blog create error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelBlockError, "libanixart channel block error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelCreateEditError, "libanixart channel create/edit error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelPermissionManageError, "libanixart channel permission manage error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelSubscribeError, "libanixart channel subscribe error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelUnsubscribeError, "libanixart channel unsubscribe error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(ChannelUploadCoverAvatarError, "libanixart channel upload cover/avatar error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(CommentAddError, "libanixart comment add error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(CommentRemoveError, "libanixart comment Remove error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(CommentEditError, "libanixart comment edit error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(CommentVoteError, "libanixart comment vote error")

    DECLARE_TEMPLATE_ERROR_MESSAGE(ReportError, "libanixart report error")
    DECLARE_TEMPLATE_ERROR_MESSAGE(PageableError, "libanixart pageable error")


};