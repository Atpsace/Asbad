#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <cstdio>
#include <string>
#include <algorithm>
#include <atomic>

class c_notify; // Forward declaration

namespace SpringGui {

    enum IconType {
        ICON_NONE = 0,
        ICON_AIMBOT,
        ICON_ASSISTANCE,
        ICON_RECOIL,
        ICON_TRIGGER,
        ICON_OTHER,
        ICON_VISUALS,
        ICON_INTERFACE,
        ICON_MOVEMENT,
        ICON_INVENTORY,
        ICON_SETTINGS,
        ICON_SEARCH,
        ICON_THEME,
        ICON_CLOSE,
        // Weapons
        ICON_WPN_GENERAL,
        ICON_WPN_SILENCED_PISTOL,
        ICON_WPN_PISTOL,
        ICON_WPN_SMG1,
        ICON_WPN_SMG2,
        ICON_WPN_SMG3,
        ICON_WPN_RIFLE,
        ICON_WPN_SNIPER,
        ICON_WPN_AUTOSNIPER
    };

    enum TabId {
        TAB_AIMBOT = 0,
        TAB_ASSISTANCE,
        TAB_RECOIL,
        TAB_TRIGGER,
        TAB_OTHER,
        TAB_VISUALS,
        TAB_INTERFACE,
        TAB_MOVEMENT,
        TAB_INVENTORY,
        TAB_SETTINGS,
        TAB_COUNT
    };

    namespace Colors {
        constexpr ImU32 White             = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 BgWindow          = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 BgSidebarTop      = IM_COL32(232, 236, 240, 255); // #E8ECF0
        constexpr ImU32 BgSidebarMid      = IM_COL32(213, 218, 224, 255); // #D5DAE0
        constexpr ImU32 BgSidebarBot      = IM_COL32(225, 229, 234, 255); // #E1E5EA
        constexpr ImU32 SidebarDivider    = IM_COL32(0, 0, 0, 22);
        constexpr ImU32 HeaderDivider     = IM_COL32(244, 244, 245, 255); // #F4F4F5
        constexpr ImU32 CardBg            = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 CardBorder        = IM_COL32(228, 228, 231, 255); // #E4E4E7
        constexpr ImU32 CardBorderSubtle  = IM_COL32(238, 238, 240, 255);
        constexpr ImU32 SubContainerBg    = IM_COL32(248, 249, 250, 255); // #F8F9FA
        constexpr ImU32 TextPrimary       = IM_COL32(24, 24, 27, 255);    // #18181B
        constexpr ImU32 TextSecondary     = IM_COL32(113, 113, 122, 255); // #71717A
        constexpr ImU32 TextMuted         = IM_COL32(161, 161, 170, 255); // #A1A1AA
        constexpr ImU32 TextNavInactive   = IM_COL32(63, 63, 70, 255);    // #3F3F46
        constexpr ImU32 SwitchTrackOff    = IM_COL32(228, 228, 231, 255); // #E4E4E7
        constexpr ImU32 SwitchBorderOff   = IM_COL32(212, 212, 216, 255); // #D4D4D8
        constexpr ImU32 SwitchTrackOn     = IM_COL32(24, 24, 27, 255);    // #18181B
        constexpr ImU32 SwitchThumb       = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 AccentGreen       = IM_COL32(16, 185, 129, 255);  // #10B981
        constexpr ImU32 AccentBlue        = IM_COL32(59, 130, 246, 255);  // #3B82F6
        constexpr ImU32 AccentRose        = IM_COL32(225, 29, 72, 255);   // #E11D48
        constexpr ImU32 AccentPurple      = IM_COL32(139, 92, 246, 255);  // #8B5CF6
        constexpr ImU32 PillActiveBg      = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 WeaponBarBg       = IM_COL32(244, 244, 245, 255); // #F4F4F5
    }

    // Procedural Vector Icon Renderer
    inline void DrawIcon(ImDrawList* dl, ImVec2 pos, IconType icon, ImU32 col, float scale = 1.0f) {
        float x = pos.x;
        float y = pos.y;
        float s = scale;

        switch (icon) {
            case ICON_AIMBOT: { // Reticle / Target
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 6*s, col, 16, 1.4f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 1*s), ImVec2(x + 8*s, y + 4*s), col, 1.4f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 12*s), ImVec2(x + 8*s, y + 15*s), col, 1.4f*s);
                dl->AddLine(ImVec2(x + 1*s, y + 8*s), ImVec2(x + 4*s, y + 8*s), col, 1.4f*s);
                dl->AddLine(ImVec2(x + 12*s, y + 8*s), ImVec2(x + 15*s, y + 8*s), col, 1.4f*s);
                break;
            }
            case ICON_ASSISTANCE:
            case ICON_WPN_GENERAL: { // Shield
                ImVec2 pts[6] = {
                    ImVec2(x + 8*s, y + 1*s),
                    ImVec2(x + 15*s, y + 4*s),
                    ImVec2(x + 15*s, y + 9.5f*s),
                    ImVec2(x + 8*s, y + 15.5f*s),
                    ImVec2(x + 1*s, y + 9.5f*s),
                    ImVec2(x + 1*s, y + 4*s)
                };
                dl->AddPolyline(pts, 6, col, ImDrawFlags_Closed, 1.5f*s);
                break;
            }
            case ICON_RECOIL: { // Sliders
                dl->AddLine(ImVec2(x + 2*s, y + 4*s), ImVec2(x + 14*s, y + 4*s), col, 1.3f*s);
                dl->AddCircleFilled(ImVec2(x + 6*s, y + 4*s), 2.2f*s, col);
                dl->AddLine(ImVec2(x + 2*s, y + 8*s), ImVec2(x + 14*s, y + 8*s), col, 1.3f*s);
                dl->AddCircleFilled(ImVec2(x + 11*s, y + 8*s), 2.2f*s, col);
                dl->AddLine(ImVec2(x + 2*s, y + 12*s), ImVec2(x + 14*s, y + 12*s), col, 1.3f*s);
                dl->AddCircleFilled(ImVec2(x + 5*s, y + 12*s), 2.2f*s, col);
                break;
            }
            case ICON_TRIGGER: { // Toggle switch icon
                dl->AddRect(ImVec2(x + 1*s, y + 4*s), ImVec2(x + 15*s, y + 12*s), col, 4*s, 0, 1.4f*s);
                dl->AddCircleFilled(ImVec2(x + 11*s, y + 8*s), 2.8f*s, col);
                break;
            }
            case ICON_OTHER: { // Zap / Lightning
                ImVec2 pts[7] = {
                    ImVec2(x + 9*s, y + 1*s),
                    ImVec2(x + 3*s, y + 9*s),
                    ImVec2(x + 8*s, y + 9*s),
                    ImVec2(x + 7*s, y + 15*s),
                    ImVec2(x + 14*s, y + 7*s),
                    ImVec2(x + 9*s, y + 7*s),
                    ImVec2(x + 10*s, y + 1*s)
                };
                dl->AddPolyline(pts, 7, col, ImDrawFlags_Closed, 1.3f*s);
                break;
            }
            case ICON_VISUALS: { // Eye
                dl->AddLine(ImVec2(x + 1*s, y + 8*s), ImVec2(x + 8*s, y + 3*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 3*s), ImVec2(x + 15*s, y + 8*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 15*s, y + 8*s), ImVec2(x + 8*s, y + 13*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 13*s), ImVec2(x + 1*s, y + 8*s), col, 1.3f*s);
                dl->AddCircleFilled(ImVec2(x + 8*s, y + 8*s), 2.6f*s, col);
                break;
            }
            case ICON_INTERFACE: { // Palette
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 6.5f*s, col, 16, 1.3f*s);
                dl->AddCircleFilled(ImVec2(x + 5.5f*s, y + 6*s), 1.3f*s, col);
                dl->AddCircleFilled(ImVec2(x + 8.5f*s, y + 5*s), 1.3f*s, col);
                dl->AddCircleFilled(ImVec2(x + 11*s, y + 7*s), 1.3f*s, col);
                break;
            }
            case ICON_MOVEMENT: { // Refresh CW
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 5.5f*s, col, 16, 1.4f*s);
                dl->AddLine(ImVec2(x + 12*s, y + 4*s), ImVec2(x + 14*s, y + 8*s), col, 1.4f*s);
                break;
            }
            case ICON_INVENTORY: { // Shopping bag
                dl->AddRect(ImVec2(x + 2*s, y + 5*s), ImVec2(x + 14*s, y + 15*s), col, 2*s, 0, 1.3f*s);
                dl->AddLine(ImVec2(x + 5*s, y + 5*s), ImVec2(x + 5*s, y + 2*s), col, 1.2f*s);
                dl->AddLine(ImVec2(x + 5*s, y + 2*s), ImVec2(x + 11*s, y + 2*s), col, 1.2f*s);
                dl->AddLine(ImVec2(x + 11*s, y + 2*s), ImVec2(x + 11*s, y + 5*s), col, 1.2f*s);
                break;
            }
            case ICON_SETTINGS: { // Gear
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 4.f*s, col, 16, 1.5f*s);
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 1.8f*s, col, 16, 1.3f*s);
                for (int a = 0; a < 6; a++) {
                    float ang = a * (3.1415926f / 3.0f);
                    float dx = std::cos(ang);
                    float dy = std::sin(ang);
                    dl->AddLine(ImVec2(x + 8*s + dx*4.f*s, y + 8*s + dy*4.f*s),
                                ImVec2(x + 8*s + dx*6.5f*s, y + 8*s + dy*6.5f*s), col, 1.4f*s);
                }
                break;
            }
            case ICON_SEARCH: {
                dl->AddCircle(ImVec2(x + 6*s, y + 6*s), 4.5f*s, col, 16, 1.4f*s);
                dl->AddLine(ImVec2(x + 9.5f*s, y + 9.5f*s), ImVec2(x + 14*s, y + 14*s), col, 1.6f*s);
                break;
            }
            case ICON_THEME: { // Sun / Brightness
                dl->AddCircle(ImVec2(x + 8*s, y + 8*s), 3.8f*s, col, 16, 1.4f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 1*s), ImVec2(x + 8*s, y + 3*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 8*s, y + 13*s), ImVec2(x + 8*s, y + 15*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 1*s, y + 8*s), ImVec2(x + 3*s, y + 8*s), col, 1.3f*s);
                dl->AddLine(ImVec2(x + 13*s, y + 8*s), ImVec2(x + 15*s, y + 8*s), col, 1.3f*s);
                break;
            }
            case ICON_CLOSE: { // Cross X
                dl->AddLine(ImVec2(x + 2*s, y + 2*s), ImVec2(x + 12*s, y + 12*s), col, 1.6f*s);
                dl->AddLine(ImVec2(x + 12*s, y + 2*s), ImVec2(x + 2*s, y + 12*s), col, 1.6f*s);
                break;
            }
            case ICON_WPN_SILENCED_PISTOL: { // USP-S Silhouette
                dl->AddRectFilled(ImVec2(x + 1*s, y + 4*s), ImVec2(x + 15*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 15*s, y + 3*s), ImVec2(x + 28*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 24*s, y + 7*s), ImVec2(x + 28*s, y + 14*s), col, 1*s);
                break;
            }
            case ICON_WPN_PISTOL: { // Glock Silhouette
                dl->AddRectFilled(ImVec2(x + 3*s, y + 3*s), ImVec2(x + 22*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 16*s, y + 7*s), ImVec2(x + 21*s, y + 14*s), col, 1*s);
                break;
            }
            case ICON_WPN_SMG1: { // MAC-10 Silhouette
                dl->AddRectFilled(ImVec2(x + 2*s, y + 3*s), ImVec2(x + 18*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 10*s, y + 7*s), ImVec2(x + 14*s, y + 15*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 16*s, y + 7*s), ImVec2(x + 20*s, y + 11*s), col, 1*s);
                break;
            }
            case ICON_WPN_SMG2: { // MP7 Silhouette
                dl->AddRectFilled(ImVec2(x + 2*s, y + 4*s), ImVec2(x + 24*s, y + 8*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 6*s, y + 8*s), ImVec2(x + 9*s, y + 13*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 17*s, y + 8*s), ImVec2(x + 20*s, y + 14*s), col, 1*s);
                break;
            }
            case ICON_WPN_SMG3: { // UMP Silhouette
                dl->AddRectFilled(ImVec2(x + 3*s, y + 3*s), ImVec2(x + 24*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 12*s, y + 7*s), ImVec2(x + 15*s, y + 15*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 19*s, y + 7*s), ImVec2(x + 26*s, y + 10*s), col, 1*s);
                break;
            }
            case ICON_WPN_RIFLE: { // AK-47 Silhouette
                dl->AddRectFilled(ImVec2(x + 1*s, y + 5*s), ImVec2(x + 16*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 16*s, y + 4*s), ImVec2(x + 32*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 25*s, y + 7*s), ImVec2(x + 28*s, y + 11*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 28*s, y + 5*s), ImVec2(x + 38*s, y + 9*s), col, 1*s);
                ImVec2 mag[4] = { ImVec2(x+18*s, y+7*s), ImVec2(x+16*s, y+14*s), ImVec2(x+19*s, y+14*s), ImVec2(x+21*s, y+7*s) };
                dl->AddConvexPolyFilled(mag, 4, col);
                break;
            }
            case ICON_WPN_SNIPER: { // AWP Silhouette
                dl->AddRectFilled(ImVec2(x + 1*s, y + 5*s), ImVec2(x + 20*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 16*s, y + 3*s), ImVec2(x + 27*s, y + 5*s), col, 1*s); // Scope
                dl->AddRectFilled(ImVec2(x + 18*s, y + 5*s), ImVec2(x + 38*s, y + 8*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 27*s, y + 8*s), ImVec2(x + 30*s, y + 13*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 33*s, y + 5*s), ImVec2(x + 44*s, y + 10*s), col, 1*s);
                break;
            }
            case ICON_WPN_AUTOSNIPER: { // SCAR Silhouette
                dl->AddRectFilled(ImVec2(x + 2*s, y + 5*s), ImVec2(x + 18*s, y + 7*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 14*s, y + 3*s), ImVec2(x + 27*s, y + 5*s), col, 1*s); // Scope
                dl->AddRectFilled(ImVec2(x + 16*s, y + 5*s), ImVec2(x + 37*s, y + 8*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 22*s, y + 8*s), ImVec2(x + 25*s, y + 13*s), col, 1*s);
                dl->AddRectFilled(ImVec2(x + 33*s, y + 5*s), ImVec2(x + 43*s, y + 9*s), col, 1*s);
                break;
            }
            default: break;
        }
    }

    inline void SidebarCategory(const char* label) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextMuted);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16.f);
        ImGui::TextUnformatted(label);
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    inline bool SidebarItem(const char* id, const char* label, IconType icon, bool isSelected, ImFont* font = nullptr) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID elemId = window->GetID(id);
        const float w = 224.f;
        const float h = 38.f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, ImVec2(pos.x + w, pos.y + h));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, elemId)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, elemId, &hovered, &held);

        ImDrawList* dl = window->DrawList;

        if (isSelected) {
            dl->AddRectFilledMultiColor(
                bb.Min, bb.Max,
                IM_COL32(110, 110, 120, 150), IM_COL32(170, 175, 185, 110),
                IM_COL32(200, 205, 215, 60),  IM_COL32(140, 145, 155, 120)
            );
            dl->AddRect(bb.Min, bb.Max, IM_COL32(255, 255, 255, 180), 10.f, 0, 1.2f);
        } else if (hovered) {
            dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(255, 255, 255, 120), 10.f);
        }

        ImU32 iconCol = isSelected ? Colors::TextPrimary : (hovered ? Colors::TextPrimary : Colors::TextNavInactive);
        DrawIcon(dl, ImVec2(pos.x + 14.f, pos.y + 11.f), icon, iconCol, 1.0f);

        if (font) ImGui::PushFont(font);
        ImU32 textCol = isSelected ? Colors::TextPrimary : (hovered ? Colors::TextPrimary : Colors::TextNavInactive);
        dl->AddText(ImVec2(pos.x + 40.f, pos.y + 10.f), textCol, label);
        if (font) ImGui::PopFont();

        return pressed;
    }

    inline bool WeaponPill(const char* id, const char* label, IconType icon, bool isSelected, float width = 0.f) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID elemId = window->GetID(id);
        const float w = (width > 0.f) ? width : 48.f;
        const float h = 32.f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, ImVec2(pos.x + w, pos.y + h));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, elemId)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, elemId, &hovered, &held);

        ImDrawList* dl = window->DrawList;

        if (isSelected) {
            dl->AddRectFilled(ImVec2(bb.Min.x + 1, bb.Min.y + 1), ImVec2(bb.Max.x - 1, bb.Max.y - 1), Colors::White, 7.f);
            dl->AddRect(ImVec2(bb.Min.x + 1, bb.Min.y + 1), ImVec2(bb.Max.x - 1, bb.Max.y - 1), IM_COL32(0, 0, 0, 15), 7.f, 0, 1.0f);
        } else if (hovered) {
            dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(0, 0, 0, 10), 7.f);
        }

        ImU32 col = isSelected ? Colors::TextPrimary : Colors::TextSecondary;

        if (label && label[0]) {
            DrawIcon(dl, ImVec2(pos.x + 10.f, pos.y + 8.f), icon, col, 1.0f);
            dl->AddText(ImVec2(pos.x + 32.f, pos.y + 8.f), col, label);
        } else {
            float iconOffset = (w - 36.f) * 0.5f;
            DrawIcon(dl, ImVec2(pos.x + std::max(4.f, iconOffset), pos.y + 9.f), icon, col, 0.85f);
        }

        return pressed;
    }

    inline bool Toggle(const char* id, const char* label, bool* v, const char* desc = nullptr) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID elemId = window->GetID(id);
        const float availW = ImGui::GetContentRegionAvail().x;
        const float rowH = (desc && desc[0]) ? 44.f : 36.f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, ImVec2(pos.x + availW, pos.y + rowH));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, elemId)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, elemId, &hovered, &held);
        if (pressed) {
            *v = !*v;
            ImGui::MarkItemEdited(elemId);
        }

        ImDrawList* dl = window->DrawList;

        float textY = (desc && desc[0]) ? (pos.y + 4.f) : (pos.y + (rowH - 16.f) * 0.5f);
        dl->AddText(ImVec2(pos.x + 2.f, textY), Colors::TextPrimary, label);
        if (desc && desc[0]) {
            dl->AddText(ImVec2(pos.x + 2.f, textY + 18.f), Colors::TextSecondary, desc);
        }

        const float switchW = 46.f;
        const float switchH = 24.f;
        const float switchX = pos.x + availW - switchW - 4.f;
        const float switchY = pos.y + (rowH - switchH) * 0.5f;

        const ImVec2 trackMin(switchX, switchY);
        const ImVec2 trackMax(switchX + switchW, switchY + switchH);

        if (*v) {
            dl->AddRectFilled(trackMin, trackMax, Colors::SwitchTrackOn, 12.f);
            dl->AddRect(trackMin, trackMax, Colors::SwitchTrackOn, 12.f, 0, 1.0f);
            float thumbX = switchX + switchW - 12.f;
            float thumbY = switchY + 12.f;
            dl->AddCircleFilled(ImVec2(thumbX, thumbY + 1.f), 9.5f, IM_COL32(0, 0, 0, 40));
            dl->AddCircleFilled(ImVec2(thumbX, thumbY), 9.5f, Colors::SwitchThumb);
        } else {
            dl->AddRectFilled(trackMin, trackMax, Colors::SwitchTrackOff, 12.f);
            dl->AddRect(trackMin, trackMax, Colors::SwitchBorderOff, 12.f, 0, 1.0f);
            float thumbX = switchX + 12.f;
            float thumbY = switchY + 12.f;
            dl->AddCircleFilled(ImVec2(thumbX, thumbY + 1.f), 9.5f, IM_COL32(0, 0, 0, 30));
            dl->AddCircleFilled(ImVec2(thumbX, thumbY), 9.5f, Colors::SwitchThumb);
        }

        return pressed;
    }

    inline bool SliderFloat(const char* id, const char* label, float* v, float v_min, float v_max, const char* fmt = "%.1f", const char* unit = "") {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiID elemId = window->GetID(id);
        const float availW = ImGui::GetContentRegionAvail().x;
        const float totalH = 46.f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, ImVec2(pos.x + availW, pos.y + totalH));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, elemId)) return false;

        ImDrawList* dl = window->DrawList;

        char valBuf[64];
        snprintf(valBuf, sizeof(valBuf), fmt, *v);
        if (unit && unit[0]) {
            std::string s = std::string(valBuf) + " " + unit;
            snprintf(valBuf, sizeof(valBuf), "%s", s.c_str());
        }

        dl->AddText(ImVec2(pos.x + 2.f, pos.y + 2.f), Colors::TextPrimary, label);
        ImVec2 valSz = ImGui::CalcTextSize(valBuf);
        dl->AddText(ImVec2(pos.x + availW - valSz.x - 4.f, pos.y + 2.f), Colors::TextSecondary, valBuf);

        const float trackY = pos.y + 28.f;
        const float trackH = 6.f;
        const float trackPad = 8.f;
        const float trackW = availW - trackPad * 2.f;
        const ImRect trackBb(ImVec2(pos.x + trackPad, trackY), ImVec2(pos.x + trackPad + trackW, trackY + trackH));

        const ImRect interactBb(ImVec2(pos.x, trackY - 8.f), ImVec2(pos.x + availW, trackY + trackH + 8.f));
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(interactBb, elemId, &hovered, &held);

        if (held) {
            float mouseRel = (g.IO.MousePos.x - (pos.x + trackPad)) / trackW;
            mouseRel = std::clamp(mouseRel, 0.0f, 1.0f);
            *v = v_min + mouseRel * (v_max - v_min);
            ImGui::MarkItemEdited(elemId);
        }

        float pct = std::clamp((*v - v_min) / (v_max - v_min), 0.0f, 1.0f);
        dl->AddRectFilled(trackBb.Min, trackBb.Max, Colors::SwitchTrackOff, 3.f);
        if (pct > 0.001f) {
            dl->AddRectFilled(trackBb.Min, ImVec2(trackBb.Min.x + trackW * pct, trackBb.Max.y), Colors::TextPrimary, 3.f);
        }

        float thumbX = trackBb.Min.x + trackW * pct;
        float thumbY = trackY + trackH * 0.5f;
        dl->AddCircleFilled(ImVec2(thumbX, thumbY + 1.f), 8.f, IM_COL32(0, 0, 0, 35));
        dl->AddCircleFilled(ImVec2(thumbX, thumbY), 8.f, Colors::White);
        dl->AddCircle(ImVec2(thumbX, thumbY), 8.f, Colors::TextPrimary, 16, 2.0f);

        return held;
    }

    inline bool SliderInt(const char* id, const char* label, int* v, int v_min, int v_max, const char* unit = "") {
        float valFloat = (float)*v;
        bool changed = SliderFloat(id, label, &valFloat, (float)v_min, (float)v_max, "%.0f", unit);
        if (changed) *v = (int)std::round(valFloat);
        return changed;
    }

    inline bool Dropdown(const char* id, const char* label, int* currentIdx, const char* const items[], int count) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextPrimary);
        ImGui::TextUnformatted(label);
        ImGui::PopStyleColor();
        ImGui::Spacing();
        bool changed = false;

        float availW = ImGui::GetContentRegionAvail().x;
        float itemW = (availW - 4.f * (count - 1)) / (float)count;

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);
        for (int i = 0; i < count; i++) {
            if (i > 0) ImGui::SameLine(0, 4.f);
            bool isSel = (*currentIdx == i);
            if (isSel) {
                ImGui::PushStyleColor(ImGuiCol_Button, Colors::TextPrimary);
                ImGui::PushStyleColor(ImGuiCol_Text, Colors::White);
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, Colors::WeaponBarBg);
                ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextNavInactive);
            }
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, isSel ? Colors::TextPrimary : IM_COL32(230, 230, 235, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::TextPrimary);

            char btnId[64]; snprintf(btnId, sizeof(btnId), "%s##%s_%d", items[i], id, i);
            if (ImGui::Button(btnId, ImVec2(itemW, 30.f))) {
                *currentIdx = i;
                changed = true;
            }
            ImGui::PopStyleColor(4);
        }
        ImGui::PopStyleVar();
        ImGui::Spacing();
        return changed;
    }

    inline void BeginSectionCard(const char* title, const char* childId, float customH = 0.f) {
        if (title && title[0]) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextPrimary);
            ImGui::TextUnformatted(title);
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 14));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::CardBg);
        ImGui::PushStyleColor(ImGuiCol_Border, Colors::CardBorder);

        ImGui::BeginChild(childId, ImVec2(0, customH), true);
    }

    inline void EndSectionCard() {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);
        ImGui::Spacing();
    }

    // Dynamic Island Watermark (Apple pill style)
    inline void RenderWatermark(
        std::atomic<int>& telemetryTargets,
        std::atomic<int>& telemetryFps,
        std::atomic<float>& telemetryScanMs,
        ImFont* Inter_S
    ) {
        ImGui::SetNextWindowSize({380, 44}, ImGuiCond_Always);
        ImGui::SetNextWindowPos({16, 16}, ImGuiCond_FirstUseEver);
        ImGui::Begin("##dynamic_island", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar);
        {
            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetWindowPos();
            float w = 380.f, h = 42.f;

            dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), IM_COL32(18, 18, 22, 235), 21.f);
            dl->AddRect(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 255, 255, 45), 21.f, 0, 1.2f);

            dl->AddCircleFilled(ImVec2(p.x + 22.f, p.y + 21.f), 12.f, IM_COL32(255, 255, 255, 240));
            dl->AddCircle(ImVec2(p.x + 22.f, p.y + 21.f), 12.f, IM_COL32(24, 24, 27, 255), 16, 1.0f);
            dl->AddCircleFilled(ImVec2(p.x + 19.f, p.y + 20.f), 1.4f, IM_COL32(24, 24, 27, 255));
            dl->AddCircleFilled(ImVec2(p.x + 25.f, p.y + 20.f), 1.4f, IM_COL32(24, 24, 27, 255));

            if (Inter_S) ImGui::PushFont(Inter_S);
            dl->AddText(ImVec2(p.x + 42.f, p.y + 12.f), IM_COL32(255, 255, 255, 255), "spring");
            if (Inter_S) ImGui::PopFont();

            float timeSec = (float)ImGui::GetTime();
            float pulse = 0.6f + 0.4f * std::sin(timeSec * 4.0f);
            ImU32 beaconCol = IM_COL32(16, 185, 129, (uint8_t)(pulse * 255));
            dl->AddCircleFilled(ImVec2(p.x + 104.f, p.y + 21.f), 3.5f, beaconCol);

            char tgtBuf[24]; snprintf(tgtBuf, sizeof(tgtBuf), "%d TGT", telemetryTargets.load());
            dl->AddText(ImVec2(p.x + 122.f, p.y + 13.f), IM_COL32(200, 205, 215, 240), tgtBuf);

            dl->AddLine(ImVec2(p.x + 185.f, p.y + 12.f), ImVec2(p.x + 185.f, p.y + 30.f), IM_COL32(255, 255, 255, 30));

            char fpsBuf[24]; snprintf(fpsBuf, sizeof(fpsBuf), "%d FPS", telemetryFps.load());
            dl->AddText(ImVec2(p.x + 198.f, p.y + 13.f), IM_COL32(255, 255, 255, 255), fpsBuf);

            dl->AddLine(ImVec2(p.x + 270.f, p.y + 12.f), ImVec2(p.x + 270.f, p.y + 30.f), IM_COL32(255, 255, 255, 30));

            char msBuf[24]; snprintf(msBuf, sizeof(msBuf), "%.2f MS", telemetryScanMs.load());
            dl->AddText(ImVec2(p.x + 284.f, p.y + 13.f), IM_COL32(16, 185, 129, 255), msBuf);
        }
        ImGui::End();
    }

    // Main Spring Menu Implementation
    inline void RenderSpringMenu(
        bool& menuOpen,
        bool& g_masterEnabled,
        bool& aimbotEnabled,
        int& aimbotKey,
        int& aimbotBone,
        float& aimbotFovDeg,
        float& aimbotSmooth,
        bool& aimbotFovCircle,
        bool& aimbotLead,
        bool& aimbotWallCheck,
        bool& bulletTracer,
        float* bulletColor,
        int& radarDistance,
        bool& rageMode,
        bool& rageInstantSnap,
        bool& rageAutoFire,
        bool& rageAutoStop,
        bool& wallshotEnabled,
        bool& wallshotIndicator,
        bool& wallshotAutoShoot,
        bool& rageTargetLaser,
        bool& rageLockedHud,
        bool& triggerbotEnabled,
        int& triggerMode,
        int& triggerDelay,
        bool& chamsEnabled,
        float* chamsColor,
        bool& espEnabled,
        bool& drawBoxes,
        int& boxStyle,
        bool& drawSkeletons,
        int& skeletonStyle,
        bool& drawHeadDot,
        bool& drawHealth,
        bool& drawArmor,
        bool& drawNames,
        bool& drawDistance,
        bool& drawWeapon,
        bool& drawAmmoBar,
        bool& offscreenEsp,
        bool& showSpectators,
        bool& soundEsp,
        bool& bombEsp,
        bool& droppedC4Esp,
        bool& nadeLineups,
        bool& liveNades,
        bool& worldLoot,
        bool& teamCheck,
        bool& autoSaveConfig,
        bool& autoLoadConfig,
        int& g_selectedPresetIdx,
        char* g_lastConfigStatus,
        ImFont* Inter_B,
        ImFont* Inter_S,
        ImFont* Inter_S_1,
        ImFont* Inter_S_2,
        ImFont* Inter_S_3,
        std::atomic<int>& telemetryTargets,
        std::atomic<int>& telemetryFps,
        std::atomic<float>& telemetryScanMs,
        bool (*SaveConfigToFile)(const std::string&),
        bool (*LoadConfigFromFile)(const std::string&),
        void (*ResetConfigDefaults)(),
        std::string (*GetConfigFilePath)(const char*)
    ) {
        static int activeTab = TAB_AIMBOT;
        static int activeWeapon = 0;

        // Render Dynamic Island watermark
        RenderWatermark(telemetryTargets, telemetryFps, telemetryScanMs, Inter_S);

        // Main Window Setup
        ImGui::SetNextWindowSize(ImVec2(960, 680), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints(ImVec2(800, 560), ImVec2(1920, 1080));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::BgWindow);
        ImGui::PushStyleColor(ImGuiCol_Border, Colors::CardBorder);

        if (ImGui::Begin("Spring CS2 Engine", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
        {
            ImVec2 p = ImGui::GetWindowPos();
            ImVec2 sz = ImGui::GetWindowSize();
            ImDrawList* dl = ImGui::GetWindowDrawList();

            // Drag area for entire header
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::InvisibleButton("##spring_drag", ImVec2(sz.x - 100.f, 48.f));
            if (ImGui::IsItemActive()) {
                ImVec2 delta = ImGui::GetIO().MouseDelta;
                ImGui::SetWindowPos(ImVec2(p.x + delta.x, p.y + delta.y));
            }

            const float sidebarW = 246.f;

            // ================= LEFT SIDEBAR (Frosted Acrylic) =================
            dl->AddRectFilledMultiColor(
                p, ImVec2(p.x + sidebarW, p.y + sz.y),
                Colors::BgSidebarTop, Colors::BgSidebarMid,
                Colors::BgSidebarBot, Colors::BgSidebarTop
            );
            dl->AddLine(ImVec2(p.x + sidebarW, p.y), ImVec2(p.x + sidebarW, p.y + sz.y), Colors::SidebarDivider, 1.0f);

            // Ambient soft radial highlights
            dl->AddCircleFilled(ImVec2(p.x + 30.f, p.y + 30.f), 90.f, IM_COL32(255, 255, 255, 45));
            dl->AddCircleFilled(ImVec2(p.x + 20.f, p.y + sz.y * 0.5f), 100.f, IM_COL32(255, 255, 255, 30));

            // Brand Header
            // Avatar Sketch Container
            dl->AddRectFilled(ImVec2(p.x + 20.f, p.y + 22.f), ImVec2(p.x + 64.f, p.y + 66.f), Colors::White, 12.f);
            dl->AddRect(ImVec2(p.x + 20.f, p.y + 22.f), ImVec2(p.x + 64.f, p.y + 66.f), Colors::CardBorder, 12.f, 0, 1.0f);

            // Anime avatar sketch
            ImU32 ink = Colors::TextPrimary;
            dl->AddCircle(ImVec2(p.x + 42.f, p.y + 44.f), 12.f, ink, 16, 1.4f);
            dl->AddCircleFilled(ImVec2(p.x + 37.f, p.y + 43.f), 1.8f, ink);
            dl->AddCircleFilled(ImVec2(p.x + 47.f, p.y + 43.f), 1.8f, ink);
            dl->AddLine(ImVec2(p.x + 40.f, p.y + 49.f), ImVec2(p.x + 44.f, p.y + 49.f), ink, 1.2f);

            // App Name & Build
            if (Inter_S_1) ImGui::PushFont(Inter_S_1);
            dl->AddText(ImVec2(p.x + 76.f, p.y + 26.f), Colors::TextPrimary, "spring");
            if (Inter_S_1) ImGui::PopFont();
            dl->AddText(ImVec2(p.x + 76.f, p.y + 46.f), Colors::TextSecondary, "staging build v1.03");

            // Sidebar Navigation Scroll Area
            ImGui::SetCursorPos(ImVec2(10.f, 84.f));
            ImGui::BeginChild("##sidebar_nav", ImVec2(sidebarW - 14.f, sz.y - 94.f), false, ImGuiWindowFlags_NoScrollbar);
            {
                // MAIN Section
                SidebarCategory("MAIN");
                if (SidebarItem("nav_aimbot", "Aimbot", ICON_AIMBOT, activeTab == TAB_AIMBOT, Inter_S_3))
                    activeTab = TAB_AIMBOT;
                if (SidebarItem("nav_assist", "Assistance", ICON_ASSISTANCE, activeTab == TAB_ASSISTANCE, Inter_S_3))
                    activeTab = TAB_ASSISTANCE;
                if (SidebarItem("nav_trigger", "Trigger Bot", ICON_TRIGGER, activeTab == TAB_TRIGGER, Inter_S_3))
                    activeTab = TAB_TRIGGER;
                if (SidebarItem("nav_other", "Other & Rage", ICON_OTHER, activeTab == TAB_OTHER, Inter_S_3))
                    activeTab = TAB_OTHER;

                // RENDER Section
                SidebarCategory("RENDER");
                if (SidebarItem("nav_visuals", "Visuals", ICON_VISUALS, activeTab == TAB_VISUALS, Inter_S_3))
                    activeTab = TAB_VISUALS;
                if (SidebarItem("nav_interface", "Interface", ICON_INTERFACE, activeTab == TAB_INTERFACE, Inter_S_3))
                    activeTab = TAB_INTERFACE;

                // MISC Section
                SidebarCategory("MISC");
                if (SidebarItem("nav_movement", "Movement", ICON_MOVEMENT, activeTab == TAB_MOVEMENT, Inter_S_3))
                    activeTab = TAB_MOVEMENT;
                if (SidebarItem("nav_inventory", "Inventory", ICON_INVENTORY, activeTab == TAB_INVENTORY, Inter_S_3))
                    activeTab = TAB_INVENTORY;
                if (SidebarItem("nav_settings", "Settings & Config", ICON_SETTINGS, activeTab == TAB_SETTINGS, Inter_S_3))
                    activeTab = TAB_SETTINGS;
            }
            ImGui::EndChild();

            // ================= RIGHT MAIN AREA =================
            const float contentX = sidebarW;
            const float contentW = sz.x - sidebarW;

            // Top Header Bar
            dl->AddLine(ImVec2(p.x + contentX, p.y + 52.f), ImVec2(p.x + sz.x, p.y + 52.f), Colors::HeaderDivider, 1.0f);

            const char* tabTitles[] = {
                "Aimbot", "Assistance", "Recoil Control", "Trigger Bot", "Other & Rage",
                "Visuals", "Interface", "Movement", "Inventory", "Settings & Config"
            };

            if (Inter_S_1) ImGui::PushFont(Inter_S_1);
            dl->AddText(ImVec2(p.x + contentX + 28.f, p.y + 16.f), Colors::TextPrimary, tabTitles[activeTab]);
            if (Inter_S_1) ImGui::PopFont();

            // Right header tool buttons: Search, Theme, Close
            float btnRx = p.x + sz.x - 38.f;
            // Close button
            ImGui::SetCursorPos(ImVec2(sz.x - 42.f, 13.f));
            if (ImGui::InvisibleButton("##btn_close", ImVec2(28.f, 28.f))) {
                menuOpen = false;
            }
            bool closeHov = ImGui::IsItemHovered();
            if (closeHov) dl->AddRectFilled(ImVec2(btnRx - 2.f, p.y + 12.f), ImVec2(btnRx + 24.f, p.y + 38.f), IM_COL32(0, 0, 0, 15), 6.f);
            DrawIcon(dl, ImVec2(btnRx + 5.f, p.y + 19.f), ICON_CLOSE, closeHov ? Colors::AccentRose : Colors::TextSecondary, 1.0f);

            // Theme toggle button
            btnRx -= 36.f;
            ImGui::SetCursorPos(ImVec2(sz.x - 78.f, 13.f));
            ImGui::InvisibleButton("##btn_theme", ImVec2(28.f, 28.f));
            if (ImGui::IsItemHovered()) dl->AddRectFilled(ImVec2(btnRx - 2.f, p.y + 12.f), ImVec2(btnRx + 24.f, p.y + 38.f), IM_COL32(0, 0, 0, 15), 6.f);
            DrawIcon(dl, ImVec2(btnRx + 4.f, p.y + 18.f), ICON_THEME, Colors::TextSecondary, 1.0f);

            // Search button
            btnRx -= 36.f;
            ImGui::SetCursorPos(ImVec2(sz.x - 114.f, 13.f));
            ImGui::InvisibleButton("##btn_search", ImVec2(28.f, 28.f));
            if (ImGui::IsItemHovered()) dl->AddRectFilled(ImVec2(btnRx - 2.f, p.y + 12.f), ImVec2(btnRx + 24.f, p.y + 38.f), IM_COL32(0, 0, 0, 15), 6.f);
            DrawIcon(dl, ImVec2(btnRx + 5.f, p.y + 18.f), ICON_SEARCH, Colors::TextSecondary, 1.0f);

            // Page Body
            ImGui::SetCursorPos(ImVec2(contentX + 24.f, 62.f));
            ImGui::BeginChild("##content_scroll", ImVec2(contentW - 48.f, sz.y - 74.f), false, ImGuiWindowFlags_None);
            {
                // Weapon Selector Bar (for weapon-related tabs)
                bool showWeaponBar = (activeTab <= TAB_OTHER);
                if (showWeaponBar) {
                    ImVec2 barPos = ImGui::GetCursorScreenPos();
                    float barW = ImGui::GetContentRegionAvail().x;
                    dl->AddRectFilled(barPos, ImVec2(barPos.x + barW, barPos.y + 38.f), Colors::WeaponBarBg, 9.f);
                    dl->AddRect(barPos, ImVec2(barPos.x + barW, barPos.y + 38.f), Colors::CardBorder, 9.f, 0, 1.0f);

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.f);

                    // Weapon Tabs
                    if (WeaponPill("wpn_gen", "General", ICON_WPN_GENERAL, activeWeapon == 0, 88.f)) activeWeapon = 0;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_usp", nullptr, ICON_WPN_SILENCED_PISTOL, activeWeapon == 1, 52.f)) activeWeapon = 1;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_glock", nullptr, ICON_WPN_PISTOL, activeWeapon == 2, 48.f)) activeWeapon = 2;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_mac", nullptr, ICON_WPN_SMG1, activeWeapon == 3, 48.f)) activeWeapon = 3;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_mp7", nullptr, ICON_WPN_SMG2, activeWeapon == 4, 48.f)) activeWeapon = 4;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_ump", nullptr, ICON_WPN_SMG3, activeWeapon == 5, 48.f)) activeWeapon = 5;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_ak", nullptr, ICON_WPN_RIFLE, activeWeapon == 6, 56.f)) activeWeapon = 6;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_awp", nullptr, ICON_WPN_SNIPER, activeWeapon == 7, 56.f)) activeWeapon = 7;
                    ImGui::SameLine(0, 4.f);
                    if (WeaponPill("wpn_scar", nullptr, ICON_WPN_AUTOSNIPER, activeWeapon == 8, 56.f)) activeWeapon = 8;

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
                }

                // ================= TAB CONTENT ROUTING =================
                if (activeTab == TAB_AIMBOT) {
                    BeginSectionCard("Main", "card_aim_main");
                    {
                        Toggle("aim_enabled", "Enabled", &aimbotEnabled, "Engage automated crosshair targeting");
                        Toggle("master_enabled", "Master Switch", &g_masterEnabled, "Master enable for entire system (Press HOME to panic)");

                        const char* prios[] = { "Crosshair FOV", "Lowest HP", "Closest 3D" };
                        Dropdown("aim_prio", "Target Priority", &rageTargetPrio, prios, 3);

                        const char* bones[] = { "Head", "Neck", "Chest" };
                        Dropdown("aim_bone", "Target Hitbox", &aimbotBone, bones, 3);

                        const char* keys[] = { "Alt", "Shift", "LMB", "RMB", "Mouse 4", "Mouse 5" };
                        int keyIdx = std::clamp(aimbotKey - 2, 0, 5);
                        if (Dropdown("aim_key", "Activation Key", &keyIdx, keys, 6)) {
                            aimbotKey = keyIdx + 2;
                        }
                    }
                    EndSectionCard();

                    BeginSectionCard("Targeting Dynamics", "card_aim_dynamics");
                    {
                        SliderFloat("aim_fov", "Field of View (FOV)", &aimbotFovDeg, 0.5f, 30.0f, "%.1f", "deg");
                        SliderFloat("aim_smooth", "Smoothing Factor", &aimbotSmooth, 1.0f, 20.0f, "%.1f", "x");
                        Toggle("aim_fovcircle", "Draw FOV Circle", &aimbotFovCircle, "Visualize target acquisition boundary");
                        Toggle("aim_lead", "Velocity Prediction", &aimbotLead, "Compensate for target moving velocity");
                        Toggle("aim_wallcheck", "Visibility Check", &aimbotWallCheck, "Acquire visible and spotted targets only");
                    }
                    EndSectionCard();

                    BeginSectionCard("Tracers & Radar", "card_aim_tracers");
                    {
                        Toggle("aim_tracer", "Bullet Tracers", &bulletTracer, "Draw trajectory path from eye to impact point");
                        SliderInt("aim_radar", "Radar Range", &radarDistance, 10, 200, "m");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_ASSISTANCE) {
                    BeginSectionCard("Assistance Engine", "card_assist_main");
                    {
                        Toggle("assist_on", "Legit Aim Assist", &aimbotEnabled, "Subtle humanized targeting assist");
                        SliderFloat("assist_fov", "Assist FOV Window", &aimbotFovDeg, 0.5f, 15.0f, "%.1f", "deg");
                        SliderFloat("assist_smooth", "Assistance Smoothing", &aimbotSmooth, 3.0f, 25.0f, "%.1f", "x");
                        Toggle("assist_wall", "Spotted Only Filter", &aimbotWallCheck, "Only engage verified targets");
                    }
                    EndSectionCard();
                else if (activeTab == TAB_TRIGGER) {
                    BeginSectionCard("Trigger Bot Configuration", "card_trigger_main");
                    {
                        Toggle("trig_enabled", "Enable Triggerbot", &triggerbotEnabled, "Instant automated shot on target intersection");
                        const char* tModes[] = { "Hold Caps Lock", "Always On" };
                        Dropdown("trig_mode", "Trigger Activation Mode", &triggerMode, tModes, 2);
                        SliderInt("trig_delay", "Shot Delay", &triggerDelay, 0, 150, "ms");
                    }
                    EndSectionCard();

                    BeginSectionCard("Model Illumination", "card_chams_main");
                    {
                        Toggle("chams_enabled", "Chams (Player Glow)", &chamsEnabled, "Color fill models through geometry");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_OTHER) {
                    BeginSectionCard("Rage Engine (Maximum Aggression)", "card_rage_main");
                    {
                        Toggle("rage_enabled", "Rage Mode [Full Send]", &rageMode, "Uncapped maximum aggression profile");
                        Toggle("rage_snap", "Instant Snap (0-Smooth)", &rageInstantSnap, "Zero-latency instantaneous orientation snap");
                        Toggle("rage_autofire", "Rage Auto-Fire", &rageAutoFire, "Fires automatically when aligned");

                        const char* hitscans[] = { "Head Only", "Neck Only", "Chest Only", "Smart Multi-Point" };
                        Dropdown("rage_hitscan", "Hitscan Target Mode", &rageHitscan, hitscans, 4);
                    }
                    EndSectionCard();

                    BeginSectionCard("Wallshot & Penetration", "card_wallshot_main");
                    {
                        Toggle("wallshot_enabled", "Wallshot Engine", &wallshotEnabled, "Calculate penetrable surfaces");
                        Toggle("wallshot_indicator", "Wallbang Indicator on ESP", &wallshotIndicator);
                        Toggle("wallshot_autoshoot", "Auto-Shoot Through Walls", &wallshotAutoShoot, "Fires through penetrable cover automatically");
                    }
                    EndSectionCard();

                    BeginSectionCard("Target Overlays", "card_rage_overlays");
                    {
                        Toggle("rage_laser", "Target Lock Laser", &rageTargetLaser, "Guide laser beam to locked opponent");
                        Toggle("rage_hud", "Target Lock HUD Badge", &rageLockedHud, "Show target info pill at top of screen");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_VISUALS) {
                    BeginSectionCard("Master ESP", "card_esp_master");
                    {
                        Toggle("esp_master", "Master ESP Switch", &espEnabled, "Enable all world & player visual overlays");
                        Toggle("esp_box", "Bounding Boxes", &drawBoxes, "2D oriented entity bounding box");
                        const char* bstyles[] = { "Corner Brackets", "Full Box" };
                        Dropdown("esp_bstyle", "Box Geometry Style", &boxStyle, bstyles, 2);

                        Toggle("esp_skeleton", "Player Skeleton Bones", &drawSkeletons, "Real-time 3D bone linkage matrix");
                        const char* skstyles[] = { "Optimized Stick", "Detailed Anatomy" };
                        Dropdown("esp_skstyle", "Skeleton Detail", &skeletonStyle, skstyles, 2);

                        Toggle("esp_headdot", "Head Target Point", &drawHeadDot, "Pinpoint dot at head bone center");
                    }
                    EndSectionCard();

                    BeginSectionCard("Entity Indicators", "card_esp_indicators");
                    {
                        Toggle("esp_hp", "Health Bar", &drawHealth, "Dynamic color-coded health gauge");
                        Toggle("esp_armor", "Armor Bar", &drawArmor, "Shield rating indicator");
                        Toggle("esp_name", "Player Names", &drawNames, "GamerTag overlay");
                        Toggle("esp_dist", "Distance Badge", &drawDistance, "Distance in meters");
                        Toggle("esp_weapon", "Active Weapon Name", &drawWeapon, "Display current equipped weapon");
                        Toggle("esp_ammo", "Ammo Count Bar", &drawAmmoBar, "Active weapon clip status");
                        Toggle("esp_offscreen", "Offscreen Warning Arrows", &offscreenEsp, "Directional radar arrows for out-of-view targets");
                        Toggle("esp_sound", "Sound & Footstep Wave ESP", &soundEsp, "Visual pulse ripples on footsteps");
                        Toggle("esp_spectators", "Spectator Warning List", &showSpectators, "Live card showing viewers watching your pawn");
                        Toggle("esp_teamcheck", "Team Filter", &teamCheck, "Filter out teammates from overlay");
                    }
                    EndSectionCard();

                    BeginSectionCard("Objectives & World", "card_esp_world");
                    {
                        Toggle("esp_bomb", "Planted C4 & Defusal Timer", &bombEsp, "Accurate detonation countdown bar");
                        Toggle("esp_dropc4", "Dropped C4 ESP", &droppedC4Esp, "Track dropped bomb in world");
                        Toggle("esp_lineups", "Grenade Lineup Trajectories", &nadeLineups, "Guided reticle for utility lineups");
                        Toggle("esp_loot", "Dropped Weapons & Loot", &worldLoot, "Highlight guns on the ground");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_INTERFACE) {
                    BeginSectionCard("Overlay Interface & Themes", "card_iface_main");
                    {
                        ImGui::TextUnformatted("Spring Design System — Staging Build v1.03");
                        ImGui::Spacing();
                        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.45f, 1.0f), "DirectX 11 Frosted Acrylic Glass Engine Active");
                        ImGui::Spacing();
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_MOVEMENT) {
                    BeginSectionCard("Movement Engine", "card_movement_main");
                    {
                        Toggle("mov_autostop", "Counter-Strafe Auto-Stop", &rageAutoStop, "Zero-velocity instant stop prior to firing");
                        ImGui::Spacing();
                        ImGui::TextDisabled("Eliminates movement inaccuracy penalty instantly.");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_INVENTORY) {
                    BeginSectionCard("Inventory & Loadout", "card_inv_main");
                    {
                        ImGui::TextUnformatted("CS2 Skin & Loadout Customizer");
                        ImGui::Spacing();
                        ImGui::TextDisabled("Loadout inventory sync ready.");
                    }
                    EndSectionCard();
                }
                else if (activeTab == TAB_SETTINGS) {
                    BeginSectionCard("Configuration Profiles", "card_cfg_main");
                    {
                        static std::vector<std::string> cfgFiles = { "config.cfg", "legit.cfg", "rage.cfg", "visuals.cfg" };
                        static bool s_scanDone = false;
                        if (!s_scanDone) {
                            std::string dir = GetConfigDirectory();
                            std::string pattern = dir + "*.cfg";
                            WIN32_FIND_DATAA fd;
                            HANDLE hFind = FindFirstFileA(pattern.c_str(), &fd);
                            if (hFind != INVALID_HANDLE_VALUE) {
                                do {
                                    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                                        std::string fn = fd.cFileName;
                                        if (std::find(cfgFiles.begin(), cfgFiles.end(), fn) == cfgFiles.end()) {
                                            cfgFiles.push_back(fn);
                                        }
                                    }
                                } while (FindNextFileA(hFind, &fd));
                                FindClose(hFind);
                            }
                            s_scanDone = true;
                        }

                        std::vector<const char*> rawPtrs;
                        for (const auto& f : cfgFiles) rawPtrs.push_back(f.c_str());

                        if (g_selectedPresetIdx < 0 || g_selectedPresetIdx >= (int)cfgFiles.size()) g_selectedPresetIdx = 0;
                        Dropdown("cfg_preset", "Select Target Profile", &g_selectedPresetIdx, rawPtrs.data(), (int)rawPtrs.size());

                        std::string fullPath = GetConfigFilePath(cfgFiles[g_selectedPresetIdx].c_str());
                        ImGui::Spacing();
                        ImGui::TextDisabled("Path: %s", fullPath.c_str());
                        ImGui::TextColored(ImVec4(0.1f, 0.75f, 0.4f, 1.0f), "Status: %s", g_lastConfigStatus);
                        ImGui::Spacing();

                        float btnW = (ImGui::GetContentRegionAvail().x - 16.f) / 3.f;
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.f);
                        ImGui::PushStyleColor(ImGuiCol_Button, Colors::TextPrimary);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(50, 50, 55, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(10, 10, 12, 255));

                        if (ImGui::Button("Save Profile", ImVec2(btnW, 34.f))) {
                            SaveConfigToFile(cfgFiles[g_selectedPresetIdx]);
                        }
                        ImGui::SameLine(0, 8.f);
                        if (ImGui::Button("Load Profile", ImVec2(btnW, 34.f))) {
                            LoadConfigFromFile(cfgFiles[g_selectedPresetIdx]);
                        }
                        ImGui::SameLine(0, 8.f);
                        if (ImGui::Button("Reset Defaults", ImVec2(btnW, 34.f))) {
                            ResetConfigDefaults();
                        }

                        ImGui::PopStyleColor(3);
                        ImGui::PopStyleVar();

                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();

                        static char newProfileBuf[64] = "";
                        ImGui::InputTextWithHint("##new_cfg_input", "New Profile Name...", newProfileBuf, sizeof(newProfileBuf));
                        ImGui::SameLine(0, 8.f);
                        if (ImGui::Button("Create Profile", ImVec2(120.f, 32.f))) {
                            if (newProfileBuf[0] != '\0') {
                                std::string newName = newProfileBuf;
                                if (newName.find(".cfg") == std::string::npos) newName += ".cfg";
                                SaveConfigToFile(newName);
                                if (std::find(cfgFiles.begin(), cfgFiles.end(), newName) == cfgFiles.end()) {
                                    cfgFiles.push_back(newName);
                                }
                                g_selectedPresetIdx = (int)cfgFiles.size() - 1;
                                newProfileBuf[0] = '\0';
                            }
                        }

                        ImGui::Spacing();
                        Toggle("cfg_autosave", "Auto-Save on Exit", &autoSaveConfig, "Automatically save config on application shutdown");
                        Toggle("cfg_autoload", "Auto-Load config.cfg on Launch", &autoLoadConfig, "Automatically load default configuration on startup");
                    }
                    EndSectionCard();

                    BeginSectionCard("Instant One-Click Presets", "card_cfg_templates");
                    {
                        float pBtnW = ImGui::GetContentRegionAvail().x;
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.f);

                        ImGui::PushStyleColor(ImGuiCol_Button, Colors::WeaponBarBg);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(230, 230, 235, 255));
                        ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextPrimary);

                        if (ImGui::Button("Apply Smooth Legit Template", ImVec2(pBtnW, 36.f))) {
                            aimbotEnabled = true; aimbotFovDeg = 3.0f; aimbotSmooth = 8.0f; aimbotBone = 0; aimbotFovCircle = true;
                            triggerbotEnabled = true; triggerMode = 0; triggerDelay = 45;
                            espEnabled = true; drawBoxes = true; boxStyle = 0; drawSkeletons = false; drawHealth = true; drawNames = true;
                            SaveConfigToFile("config.cfg");
                        }
                        ImGui::Spacing();
                        if (ImGui::Button("Apply Full Visuals Template", ImVec2(pBtnW, 36.f))) {
                            aimbotEnabled = false; triggerbotEnabled = false;
                            espEnabled = true; drawBoxes = true; boxStyle = 1; drawSkeletons = true; drawHeadDot = true;
                            drawHealth = true; drawArmor = true; drawNames = true; drawDistance = true; drawWeapon = true;
                            soundEsp = true; bombEsp = true; nadeLineups = true; offscreenEsp = true; showSpectators = true;
                            SaveConfigToFile("config.cfg");
                        }
                        ImGui::Spacing();
                        if (ImGui::Button("Apply Max Rage Template", ImVec2(pBtnW, 36.f))) {
                            rageMode = true; rageInstantSnap = true; rageAutoFire = true; rageTargetPrio = 1; rageHitscan = 3;
                            rageAutoStop = true; rageTargetLaser = true; rageLockedHud = true;
                            wallshotEnabled = true; wallshotIndicator = true; wallshotAutoShoot = true;
                            aimbotEnabled = true; aimbotFovDeg = 25.0f; aimbotSmooth = 1.0f;
                            espEnabled = true; drawBoxes = true; drawSkeletons = true;
                            SaveConfigToFile("config.cfg");
                        }

                        ImGui::PopStyleColor(3);
                        ImGui::PopStyleVar();
                    }
                    EndSectionCard();
                }
            }
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);
    }

} // namespace SpringGui
