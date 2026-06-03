#include <anixart/Random.hpp>
#include <netsess/StringTools.hpp>

namespace anixart::random {
#ifdef LIBANIXART_TESTS_BUILD
	//extern decltype(&gen_random_string) mock_gen_random_string;
#endif // LIBANIXART_TESTS_BUILD

	std::string gen_random_string(const size_t length, std::string_view chars) {
#ifndef LIBANIXART_TESTS_BUILD
		return network::StringTools::gen_random_string(length, chars);
#else
		//return mock_gen_random_string(length, chars);
		return std::string(length, 'A');
#endif // !LIBANIXART_TESTS_BUILD
	}
};
