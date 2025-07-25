#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
	//PingPong,
};



struct AnimationFrame
{
	std::string texId;
	sf::IntRect texCoord;

	AnimationFrame() {}
	AnimationFrame(const std::string& texId, const sf::IntRect& texCoord) : texId(texId), texCoord(texCoord) {}
};

struct AnimationClip
{
	std::string id;
	AnimationLoopTypes loopType = AnimationLoopTypes::Loop;
	int fps = 30;

	std::vector<AnimationFrame> frames;

	bool loadFromFile(const std::string& filePath);
};
namespace AnimationIO
{
	/// @brief CSV에서 프레임(Rect) 목록을 읽어옵니다.
	std::vector<sf::IntRect> loadFromCSV(const std::string& csvPath);
}

