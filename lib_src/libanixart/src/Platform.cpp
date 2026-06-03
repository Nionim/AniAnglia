#include <anixart/Platform.hpp>
#include <netsess/StringTools.hpp>

#include <memory>

namespace anixart {
    using namespace network;

#if defined(_WIN32)
# define WIN32_MEAN_AND_LEAN
# include <Windows.h>
# pragma comment(lib, "version.lib" )
    std::string get_platform_version() {
        WCHAR path[_MAX_PATH] = {};
        if (!GetSystemDirectoryW(path, _MAX_PATH))
            return "Unknown";

        wcscat_s(path, L"\\kernel32.dll");

        //
        // Based on example code from this article
        // http://support.microsoft.com/kb/167597
        //

        DWORD handle;
# if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
        DWORD len = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, path, &handle);
# else
        DWORD len = GetFileVersionInfoSizeW(path, &handle);
# endif
        if (!len)
            return "Unknown";

        std::unique_ptr<uint8_t> buff(new (std::nothrow) uint8_t[len]);
        if (!buff)
            return "Unknown";

# if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
        if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, path, 0, len, buff.get()))
# else
        if (!GetFileVersionInfoW(path, 0, len, buff.get()))
# endif
            return "Unknown";

        VS_FIXEDFILEINFO* vinfo = nullptr;
        UINT infoSize;

        if (!VerQueryValueW(buff.get(), L"\\", reinterpret_cast<LPVOID*>(&vinfo), &infoSize))
            return "Unknown";

        if (!infoSize)
            return "Unknown";

        return StringTools::snformat("%u.%u", HIWORD(vinfo->dwProductVersionMS), LOWORD(vinfo->dwProductVersionMS));
    }
    std::string get_product_name() {
        return "PC";
    }
    std::string get_product_model() {
        return "Station";
    }
#elif defined(__CYGWIN__) && !defined(_WIN32)

#elif defined(__ANDROID__)

#elif defined(__linux__)

#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__) && defined (BSD)

#elif defined(__hpux)

#elif defined(_AIX)

#elif defined(__APPLE__) && defined(__MACH__)
# include <TargetConditionals.h>
# if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
    /* Should be defined in objc file */
    extern std::string get_platform_version();
    std::string get_product_name() {
        return "Apple";
    }
    /* Should be defined in objc file */
    extern std::string get_product_model();
# elif TARGET_OS_MAC == 1

# endif
#elif defined(__sun) && defined(__SVR4)

#else
    std::string get_platform_version() {
        return "Unknown";
    }
    std::string get_product_name() {
        return "Unknown";
    }
    std::string get_product_model() {
        return "Unknown";
    }
#endif
};