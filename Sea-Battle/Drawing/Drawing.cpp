#include "Drawing.h"
bool Drawing::bDraw = true;
bool Drawing::playerField[gridSize][gridSize] = { false };
bool Drawing::opponentField[gridSize][gridSize] = { false };
static Ship* field[Drawing::gridSize][Drawing::gridSize] = { nullptr };
static bool debug = false;

// Функция для отрисовки точки
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

void Drawing::DrawTextAboveGrid(ImVec2 cellSize)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImVec2 absolutePos = ImGui::GetWindowPos();

    // Создание дочернего окна для текста над сеткой
   ImGui::BeginChild("##TextAboveGridContainer", ImVec2(gridSize * 35, 40));
   // Отрисовка текста над сеткой
   for (int i = 0; i < gridSize; ++i)
   {
       // Позиция текста
       const float text_PosY = absolutePos.y + cellSize.y / 0.3f /** 0.5f - 20.0f*/;
       const float text_PosX = absolutePos.x + i * cellSize.x + cellSize.x * 3.3f;
       const char letter = static_cast<char>('A' + i);

       // Установка масштаба шрифта (увеличение в 1.5 раза)
       ImGui::SetWindowFontScale(1.5f);

       // Вызов функции для отрисовки текста с центрированием
       drawList->AddText(ImVec2(text_PosX, text_PosY), Color::Black(), std::string(1, letter).c_str());

       // Сброс масштаба шрифта после отрисовки
       ImGui::SetWindowFontScale(1.0f); // Возвращение к обычному масштабу
   }
   ImGui::EndChild();
}

// Функция для генерации кораблей в рандомном месте на поле игрока
void Drawing::GenerateShips()
{
    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, Drawing::gridSize - 1);

    // Размеры кораблей
    /*
     1 клетка - 4 корабля
     2 клетки - 3 корбля
     3 клетки - 2 корабля
     4 клетки - 1 корабль
     */
    const std::vector<int> shipSizes = { 1, 1, 1, 1, 2, 2, 2, 3, 3, 4 };

    // Генерация кораблей
    for (const int shipSize : shipSizes)
    {
        // Попытки размещения корабля
        for (int attempts = 0; attempts < 100; ++attempts)
        {
            // Начальные координаты корабля
            const int shipStartX = dis(gen);
            const int shipStartY = dis(gen);
            int shipEndX = shipStartX;
            int shipEndY = shipStartY;

            // Определение направления корабля
            if (dis(gen) % 2 == 0) // Горизонтальное направление
            {
                shipEndX += shipSize - 1;
            }
            else // Вертикальное направление
            {
                shipEndY += shipSize - 1;
            }

            // Проверка, не выходит ли корабль за границы поля
            if (shipEndX >= Drawing::gridSize || shipEndY >= Drawing::gridSize) continue;

            // Проверка, можно ли разместить корабль
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

            // Если можно разместить корабль, то размещаем
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

// Функция для отрисовки поля игрока
void Drawing::DrawGrid(ImVec2 cellSize)
{
    GenerateShips(); // Вызываю функцию рандомного генерирования кораблей

    // Записываю начальные координаты окна отрисовки
    const ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Массивы для отслеживания состояния клеток
    static bool clicks[gridSize][gridSize] = { false }; // Клики по клеткам
    static bool hits[gridSize][gridSize] = { false };   // Попадания по клеткам
    constexpr float padding = 2.0f; // Отступ для чёткой области нажатия

    // Координаты потопленных кораблей
    std::vector<std::pair<int, int>> sunkShips;

    // Проверяю наличие потопленных кораблей
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
            if (field[x][y] != nullptr && field[x][y]->isSunk())
            {
                // Корабль потоплен, добавляю его координаты в sunkShips
                sunkShips.push_back(std::make_pair(x, y));
            }
        }
    }

    // Отмечаю клетки вокруг потопленных кораблей
    for (const auto& sunkShipCoordinate : sunkShips)
    {
        const int sunkShipX = sunkShipCoordinate.first;
        const int sunkShipY = sunkShipCoordinate.second;

        // Перебираю соседние клетки вокруг потопленного корабля
        for (int neighborOffsetX = -1; neighborOffsetX <= 1; ++neighborOffsetX)
        {
            for (int neighborOffsetY = -1; neighborOffsetY <= 1; ++neighborOffsetY)
            {
                const int neighborX = sunkShipX + neighborOffsetX;
                const int neighborY = sunkShipY + neighborOffsetY;

                // Проверяю, что клетка находится внутри поля и не является кораблём
                if (neighborX >= 0 && neighborX < gridSize && neighborY >= 0 && neighborY < gridSize &&
                    field[neighborX][neighborY] == nullptr)
                {
                    // Отмечаю клетку как "кликнутую"
                    clicks[neighborX][neighborY] = true;
                }
            }
        }
    }

    // Основной цикл отрисовки
    for (int x = 0; x < gridSize; x++)
    {
        for (int y = 0; y < gridSize; y++)
        {
	        const ImVec2 cellStart = ImVec2(startPos.x + static_cast<float>(x) * cellSize.x, startPos.y + static_cast<float>(y) * cellSize.y);
	        const ImVec2 cellEnd = ImVec2(cellStart.x + cellSize.x, cellStart.y + cellSize.y);

            // Проверка на нажатие мыши
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                const ImVec2 mousePos = ImGui::GetMousePos();

                // Проверяю, что мышь находится в пределах текущей клетки
                if (mousePos.x >= cellStart.x && mousePos.x <= cellEnd.x + padding &&
                    mousePos.y >= cellStart.y && mousePos.y <= cellEnd.y + padding)
                {
                    // Проверяю, что клетка не была еще нажата и не отмечена как кликнутая
                    if (!hits[x][y] && !clicks[x][y])
                    {
                        // Проверяю, что на клетке есть корабль и мы попали внутрь корабля
                        if (field[x][y] != nullptr && field[x][y]->isInside(x, y))
                        {
                            // Проверяю, была ли клетка уже поражена
                            if (!field[x][y]->isHit(x, y))
                            {
                                // Предполагается, что корабли расположены вертикально
                                field[x][y]->hit(x, y);

                                // Показываю сообщение о попадании или полном потоплении корабля.
                                if (field[x][y]->isSunk()) // Проверяю, был ли корабль полностью потоплен
                                {
                                    MessageBoxA(nullptr, "You sunk my battleship!", "Hit", MB_OK);
                                }
                                else
                                {
                                    MessageBoxA(nullptr, "Hit!", "Hit", MB_OK);
                                }

                                // Записываю попадание в массиве hits
                                hits[x][y] = true;

                                // Выхожу из цикла, чтобы избежать двойного клика, тем самым уберу баг с двойным выстрелом за 1 клик
                                break;
                            }
                        }
                        else
                        {
                            // Сообщение о нажатии на пустую ячейку
                            MessageBoxA(nullptr, "Miss!", "Miss", MB_OK);
                            clicks[x][y] = true; // Обновление массива при клике, на true
                        }
                    }
                }
            }

            // Отрисовка корабля или точки на пустой клетке
            if (field[x][y] != nullptr) // Проверка, была ли клетка не пустой
            {
                constexpr float shipPadding = 2.0f;
                ImVec2 shipStart = ImVec2(cellStart.x + shipPadding, cellStart.y + shipPadding);
                ImVec2 shipEnd = ImVec2(cellEnd.x - shipPadding, cellEnd.y - shipPadding);

                // Отрисовка корабля с отступами, что бы корабль не накладывался на края чёрного поля
                if (!field[x][y]->isSunk())
                {
                    drawList->AddRectFilled(shipStart, shipEnd, Color::Green()); // Отрисовка зеленой клетки - корабль

                    // Рисую крестик, если попали в часть корабля
                    if (hits[x][y])
                    {
                        ImVec2 crossPosition1 = ImVec2(shipStart.x, shipStart.y);
                        ImVec2 crossPosition2 = ImVec2(shipEnd.x, shipEnd.y);
                        AddCross(crossPosition1, crossPosition2, Color::Red(), 2.0f);
                    }
                }
                else
                {
                    // Рисую корабль с отступами
                    drawList->AddRectFilled(shipStart, shipEnd, Color::Green()); // Зеленая клетка - корабль

                    // TODO: пока не используется, думаю нужно ли применять.
                    //ImVec2 crossCenter = ImVec2(p1.x + cellSize.x * 0.5f, p1.y + cellSize.y * 0.5f);
					//constexpr float crossSize = 10.f;

                    // Вызываю функцию для отрисовки крестика, при полном уничтожении корабля
                    AddCross(cellStart, cellEnd, Color::Brown(), 2.0f);
                }
            }
            else
            {
                // Если был сделан клик, рисую кружок только на пустой клетке
                if (clicks[x][y])
                {
                    constexpr float circleRadius = 5.f; // Задаю радиус кружочка
                    ImVec2 circleCenter = ImVec2(cellStart.x + cellSize.x / 2, cellStart.y + cellSize.y / 2);
                    constexpr Color circleColor = Color::Gray(); // Задаю цвет точки

                    // Вызываю функцию для отрисовки кружка
                    AddDot(circleCenter, circleRadius, circleColor);
                }
            }

            // Добавляю отрисовку чёрной сетки
            drawList->AddRect(cellStart, cellEnd, Color::Black());
        }
    }

    // Отчищаю список потопленных кораблей
    sunkShips.clear();
}

void Drawing::DrawTextCentered(ImDrawList* drawList, const ImVec2& position, const char* text, ImU32 color, float fontSize)
{
	const ImVec2 textSize = ImGui::CalcTextSize(text) * fontSize;

    // Рассчитайте позицию так, чтобы текст был по центру относительно переданной позиции
	const ImVec2 textPosition = ImVec2(position.x - textSize.x / 2, position.y - textSize.y / 2);

    // Установите масштаб шрифта (в данном случае, увеличение в 1.5 раза)
    ImGui::SetWindowFontScale(fontSize);

    // Добавьте текст на рисунок
    drawList->AddText(textPosition, color, text);

    // Сбросьте масштаб шрифта после отрисовки
    ImGui::SetWindowFontScale(1.0f);
}

void Drawing::DrawCube()
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Определение размера клетки поля
    constexpr ImVec2 cellSize(35, 35);

    // Определение смещения по осям X и Y
    constexpr float offsetX = 105.0f; // Смещение по X
    constexpr float offsetY = 140.0f; // Смещение по Y

    // Get the absolute position of the window
    const ImVec2 absolutePos = ImGui::GetWindowPos();

    // Смещение поля игрока по осям X и Y
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + offsetX, ImGui::GetCursorPos().y + offsetY));

    // Отображение игровых полей с использованием DrawGrid
    //ImGui::SetNextWindowSize(ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
    ImGui::BeginChild("##PC_Field", ImVec2(gridSize * cellSize.x, gridSize * cellSize.y));
    DrawGrid(cellSize); // Передаем размер клетки в функцию
    ImGui::EndChild();
    DrawTextAboveGrid(cellSize);
    //DrawNULLDot();
	//// Создание дочернего окна для текста над сеткой
 //   ImGui::BeginChild("##TextAboveGridContainer", ImVec2(gridSize * 35, 40));
 //   // Отрисовка текста над сеткой
 //   
 //   for (int i = 0; i < gridSize; ++i)
 //   {
 //       // Позиция текста
	//    constexpr float text_PosY = 142.f;
	//    constexpr float text_PosX = 105.f;
	//    const char letter = static_cast<char>('A' + i);

 //       ImVec2 textPos = ImVec2(absolutePos.x + text_PosX + i * cellSize.x + cellSize.x * 0.5f, absolutePos.y + text_PosY - 20.0f);

 //       // Вызов функции для отрисовки текста с центрированием
 //       DrawTextCentered(drawList, textPos, std::string(1, letter).c_str(), Color::Black(), 1.5f);
 //   }
 //   ImGui::EndChild();


    // Создание дочернего окна для текста слева от сетки (цифры)
    ImGui::SameLine();
    ImGui::BeginChild("##TextLeftOfGridContainer", ImVec2(35, gridSize * cellSize.y));

    // Отрисовка текста слева от сетки (цифры)
    for (int i = 0; i < gridSize; ++i)
    {
        constexpr float text_PosY = 140.f;
        constexpr float text_PosX = 90.f;

        ImVec2 textPos = ImVec2(absolutePos.x + text_PosX, absolutePos.y + text_PosY + i * cellSize.y + cellSize.y * 0.5f);

        // Вызов функции для отрисовки текста с центрированием
        DrawTextCentered(drawList, textPos, std::to_string(i + 1).c_str(), Color::Black(), 1.5f);
    }
    ImGui::EndChild();

    // Создание дочернего окна для текста ниже по центру (поле противника на английском)
    ImGui::SameLine();
    ImGui::BeginChild("##TextBelowCenter", ImVec2(gridSize * cellSize.x, 40));

    // Установка масштаба шрифта (увеличение в 1.5 раза)
    ImGui::SetWindowFontScale(1.5f);

    // Отрисовка текста ниже по центру
    ImVec2 textPos = ImVec2(absolutePos.x + offsetX + (gridSize * cellSize.x - ImGui::CalcTextSize("Enemy GRID").x) * 0.5f, absolutePos.y + offsetY + gridSize * cellSize.y + 5.0f);
    drawList->AddText(textPos, Color::Black(), "Enemy GRID");

    // Сброс масштаба шрифта после отрисовки
    ImGui::SetWindowFontScale(1.0f); // Возвращение к обычному масштабу

    ImGui::EndChild();


    ImGui::SameLine();
    ImGui::Spacing(); // Добавить расстояние между полями
    ImGui::SameLine();


    // Наше поле
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

        // Установите цвет фона на бежевый
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