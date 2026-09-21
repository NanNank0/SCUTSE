#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include "Guard.h"

std::vector<sf::Vector2i> findOptimalPath(
    int n,
    const sf::Vector2i& spawn,
    const sf::Vector2i& defense,
    const std::vector<std::vector<std::unique_ptr<Guard>>>& guards);
