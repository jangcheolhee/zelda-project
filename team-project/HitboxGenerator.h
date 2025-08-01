#pragma once
#include "HitBox.h"
#include "TileMap.h"
#include <vector>
#include <set>

class HitboxGenerator
{
public:
    static void SpawnSquareHitBox(
        TileMap* tileMapName,
        std::vector<HitBox>& collisions,
        sf::RectangleShape& collisionBox,
        std::string name
    );
};