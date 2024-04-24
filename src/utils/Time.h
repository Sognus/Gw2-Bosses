#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

#ifndef GW2_BOSSES_TIME
#define GW2_BOSSES_TIME

const long TIME_CLAMP_MIDNIGHT_LOWER = 0;
const long TIME_CLAMP_MIDNIGHT_UPPER = 86399;

const long HOUR_TO_SEC = 3600;
const long MIN_TO_SEC = 60;

long SECONDS(long hours = 0, long minutes = 0, long seconds = 0);

long get_time_since_midnight();

std::string format_countdown_time(int seconds);

std::string format_countdown_time_minutes(int seconds);

#endif // GW2_BOSSES_TIME