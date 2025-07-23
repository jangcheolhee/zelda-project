#include "stdafx.h"
#include "TileMap.h"
#include <fstream>

TileMap::TileMap(const std::string& name) : GameObject(name)
{
}

bool TileMap::LoadTileMap()
{
    tileMap = "data/posTest.tmj";
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

    //tileset
    tilesets.clear();

    for (const auto& ts : tmJ["tilesets"])
    {
        Tileset tileset;

        std::string tsPath = "data/" + std::string(ts["source"]);
        tileset.firstgid = ts["firstgid"];

        std::ifstream tsFile(tsPath);
        json tsJ;
        tsFile >> tsJ;

        //png
        std::string imageFile = tsJ["image"];
        std::string tsDirectory = tsPath.substr(0, tsPath.find_last_of("/\\") + 1);
        std::string imagePath = tsDirectory + imageFile;

        if (!tileset.texture.loadFromFile(imagePath)) {
            std::cerr << "Failed to load tileset image: " << imagePath << std::endl;
            return false;
        }
        tileset.columns = tsJ["columns"];
        tilesets.push_back(std::move(tileset));
    }
    //타일 그리기
    for (const auto& layer : tmJ["layers"])
    {
        if (layer["type"] != "tilelayer") continue;
        //data
        const std::vector<int>& data = layer["data"];
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int index = y * mapWidth + x;
                int gid = data[index];
                if (gid == 0) continue;

                int tsIndex = -1;
                for(int i=(int)tilesets.size()-1;i>=0; --i)
                {
                    if (gid >= tilesets[i].firstgid)
                    {
                        tsIndex = i;
                        break;
                    }
                }
                if (tsIndex == -1) continue;
                
                Tileset& ts = tilesets[tsIndex];
                int localId = gid - ts.firstgid;
                int tu = localId % ts.columns;
                int tv = localId / ts.columns;
                
                //VertexArray
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
                    ts.va.append(quad[i]);
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
    for (auto& ts : tilesets)
        ts.va.clear();
}

void TileMap::Reset()
{
    Release();
    LoadTileMap();
}

void TileMap::Update(float dt)
{
}

void TileMap::Draw(sf::RenderWindow& window)
{
    for (auto& ts : tilesets)
    {
        sf::RenderStates states;
        states.texture = &ts.texture;
        window.draw(ts.va, states);
    }
}