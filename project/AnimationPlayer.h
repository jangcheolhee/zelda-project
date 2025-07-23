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

    void SetClip(AnimationClip* newClip);
    
    void Update(float dt) 
    {
        /*if (!playing || !clip || clip->frames.empty()) return;

        animTimer += dt;
        if (animTimer >= clip->frames[currentFrame].duration) {
            animTimer = 0.f;
            currentFrame++;
            if (currentFrame >= clip->frames.size()) {
                if (clip->loop) currentFrame = 0;
                else playing = false;
            }
        }*/
    }

    sf::IntRect GetCurrentFrameRect() const
    {
        if (!clip || clip->frames.empty()) return sf::IntRect();
        return clip->frames[currentFrameIndex].rect;
    }

   

    int GetCurrentFrameIndex() const { return currentFrameIndex; }
};

