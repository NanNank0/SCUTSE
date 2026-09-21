#include "Map.h"
#include "Utils.h"
#include <cstdlib>
#include <ctime>

Map::Map(int n) : m_size(n), m_cells(n, std::vector<CellType>(n, CellType::Empty)) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Map::generateSpawnAndDefense(int minDist) {
    do {
        m_spawn.x = std::rand() % m_size;
        m_spawn.y = std::rand() % m_size;
        m_defense.x = std::rand() % m_size;
        m_defense.y = std::rand() % m_size;
    } while (manhattanDistance(m_spawn.x, m_spawn.y, m_defense.x, m_defense.y) < minDist);
    m_cells[m_spawn.x][m_spawn.y] = CellType::Spawn;
    m_cells[m_defense.x][m_defense.y] = CellType::Defense;
}

int Map::getSize() const { return m_size; }
CellType Map::getCellType(int row, int col) const { return m_cells[row][col]; }
sf::Vector2i Map::getSpawnPos() const { return m_spawn; }
sf::Vector2i Map::getDefensePos() const { return m_defense; }
