#pragma once
#include "HitBox.h"
#include "TileMap.h"
#include <vector>
#include <set>

class HitboxGenerator
{
public:
    static void SpawnSquareHitBox(
        TileMap* tileMapGame,
        std::vector<HitBox>& collisions,
        sf::RectangleShape& collisionBox
    );
};