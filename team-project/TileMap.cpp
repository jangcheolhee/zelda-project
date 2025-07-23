#include "stdafx.h"
#include "TileMap.h"
#include <fstream>

TileMap::TileMap(const std::string& name, const std::string& tileMapFile)
    : GameObject(name), tileMap(tileMapFile)
{
}

void TileMap::Init()
{
    loadTileMap();
}

void TileMap::Release()
{
    va.clear();
}

void TileMap::Reset()
{
    va.clear();
    loadTileMap();
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

bool TileMap::loadTileMap()
{
    // JSON 로딩
    std::ifstream inFile(tileMap);
    if (!inFile.is_open())
    {
        std::cerr << "Failed to load tile map: " << tileMap << std::endl;
        return false;
    }

    inFile >> j;

    // 맵 사이즈
    int mapWidth = j["width"];
    int mapHeight = j["height"];
    int tileWidth = j["tilewidth"];
    int tileHeight = j["tileheight"];
    cellSize = { (float)tileWidth, (float)tileHeight };
    cellCount = { (float)mapWidth, (float)mapHeight };

    // tileset 경로
    if (j["tilesets"].empty())
    {
        std::cerr << "No tilesets found." << std::endl;
        return false;
    }

    std::string tilesetPath = j["tilesets"][0]["source"];
    firstgid = j["tilesets"][0]["firstgid"];

    // TSJ 로딩
    std::ifstream tsFile("data/" + tilesetPath);
    if (!tsFile.is_open())
    {
        std::cerr << "Could not open tileset file: " << tilesetPath << std::endl;
        return false;
    }

    json tilesetJson;
    tsFile >> tilesetJson;

    std::string imagePath = "data/" + tilesetJson["image"].get<std::string>();
    int imageWidth = tilesetJson["imagewidth"];
    int imageHeight = tilesetJson["imageheight"];
    int columns = tilesetJson["columns"];
    int tilecount = tilesetJson["tilecount"];

    // 텍스처 로딩
    if (!texture.loadFromFile(imagePath))
    {
        std::cerr << "Failed to load tileset image: " << imagePath << std::endl;
        return false;
    }

    // 전체 레이어를 VertexArray로 구성
    va.setPrimitiveType(sf::Quads);
    va.clear();

    for (const auto& layer : j["layers"])
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
