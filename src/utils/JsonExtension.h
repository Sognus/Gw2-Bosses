#include "../nlohmann/json.hpp"

#pragma once
#ifndef JSON_EXTENSION_GUARD_H
#define JSON_EXTENSION_GUARD_H

using json = nlohmann::json;

template<typename T>
T json_getOrElse(const json& j, const std::string& key, const T& defaultValue) {
    if (j.find(key) != j.end()) {
        try {
            return j[key].get<T>();
        }
        catch (json::type_error&) {;
        return defaultValue;
        }

    }
    else {
        return defaultValue;
    }
}

#endif // !JSON_EXTENSION_GUARD_H
