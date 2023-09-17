#ifndef IMGUI_EXTENSIONS_H
#define IMGUI_EXTENSIONS_H

#include "imgui.h"

namespace ImGui
{
    static void TextOutlined(const char* fmt, ...)
    {
        ImVec2 pos = GetCursorPos();

        va_list args;
        va_start(args, fmt);
        pos.x += 1;
        pos.y += 1;
        SetCursorPos(pos);
        TextColoredV(ImVec4(0, 0, 0, 255), fmt, args);
        pos.x -= 1;
        pos.y -= 1;
        SetCursorPos(pos);
        TextV(fmt, args);
        va_end(args);
    }

    static void TextOutlinedT(int transparency, const char* fmt, ...)
    {
        ImVec2 pos = GetCursorPos();

        va_list args;
        va_start(args, fmt);
        pos.x += 1;
        pos.y += 1;
        SetCursorPos(pos);
        PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, transparency));
        TextV(fmt, args);
        PopStyleColor();
        pos.x -= 1;
        pos.y -= 1;
        SetCursorPos(pos);
        TextV(fmt, args);
        va_end(args);
    }
}

#endif