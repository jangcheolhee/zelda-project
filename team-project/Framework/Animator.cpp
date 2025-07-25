#include "stdafx.h"
#include "Animator.h"

const std::string Animator::emptyString = "";

void Animator::AddEvent(const std::string& id, int frame, std::function<void()> action)
{
	auto key = std::pair<std::string, int>(id, frame);
	auto it = events.find(key);
	if (it == events.end())
	{
		events.insert({ key, {id, frame} });
	}
	events[key].actions.push_back(action);
}

void Animator::Update(float dt)
{
	
	if (!isPlaying || !currentClip || totalFrame == 0 || !sprite)
		return;


	accumTime += dt * std::fabs(speed);
	if (accumTime < frameDuration)
		return;
	accumTime = 0.f;

	currentFrame += (speed >= 0.f ? +1 : -1);

	if (currentFrame >= totalFrame || currentFrame < 0)
	{
		if (!playQueue.empty())
		{
			Play(playQueue.front(), false);
			playQueue.pop();
			return;
		}

		switch (currentClip->loopType)
		{
		case AnimationLoopTypes::Single:
			currentFrame = speed > 0.f ? totalFrame - 1 : 0;
			break;
		case AnimationLoopTypes::Loop:
			currentFrame = speed > 0.f ? 0 : totalFrame - 1;
			break;
		}
	}

	// 안전 보정: 
	// totalFrame 이상이면 순환
	std::cerr
		<< "[Animator::Update] clip=" << currentClip->id
		<< " frame=" << currentFrame
		<< "/" << totalFrame
		<< "\n";
	if (currentFrame < 0 || currentFrame >= totalFrame)
	{
		std::cerr << "[Animator::Update] INVALID INDEX! frame="
			<< currentFrame << " total=" << totalFrame << "\n";
		return;
	}


	SetFrame(currentClip->frames[currentFrame]);
	// 이벤트 처리(기존 로직)
	auto it = events.find({ currentClip->id, currentFrame });
	if (it != events.end())
	{
		for (auto& action : it->second.actions)
			if (action) action();
	}
	
}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	Play(&ANI_CLIP_MGR.Get(clipId), clearQueue);
}

void Animator::Play(AnimationClip* clip, bool clearQueue)
{
	if (clearQueue)
	{
		while (!playQueue.empty())
		{
			playQueue.pop();
		}
	}
	if (!clip)
	{
		std::cerr << "[Animator] Play 호출 실패: clip 포인터 nullptr\n";
		isPlaying = false;
		return;
	}
	isPlaying = true;

	currentClip = clip;
	totalFrame = clip->frames.size();
	std::cerr
		<< "[Animator::Play] clip=" << clip->id
		<< " totalFrame=" << totalFrame
		<< "\n";
	// 빈 프레임 방어
	if (totalFrame == 0)
	{
		std::cerr << "[Animator] 빈 애니메이션: " << clip->id << "\n";
		isPlaying = false;
		return;
	}

	checkFrame = this->speed > 0.f ? totalFrame : -1;
	currentFrame = speed > 0.f ? 0 : totalFrame - 1;

	frameDuration = 1.f / clip->fps;
	accumTime = 0.f;
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	playQueue.push(clipId);
}

void Animator::Stop()
{
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	sprite->setTexture(TEXTURE_MGR.Get(frame.texId));
	sprite->setTextureRect(frame.texCoord);
	//sprite->
}
