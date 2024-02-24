#include "Strings.h"

std::string _preferred_separator_u8_cache;

const std::string preferred_separator_char() {
    if (_preferred_separator_u8_cache.empty()) {
        const wchar_t preferredSeparator = fs::path::preferred_separator;

        // Buffer
        char narrowChar;
            
        // Convert wchar to 
        if (wctomb(&narrowChar, preferredSeparator) == 0) {
            _preferred_separator_u8_cache = narrowChar;
        }
        else {
            // Fallback - dont really want to store fallback value
            return "/";
        }
    }
    return _preferred_separator_u8_cache;
}