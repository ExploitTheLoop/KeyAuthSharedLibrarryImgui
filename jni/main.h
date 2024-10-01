#pragma once

#include "Includes/Includes.h"
#include "Includes/obfuscate.h"
#include "Includes/Tools.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_styles.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Includes/json.hpp"
#include "Includes/tahoma.h"
using json = nlohmann::json;
using namespace std;
android_app *g_App = nullptr;
int screenWidth = -1, glWidth, screenHeight = -1, glHeight;
#include "Includes/Fonts.hpp"
#include "User.h"
#include "virtual_keyboard.h"
uintptr_t UE4;
#include <cstring>
#include "KeyAuth.hpp"
bool iLogin = false;
#define SLEEP_TIME 1000LL / 90LL
bool initImGui = false;
float density = -1;
static bool opened = true;
static bool activehack = true;
bool ifismembotenable = false;
int selectedTab = 1;
#define STB_IMAGE_IMPLEMENTATION
#include "imgui/stb_image.h"

struct TextureInfo{
    ImTextureID textureID;
    int w; 
    int h;
};

TextureInfo CreateTexture(const unsigned char* buf, int len)
{
    TextureInfo image;
   
    unsigned char* image_data = stbi_load_from_memory(buf, len, &image.w, &image.h, NULL, 0);
    if (image_data == NULL)
        perror("文件不存在");
    
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);
    image.textureID = image_texture;

    return image;
}
TextureInfo User;

void CustomImGUIStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(4, 9);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(3, 3);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(1, 10);
	style.CellPadding = ImVec2(5, 5);
	style.IndentSpacing = 23.0f;
	style.ScrollbarSize = 11.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 8.0f;
	style.GrabRounding = 3.0f;
	style.PopupRounding = 2.f;
	style.Alpha = 1.0;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.TabRounding = 7.0f;

	style.ChildRounding = 7.0f;

	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);

	style.ChildBorderSize = 0.100f;

	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.f;
	style.TabBorderSize = 0.000f;
	ImVec4* colors = style.Colors;

	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.95f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImColor(255, 255, 0, 215);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.35f);
}
void AddUnderLine(ImColor col_)
{
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    min.y = max.y;
    ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
}
void TextURL(const char *name_, const char *URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_)
{
    if (1 == SameLineBefore_)
    {
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    }
	
	ImGui::PushStyleColor(ImGuiCol_Text, {255, 255, 255, 255});
    ImGui::Text(name_);
    ImGui::PopStyleColor();
	
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            OpenURL(URL_);
        }
        AddUnderLine({255, 193, 000, 255});
     //   ImGui::SetTooltip(ICON_FA_LINK "  Open in browser\n%s", URL_);
    }
    else
    {
        AddUnderLine({255, 255, 255, 255});
    }
    if (1 == SameLineAfter_)
    {
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    }
}
void linevertical()
    {
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
}
