#include "draw.h"
#include "imgui.h"
#include <cmath>
#include <algorithm>

namespace Renderer {

void Draw::Box2D(ImDrawList* draw, Vec2 tl, Vec2 br, uint32_t color, float thickness) {
    if (!draw) return;
    // Shadow
    draw->AddRect(ImVec2(tl.x + 1, tl.y + 1), ImVec2(br.x + 1, br.y + 1), 
                  IM_COL32(0, 0, 0, 160), 0.0f, 0, thickness + 0.8f);
    // Main box
    draw->AddRect(ImVec2(tl.x, tl.y), ImVec2(br.x, br.y), color, 0.0f, 0, thickness);
}

void Draw::BoxCorners(ImDrawList* draw, Vec2 tl, Vec2 br, float armLength, uint32_t color, float thickness) {
    if (!draw) return;
    float w = br.x - tl.x;
    float h = br.y - tl.y;
    float len = std::min(armLength, std::min(w, h) * 0.25f);
    
    // Top-left
    draw->AddLine(ImVec2(tl.x, tl.y), ImVec2(tl.x + len, tl.y), color, thickness);
    draw->AddLine(ImVec2(tl.x, tl.y), ImVec2(tl.x, tl.y + len), color, thickness);
    
    // Top-right
    draw->AddLine(ImVec2(br.x, tl.y), ImVec2(br.x - len, tl.y), color, thickness);
    draw->AddLine(ImVec2(br.x, tl.y), ImVec2(br.x, tl.y + len), color, thickness);
    
    // Bottom-left
    draw->AddLine(ImVec2(tl.x, br.y), ImVec2(tl.x + len, br.y), color, thickness);
    draw->AddLine(ImVec2(tl.x, br.y), ImVec2(tl.x, br.y - len), color, thickness);
    
    // Bottom-right
    draw->AddLine(ImVec2(br.x, br.y), ImVec2(br.x - len, br.y), color, thickness);
    draw->AddLine(ImVec2(br.x, br.y), ImVec2(br.x, br.y - len), color, thickness);
}

void Draw::HealthBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent, bool gradient) {
    if (!draw) return;
    percent = std::clamp(percent, 0.0f, 1.0f);
    
    // Background
    draw->AddRectFilled(ImVec2(pos.x - 1, pos.y - 1), 
                       ImVec2(pos.x + width + 1, pos.y + height + 1), 
                       IM_COL32(0, 0, 0, 180));
    
    // Health fill
    uint32_t color;
    if (gradient) {
        if (percent > 0.5f) color = IM_COL32(0, 255, 120, 255);
        else if (percent > 0.25f) color = IM_COL32(255, 180, 0, 255);
        else color = IM_COL32(255, 40, 40, 255);
    } else {
        color = IM_COL32(0, 255, 0, 255);
    }
    
    float fillHeight = height * percent;
    draw->AddRectFilled(ImVec2(pos.x, pos.y + height - fillHeight), 
                       ImVec2(pos.x + width, pos.y + height), color);
}

void Draw::ArmorBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent) {
    if (!draw) return;
    percent = std::clamp(percent, 0.0f, 1.0f);
    
    draw->AddRectFilled(ImVec2(pos.x - 1, pos.y - 1), 
                       ImVec2(pos.x + width + 1, pos.y + height + 1), 
                       IM_COL32(0, 0, 0, 180));
    
    float fillHeight = height * percent;
    draw->AddRectFilled(ImVec2(pos.x, pos.y + height - fillHeight), 
                       ImVec2(pos.x + width, pos.y + height), 
                       IM_COL32(80, 180, 255, 230));
}

void Draw::AmmoBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent) {
    if (!draw) return;
    percent = std::clamp(percent, 0.0f, 1.0f);
    
    draw->AddRectFilled(ImVec2(pos.x - 1, pos.y - 1), 
                       ImVec2(pos.x + width + 1, pos.y + height + 1), 
                       IM_COL32(0, 0, 0, 180));
    
    float fillWidth = width * percent;
    draw->AddRectFilled(ImVec2(pos.x, pos.y), 
                       ImVec2(pos.x + fillWidth, pos.y + height), 
                       IM_COL32(175, 95, 255, 230));
}

void Draw::Text(ImDrawList* draw, Vec2 pos, const char* text, uint32_t color, float fontSize, bool shadow) {
    if (!draw || !text) return;
    
    if (shadow) {
        draw->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32(0, 0, 0, 200), text);
    }
    draw->AddText(ImVec2(pos.x, pos.y), color, text);
}

void Draw::TextCentered(ImDrawList* draw, Vec2 center, const char* text, uint32_t color, float fontSize, bool shadow) {
    if (!draw || !text) return;
    
    ImVec2 size = ImGui::CalcTextSize(text);
    Vec2 pos = {center.x - size.x * 0.5f, center.y - size.y * 0.5f};
    Text(draw, pos, text, color, fontSize, shadow);
}

void Draw::Line(ImDrawList* draw, Vec2 start, Vec2 end, uint32_t color, float thickness) {
    if (!draw) return;
    draw->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), color, thickness);
}

void Draw::Circle(ImDrawList* draw, Vec2 center, float radius, uint32_t color, bool filled, float thickness) {
    if (!draw) return;
    if (filled) {
        draw->AddCircleFilled(ImVec2(center.x, center.y), radius, color);
    } else {
        draw->AddCircle(ImVec2(center.x, center.y), radius, color, 0, thickness);
    }
}

void Draw::Rect(ImDrawList* draw, Vec2 tl, Vec2 br, uint32_t color, bool filled, float rounding, float thickness) {
    if (!draw) return;
    if (filled) {
        draw->AddRectFilled(ImVec2(tl.x, tl.y), ImVec2(br.x, br.y), color, rounding);
    } else {
        draw->AddRect(ImVec2(tl.x, tl.y), ImVec2(br.x, br.y), color, rounding, 0, thickness);
    }
}

void Draw::Triangle(ImDrawList* draw, Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color, bool filled, float thickness) {
    if (!draw) return;
    if (filled) {
        draw->AddTriangleFilled(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), ImVec2(p3.x, p3.y), color);
    } else {
        draw->AddTriangle(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), ImVec2(p3.x, p3.y), color, thickness);
    }
}

} // namespace Renderer
