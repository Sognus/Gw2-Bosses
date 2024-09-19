#include "Time.h"

long SECONDS(long hours, long minutes, long seconds) {
	return hours * HOUR_TO_SEC + minutes * MIN_TO_SEC + seconds;
}


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

std::string format_countdown_time_minutes(int seconds) {
	if (seconds <= 0) {
		return "";
	}

	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int secs = seconds % 60;

	std::ostringstream oss;

	// Add hours only if it's greater than zero
	if (hours > 0) {
		oss << std::setfill('0') << std::setw(1) << hours << " hours";
		oss << " ";

	}

	// Add minutes only if it's greater than zero or hours are present
	if (minutes > 0 || hours > 0) {
		oss << std::setfill('0') << std::setw(1) << minutes << " minutes";
	}

	// If minutes and hours are zero, just print seconds
	if (minutes < 1 && hours < 1) {
		oss << std::setfill('0') << std::setw(2) << seconds << " seconds";
	}

	return oss.str();
}

std::string format_countdown_time(int seconds) {

	if (seconds <= 0) {
		return "";
	}

	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int secs = seconds % 60;

	std::ostringstream oss;

	// Add hours only if it's greater than zero
	if (hours > 0) {
		oss << std::setfill('0') << std::setw(2) << hours << ":";
	}

	// Add minutes only if it's greater than zero or hours are present
	if (minutes >= 0 || hours > 0) {
		oss << std::setfill('0') << std::setw(2) << minutes << ":";
	}

	// Add seconds
	oss << std::setfill('0') << std::setw(2) << secs;

	return oss.str();
}