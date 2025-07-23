#pragma once
#include "stdafx.h"
#include "AnimationClip.h"
#include <map>
#include <string>

class AnimationClipManager
{
private:
	std::unordered_map<std::string, AnimationClip> clips;
public:
	void Load(const std::string& clipId, const std::string& filePath);
	AnimationClip& Get(const std::string& clipId);
	bool Exists(const std::string& clipId);

};

#define ANI_CLIP_MGR Singleton<ResourceMgr<AnimationClip>>::Instance()
