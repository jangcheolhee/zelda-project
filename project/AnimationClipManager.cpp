#include "stdafx.h"
#include "AnimationClipManager.h"


void  AnimationClipManager::Load(const std::string& clipId, const std::string& filePath)
{
    AnimationClip clip;
    
    clip.id = clipId;
    if (clip.loadFromFile(filePath))
    {
        clips[clipId] = clip;
    }
}
AnimationClip& AnimationClipManager::Get(const std::string& clipId)
{
    return clips.at(clipId);
}
bool AnimationClipManager::Exists(const std::string& clipId)
{
    return clips.find(clipId) != clips.end();
}