#include <anixart/CachingJson.hpp>

using namespace network;

namespace anixart::json {
	CacheMissError::CacheMissError() : std::runtime_error("CachingJson: cache miss") {}

	namespace detail {
		CachedObject::CachedObject(JsonObject object, const std::shared_ptr<std::vector<CachedObject>>& cache) : _object(object), _cache(cache)
#ifdef LIBANIXART_CACHE_JSON_TYPE_CHECK
			, _constructed_object_info(&typeid(void)) 
#endif  
		{
		}
		bool CachedObject::is_constructed() const {
			return _constructed_object.get() != nullptr;
		}
		std::shared_ptr<void>& CachedObject::get_constructed() {
			assert(is_constructed());
			return _constructed_object;
		}
		SharedCache::SharedCache() : _cache(std::make_shared<std::vector<CachedObject>>()) {

		}
		SharedCache::SharedCache(network::JsonObject& root_cache_object) : SharedCache(){
			_cache->reserve(get_cached_items_count(root_cache_object));
			traverse_and_add_to_cache(root_cache_object);
		}
		SharedCache::SharedCache(network::JsonArray& root_cache_array) : SharedCache() {
			_cache->reserve(get_cached_items_count(root_cache_array));
			traverse_and_add_to_cache(root_cache_array);
		}
		SharedCache::SharedCache(JsonValue& root_cache_value) : SharedCache() {
			_cache->reserve(get_cached_items_count(root_cache_value));
			traverse_and_add_to_cache(root_cache_value);
		}
		SharedCache::SharedCache(const size_t initial_capacity) : SharedCache() {
			_cache->reserve(initial_capacity);
		}
		SharedCache::SharedCache(const std::shared_ptr<std::vector<CachedObject>>& cache) : _cache(cache) {
		}
		CachedObject& SharedCache::get_item(int64_t id) {
			int64_t index = id - 1;
			if (index < 0 || static_cast<size_t>(index) >= _cache->size()) {
				throw CacheMissError();
			}
			return (*_cache)[index];
		}
		void SharedCache::add_object(const JsonObject& object) {
			_cache->emplace_back(object, _cache);
		}
		void SharedCache::traverse_and_add_to_cache(JsonObject& object) {
			if (object.contains("@id")) {
				add_object(object);
			}
			for (auto& [key, val] : object) {
				traverse_and_add_to_cache(val);
			}
		}
		void SharedCache::traverse_and_add_to_cache(JsonArray& array) {
			for (auto& val : array) {
				traverse_and_add_to_cache(val);
			}
		}
		void SharedCache::traverse_and_add_to_cache(JsonValue& value) {
			if (!value.is_object() && !value.is_array()) {
				return;
			}
			if (value.is_object()) {
				traverse_and_add_to_cache(value.get_object());
			}
			if (value.is_array()) {
				traverse_and_add_to_cache(value.get_array());
			}
		}

		int64_t SharedCache::get_cached_items_count(JsonObject& object) {
			int64_t cached_count = 0;
			cached_count += object.contains("@id");
			for (auto& [key, val] : object) {
				cached_count += get_cached_items_count(val);
			}
			return cached_count;
		}
		int64_t SharedCache::get_cached_items_count(JsonArray& array) {
			int64_t cached_count = 0;
			for (auto& val : array) {
				cached_count += get_cached_items_count(val);
			}
			return cached_count;
		}
		int64_t SharedCache::get_cached_items_count(JsonValue& value) {
			if (value.is_object()) {
				return get_cached_items_count(value.get_object());
			}
			if (value.is_array()) {
				return get_cached_items_count(value.get_array());
			}
			return 0;
		}
	};

	CachingJsonObject::CachingJsonObject(JsonObject object) : _object(object), _shared_cache(object) {

	}

	CachingJsonValue CachingJsonObject::operator[](std::string_view key) {
		return CachingJsonValue(_object[key], _shared_cache);
	}

	CachingJsonObject::operator JsonObject& () {
		return _object;
	}

	CachingJsonObject::CachingJsonObject(JsonObject& object, const detail::SharedCache& cache) : _object(object), _shared_cache(cache) {
	}

	bool CachingJsonObject::is_caching_object() const {
		return _object.contains("@id");
	}
	int64_t CachingJsonObject::get_cache_id() {
		return _object["@id"].as_int64();
	}

	CachingJsonArray::CachingJsonArray(JsonArray array) : _array(array), _shared_cache(_array) {

	}

	CachingJsonValue CachingJsonArray::operator[](size_t index) {
		assert(index < _array.size());
		return CachingJsonValue(_array[index], _shared_cache);
	}

	size_t CachingJsonArray::size() {
		return _array.size();
	}

	CachingJsonArray::operator JsonArray& () {
		return _array;
	}

	CachingJsonArray::CachingJsonArray(JsonArray& array, const detail::SharedCache& cache) : _array(array), _shared_cache(cache) {
	}

	bool CachingJsonValue::is_null() const {
		return _value.is_null();
	}

	CachingJsonValue::CachingJsonValue(JsonValue value) : _value(value), _shared_cache(value) {

	}

	CachingJsonValue::CachingJsonValue(JsonValue& value, const detail::SharedCache& cache) : _value(value), _shared_cache(cache) {

	}

	CachingJsonObject CachingJsonValue::as_object() {
		return CachingJsonObject(_value.as_object(), _shared_cache);
	}

	CachingJsonArray CachingJsonValue::as_array() {
		return CachingJsonArray(_value.as_array(), _shared_cache);
	}

	int64_t CachingJsonValue::as_int64() {
		return _value.as_int64();
	}

	uint64_t CachingJsonValue::as_uint64() {
		return _value.as_uint64();
	}

	double CachingJsonValue::as_double() {
		return _value.as_double();
	}

	bool CachingJsonValue::as_bool() {
		return _value.as_bool();
	}

	detail::CachedObject& CachingJsonValue::as_cached_object() {
		return _shared_cache.get_item(as_int64());
	}
};