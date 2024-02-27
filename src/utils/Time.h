#include <iostream>
#include <chrono>

#ifndef GW2_BOSSES_TIME
#define GW2_BOSSES_TIME

const long TIME_CLAMP_MIDNIGHT_LOWER = 0;
const long TIME_CLAMP_MIDNIGHT_UPPER = 86399;

long get_time_since_midnight();

#endif // GW2_BOSSES_TIME