#include "stdafx.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include <algorithm>





void AnimationPlayer::SetCurrentFrameIndex(int index)
{
    if (!clip || clip->frames.empty())
        return;

    currentFrameIndex = std::clamp(index, 0, static_cast<int>(clip->frames.size()) - 1);
    elapsedTime = 0.f;
}

const FrameData& AnimationPlayer::GetCurrentFrame() const
{
    static FrameData dummy = { sf::IntRect(0, 0, 0, 0), 0.f };

    if (!clip || clip->frames.empty())
        return dummy;

    return clip->frames[currentFrameIndex];
}

void AnimationPlayer::SetClip(AnimationClip* newClip)
{
    this->clip = newClip;
    currentFrameIndex = 0;
    elapsedTime = 0.f;
}

sf::IntRect AnimationPlayer::GetCurrentFrameRect() const
{
    if (this == nullptr || !clip || clip->frames.empty())
        return sf::IntRect();

    return clip->frames[currentFrameIndex].rect;
}
