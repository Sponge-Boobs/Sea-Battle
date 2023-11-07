#include "Drawing.h"
bool Drawing::bDraw = true;

void Drawing::CenterWindow()
{
	// Get window display
	const ImGuiIO& io = ImGui::GetIO();
	const float screenWidth = io.DisplaySize.x;
	const float screenHeight = io.DisplaySize.y;

	// Get window size
	const float windowWidth = ImGui::GetWindowWidth();
	const float windowHeight = ImGui::GetWindowHeight();

	// Calculate the position for centering the window
	const float posX = (screenWidth - windowWidth) * 0.5f;
	const float posY = (screenHeight - windowHeight) * 0.5f;

	// Set window position
	ImGui::SetWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
}

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw;
}

const int gridSize = 10; // Размер сетки поля
bool playerField[gridSize][gridSize] = { false }; // Игровое поле игрока
bool opponentField[gridSize][gridSize] = { false }; // Игровое поле оппонента

void DrawGrid(bool field[gridSize][gridSize], ImVec2 cellSize)
{
    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            ImVec2 p1 = ImVec2(startPos.x + x * cellSize.x, startPos.y + y * cellSize.y);
            ImVec2 p2 = ImVec2(p1.x + cellSize.x, p1.y + cellSize.y);

            if (field[x][y])
            {
                drawList->AddRectFilled(p1, p2, Color::Green()); // Зеленая клетка - корабль
            }
            else
            {
                drawList->AddRect(p1, p2, Color::Black()); // Чёрная клетка - пустое место
            }
        }
    }
}

void Drawing::RenderNote()
{
    constexpr int cellSize = 35; // Размер ячейки сетки
    const int notebookGridSizeX = (static_cast<int>(iw.window_size.x) + cellSize - 1) / cellSize; // Размер сетки тетради по X
    const int notebookGridSizeY = (static_cast<int>(iw.window_size.y) + cellSize - 1) / cellSize; // Размер сетки тетради по Y

    // Устанавливаем позицию курсора в верхний левый угол окна
    ImGui::SetCursorPos(ImVec2(0, 0));

    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (int x = 0; x < notebookGridSizeX; x++)
    {
        for (int y = 0; y < notebookGridSizeY; y++)
        {
            ImVec2 p1 = ImVec2(startPos.x + x * cellSize, startPos.y + y * cellSize);
            ImVec2 p2 = ImVec2(p1.x + cellSize, p1.y + cellSize);

            drawList->AddRect(p1, p2, Color(219, 112, 147, 150)); // Темно-розовый цвет
        }
    }
}

void Drawing::Draw()
{
    if (bDraw)
    {
        SetStyles();
        CenterWindow();
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::SetNextWindowSize(iw.window_size);

        // Установите цвет фона на бежевый
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Color(244, 244, 219, 255));

        ImGui::Begin(iw.window_tittle, &bDraw, iw.window_flags);
        {
            RenderNote();

            // Определение размера клетки поля
            constexpr ImVec2 cellSize(35, 35);

            // Определение размера поля (количество клеток)
            constexpr int gridSize = 10; // В данном случаи 10x10

            // Определите смещение по осям X и Y
            constexpr float offsetX = 105.0f; // Смещение по X
            constexpr float offsetY = 105.0f; // Смещение по Y

            // Сместите поле игрока по осям X и Y
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + offsetX, ImGui::GetCursorPos().y + offsetY));

            // Отображение игровых полей с использованием DrawGrid
            ImGui::SetNextWindowSize(ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
            ImGui::BeginChild("##PC_Field", ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
            DrawGrid(playerField, cellSize); // Передайте размер клетки в функцию
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::Spacing(); // Добавить расстояние между полями

            ImGui::SameLine();

            // Наше поле
            // code..
        }
        ImGui::End();

        ImGui::PopStyleColor();
    }

#ifdef _WINDLL
    if (GetAsyncKeyState(VK_INSERT) & 1)
        bDraw = !bDraw;
#endif
}

void Drawing::SetStyles()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    Colors[ImGuiCustomCol_Text] = Color(255, 255, 255, 255);
    Colors[ImGuiCustomCol_TextDisabled] = Color(195, 195, 195, 255);
	Colors[ImGuiCustomCol_Border] = Color(242, 244, 255, 90);
    Colors[ImGuiCustomCol_BorderHovered] = Color(255, 128, 0, 160);
    Colors[ImGuiCustomCol_WindowBg] = Color(35, 33, 38, 255);
    Colors[ImGuiCustomCol_FrameBg] = Color(55, 53, 58, 225);
	Colors[ImGuiCustomCol_ChildTabs] = Color(55, 53, 58, 255);
    Colors[ImGuiCustomCol_Button] = Color(38, 37, 39, 255);
    Colors[ImGuiCustomCol_TabBg] = Color(38, 37, 39, 255);
    Colors[ImGuiCustomCol_TabActive] = Color(255, 128, 0, 255);
    Colors[ImGuiCustomCol_TabIcon] = Color(128, 128, 128, 255);
    Colors[ImGuiCustomCol_TabIconHovered] = Color(255, 255, 255, 255);
    Colors[ImGuiCustomCol_CheckMark] = Color(255, 128, 0, 255);
    Colors[ImGuiCustomCol_SliderGrab] = Color(255, 128, 0, 255);

    style.Colors[ImGuiCol_Header] = ImVec4(1.f, 0.501f, 0.f, 1.f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.f, 0.647f, 0.f, 1.f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.f, 0.501f, 0.f, 1.f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.f, 0.501f, 0.f, 1.f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.f, 0.647f, 0.f, 1.f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.f, 0.501f, 0.f, 1.f);

    style.WindowRounding = 0.f;
    style.FrameRounding = 0.f;
    style.PopupRounding = 0.f;
    style.GrabMinSize = 8.f;
    style.WindowPadding.x = 6.f;
    style.ItemSpacing.y = 6.f;
}

Color Drawing::GetColor(const ImGuiCustomCol idx)
{
	if (idx < ImGuiCustomCol_COUNT)
		return Drawing::Colors[idx];

	return Color::BlackTrans();
}