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

struct FrameData
{
	sf::IntRect rect;
	float duration;
	sf::Sprite* sprite = nullptr;
};

struct AnimationClip
{
	std::string name;
	bool loop = true;

	std::string id;
	int fps = 30;
	AnimationLoopTypes loopType = AnimationLoopTypes::Loop;
	std::vector<FrameData> frames;

	bool loadFromFile(const std::string& filePath);
	//std::vector<AnimationFrame> frames;

	
};

