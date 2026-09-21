#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include "Guard.h"

// Use Dijkstra to find optimal weighted path from spawn to defense
// Empty cells: weight 1, Guard cells: weight = guard->getWeight()
// Returns path (inclusive of start & end), or empty vector if no path
std::vector<sf::Vector2i> findOptimalPath(
    int n,
    const sf::Vector2i& spawn,
    const sf::Vector2i& defense,
    const std::vector<std::vector<std::unique_ptr<Guard>>>& guards);
