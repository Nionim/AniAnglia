#include <anixart/Parser.hpp>

namespace anixart::parsers {
    Parser::Parser() : _session(), _is_debug(false) {
    }

    std::list<std::string> Parser::get_default_headers() const {
        return {
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36",
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
        };
    }

    void Parser::process_params(const std::vector<ParserParameter>& params) {
        for (auto& parameter : params) {
            switch (parameter.id) {
            case ParserParameterType::CustomHeader:
                _session.add_default_header(parameter.value);
                break;
            case ParserParameterType::Debug:
                _is_debug = true;
                break;
            default:
                // ?
                break;
            };
        }
    }
}
