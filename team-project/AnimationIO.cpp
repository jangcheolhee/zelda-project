#include "stdafx.h"
#include "AnimationIO.h"
#include <fstream>
#include <sstream>

bool AnimationIO::LoadClipFromJson(const std::string& path, AnimationClip& clip)
{
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        std::cerr << "JSON 열기 실패: " << path << "\n";
        return false;
    }

    nlohmann::json j;
    ifs >> j;

    clip.frames.clear();
    clip.name = j.value("name", "noname");
    clip.loop = j.value("loop", true);
    clip.frames.clear();

    for (auto& f : j["frames"]) {
        FrameData frame;
        frame.rect.left = f["x"];
        frame.rect.top = f["y"];
        frame.rect.width = f["w"];
        frame.rect.height = f["h"];
        frame.duration = f.value("duration", 0.1f);
        clip.frames.push_back(frame);
    }

    std::cout << "애니메이션 로드 완료: " << path << "\n";
    return true;
}

bool AnimationIO::LoadClipFromCsv(const std::string& path, AnimationClip& clip, float defaultDuration)
{
    std::ifstream file(path);
    if (!file) return false;

    std::string line;
    std::getline(file, line); // header skip

    clip.frames.clear();

    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string value;
        // Direction,Frame,X,Y,Width,Height
        std::getline(ss, value, ','); // Direction
        std::getline(ss, value, ','); // Frame idx (unused)

        int x = 0, y = 0, w = 0, h = 0;
        std::getline(ss, value, ','); x = std::stoi(value);
        std::getline(ss, value, ','); y = std::stoi(value);
        std::getline(ss, value, ','); w = std::stoi(value);
        std::getline(ss, value, ','); h = std::stoi(value);

        FrameData fr;
        fr.rect = { x,y,w,h };
        fr.duration = defaultDuration;
        clip.frames.push_back(fr);
    }
    return true;
}

bool AnimationIO::SaveClipToJson(const AnimationClip& clip, const std::string& path)
{
    json j;
    j["name"] = clip.name;
    j["loop"] = clip.loop;
    for (const auto& f : clip.frames)
    {
        j["frames"].push_back({
            {"x", f.rect.left},
            {"y", f.rect.top},
            {"w", f.rect.width},
            {"h", f.rect.height},
            {"duration", f.duration}
            });
    }
    std::ofstream ofs(path);

    if (!ofs.is_open()) return false;
    ofs << j.dump(4);
    return true;
}
bool AnimationIO::SaveClipToCsv(const AnimationClip& clip, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << "Direction,Frame,X,Y,Width,Height\n";
    for (size_t i = 0; i < clip.frames.size(); ++i)
    {
        const FrameData& f = clip.frames[i];
        file << "None," << i << ","
            << f.rect.left << "," << f.rect.top << ","
            << f.rect.width << "," << f.rect.height << "\n";
    }
    return true;
}

bool AnimationIO::EndsWith(const std::string& s, const std::string& suffix)
{
    if (s.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin(),
        [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}
