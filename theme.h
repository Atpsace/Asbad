#pragma once
#include "imgui.h"

namespace Theme {
    struct Palette {
        ImVec4 WindowBg{0.08f, 0.09f, 0.12f, 0.95f};
        ImVec4 CardBorder{0.2f, 0.22f, 0.28f, 0.6f};
        ImVec4 TextPrimary{0.9f, 0.9f, 0.95f, 1.0f};
        ImVec4 TextSecondary{0.6f, 0.6f, 0.65f, 1.0f};
    };

    inline Palette darkPal;
    inline Palette lightPal{ImVec4(0.95f, 0.95f, 0.97f, 0.95f), ImVec4(0.8f, 0.8f, 0.85f, 0.6f), ImVec4(0.1f, 0.1f, 0.15f, 1.0f), ImVec4(0.4f, 0.4f, 0.45f, 1.0f)};

    inline const Palette& Get(bool is_dark) {
        return is_dark ? darkPal : lightPal;
    }

    inline ImU32 ToU32(const ImVec4& col) {
        return ImGui::ColorConvertFloat4ToU32(col);
    }

    inline void SetupImGuiStyle(bool is_dark, float opacity = 0.95f) {
        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowRounding = 14.0f;
        s.ChildRounding = 10.0f;
        s.FrameRounding = 7.0f;
        s.PopupRounding = 10.0f;
        s.ScrollbarRounding = 9.0f;
        s.GrabRounding = 6.0f;
        s.TabRounding = 6.0f;

        s.Alpha = ImClamp(opacity, 0.20f, 1.0f);

        const auto& pal = Get(is_dark);
        ImVec4* colors = s.Colors;
        if (is_dark) {
            colors[ImGuiCol_Text]                  = ImVec4(0.92f, 0.92f, 0.96f, 1.00f);
            colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.52f, 0.60f, 1.00f);
            colors[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.09f, 0.12f, 0.95f);
            colors[ImGuiCol_ChildBg]               = ImVec4(0.11f, 0.12f, 0.16f, 0.70f);
            colors[ImGuiCol_PopupBg]               = ImVec4(0.10f, 0.11f, 0.15f, 0.98f);
            colors[ImGuiCol_Border]                = ImVec4(0.20f, 0.22f, 0.30f, 0.60f);
            colors[ImGuiCol_FrameBg]               = ImVec4(0.14f, 0.15f, 0.20f, 0.80f);
            colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.20f, 0.22f, 0.30f, 0.90f);
            colors[ImGuiCol_FrameBgActive]         = ImVec4(0.24f, 0.26f, 0.36f, 1.00f);
            colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.09f, 0.12f, 1.00f);
            colors[ImGuiCol_TitleBgActive]         = ImVec4(0.10f, 0.11f, 0.16f, 1.00f);
            colors[ImGuiCol_Button]                = ImVec4(0.18f, 0.19f, 0.26f, 0.85f);
            colors[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 0.27f, 0.38f, 1.00f);
            colors[ImGuiCol_ButtonActive]          = ImVec4(0.32f, 0.35f, 0.48f, 1.00f);
            colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.22f, 0.32f, 0.75f);
            colors[ImGuiCol_HeaderHovered]         = ImVec4(0.28f, 0.30f, 0.42f, 0.90f);
            colors[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.38f, 0.52f, 1.00f);
            colors[ImGuiCol_Separator]             = ImVec4(0.22f, 0.24f, 0.32f, 0.50f);
            colors[ImGuiCol_CheckMark]             = ImVec4(0.70f, 0.58f, 0.95f, 1.00f);
            colors[ImGuiCol_SliderGrab]            = ImVec4(0.68f, 0.56f, 0.92f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.78f, 0.68f, 0.98f, 1.00f);
        } else {
            colors[ImGuiCol_Text]                  = ImVec4(0.12f, 0.13f, 0.18f, 1.00f);
            colors[ImGuiCol_TextDisabled]          = ImVec4(0.55f, 0.57f, 0.64f, 1.00f);
            colors[ImGuiCol_WindowBg]              = ImVec4(0.95f, 0.96f, 0.98f, 0.96f);
            colors[ImGuiCol_ChildBg]               = ImVec4(0.90f, 0.91f, 0.94f, 0.75f);
            colors[ImGuiCol_PopupBg]               = ImVec4(0.98f, 0.98f, 1.00f, 0.98f);
            colors[ImGuiCol_Border]                = ImVec4(0.78f, 0.80f, 0.86f, 0.65f);
            colors[ImGuiCol_FrameBg]               = ImVec4(0.85f, 0.86f, 0.90f, 0.85f);
            colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.78f, 0.80f, 0.86f, 0.95f);
            colors[ImGuiCol_FrameBgActive]         = ImVec4(0.72f, 0.74f, 0.82f, 1.00f);
            colors[ImGuiCol_TitleBg]               = ImVec4(0.92f, 0.93f, 0.96f, 1.00f);
            colors[ImGuiCol_TitleBgActive]         = ImVec4(0.88f, 0.90f, 0.94f, 1.00f);
            colors[ImGuiCol_Button]                = ImVec4(0.84f, 0.86f, 0.92f, 0.85f);
            colors[ImGuiCol_ButtonHovered]         = ImVec4(0.76f, 0.78f, 0.86f, 1.00f);
            colors[ImGuiCol_ButtonActive]          = ImVec4(0.68f, 0.71f, 0.80f, 1.00f);
            colors[ImGuiCol_Header]                = ImVec4(0.82f, 0.84f, 0.90f, 0.75f);
            colors[ImGuiCol_HeaderHovered]         = ImVec4(0.74f, 0.77f, 0.85f, 0.90f);
            colors[ImGuiCol_HeaderActive]          = ImVec4(0.66f, 0.69f, 0.80f, 1.00f);
            colors[ImGuiCol_Separator]             = ImVec4(0.78f, 0.80f, 0.86f, 0.50f);
            colors[ImGuiCol_CheckMark]             = ImVec4(0.55f, 0.40f, 0.88f, 1.00f);
            colors[ImGuiCol_SliderGrab]            = ImVec4(0.55f, 0.40f, 0.88f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.65f, 0.50f, 0.95f, 1.00f);
        }
    }
}
