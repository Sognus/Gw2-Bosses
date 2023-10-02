#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "../Globals.h"
#include "../Event.h"
#include "../PeriodicEvent.h"
#include "BoundingBox.h"
#include "Map.h"

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


void render_periodic_event(PeriodicEvent event) {

	BoundingBox viewport = map_get_bounding_box();

	// Calculate scaling factors for X and Y axes;
	ImVec2 mapScaleX = map_get_scale();

	// Update necessary data for render
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 location = map_coords_to_pixels(event.GetLocation(), viewport, mapScaleX);

	float mapZoomScale = map_zoom_scale();
	float mapObjectScale = map_object_scale();

	float width = 200 * mapZoomScale; // Zoom in = more wide
	float height = 50 * mapObjectScale; // Zoom in = less tall

	float width_per_second = width / event.GetPeriodicitySeconds();

	const std::vector<json>& entries = event.GetPeriodicEntries();

	ImFont* originalFont = ImGui::GetFont();
	const float originalFontSize = originalFont->FontSize;

	// Location of top left of render
	ImVec2 topLeft = ImVec2(location.x - width /2, location.y - height / 2);

	// Add filled rectangle
	drawList->AddRectFilled(
		topLeft,
		ImVec2(topLeft.x + width, topLeft.y + height),
		RED
	);

	for (const json& entry : entries) {
		float offset_seconds = entry["offset_seconds"];
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

		
		ImFont* font = ImGui::GetFont();
		font->FontSize = originalFontSize * map_font_scale();
		ImGui::PushFont(font);

		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
		ImVec2 textPos = ImVec2(
			start.x + 5,
			start.y + height / 2 - textSize.y / 2);

		// Event Text
		ImGui::PushClipRect(start, end, true);
		ImGui::SetCursorPos(textPos);
		ImGui::Text(name.c_str());
		ImGui::PopClipRect();
		ImGui::PopFont();

		// Reset text 
		ImFont* fontReset = ImGui::GetFont();
		font->FontSize = originalFontSize;
		ImGui::PushFont(fontReset);
		ImGui::PopFont();

		// Tooltip
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 mousePos = io.MousePos;

		if (mousePos.x >= start.x && mousePos.x <= end.x &&
			mousePos.y >= start.y && mousePos.y <= end.y) {
			
			std::string time = calculate_tooltip_time(offset_seconds);
			std::string next = calculate_tooltip_time(offset_seconds + 7200);
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
	
	drawList->AddRectFilled(
		indicator_top,
		indicator_top_right,
		RED
	);

	ImVec2 textPos = ImVec2(
		indicator_top.x + 3 * mapObjectScale,
		indicator_top_right.y + (textSize.y / 2) + ((10 * mapObjectScale) / 4)
	);
	ImGui::SetCursorPos(textPos);
	ImGui::Text(current_time.c_str());




}