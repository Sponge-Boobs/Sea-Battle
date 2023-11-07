#ifndef DRAWING_H
#define DRAWING_H

#include "../Includes.h"
#include "../imgui_render.h"
#include "../globals.h"
#include "../image_bytes.h"

typedef int ImGuiCustomCol;
typedef int ImGuiCustomTexture;
typedef int ImGuiCustomTab;

enum ImGuiCustomCol_
{
	ImGuiCustomCol_Text,
	ImGuiCustomCol_TextDisabled,
	ImGuiCustomCol_Border,
	ImGuiCustomCol_BorderHovered,
	ImGuiCustomCol_WindowBg,
	ImGuiCustomCol_FrameBg,
	ImGuiCustomCol_Button,
	ImGuiCustomCol_ChildTabs,
	ImGuiCustomCol_TabBg,
	ImGuiCustomCol_TabActive,
	ImGuiCustomCol_TabIcon,
	ImGuiCustomCol_TabIconHovered,
	ImGuiCustomCol_CheckMark,
	ImGuiCustomCol_SliderGrab,
	ImGuiCustomCol_COUNT
};

enum ImGuiCustomTexture_
{
	ImGuiCustomTexture_background = 0,
	ImGuiCustomTexture_COUNT
};

class Drawing
{
private:
	//static LPCSTR lpWindowName;
	//static ImVec2 vWindowSize;
	//static DWORD WindowFlags;
	static bool bDraw;

public:
	static void CenterWindow();
	static void Active();
	static bool isActive();
	static void Draw();
	static void RenderNote();

	static void SetStyles();
	static Color GetColor(ImGuiCustomCol idx);
	static inline Color Colors[ImGuiCustomCol_COUNT];
};

inline class initWindow
{
public:
	LPCSTR window_tittle = "ImGui Sea-Battle";
	ImVec2 window_size{ 1050, 660 };


	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize /* | ImGuiWindowFlags_NoTitleBar*/ | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
} iw;

//namespace imgui_drawing
//{
//	static Color GetColor(ImGuiCustomCol idx);
//	inline Color Colors[ImGuiCustomCol_COUNT];
//}

#endif
