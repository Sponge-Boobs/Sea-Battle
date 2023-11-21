#pragma once
#include <vector>

// ����������� ������ Ship
class Ship {
public:
    /*
     ��� ����������� ������ Ship.
	 �� ��������� ��������� � �������� ���������� ������� �� ����
	*/
    Ship(int startX, int startY, int endX, int endY);

    // ���������, ��������� �� ������ ������ ������ �������
    bool isInside(int x, int y) const;

    // ���������, ���� �� ������ ������ ������� ��������
    bool isHit(int x, int y) const;

    // �������� ������ ������ ������� ��� ����������
    void hit(int x, int y);

    // ���������, ��� �� ������� ��������� ��������
    bool isSunk() const;

    // ����� getHits
    const std::vector<bool>& getHits() const { return hits; }

private:
    // ���������� ������ ������ ������� �� �� �����������
    int getIndex(int x, int y) const;

    // ���������� ������ � ����� �������
    int startX, startY, endX, endY;

    // ��������� ������ ������ ������� (true - ��������, false - ���)
    std::vector<bool> hits;
};