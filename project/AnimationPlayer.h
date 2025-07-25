#pragma once
#include "stdafx.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"

class AnimationPlayer
{
private:
    const AnimationClip* clip;
    float animTimer = 0.f;
    size_t currentFrame = 0;
    bool playing = false;
    int currentFrameIndex = 0;
    float elapsed = 0.f;
    float elapsedTime = 0.f;
   


public:
    AnimationPlayer(const AnimationClip* clip) : clip(clip),currentFrameIndex(0) {}
    void SetCurrentFrameIndex(int index);
    void Play() { playing = true; animTimer = 0.f; currentFrame = 0; }
    void Stop() { playing = false; }
    bool IsPlaying() const { return playing; }

    void NextFrame() {
        if (!clip || clip->frames.empty()) return;
        currentFrameIndex = (currentFrameIndex + 1) % clip->frames.size();
    }

    void PrevFrame() {
        if (!clip || clip->frames.empty()) return;
        currentFrameIndex = (currentFrameIndex - 1 + clip->frames.size()) % clip->frames.size();
    }
    const FrameData& GetCurrentFrame() const;
    void SetClip(AnimationClip* newClip);
    
    void Update(float dt) 
    {
     
    }
   
    sf::IntRect GetCurrentFrameRect() const;

   

    int GetCurrentFrameIndex() const { return currentFrameIndex; }
};

