#include <netsess/NetTypes.hpp>
#include <curlpp/cURLpp.hpp>

namespace network {

    UrlParameters::UrlParameters(std::initializer_list<std::pair<std::string_view, std::string_view>> initial) : UrlParameters(UrlParameters(), initial) {}

    UrlParameters::UrlParameters(const UrlParameters& copy, std::initializer_list<std::pair<std::string_view, std::string_view>> expanded) {
        _params = copy._params;
        append_items(expanded);
    }

    UrlParameters UrlParameters::expand_copy(std::initializer_list<std::pair<std::string_view, std::string_view>> added) const {
        return UrlParameters(*this, added);
    }

    UrlParameters& UrlParameters::operator=(std::initializer_list<std::pair<std::string_view, std::string_view>> initial)& {
        _params.clear();
        append_items(initial);
        return *this;
    }

    std::string UrlParameters::apply(std::string_view url) const {
        std::string out(url);
        out += "?";
        out += _params;
        return out;
    }

    std::string UrlParameters::get() const {
        return _params;
    }

    bool UrlParameters::empty() const {
        return _params.empty();
    }

    void UrlParameters::append_items(std::initializer_list<std::pair<std::string_view, std::string_view>> items) {
        size_t new_size = _params.size();
        new_size += !_params.empty() ? sizeof('&') : 0;
        for (auto& [key, value] : items) {
            new_size += key.size() + value.size() + 2;
        }
        _params.reserve(new_size);

        if (!_params.empty()) {
            _params += "&";
        }
        for (auto& [key, value] : items) {
            _params += key;
            _params += "=";
            _params += value;
            _params += "&";
        }
        if (!_params.empty()) {
            _params.resize(_params.size() - sizeof('&'));
        }
    }

    JsonObject parse_json(std::string_view from) {
        return boost::json::parse(from).as_object();
    }
    std::string UrlEncoded::escape(const std::string& str) {
        return curlpp::escape(str);
    }
};
