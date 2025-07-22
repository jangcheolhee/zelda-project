#pragma once
#include "stdafx.h"
#include "GameObject.h"

class TileMap : public GameObject
{
protected:
    std::string tileMap;
    sf::Texture texture;
    sf::VertexArray va;

    sf::Vector2f cellSize;
    sf::Vector2f cellCount;

    int firstgid = 1;

    json j;

public:
    TileMap(const std::string& name = "", const std::string& tileMapFile = "data/testMap.tmj");
    virtual ~TileMap() = default;

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

private:
    bool loadTileMap();
};
