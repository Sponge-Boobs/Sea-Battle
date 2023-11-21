#include "Drawing.h"
bool Drawing::bDraw = true;
bool Drawing::playerField[gridSize][gridSize] = { false };
bool Drawing::opponentField[gridSize][gridSize] = { false };
static Ship* field[Drawing::gridSize][Drawing::gridSize] = { nullptr };
static bool debug = false;

// ������� ��� ��������� �����
void Drawing::AddDot(const ImVec2& center, float radius, const Color& color)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(center, radius, color, 12);
}

void Drawing::AddCross(const ImVec2& a, const ImVec2& b, Color color, float thickness)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddLine(a, b, color, thickness);
    drawList->AddLine(ImVec2(a.x, b.y), ImVec2(b.x, a.y), color, thickness);
}

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
	const float posX = (screenWidth - windowWidth) / 2;
	const float posY = (screenHeight - windowHeight) / 2;

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

void Drawing::RenderNote()
{
    constexpr int cellSize = 35; // ������ ������ �����
    const int notebookGridSizeX = (static_cast<int>(iw.window_size.x) + cellSize - 1) / cellSize; // ������ ����� ������� �� X
    const int notebookGridSizeY = (static_cast<int>(iw.window_size.y) + cellSize - 1) / cellSize; // ������ ����� ������� �� Y

    // ������������� ������� ������� � ������� ����� ���� ����
    ImGui::SetCursorPos(ImVec2(0, 0));

    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (int x = 0; x < notebookGridSizeX; x++)
    {
        for (int y = 0; y < notebookGridSizeY; y++)
        {
            ImVec2 p1 = ImVec2(startPos.x + x * cellSize, startPos.y + y * cellSize);
            ImVec2 p2 = ImVec2(p1.x + cellSize, p1.y + cellSize);

            drawList->AddRect(p1, p2, Color(219, 112, 147, 150)); // �����-������� ����
        }
    }
}

void Drawing::DrawTextAboveGrid(ImVec2 cellSize)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImVec2 absolutePos = ImGui::GetWindowPos();

    // �������� ��������� ���� ��� ������ ��� ������
   ImGui::BeginChild("##TextAboveGridContainer", ImVec2(gridSize * 35, 40));
   // ��������� ������ ��� ������
   for (int i = 0; i < gridSize; ++i)
   {
       // ������� ������
       const float text_PosY = absolutePos.y + cellSize.y / 0.3f /** 0.5f - 20.0f*/;
       const float text_PosX = absolutePos.x + i * cellSize.x + cellSize.x * 3.3f;
       const char letter = static_cast<char>('A' + i);

       // ��������� �������� ������ (���������� � 1.5 ����)
       ImGui::SetWindowFontScale(1.5f);

       // ����� ������� ��� ��������� ������ � ��������������
       drawList->AddText(ImVec2(text_PosX, text_PosY), Color::Black(), std::string(1, letter).c_str());

       // ����� �������� ������ ����� ���������
       ImGui::SetWindowFontScale(1.0f); // ����������� � �������� ��������
   }
   ImGui::EndChild();
}

// ������� ��� ��������� �������� � ��������� ����� �� ���� ������
void Drawing::GenerateShips()
{
    // ������������� ���������� ��������� �����
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, Drawing::gridSize - 1);

    // ������� ��������
    /*
     1 ������ - 4 �������
     2 ������ - 3 ������
     3 ������ - 2 �������
     4 ������ - 1 �������
     */
    const std::vector<int> shipSizes = { 1, 1, 1, 1, 2, 2, 2, 3, 3, 4 };

    // ��������� ��������
    for (const int shipSize : shipSizes)
    {
        // ������� ���������� �������
        for (int attempts = 0; attempts < 100; ++attempts)
        {
            // ��������� ���������� �������
            const int shipStartX = dis(gen);
            const int shipStartY = dis(gen);
            int shipEndX = shipStartX;
            int shipEndY = shipStartY;

            // ����������� ����������� �������
            if (dis(gen) % 2 == 0) // �������������� �����������
            {
                shipEndX += shipSize - 1;
            }
            else // ������������ �����������
            {
                shipEndY += shipSize - 1;
            }

            // ��������, �� ������� �� ������� �� ������� ����
            if (shipEndX >= Drawing::gridSize || shipEndY >= Drawing::gridSize) continue;

            // ��������, ����� �� ���������� �������
            bool canPlaceShip = true;
            for (int x = max(0, shipStartX - 1); x <= min(Drawing::gridSize - 1, shipEndX + 1); ++x)
            {
                for (int y = max(0, shipStartY - 1); y <= min(Drawing::gridSize - 1, shipEndY + 1); ++y)
                {
                    if (field[x][y] != nullptr)
                    {
                        canPlaceShip = false;
                        break;
                    }
                }
                if (!canPlaceShip) break;
            }

            // ���� ����� ���������� �������, �� ���������
            if (canPlaceShip)
            {
                Ship* ship = new Ship(shipStartX, shipStartY, shipEndX, shipEndY);
                for (int x = shipStartX; x <= shipEndX; ++x)
                {
                    for (int y = shipStartY; y <= shipEndY; ++y)
                    {
                        field[x][y] = ship;
                    }
                }
                break;
            }
        }
    }
}

// ������� ��� ��������� ���� ������
void Drawing::DrawGrid(ImVec2 cellSize)
{
    GenerateShips(); // ������� ������� ���������� ������������� ��������

    // ��������� ��������� ���������� ���� ���������
    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // ������� ��� ������������ ��������� ������
    static bool clicks[gridSize][gridSize] = { false }; // ����� �� �������
    static bool hits[gridSize][gridSize] = { false };   // ��������� �� �������
    constexpr float padding = 2.0f; // ������ ��� ������ ������� �������

    // ���������� ����������� ��������
    std::vector<std::pair<int, int>> sunkShips;

    // �������� ������� ����������� ��������
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            if (field[x][y] != nullptr && field[x][y]->isSunk())
            {
                // ������� ��������, �������� ��� ���������� � sunkShips
                sunkShips.push_back(std::make_pair(x, y));
            }
        }
    }

    // ������� ������ ������ ����������� ��������
    for (const auto& sunkShipCoordinate : sunkShips)
    {
        const int sunkShipX = sunkShipCoordinate.first;
        const int sunkShipY = sunkShipCoordinate.second;

        // ��������� �������� ������ ������ ������������ �������
        for (int neighborOffsetX = -1; neighborOffsetX <= 1; ++neighborOffsetX)
        {
            for (int neighborOffsetY = -1; neighborOffsetY <= 1; ++neighborOffsetY)
            {
                const int neighborX = sunkShipX + neighborOffsetX;
                const int neighborY = sunkShipY + neighborOffsetY;

                // ��������, ��� ������ ��������� ������ ���� � �� �������� �������
                if (neighborX >= 0 && neighborX < gridSize && neighborY >= 0 && neighborY < gridSize &&
                    field[neighborX][neighborY] == nullptr)
                {
                    // ������� ������ ��� "���������"
                    clicks[neighborX][neighborY] = true;
                }
            }
        }
    }

    // �������� ���� ���������
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
	        const ImVec2 cellStart = ImVec2(startPos.x + static_cast<float>(x) * cellSize.x, startPos.y + static_cast<float>(y) * cellSize.y);
	        const ImVec2 cellEnd = ImVec2(cellStart.x + cellSize.x, cellStart.y + cellSize.y);

            // �������� �� ������� ����
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                const ImVec2 mousePos = ImGui::GetMousePos();

                // ��������, ��� ���� ��������� � �������� ������� ������
                if (mousePos.x >= cellStart.x && mousePos.x <= cellEnd.x + padding &&
                    mousePos.y >= cellStart.y && mousePos.y <= cellEnd.y + padding)
                {
                    // ��������, ��� ������ �� ���� ��� ������ � �� �������� ��� ���������
                    if (!hits[x][y] && !clicks[x][y])
                    {
                        // ��������, ��� �� ������ ���� ������� � �� ������ ������ �������
                        if (field[x][y] != nullptr && field[x][y]->isInside(x, y))
                        {
                            // ��������, ���� �� ������ ��� ��������
                            if (!field[x][y]->isHit(x, y))
                            {
                                // ��������������, ��� ������� ����������� �����������
                                field[x][y]->hit(x, y);

                                // ��������� ��������� � ��������� ��� ������ ���������� �������.
                                if (field[x][y]->isSunk()) // ��������, ��� �� ������� ��������� ��������
                                {
                                    MessageBoxA(nullptr, "You sunk my battleship!", "Hit", MB_OK);
                                }
                                else
                                {
                                    MessageBoxA(nullptr, "Hit!", "Hit", MB_OK);
                                }

                                // ��������� ��������� � ������� hits
                                hits[x][y] = true;

                                // ������ �� �����, ����� �������� �������� �����, ��� ����� ����� ��� � ������� ��������� �� 1 ����
                                break;
                            }
                        }
                        else
                        {
                            // ��������� � ������� �� ������ ������
                            MessageBoxA(nullptr, "Miss!", "Miss", MB_OK);
                            clicks[x][y] = true; // ���������� ������� ��� �����, �� true
                        }
                    }
                }
            }

            // ��������� ������� ��� ����� �� ������ ������
            if (field[x][y] != nullptr) // ��������, ���� �� ������ �� ������
            {
                constexpr float shipPadding = 2.0f;
                ImVec2 shipStart = ImVec2(cellStart.x + shipPadding, cellStart.y + shipPadding);
                ImVec2 shipEnd = ImVec2(cellEnd.x - shipPadding, cellEnd.y - shipPadding);

                // ��������� ������� � ���������, ��� �� ������� �� ������������ �� ���� ������� ����
                if (!field[x][y]->isSunk())
                {
                    drawList->AddRectFilled(shipStart, shipEnd, Color::Green()); // ��������� ������� ������ - �������

                    // ����� �������, ���� ������ � ����� �������
                    if (hits[x][y])
                    {
                        ImVec2 crossPosition1 = ImVec2(shipStart.x, shipStart.y);
                        ImVec2 crossPosition2 = ImVec2(shipEnd.x, shipEnd.y);
                        AddCross(crossPosition1, crossPosition2, Color::Red(), 2.0f);
                    }
                }
                else
                {
                    // ����� ������� � ���������
                    drawList->AddRectFilled(shipStart, shipEnd, Color::Green()); // ������� ������ - �������

                    // TODO: ���� �� ������������, ����� ����� �� ���������.
                    //ImVec2 crossCenter = ImVec2(p1.x + cellSize.x * 0.5f, p1.y + cellSize.y * 0.5f);
					//constexpr float crossSize = 10.f;

                    // ������� ������� ��� ��������� ��������, ��� ������ ����������� �������
                    AddCross(cellStart, cellEnd, Color::Brown(), 2.0f);
                }
            }
            else
            {
                // ���� ��� ������ ����, ����� ������ ������ �� ������ ������
                if (clicks[x][y])
                {
                    constexpr float circleRadius = 5.f; // ����� ������ ��������
                    ImVec2 circleCenter = ImVec2(cellStart.x + cellSize.x / 2, cellStart.y + cellSize.y / 2);
                    constexpr Color circleColor = Color::Gray(); // ����� ���� �����

                    // ������� ������� ��� ��������� ������
                    AddDot(circleCenter, circleRadius, circleColor);
                }
            }

            // �������� ��������� ������ �����
            drawList->AddRect(cellStart, cellEnd, Color::Black());
        }
    }

    // ������� ������ ����������� ��������
    sunkShips.clear();
}

void Drawing::DrawTextCentered(ImDrawList* drawList, const ImVec2& position, const char* text, ImU32 color, float fontSize)
{
	const ImVec2 textSize = ImGui::CalcTextSize(text) * fontSize;

    // ����������� ������� ���, ����� ����� ��� �� ������ ������������ ���������� �������
	const ImVec2 textPosition = ImVec2(position.x - textSize.x / 2, position.y - textSize.y / 2);

    // ���������� ������� ������ (� ������ ������, ���������� � 1.5 ����)
    ImGui::SetWindowFontScale(fontSize);

    // �������� ����� �� �������
    drawList->AddText(textPosition, color, text);

    // �������� ������� ������ ����� ���������
    ImGui::SetWindowFontScale(1.0f);
}

void Drawing::DrawCube()
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // ����������� ������� ������ ����
    constexpr ImVec2 cellSize(35, 35);

    // ����������� �������� �� ���� X � Y
    constexpr float offsetX = 105.0f; // �������� �� X
    constexpr float offsetY = 140.0f; // �������� �� Y

    // Get the absolute position of the window
    const ImVec2 absolutePos = ImGui::GetWindowPos();

    // �������� ���� ������ �� ���� X � Y
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + offsetX, ImGui::GetCursorPos().y + offsetY));

    // ����������� ������� ����� � �������������� DrawGrid
    //ImGui::SetNextWindowSize(ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
    ImGui::BeginChild("##PC_Field", ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
    DrawGrid(cellSize); // �������� ������ ������ � �������
    ImGui::EndChild();
    DrawTextAboveGrid(cellSize);
    //DrawNULLDot();
	//// �������� ��������� ���� ��� ������ ��� ������
 //   ImGui::BeginChild("##TextAboveGridContainer", ImVec2(gridSize * 35, 40));
 //   // ��������� ������ ��� ������
 //   
 //   for (int i = 0; i < gridSize; ++i)
 //   {
 //       // ������� ������
	//    constexpr float text_PosY = 142.f;
	//    constexpr float text_PosX = 105.f;
	//    const char letter = static_cast<char>('A' + i);

 //       ImVec2 textPos = ImVec2(absolutePos.x + text_PosX + i * cellSize.x + cellSize.x * 0.5f, absolutePos.y + text_PosY - 20.0f);

 //       // ����� ������� ��� ��������� ������ � ��������������
 //       DrawTextCentered(drawList, textPos, std::string(1, letter).c_str(), Color::Black(), 1.5f);
 //   }
 //   ImGui::EndChild();


    // �������� ��������� ���� ��� ������ ����� �� ����� (�����)
    ImGui::SameLine();
    ImGui::BeginChild("##TextLeftOfGridContainer", ImVec2(35, gridSize * cellSize.y));

    // ��������� ������ ����� �� ����� (�����)
    for (int i = 0; i < gridSize; ++i)
    {
        constexpr float text_PosY = 140.f;
        constexpr float text_PosX = 90.f;

        ImVec2 textPos = ImVec2(absolutePos.x + text_PosX, absolutePos.y + text_PosY + i * cellSize.y + cellSize.y * 0.5f);

        // ����� ������� ��� ��������� ������ � ��������������
        DrawTextCentered(drawList, textPos, std::to_string(i + 1).c_str(), Color::Black(), 1.5f);
    }
    ImGui::EndChild();

    // �������� ��������� ���� ��� ������ ���� �� ������ (���� ���������� �� ����������)
    ImGui::SameLine();
    ImGui::BeginChild("##TextBelowCenter", ImVec2(gridSize * cellSize.x, 40));

    // ��������� �������� ������ (���������� � 1.5 ����)
    ImGui::SetWindowFontScale(1.5f);

    // ��������� ������ ���� �� ������
    ImVec2 textPos = ImVec2(absolutePos.x + offsetX + (gridSize * cellSize.x - ImGui::CalcTextSize("Enemy GRID").x) * 0.5f, absolutePos.y + offsetY + gridSize * cellSize.y + 5.0f);
    drawList->AddText(textPos, Color::Black(), "Enemy GRID");

    // ����� �������� ������ ����� ���������
    ImGui::SetWindowFontScale(1.0f); // ����������� � �������� ��������

    ImGui::EndChild();


    ImGui::SameLine();
    ImGui::Spacing(); // �������� ���������� ����� ������
    ImGui::SameLine();


    // ���� ����
    // code..


}

void Drawing::Draw()
{
    if (bDraw)
    {
        SetStyles();
        //CenterWindow();
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::SetNextWindowSize(iw.window_size);

        // ���������� ���� ���� �� �������
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Color(244, 244, 219, 255));

        ImGui::Begin(iw.window_tittle, &bDraw, iw.window_flags);
        {
            RenderNote();

//#if _DEBUG
//            ImGui::Checkbox("debug", &debug);
//#endif 

            DrawCube();
        }
        ImGui::End();

        ImGui::PopStyleColor();

        if (debug)
            ImGui::ShowStyleEditor();
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
    style.FrameBorderSize = 1.f;

    style.WindowPadding.x = 6.f;

    style.GrabMinSize = 8.f;
    style.ItemSpacing.y = 6.f;

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
}

Color Drawing::GetColor(const ImGuiCustomCol idx)
{
	if (idx < ImGuiCustomCol_COUNT)
		return Colors[idx];

	return Color::BlackTrans();
}