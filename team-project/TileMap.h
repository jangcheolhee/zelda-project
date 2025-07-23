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

    json tmJ;

public:
    TileMap(const std::string& name="");
    virtual ~TileMap() = default;

    bool LoadTileMap();

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};
