#include <anixart/Escaping.hpp>
#include <stdexcept>

namespace anixart {

	std::string_view escape_html_tag(std::string_view tag) {
		if (tag == "<br>") {
			return "\n";
		}
		return tag;
	}

	std::string escape_html_text(std::string_view str) {
		std::string escaped;
		escaped.reserve(str.size());

		size_t i = 0;
		while (i < str.length()) {
			if (str[i] == '<') {
				auto iter = std::find(str.begin() + i, str.end(), '>');
				if (iter == str.end()) {
					throw std::runtime_error("Invalid html escape tag: expected '>', but got EOF");
				}

				std::string_view tag(str.begin() + i, iter + 1);

				escaped += escape_html_tag(tag);
				i += tag.length();
			}
			else {
				escaped += str[i];
				i += 1;
			}
		}

		return escaped;
	}

	std::string unescape_html_text(std::string_view str) {
		std::string unescaped;
		unescaped.reserve(str.size());

		for (const char& c : str) {
			switch (c) {
			case '\n':
				unescaped += "<br>";
				break;
			default:
				unescaped += c;
			}
		}

		return unescaped;
	}
}