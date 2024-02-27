#include "Time.h"

long get_time_since_midnight() {
	// Get current time in UTC
	auto now = std::chrono::system_clock::now();

	// Convert current time to time_t
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// Create a tm struct to store the time information
	std::tm timeInfo;

	// Use gmtime_s to convert time_t to tm struct in UTC
	if (gmtime_s(&timeInfo, &currentTime) != 0) {
		// print error
		std::cerr << "Error in gmtime_s" << std::endl;
		return -1;
	}

	// Calculate number of seconds since midnight
	long secondsSinceMidnight = timeInfo.tm_hour * 3600 + timeInfo.tm_min * 60 + timeInfo.tm_sec;

	return secondsSinceMidnight;
}