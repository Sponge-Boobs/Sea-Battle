#pragma once
#include <vector>

// ќпределение класса Ship
class Ship {
public:
    /*
     Ёто конструктор класса Ship.
	 ќн принимает начальные и конечные координаты корабл€ на поле
	*/
    Ship(int startX, int startY, int endX, int endY);

    // ѕровер€ет, находитс€ ли данна€ клетка внутри корабл€
    bool isInside(int x, int y) const;

    // ѕровер€ет, была ли данна€ клетка корабл€ поражена
    bool isHit(int x, int y) const;

    // ќтмечает данную клетку корабл€ как пораженную
    void hit(int x, int y);

    // ѕровер€ет, был ли корабль полностью потоплен
    bool isSunk() const;

    // ћетод getHits
    const std::vector<bool>& getHits() const { return hits; }

private:
    // ¬озвращает индекс клетки корабл€ по ее координатам
    int getIndex(int x, int y) const;

    //  оординаты начала и конца корабл€
    int startX, startY, endX, endY;

    // —осто€ние каждой клетки корабл€ (true - поражена, false - нет)
    std::vector<bool> hits;
};