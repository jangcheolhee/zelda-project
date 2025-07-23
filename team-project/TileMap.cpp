#include "stdafx.h"
#include "TileMap.h"
#include <fstream>

TileMap::TileMap(const std::string& name) : GameObject(name)
{
}

bool TileMap::LoadTileMap()
{
    tileMap = "data/testMap.tmj";
    std::ifstream tmFile(tileMap);
    if (!tmFile.is_open())
    {
        std::cerr << "Failed to load tile map: " << tileMap << std::endl;
        return false;
    }
    tmFile >> tmJ;

    int mapWidth = tmJ["width"];
    int mapHeight = tmJ["height"];

    int tileWidth = tmJ["tilewidth"];
    int tileHeight = tmJ["tileheight"];

    cellSize = { (float)tileWidth, (float)tileHeight };
    cellCount = { (float)mapWidth, (float)mapHeight };

    //반복문 타일셋 로드
    std::string tilesetPath = tmJ["tilesets"][0]["source"];
    firstgid = tmJ["tilesets"][0]["firstgid"];

    //반복문 -> tilesetPath로 가져와야 함. data+/ string 으로 가져오기
    std::ifstream tsFile("data/filed.tsj");
    json tsJ;
    tsFile >> tsJ;

    //반복문 Png 로드 
    std::string imagePath = "data/filed.png";
    int imageWidth = tsJ["imagewidth"];
    int imageHeight = tsJ["imageheight"];
    int columns = tsJ["columns"];
    int tilecount = tsJ["tilecount"];
    if (!texture.loadFromFile(imagePath))
    {
        std::cerr << "Failed to load tileset image: " << imagePath << std::endl;
        return false;
    }

    // 전체 레이어를 VertexArray로 구성
    va.setPrimitiveType(sf::Quads);
    va.clear();

    for (const auto& layer : tmJ["layers"])
    {
        if (layer["type"] != "tilelayer")
            continue;

        const std::vector<int>& data = layer["data"];
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int tileIndex = y * mapWidth + x;
                int tileId = data[tileIndex] - firstgid;
                if (tileId < 0)
                    continue;

                int tu = tileId % columns;
                int tv = tileId / columns;

                sf::Vertex quad[4];

                quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
                quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                for (int i = 0; i < 4; ++i)
                    va.append(quad[i]);
            }
        }
    }
    return true;
}

void TileMap::Init()
{
    LoadTileMap();
}

void TileMap::Release()
{
    va.clear();
}

void TileMap::Reset()
{
    va.clear();
    LoadTileMap();
}

void TileMap::Update(float dt)
{
}

void TileMap::Draw(sf::RenderWindow& window)
{
    sf::RenderStates states;
    states.texture = &texture;
    window.draw(va, states);
}