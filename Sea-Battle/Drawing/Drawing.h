#ifndef DRAWING_H
#define DRAWING_H

#include "../Includes.h"
#include "../Ship/Ship.h"
#include "../imgui_render.h"
#include "../globals.h"

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
	static bool bDraw;


public:
	static constexpr int gridSize = 10; // Размер сетки поля
	static bool playerField[gridSize][gridSize]; // Игровое поле игрока
	static bool opponentField[gridSize][gridSize]; // Игровое поле оппонента

	static void CenterWindow();
	static void Active();
	static bool isActive();

	// Функции для генерации кораблей
	static bool CanPlaceShip(int shipStartX, int shipStartY, int shipEndX, int shipEndY);
	static void PlaceShip(int shipStartX, int shipStartY, int shipEndX, int shipEndY);
	static void GenerateShips();


	static void RenderNote();
	static void DrawCube();
	static void Draw();

	static void DrawTextAboveGrid(ImVec2 cellSize);
	static void AddDot(const ImVec2& center, float radius, const Color& color);
	static void AddCross(const ImVec2& a, const ImVec2& b, Color color, float thickness);


	static void DrawGrid(ImVec2 cellSize);
	static void DrawTextCentered(ImDrawList* drawList, const ImVec2& position, const char* text, ImU32 color, float fontSize);

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
