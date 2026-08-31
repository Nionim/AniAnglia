#pragma once
#include <string_view>
#include <string>
#include <vector>

namespace de_anixart::urls {

    // Kodik
    inline constexpr std::string_view LINKS_URL_S = "https://kodik.biz/api/video-links?p=56a768d08f43091901c44b54fe970049&link=";
    inline constexpr std::string_view KODIK_HOST = "kodikplayer.com";
    inline constexpr std::string_view KODIK_FTOR_URL = "https://kodikplayer.com/ftor";
    inline constexpr std::string_view KODIK_INFO_URL = "https://kodikplayer.com";

    // Libria v2
    inline constexpr std::string_view EP_INFO_URL_S = "https://api.anilibria.tv/v2/getTitle?filter=player.host,player.playlist.";

    // Libria v1
    inline constexpr std::string_view RELEASE_API_URL_V1 = "https://anilibria.top/api/v1/anime/releases/";
}

namespace de_anixart::de_kodik::domains {
    inline const std::vector<std::string> kodik_exact = {"aniqit.com", "kodik.cc", "kodikplayer.com"};
    inline const std::vector<std::string> kodik_partial = {};
}

namespace de_anixart::de_libria::domains {
    inline const std::vector<std::string> libria_exact = {
        "anilibria.tv", "new.anilib.one", 
        "inori.anilib.one", "emilia.anilib.one", 
        "new.anilib.moe", "inori.anilib.moe", 
        "emilia.anilib.moe"};
    inline const std::vector<std::string> libria_partial = {"anilib.one", "libria.fun"};
}