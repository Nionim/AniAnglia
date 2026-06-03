#include <netsess/StringTools.hpp>
#include <random>


namespace network {
    constexpr std::string_view StringTools::ASCII = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";

    std::string StringTools::gen_random_string(size_t length, std::string_view chars) {
        std::string result;

        std::random_device random_device;
        std::mt19937 random_seed(random_device());
        std::uniform_int_distribution<std::size_t> generator(0, chars.length() - 1);

        for (std::size_t i = 0; i < length; ++i) {
            result += chars[generator(random_seed)];
        }
        return result;
    }
}