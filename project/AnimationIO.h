#pragma once
#include "AnimationClip.h"
class AnimationIO
{
protected:
public:

	static bool LoadClipFromJson(const std::string& path, AnimationClip& clip);
	static bool LoadClipFromCsv(const std::string& path, AnimationClip& clip, float defaultDuration=0.1f);
	// 저장 
	static bool SaveClipToJson(const AnimationClip& clip, const std::string& path);
	static bool SaveClipToCsv(const AnimationClip& clip, const std::string& path);
	// 문자열 유틸
	static bool EndsWith(const std::string& s, const std::string& suffix);
};

