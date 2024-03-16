#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "../Globals.h"
#include "../Event.h"
#include "../PeriodicEvent.h"
#include "BoundingBox.h"
#include "Map.h"
#include "Time.h"

#ifdef _WIN32
#define timegm _mkgmtime
#endif

// Suppress the C4244 warning for the entire file
#pragma warning(disable : 4244)

void render_debug_crosshair() {
	if (!MumbleLink->Context.IsMapOpen) return;

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 center = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 horizontalStart(center.x - DEBUG_LINE_LENGTH, center.y);
	ImVec2 horizontalEnd(center.x + DEBUG_LINE_LENGTH, center.y);
	drawList->AddLine(horizontalStart, horizontalEnd, RED, DEBUG_LINE_THICKNESS);

	ImVec2 verticalStart(center.x, center.y - DEBUG_LINE_LENGTH);
	ImVec2 verticalEnd(center.x, center.y + DEBUG_LINE_LENGTH);
	drawList->AddLine(verticalStart, verticalEnd, RED, DEBUG_LINE_THICKNESS);
}


void render_event_circle(ImVec2 location, float scale) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float radius = ENTRY_RADIUS * scale;
	drawList->AddCircleFilled(location, radius, GREEN, ENTRY_SEGMENTS);
}

void render_event_progress(ImVec2 location, float percentage, float scale) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float radius = ENTRY_RADIUS * scale;
	float totalAngle = (percentage / 100.0f) * (2.0f * M_PI);

	float angleStep = totalAngle / ENTRY_SEGMENTS;
	float startingAngle = ENTRY_ARC_OFFSET;

	for (int i = 0; i < ENTRY_SEGMENTS; ++i) {
		float startAngle = startingAngle + angleStep * i;
		float endAngle = startingAngle + angleStep * (i + 1);
		drawList->PathLineTo(location);
		drawList->PathArcTo(location, radius, startAngle, endAngle);
		drawList->PathFillConvex(RED);
	}

}

void render_event_name(std::string name, ImVec2 location, float scale) {
	float radius = ENTRY_RADIUS * scale;
	ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
	ImVec2 textPos = ImVec2(location.x - (textSize.x / 2), location.y + radius + (ENTRY_NAME_OFFSET * scale));
	ImGui::SetCursorPos(textPos);
	ImGui::Text(name.c_str());
}

void render_tooltip(Event event, ImVec2 location) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;
	float radius = ENTRY_RADIUS * event.GetScale();

	float distance = sqrt((mousePos.x - location.x) * (mousePos.x - location.x) + (mousePos.y - location.y) * (mousePos.y - location.y));
	if (distance <= radius) {
		ImGui::SetTooltip("Event %s\nPercentage: %.2f\nScale: %.2f", event.GetName().c_str(), event.GetPercentage(), event.GetScale());
	}
}

void render_base_event(Event event) {
	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);
	event.SetScale(map_object_scale());

	// Render circle and progress arc
	ImGui::SetCursorPos(location);
	render_event_circle(location, event.GetScale());
	render_event_progress(location, event.GetPercentage(), event.GetScale());

	// Render text
	render_event_name(event.GetName(), location, event.GetScale());

	// Render tooltip
	render_tooltip(event, location);
}

ImU32 hex_to_color(const std::string& hex) {
	if (hex.length() != 6) {
		// Invalid input length
		return -1;
	}

	std::string rStr = hex.substr(0, 2);
	std::string gStr = hex.substr(2, 2);
	std::string bStr = hex.substr(4, 2);

	// Convert hexadecimal strings to integers
	std::uint32_t R = std::stoul(rStr, nullptr, 16);
	std::uint32_t G = std::stoul(gStr, nullptr, 16);
	std::uint32_t B = std::stoul(bStr, nullptr, 16);

	// Create the IM_COL32 color
	return IM_COL32(R, G, B, 255);
}


std::string format_time(std::tm* time) {
	// Format the struct tm to HH:MM string
	char buffer[6]; // HH:MM + null terminator
	std::strftime(buffer, sizeof(buffer), "%H:%M", time);

	return buffer;
}


std::string calculate_tooltip_time(float offset_seconds) {
	// Define your time point (e.g., current time)
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// Convert the time point to a time_t
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	// Convert into latest 2h block
	double seconds_in_2h_block = 2 * 60 * 60; // 2 hours in seconds
	std::time_t aligned_time = floorf(time / seconds_in_2h_block) * seconds_in_2h_block;

	// Offset time 
	std::time_t offset_aligned_time = aligned_time + (long)(offset_seconds);

	// Convert the time_t to a struct tm (for local time)
	std::tm local_time;
	localtime_s(&local_time, &offset_aligned_time); // Using localtime_s for safety on Windows

	return format_time(&local_time);
}

// Time in seconds since last aligned block
long aligned_time_offset() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// Convert the time point to a time_t
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	// Convert into latest 2h block
	long seconds_in_2h_block = 2 * 60 * 60; // 2 hours in seconds
	std::time_t aligned_time = (time / seconds_in_2h_block) * seconds_in_2h_block;

	return time - aligned_time;
}

std::string time_now_formatted() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// Convert the time point to a time_t
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	// Convert the time_t to a struct tm (for local time)
	std::tm local_time;
	localtime_s(&local_time, &time); // Using localtime_s for safety on Windows

	return format_time(&local_time);
}


// DEPRECATED
void render_periodic_event(PeriodicEvent event) {

	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);

	float mapZoomScale = map_zoom_scale();
	float mapObjectScale = map_object_scale();

	float width = 200.0f * mapZoomScale; // Zoom in = more wide
	float height = 50.0f * mapObjectScale; // Zoom in = less tall

	float width_per_second = width / event.GetPeriodicitySeconds();

	const std::vector<json>& entries = event.GetPeriodicEntries();

	ImFont* originalFont = ImGui::GetFont();
	const float originalFontSize = originalFont->FontSize;

	// Location of top left of render
	ImVec2 topLeft = ImVec2(location.x - width / 2, location.y - height / 2);

	// Add filled rectangle
	drawList->AddRectFilled(
		topLeft,
		ImVec2(topLeft.x + width, topLeft.y + height),
		RED
	);

	for (const json& entry : entries) {
		float offset_seconds = entry["offset_seconds"];
		float offset_next = entry["offset_next"];
		float duration_seconds = entry["duration_seconds"];
		std::string hex_color = entry["color_hex"];

		if (offset_seconds < 0 || duration_seconds < 0) {
			continue;
		}

		ImU32 color = hex_to_color(hex_color);

		if (!color) {
			continue;
		}

		ImVec2 start = ImVec2(
			topLeft.x + width_per_second * offset_seconds,
			topLeft.y
		);
		ImVec2 end = ImVec2(
			start.x + width_per_second * duration_seconds,
			topLeft.y + height
		);

		// Filled rect
		drawList->AddRectFilled(
			start,
			end,
			color
		);
		// Outline
		drawList->AddRect(
			start,
			end,
			IM_COL32(192, 192, 192, 255) // silver outline
		);


		std::string name = entry["name"];

		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());


		if (textSize.x > (end.x - start.x) || textSize.y > (end.y - start.y)) {
			// Calculate minimum scale
			float scale_x = (end.x - start.x) / textSize.x;
			float scale_y = (end.y - start.y) / textSize.y;
			float min_scale = std::min(scale_x, scale_y);
			// Set font scale
			ImGui::SetWindowFontScale(min_scale);
			// Recalculate text size
			textSize = ImGui::CalcTextSize(name.c_str());
		}

		ImVec2 textPos = ImVec2(
			start.x + 5,
			start.y + (end.y - start.y) / 2 - textSize.y / 2);

		// Event Text
		ImGui::PushClipRect(start, end, true);
		ImGui::SetCursorPos(textPos);
		ImGui::Text(name.c_str());
		ImGui::PopClipRect();

		// Restore font scale
		ImGui::SetWindowFontScale(1.0f);

		// Tooltip
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 mousePos = io.MousePos;

		if (mousePos.x >= start.x && mousePos.x <= end.x &&
			mousePos.y >= start.y && mousePos.y <= end.y) {

			std::string time = calculate_tooltip_time(offset_seconds);
			std::string next = calculate_tooltip_time(offset_seconds + offset_next);
			ImGui::SetTooltip("%s\n\nstarts: %s\n\nnext: %s", name.c_str(), time.c_str(), next.c_str());
		}

	}

	// Draw Gold outline
	drawList->AddRect(
		topLeft,
		ImVec2(topLeft.x + width, topLeft.y + height),
		IM_COL32(212, 175, 55, 255)
	);

	// Draw current time
	long current_time_offset = aligned_time_offset();
	float current_time_position = current_time_offset / 7200.0f;

	ImVec2 indicator_top = ImVec2(topLeft.x + width * current_time_position, topLeft.y - 10 * mapObjectScale);

	drawList->AddLine(
		indicator_top,
		ImVec2(indicator_top.x, indicator_top.y + height + 10 * mapObjectScale),
		RED,
		1.5f
	);

	std::string current_time = time_now_formatted();
	ImVec2 textSize = ImGui::CalcTextSize(current_time.c_str());
	ImVec2 indicator_top_right = ImVec2(indicator_top.x + textSize.x + 10 * mapObjectScale, indicator_top.y - textSize.y - 10 * mapObjectScale);

	if (textSize.x > (indicator_top_right.x - indicator_top.x) || textSize.y > (indicator_top.y - indicator_top_right.y)) {
		// Calculate minimum scale
		float scale_x = (indicator_top_right.x - indicator_top.x) / textSize.x;
		float scale_y = (indicator_top.y - indicator_top_right.y) / textSize.y;
		float min_scale = std::min(scale_x, scale_y);
		// Set font scale
		ImGui::SetWindowFontScale(min_scale);
		// Recalculate text size
		textSize = ImGui::CalcTextSize(current_time.c_str());
	}

	drawList->AddRectFilled(
		indicator_top,
		indicator_top_right,
		RED
	);

	ImVec2 textPos = ImVec2(
		indicator_top.x + 3 * mapObjectScale,
		indicator_top_right.y + (indicator_top.y - indicator_top_right.y) / 2 - textSize.y / 2
	);
	ImGui::SetCursorPos(textPos);
	ImGui::Text(current_time.c_str());
	// Restore font scale
	ImGui::SetWindowFontScale(1.0f);
}


void render_ring(ImDrawList* drawList, ImVec2 center, float innerRadius, float outerRadius, ImU32 color, int numSegments) {
	for (int i = 0; i < numSegments; ++i) {
		float t0 = static_cast<float>(i) / static_cast<float>(numSegments);
		float t1 = static_cast<float>(i + 1) / static_cast<float>(numSegments);

		float angle0 = t0 * 2 * M_PI;
		float angle1 = t1 * 2 * M_PI;

		ImVec2 p0(center.x + innerRadius * cosf(angle0), center.y + innerRadius * sinf(angle0));
		ImVec2 p1(center.x + innerRadius * cosf(angle1), center.y + innerRadius * sinf(angle1));
		ImVec2 p2(center.x + outerRadius * cosf(angle1), center.y + outerRadius * sinf(angle1));
		ImVec2 p3(center.x + outerRadius * cosf(angle0), center.y + outerRadius * sinf(angle0));

		drawList->AddQuad(p0, p1, p2, p3, color);
	}
}

bool is_point_inside_arc(ImVec2 point, ImVec2 center, float radius, float startAngle, float endAngle) {
	float angle = atan2f(point.y - center.y, point.x - center.x);

	// Normalize the angle to be between 0 and 2π
	while (angle < 0) {
		angle += 2 * M_PI;
	}
	while (angle >= 2 * M_PI) {
		angle -= 2 * M_PI;
	}

	// Normalize start and end angles to be between 0 and 2π
	while (startAngle < 0) {
		startAngle += 2 * M_PI;
	}
	while (startAngle >= 2 * M_PI) {
		startAngle -= 2 * M_PI;
	}
	while (endAngle < 0) {
		endAngle += 2 * M_PI;
	}
	while (endAngle >= 2 * M_PI) {
		endAngle -= 2 * M_PI;
	}

	if (startAngle <= endAngle) {
		return (angle >= startAngle && angle <= endAngle) &&
			(sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y)) <= radius);
	}
	else {
		return (angle >= startAngle || angle <= endAngle) &&
			(sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y)) <= radius);
	}
}

void render_periodic_circular_event(PeriodicEvent event) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;

	BoundingBox screen = BoundingBox(0, 0, io.DisplaySize.y, io.DisplaySize.x);
	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);

	float mapZoomScale = map_zoom_scale();
	float mapObjectScale = map_object_scale();

	float size = 50.0f * mapObjectScale;

	// CHECK IF EVENT IS OUTSIDE VIEWPORT
	BoundingBox eventBox = BoundingBox(location, size, true);

	if (!screen.Overlaps(eventBox)) {
		return;
	}

	// Draw base circle
	render_ring(
		drawList,
		location,						// center
		size,							// inner radius
		size + 1.0f,					// outer radius
		IM_COL32(212, 175, 55, 255),	// color
		ENTRY_SEGMENTS);				// segments 

	const std::vector<json>& entries = event.GetPeriodicEntries();
	int current_entry_index = -1;
	int next_entry_index = -1;

	// Draw arcs
	for (int i = 0; i < entries.size(); ++i) {
		const json& entry = entries[i];

		float offset_seconds = entry["offset_seconds"];
		float offset_next = entry["offset_next"];
		float duration_seconds = entry["duration_seconds"];
		std::string name = entry["name"];
		std::string description = entry["description"];

		std::string hex_color = entry["color_hex"];
		ImU32 color = hex_to_color(hex_color);

		// Size of offset 
		float offset = (offset_seconds / event.GetPeriodicitySeconds()) * 100; // %
		float offset_angle_radians = (offset / 100.f) * (2.0f * M_PI); // rad

		// Size of arc 
		float percentage = (duration_seconds / event.GetPeriodicitySeconds()) * 100; // %
		float totalAngle = (percentage / 100.f) * (2.0f * M_PI); // rad

		float angleStep = totalAngle / ENTRY_SEGMENTS;
		float startingAngle = ENTRY_ARC_OFFSET + offset_angle_radians;

		for (int i = 0; i < ENTRY_SEGMENTS; ++i) {
			float startAngle = startingAngle + angleStep * i;
			float endAngle = startingAngle + angleStep * (i + 1);
			drawList->PathLineTo(location);
			drawList->PathArcTo(location, size, startAngle, endAngle);
			drawList->PathFillConvex(color);
			drawList->PathClear();
		}

		if (is_point_inside_arc(mousePos, location, size, startingAngle, startingAngle + totalAngle)) {
			std::string time = calculate_tooltip_time(offset_seconds);
			std::string next = calculate_tooltip_time(offset_seconds + offset_next);
			ImGui::SetTooltip("%s\n\nstarts: %s\n\nnext: %s", description.c_str(), time.c_str(), next.c_str());
		}

		// Set current entry
		float startTime = offset_seconds;
		float endTime = offset_seconds + duration_seconds;
		float alignedOffset = aligned_time_offset();
		if (alignedOffset >= startTime && alignedOffset <= endTime) {
			current_entry_index = i;
			next_entry_index = (i + 1) % entries.size();
		}

	}

	if (current_entry_index < 0 || next_entry_index < 0) {
		current_entry_index = 0;
		next_entry_index = 0;
	}

	json current_entry = entries[current_entry_index];
	json next_entry = entries[next_entry_index];

	// Special case when next entry is split
	int startIndex = next_entry_index;
	while (current_entry["description"] == next_entry["description"]) {
		next_entry_index = (next_entry_index + 1) % entries.size();
		next_entry = entries[next_entry_index];

		// Just to be sure
		if (next_entry_index == startIndex) {
			break;
		}
	}

	// Render current time line
	float aligned_time = aligned_time_offset();
	float aligned_time_percentage = aligned_time / 7200.0f;
	float angle = aligned_time_percentage * (2 * M_PI);
	angle = angle - (M_PI / 2);

	ImVec2 endPoint(location.x + size * cosf(angle), location.y + size * sinf(angle));
	drawList->AddLine(location, endPoint, RED, 1.5f);

	// Time background + text
	std::string current_time = time_now_formatted();
	ImVec2 textSize = ImGui::CalcTextSize(current_time.c_str());

	float margin = 5.0f * mapObjectScale;
	float padding = 3.0f * mapObjectScale;
	ImVec2 textTopLeft = ImVec2(
		location.x - textSize.x / 2 - padding / 2,
		location.y - size - margin - textSize.y - padding / 2
	);
	ImVec2 textBottomRight = ImVec2(
		textTopLeft.x + padding + textSize.x,
		textTopLeft.y + padding + textSize.y
	);

	drawList->AddRectFilled(
		textTopLeft,
		textBottomRight,
		RED
	);

	ImVec2 textLocation = ImVec2(
		location.x - textSize.x / 2,
		location.y - size - margin - textSize.y
	);
	ImGui::SetCursorPos(textLocation);
	ImGui::Text(current_time.c_str());

	// Event name + info
	if (MumbleLink->Context.Compass.Scale <= ENTRY_MAX_ZOOM_TEXT_VISIBILITY) {
		char buffer[1024];
		std::string currentEntryDesc = current_entry == nullptr ? "?" : current_entry["description"];
		std::string nextEntryDesc = next_entry == nullptr ? "?" : next_entry["description"];
		snprintf(buffer, sizeof(buffer),
			"%s - %s\n\nNext: %s",
			event.GetName().c_str(),
			currentEntryDesc.c_str(),
			nextEntryDesc.c_str()
		);
		ImVec2 descTextSize = ImGui::CalcTextSize(buffer);
		ImVec2 descTextPosition = ImVec2(
			location.x - descTextSize.x / 2,
			location.y + size + margin
		);
		ImGui::SetCursorPos(descTextPosition);
		ImGui::Text(buffer);
	}

}


float calculate_distance(const ImVec2& point1, const ImVec2& point2) {
	float dx = point2.x - point1.x;
	float dy = point2.y - point1.y;
	return std::sqrt(dx * dx + dy * dy);
}


std::chrono::system_clock::time_point from_offset(auto duration) {
	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::system_clock::to_time_t(now);

	std::tm tm_struct;
	if (gmtime_s(&tm_struct, &time) != 0) {
		// Handle error
		throw std::runtime_error("Error converting time");
	}

	tm_struct.tm_hour = 0;
	tm_struct.tm_min = 0;
	tm_struct.tm_sec = 0;

	// Use _mkgmtime to convert to UTC
	const auto midnight_utc = std::chrono::system_clock::from_time_t(timegm(&tm_struct));
	return midnight_utc + duration;
}

std::string calculate_tooltip_time_absolute(long seconds_since_midnight) {
	auto duration = std::chrono::seconds(seconds_since_midnight);
	auto timestamp = from_offset(duration);

	std::ostringstream oss;
	oss << std::format("{:%R}", std::chrono::current_zone()->to_local(timestamp));

	return oss.str();
}



void render_periodic_circular_event_convergences(PeriodicEvent event) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;

	BoundingBox screen = BoundingBox(0, 0, io.DisplaySize.y, io.DisplaySize.x);
	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);

	float mapZoomScale = map_zoom_scale();
	float mapObjectScale = map_object_scale();

	float size = 50.0f * mapObjectScale;

	// CHECK IF EVENT IS OUTSIDE VIEWPORT
	BoundingBox eventBox = BoundingBox(location, size, true);

	if (!screen.Overlaps(eventBox)) {
		return;
	}

	// Draw base circle
	render_ring(
		drawList,
		location,						// center
		size,							// inner radius
		size + 1.0f,					// outer radius
		IM_COL32(212, 175, 55, 255),	// color
		ENTRY_SEGMENTS);				// segments 

	std::string event_hex_color = event.GetColorHex();
	ImU32 event_color = hex_to_color(event_hex_color);
	drawList->AddCircleFilled(location, size, event_color, ENTRY_SEGMENTS);

	// Imgui tooltip
	if (calculate_distance(location, io.MousePos) < size) {
		ImGui::SetTooltip("No convergence");
	}

	const std::vector<json>& entries = event.GetPeriodicEntries();
	int current_entry_index = -1;
	int next_entry_index = -1;

	// Draw arcs
	for (int i = 0; i < entries.size(); ++i) {
		const json& entry = entries[i];

		float offset_seconds = entry["offset_seconds"];
		float offset_next = entry["offset_next"];
		float duration_seconds = entry["duration_seconds"];
		std::string name = entry["name"];
		std::string description = entry["description"];

		std::string hex_color = entry["color_hex"];
		ImU32 color = hex_to_color(hex_color);

		// Determine if convergence is in current block
		long periodicity = entry.find("periocitity_override") == entry.end() ? 7200L : long(entry["periocitity_override"]);
		long current_time_offset = get_time_since_midnight();
		long block_2h = floor(current_time_offset / 7200.0f);
		long block_periodicity = floor(current_time_offset / periodicity);

		long block_periodicity_start = block_periodicity * periodicity;
		long periodicity_absolute = block_periodicity_start + offset_seconds;

		long which_2h_block = floor(periodicity_absolute / 7200.0f);
		bool happening = block_2h == which_2h_block;

		if (!happening) {
			continue; // Convergences are not happening in this block
		}

		long offset_2h = periodicity_absolute - (which_2h_block * 7200L);

		// Size of offset 
		float offset = (offset_2h / 7200.0F) * 100; // %
		float offset_angle_radians = (offset / 100.f) * (2.0f * M_PI); // rad

		// Size of arc 
		float percentage = (duration_seconds / 7200.0F) * 100; // %
		float totalAngle = (percentage / 100.f) * (2.0f * M_PI); // rad

		float angleStep = totalAngle / ENTRY_SEGMENTS;
		float startingAngle = ENTRY_ARC_OFFSET + offset_angle_radians;

		for (int i = 0; i < ENTRY_SEGMENTS; ++i) {
			float startAngle = startingAngle + angleStep * i;
			float endAngle = startingAngle + angleStep * (i + 1);
			drawList->PathLineTo(location);
			drawList->PathArcTo(location, size, startAngle, endAngle);
			drawList->PathFillConvex(color);
			drawList->PathClear();
		}

		if (is_point_inside_arc(mousePos, location, size, startingAngle, startingAngle + totalAngle)) {
			std::string time = calculate_tooltip_time_absolute(periodicity_absolute);
			std::string next = calculate_tooltip_time_absolute(periodicity_absolute + periodicity);
			ImGui::SetTooltip("%s\n\nstarts: %s\n\nnext: %s", description.c_str(), time.c_str(), next.c_str());
		}

		// Set current entry
		float startTime = offset_seconds;
		float endTime = offset_seconds + duration_seconds;
		float alignedOffset = aligned_time_offset();
		if (alignedOffset >= startTime && alignedOffset <= endTime) {
			current_entry_index = i;
			next_entry_index = (i + 1) % entries.size();
		}

	}

	if (current_entry_index < 0 || next_entry_index < 0) {
		current_entry_index = 0;
		next_entry_index = 0;
	}

	json current_entry = entries[current_entry_index];
	json next_entry = entries[next_entry_index];

	// Special case when next entry is split
	int startIndex = next_entry_index;
	while (current_entry["description"] == next_entry["description"]) {
		next_entry_index = (next_entry_index + 1) % entries.size();
		next_entry = entries[next_entry_index];

		// Just to be sure
		if (next_entry_index == startIndex) {
			break;
		}
	}

	// Render current time line
	float aligned_time = aligned_time_offset();
	float aligned_time_percentage = aligned_time / 7200.0f;
	float angle = aligned_time_percentage * (2 * M_PI);
	angle = angle - (M_PI / 2);

	ImVec2 endPoint(location.x + size * cosf(angle), location.y + size * sinf(angle));
	drawList->AddLine(location, endPoint, RED, 1.5f);

	// Time background + text
	std::string current_time = time_now_formatted();
	ImVec2 textSize = ImGui::CalcTextSize(current_time.c_str());

	float margin = 5.0f * mapObjectScale;
	float padding = 3.0f * mapObjectScale;
	ImVec2 textTopLeft = ImVec2(
		location.x - textSize.x / 2 - padding / 2,
		location.y - size - margin - textSize.y - padding / 2
	);
	ImVec2 textBottomRight = ImVec2(
		textTopLeft.x + padding + textSize.x,
		textTopLeft.y + padding + textSize.y
	);

	drawList->AddRectFilled(
		textTopLeft,
		textBottomRight,
		RED
	);

	ImVec2 textLocation = ImVec2(
		location.x - textSize.x / 2,
		location.y - size - margin - textSize.y
	);
	ImGui::SetCursorPos(textLocation);
	ImGui::Text(current_time.c_str());

	// Event name + info
	if (MumbleLink->Context.Compass.Scale <= ENTRY_MAX_ZOOM_TEXT_VISIBILITY) {
		char buffer[1024];
		std::string currentEntryDesc = current_entry == nullptr ? "?" : current_entry["description"];
		std::string nextEntryDesc = next_entry == nullptr ? "?" : next_entry["description"];

		if (event.GetName().compare(currentEntryDesc) == 0) {
			snprintf(buffer, sizeof(buffer),
				"%s\n\nNext: %s",
				currentEntryDesc.c_str(),
				nextEntryDesc.c_str()
			);
		}
		else {
			snprintf(buffer, sizeof(buffer),
				"%s - %s\n\nNext: %s",
				event.GetName().c_str(),
				currentEntryDesc.c_str(),
				nextEntryDesc.c_str()
			);
		}


		ImVec2 descTextSize = ImGui::CalcTextSize(buffer);
		ImVec2 descTextPosition = ImVec2(
			location.x - descTextSize.x / 2,
			location.y + size + margin
		);
		ImGui::SetCursorPos(descTextPosition);
		ImGui::Text(buffer);
	}

}

void render_map_notification(Event* notificationEvent, Texture* texture) {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;

	BoundingBox screen = BoundingBox(0, 0, io.DisplaySize.y, io.DisplaySize.x);
	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(notificationEvent->GetLocation(), viewport, mapScaleX);

	float mapZoomScale = map_zoom_scale();
	float mapObjectScale = map_object_scale();

	float size = 100.0f * mapObjectScale;

	// CHECK IF EVENT IS OUTSIDE VIEWPORT
	BoundingBox eventBox = BoundingBox(location, size, true);

	if (!screen.Overlaps(eventBox)) {
		return;
	}

	ImVec2 cursorStack = ImGui::GetCursorPos();
	ImVec2 renderLocation = ImVec2(location.x - size / 2, location.y - size / 2);
	ImGui::SetCursorPos(renderLocation);
	ImGui::Image(texture->Resource, ImVec2(size, size));
	ImGui::SetCursorPos(cursorStack);



	if (eventBox.OverlapsVector(mousePos)) {
		std::string formattedTooltip = "";

		std::string eventName = notificationEvent->GetName();
		size_t spacePos = eventName.find_last_of(" ");
		std::string name;

		if (spacePos != std::string::npos) {
			name = eventName.substr(0, spacePos);
		}
		else {
			name = eventName;
		}

		if (notificationEvent->GetEventType().starts_with("core_world_bosses")) {
			CoreWorldbossEvent* coreEvent = static_cast<CoreWorldbossEvent*>(notificationEvent);
			long current_time = get_time_since_midnight();
			int time = coreEvent->GetMidnightOffsetSeconds() - current_time;
			std::string bracketText = time < 0 ? "active" : "in";
			time = abs(time);
			formattedTooltip = name + " (" + bracketText + " " + format_countdown_time(time) + ")";
		}
		else {
			formattedTooltip = name;

		}

		ImGui::SetTooltip(formattedTooltip.c_str());

	}

}

void render_map_notification_upcoming(Event* notificationEvent)
{
	if (notificationEvent->GetEventType().starts_with("core_world_bosses")) {
		CoreWorldbossEvent* coreEvent = static_cast<CoreWorldbossEvent*>(notificationEvent);
		long current_time = get_time_since_midnight();

		int timeUntil = (coreEvent->GetMidnightOffsetSeconds()) - current_time;


		Texture* texture =
			(resource_textures.find(GW2BOSSES_RESOURCE_COREWORLDBOSSES_UPCOMING) != resource_textures.end()) ?
			resource_textures[GW2BOSSES_RESOURCE_COREWORLDBOSSES_UPCOMING] :
			nullptr;

		if (timeUntil > SECONDS(0, 15, 0)) {
			texture = (resource_textures.find(GW2BOSSES_RESOURCE_COREWORLDBOSSES_GRAY) != resource_textures.end()) ?
				resource_textures[GW2BOSSES_RESOURCE_COREWORLDBOSSES_GRAY] :
				nullptr;
		}



		render_map_notification(notificationEvent, texture);
	}
}

void render_map_notification_in_progress(Event* notificationEvent)
{
	Texture* texture =
		(resource_textures.find(GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_PROGRESS) != resource_textures.end()) ?
		resource_textures[GW2BOSSES_RESOURCE_COREWORLDBOSSES_IN_PROGRESS] :
		nullptr;
	render_map_notification(notificationEvent, texture);
}



// Re-enable warning
#pragma warning(default : 4244)