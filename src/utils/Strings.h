#include <string>
#include <filesystem>
#include <Windows.h>

#ifndef BOSSES_STRINGS_H
#define BOSSES_STRINGS_H

namespace fs = std::filesystem;

extern std::string _preferred_separator_u8_cache;

const std::string preferred_separator_char();

#endif // BOSSES_STRINGS_H