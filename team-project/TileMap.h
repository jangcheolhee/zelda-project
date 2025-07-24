#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Enemy.h"

struct Tileset
{
    sf::Texture texture;
    int firstgid = 0;
    int columns = 0;
    sf::VertexArray va;

    Tileset() :va(sf::Quads) {}
};

class TileMap : public GameObject
{
protected:
    std::vector<Tileset> tilesets;

    std::string tileMap;
    sf::Texture texture;

    sf::Vector2f cellSize;
    sf::Vector2f cellCount;

    json tmJ;
public:
    TileMap(const std::string& name="");
    virtual ~TileMap() = default;

    bool LoadTileMap();
    sf::Vector2f getPosition(int layerIndex, int targetGid); //return pos
    std::vector<sf::Vector2f> getPositions(int layerIndex, int targetGid); //return <vector>positions

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};