#include "Ship.h"
#include <algorithm>

//  онструктор класса Ship
Ship::Ship(int startX, int startY, int endX, int endY)
    : startX(startX), startY(startY), endX(endX), endY(endY),
    hits(std::max(endX - startX, endY - startY) + 1, false) {}

// ѕровер€ет, находитс€ ли данна€ клетка внутри корабл€
bool Ship::isInside(int x, int y) const {
    return x >= startX && x <= endX && y >= startY && y <= endY;
}

// ѕровер€ет, была ли данна€ клетка корабл€ поражена
bool Ship::isHit(int x, int y) const {
    return isInside(x, y) ? hits[getIndex(x, y)] : false;
}

// ќтмечает данную клетку корабл€ как пораженную
void Ship::hit(int x, int y) {
    if (isInside(x, y)) hits[getIndex(x, y)] = true;
}

// ѕровер€ет, был ли корабль полностью потоплен
bool Ship::isSunk() const {
    for (const bool hit : hits) {
        if (!hit) return false;
    }
    return true;
}

// ¬озвращает индекс клетки корабл€ по ее координатам
int Ship::getIndex(int x, int y) const {
    return startX == endX ? y - startY : x - startX;
}