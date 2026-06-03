#include <anixart/Parsers.hpp>
#include <anixart/KodikParser.hpp>
#include <anixart/LibriaParser.hpp>

#include <boost/regex.hpp>

namespace anixart::parsers {
	const boost::regex URL_HOST_RE = boost::regex(R"(https?:\/\/(?:www\.)?((?:\w+\.)+\w+))");

	Parsers::Parsers() {
		_parsers.push_back(std::make_shared<KodikParser>());
		_parsers.push_back(std::make_shared<LibriaParser>());
		// ...
	}
	Parser::Ptr Parsers::get_parser(const std::string& url) const {
		boost::smatch match;
		if (!boost::regex_search(url, match, URL_HOST_RE)) {
			return nullptr;
		}
		const std::string& host = match[1];
		for (auto& parser : _parsers) {
			if (parser->valid_host(host)) {
				return parser;
			}
		}
		return nullptr;
	}
	std::unordered_map<std::string, std::string> Parsers::extract_info(const std::string& url, const std::vector<ParserParameter>& params) {
		Parser::Ptr parser = get_parser(url);
		if (parser == nullptr) {
			return {};
		}
		return parser->extract_info(url, params);
	}
};
