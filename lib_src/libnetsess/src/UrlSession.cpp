#include <netsess/UrlSession.hpp>
#include <netsess/StringTools.hpp>

#include <sstream>
#include <curlpp/Options.hpp>

namespace network {
#if defined(_WIN32)
	constexpr std::string_view UrlSession::COOKIEFILE_MEMORY = "NULL";
#else
	constexpr std::string_view UrlSession::COOKIEFILE_MEMORY = "/dev/null";
#endif

	const char* UrlSessionError::what() const noexcept {
		return "URL Session exception";
	}

	void UrlSession::init() {
		curlpp::initialize();
	}

	void UrlSession::deinit() {
		curlpp::terminate();
	}

	UrlSession::UrlSession() {
		set_verbose(false);
		set_timeout(10);
		set_cookie_file(std::string(COOKIEFILE_MEMORY));

		_easy_handle.setOpt(curlpp::options::SslVerifyPeer(true));
		_easy_handle.setOpt(curlpp::options::HttpHeader(_default_headers));
	}

	UrlSession::~UrlSession() {
	}

	void UrlSession::enable_redirects(const bool value, const long max) {
		_easy_handle.setOpt(cURLpp::Options::FollowLocation(value));
		if (value) {
			_easy_handle.setOpt(cURLpp::Options::MaxRedirs(max));
		}

	}

	void UrlSession::set_verbose(const bool value) {
		_easy_handle.setOpt(curlpp::options::Verbose(value));
	}

	void UrlSession::set_timeout(const long value) {
		_easy_handle.setOpt(curlpp::options::Timeout(value));
	}

	void UrlSession::set_default_headers(const std::list<std::string>& headers) {
		_default_headers = headers;
		_easy_handle.setOpt(curlpp::options::HttpHeader(_default_headers));
	}

	void UrlSession::add_default_header(const std::string& header) {
		_default_headers.push_back(header);
	}

	void UrlSession::set_cookie_file(const std::string& filename) {
		_easy_handle.setOpt(curlpp::options::CookieFile(filename));
	}

	std::string UrlSession::get_request(const std::string& url, const std::vector<std::string>& extra_headers, const UrlParameters& params) const {
		curlpp::Easy op_handle(_easy_handle.getCurlHandle().clone());
		std::ostringstream stream;

		op_handle.setOpt(curlpp::options::WriteStream(&stream));
		if (!params.empty()) {
			op_handle.setOpt(curlpp::options::Url(params.apply(url)));
		}
		else {
			op_handle.setOpt(curlpp::options::Url(url));
		}

		if (!extra_headers.empty()) {
			op_handle.setOpt(curlpp::options::HttpHeader(make_expanded_header(extra_headers)));
		}

		try {
			op_handle.perform();
			return stream.str();
		}
		catch (const curlpp::LogicError& e) {
			(e);
			throw UrlSessionError();
		}
		catch (const curlpp::RuntimeError& e) {
			(e);
			throw UrlSessionError();
		}
	}

	std::string UrlSession::post_request(const std::string& url, const std::string& data, std::string_view content_type, const std::vector<std::string>& extra_headers, const UrlParameters& params) const {
		curlpp::Easy op_handle(_easy_handle.getCurlHandle().clone());
		std::ostringstream stream;

		op_handle.setOpt(curlpp::options::WriteStream(&stream));
		if (!params.empty()) {
			op_handle.setOpt(curlpp::options::Url(params.apply(url)));
		}
		else {
			op_handle.setOpt(curlpp::options::Url(url));
		}
		op_handle.setOpt(curlpp::options::PostFields(data));
		op_handle.setOpt(curlpp::options::PostFieldSizeLarge(data.length()));

		std::list<std::string> new_headers = make_expanded_header(extra_headers);
		if (!content_type.empty()) {
			new_headers.push_back(StringTools::sformat("Content-Type: %s; Charset=utf-8", content_type));
		}
		op_handle.setOpt(curlpp::options::HttpHeader(new_headers));

		try {
			op_handle.perform();
			return stream.str();
		}
		catch (const curlpp::LogicError& e) {
			(e);
			throw UrlSessionError();
		}
		catch (const curlpp::RuntimeError& e) {
			(e);
			throw UrlSessionError();
		}
	}

	std::string UrlSession::post_multipart_request(const std::string& url, const MultipartForms& forms, const std::vector<std::string>& extra_headers, const UrlParameters& params) const {
		curlpp::Easy op_handle(_easy_handle.getCurlHandle().clone());
		std::ostringstream stream;

		op_handle.setOpt(curlpp::options::WriteStream(&stream));
		if (!params.empty()) {
			op_handle.setOpt(curlpp::options::Url(params.apply(url)));
		}
		else {
			op_handle.setOpt(curlpp::options::Url(url));
		}
		op_handle.setOpt(curlpp::options::HttpPost(forms));

		if (!extra_headers.empty()) {
			op_handle.setOpt(curlpp::options::HttpHeader(make_expanded_header(extra_headers)));
		}

		try {
			op_handle.perform();
			return stream.str();
		}
		catch (const curlpp::LogicError& e) {
			(e);
			throw UrlSessionError();
		}
		catch (const curlpp::RuntimeError& e) {
			(e);
			throw UrlSessionError();
		}
	}

	std::list<std::string> UrlSession::make_expanded_header(const std::vector<std::string>& extra_headers) const {
		std::list<std::string> new_headers = _default_headers;
		for (auto& header : extra_headers) {
			new_headers.push_back(header);
		}
		return new_headers;
	}

};
