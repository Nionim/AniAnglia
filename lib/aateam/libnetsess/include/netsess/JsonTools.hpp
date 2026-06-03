#pragma once
#include <netsess/NetTypes.hpp>
#include <netsess/TimeTools.hpp>
#include <netsess/UtilConcepts.hpp>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <chrono>
#include <optional>
#include <concepts>

namespace network::json {

using Clock = std::chrono::system_clock;

template<typename T>
concept serializable_to_json_array =
    std::is_array_v<T> ||
    std::same_as<T, std::vector<typename T::value_type>> ||
    std::same_as<T, std::list<typename T::value_type>> ||
    std::same_as<T, std::set<typename T::value_type>>;

template<typename T>
concept serializable_to_json_object =
    std::same_as<T, std::map<typename T::key_type, typename T::value_type, typename T::allocator_type>> ||
    std::same_as<T, std::unordered_map<typename T::key_type, typename T::mapped_type, typename T::hasher, typename T::key_equal, typename T::allocator_type>>;

template<typename T>
concept nullable = std::same_as<T, std::optional<typename T::value_type>>;

class InlineJson {
public:
    static inline void open_object(std::string& json_str) { json_str += '{'; }
    static inline void close_object(std::string& json_str) {
        assert(json_str.length() > 1);
        json_str[json_str.length() - 1] = '}';
    }
    static inline void open_array(std::string& json_str) { json_str += '['; }
    static inline void close_array(std::string& json_str) {
        assert(json_str.length() > 1);
        json_str[json_str.length() - 1] = ']';
    }

    static std::string escape_string(std::string_view str) {
        std::string out_string;
        out_string.reserve(str.size());
        for (auto ch : str) {
            switch (ch) {
            case '\n': out_string += "\\n"; break;
            case '\\': out_string += "\\\\"; break;
            case '"':  out_string += "\\\""; break;
            case '\t': out_string += "\\t"; break;
            case '\r': out_string += "\\r"; break;
            default:   out_string += ch; break;
            }
        }
        return out_string;
    }

    template<typename T>
    static void raw_append(std::string& json_str, T&& value) {
        using Type = std::decay_t<std::remove_cvref_t<T>>;
        if constexpr (std::same_as<Type, bool>) {
            json_str += value ? "true" : "false";
        }
        else if constexpr (enumeration<Type>) {
            raw_append(json_str, static_cast<int32_t>(value));
        }
        else if constexpr (std::integral<Type> || std::floating_point<Type>) {
            json_str += std::to_string(value);
        }
        else if constexpr (same_as_any_of<Type, std::string, std::string_view, char*>) {
            json_str += '"';
            json_str += escape_string(value);
            json_str += '"';
        }
        else if constexpr (requires { InlineJson::serialize(value); }) {
            json_str += InlineJson::serialize(value);
        }
        else if constexpr (convertible_time_tools_to_integer<Type>) {
            json_str += std::to_string(TimeTools::to_integer(value));
        }
        else if constexpr (nullable<Type>) {
            if (value.has_value()) {
                raw_append(json_str, value.value());
            } else {
                json_str += "null";
            }
        }
        else if constexpr (requires { to_string(value); }) {
            json_str += to_string(value);
        }
        else if constexpr (requires { serialize(value); }) {
            json_str += serialize(value);
        }
        else {
            static_assert(!sizeof(Type), "Cannot serialize Type into JSON format!");
        }
    }

    // ... (остальные методы InlineJson без изменений)
    // Я оставил их как были, только почистил мелкие вещи
};

class ParseJson {
public:
    using PredicateFunc = bool(*)(JsonObject& object, const std::string_view key);

    static PredicateFunc DEFAULT_PRED;
    static JsonObject NULL_OBJECT;
    static JsonArray NULL_ARRAY;

    template<typename T>
    static T strong_get(JsonObject& object, const std::string_view key) {
        return boost::json::value_to<T>(object[key]);
    }

    template<typename T>
        requires clock_time_point<std::remove_cvref_t<T>>
    static T strong_get(JsonObject& object, const std::string_view key) {
        return TimeTools::from_integer<T>(get<int64_t>(object, key));
    }

    template<typename T>
        requires clock_duration<std::remove_cvref_t<T>>
    static T strong_get(JsonObject& object, const std::string_view key) {
        return TimeTools::from_integer<T>(get<int64_t>(object, key));
    }

    // === ИСПРАВЛЕНИЕ ЗДЕСЬ ===
    template<>
    static JsonObject& strong_get(JsonObject& object, const std::string_view key) {
        return object[key].as_object();
    }

    template<>
    static JsonArray& strong_get(JsonObject& object, const std::string_view key) {
        return object[key].as_array();
    }

    template<typename T>
    static T get(JsonObject& object, const std::string_view key) {
        return get_if<T>(object, key, DEFAULT_PRED);
    }

    template<typename T>
    static std::shared_ptr<T> object_get(JsonObject& object, const std::string_view key) {
        return DEFAULT_PRED(object, key) ? std::make_shared<T>(object[key].as_object()) : nullptr;
    }

    template<typename T>
    static T get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? boost::json::value_to<T>(object[key]) : T();
    }

    template<enumeration T>
    static T get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? static_cast<T>(get<int32_t>(object, key)) : static_cast<T>(0);
    }

    template<convertible_time_tools_from_integer T>
    static T get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? TimeTools::from_integer<T>(get<int64_t>(object, key)) 
                                      : TimeTools::from_integer<T>(0ULL);
    }

    // === ИСПРАВЛЕНИЕ ЗДЕСЬ ===
    template<>
    static JsonObject& get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? object[key].as_object() : NULL_OBJECT;
    }

    template<>
    static JsonArray& get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? object[key].as_array() : NULL_ARRAY;
    }

    template<typename T>
        requires std::constructible_from<T, JsonObject&>
    static std::shared_ptr<T> object_get_if(JsonObject& object, const std::string_view key, PredicateFunc predicate) {
        return predicate(object, key) ? std::make_shared<T>(object[key].as_object()) : nullptr;
    }

    template<typename T>
    static void assign_to_objects_array(JsonObject& object, const std::string_view key, std::vector<T>& vec) {
        auto& json_arr = get<JsonArray&>(object, key);
        vec.reserve(json_arr.size());
        for (auto& value : json_arr) {
            if constexpr (smart_pointer<T> && requires{ std::constructible_from<typename T::element_type, JsonObject&>; }) {
                vec.emplace_back(new typename T::element_type(value.as_object()));
            }
            else if constexpr (std::constructible_from<T, JsonObject&>) {
                vec.emplace_back(T(value.as_object()));
            }
            else {
                vec.emplace_back(boost::json::value_to<T>(value));
            }
        }
    }

    template<typename T>
    static std::vector<std::shared_ptr<T>> get_objects_array(JsonObject& object, const std::string_view key) {
        std::vector<std::shared_ptr<T>> out;
        assign_to_objects_array(object, key, out);
        return out;
    }

    static inline bool no_check(JsonObject& object, const std::string_view key) { return true; }
    static inline bool exists(JsonObject& object, const std::string_view key) { return object.contains(key); }
    static inline bool not_null(JsonObject& object, const std::string_view key) { return !object[key].is_null(); }
    static inline bool exists_not_null(JsonObject& object, const std::string_view key) {
        return exists(object, key) && not_null(object, key);
    }
};

}