#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

enum class CellType {
    Empty,
    Spawn,
    Defense
};

class Map {
public:
    Map(int n);
    void generateSpawnAndDefense(int minDist);
    int getSize() const;
    CellType getCellType(int row, int col) const;
    sf::Vector2i getSpawnPos() const;
    sf::Vector2i getDefensePos() const;

private:
    int m_size;
    std::vector<std::vector<CellType>> m_cells;
    sf::Vector2i m_spawn;
    sf::Vector2i m_defense;
};
