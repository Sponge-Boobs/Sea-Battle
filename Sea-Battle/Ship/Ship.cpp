#include "Ship.h"
#include <algorithm>

// ����������� ������ Ship
Ship::Ship(int startX, int startY, int endX, int endY)
    : startX(startX), startY(startY), endX(endX), endY(endY),
    hits(std::max(endX - startX, endY - startY) + 1, false) {}

// ���������, ��������� �� ������ ������ ������ �������
bool Ship::isInside(int x, int y) const {
    return x >= startX && x <= endX && y >= startY && y <= endY;
}

// ���������, ���� �� ������ ������ ������� ��������
bool Ship::isHit(int x, int y) const {
    return isInside(x, y) ? hits[getIndex(x, y)] : false;
}

// �������� ������ ������ ������� ��� ����������
void Ship::hit(int x, int y) {
    if (isInside(x, y)) hits[getIndex(x, y)] = true;
}

// ���������, ��� �� ������� ��������� ��������
bool Ship::isSunk() const {
    for (const bool hit : hits) {
        if (!hit) return false;
    }
    return true;
}

// ���������� ������ ������ ������� �� �� �����������
int Ship::getIndex(int x, int y) const {
    return startX == endX ? y - startY : x - startX;
}