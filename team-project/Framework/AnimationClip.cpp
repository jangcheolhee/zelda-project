#include "stdafx.h"
#include "AnimationClip.h"
#include "rapidcsv.h"

bool AnimationClip::loadFromFile(const std::string& filePath)
{
	rapidcsv::Document doc(filePath);
	id = doc.GetCell<std::string>(0, 0);
	fps = doc.GetCell<int>(1, 0);	
	loopType = (AnimationLoopTypes)doc.GetCell<int>(2, 0);

	frames.clear();
	for (int i = 3; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		frames.push_back(
			{ 
				row[0], 
				{ std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4]) }
			});
	}

	return true;
}

std::vector<sf::IntRect> AnimationIO::loadFromCSV(const std::string& csvPath)
{
    std::vector<sf::IntRect> frames;
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV: " << csvPath << "\n";
        return frames;
    }

    std::string line;
    bool isFirstLine = true;
    //std::getline(file, line); // 헤더 스킵

    while (std::getline(file, line))
    {
        if (isFirstLine)
        {
            // 첫 줄은 헤더이므로 건너뛴다
            isFirstLine = false;
            continue;
        }

        std::stringstream ss(line);
        std::string dir, frameStr;
        int x, y, w, h;

        // 6열 중 네 번째부터 읽어온다고 가정
        std::getline(ss, dir, ',');       // Direction
        std::getline(ss, frameStr, ',');  // Frame
        ss >> x; ss.ignore();             // X
        ss >> y; ss.ignore();             // Y
        ss >> w; ss.ignore();             // Width
        ss >> h;                          // Height

        frames.emplace_back(x, y, w, h);
    }
    
    return frames;
    std::cerr
        << "[AnimationIO] Loaded " << frames.size()
        << " frames from " << csvPath << "\n";
}
