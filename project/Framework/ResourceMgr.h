#pragma once
#include "Singleton.h"

template <typename T>
class ResourceMgr : public Singleton<ResourceMgr<T>>
{
	friend Singleton<ResourceMgr<T>>;

protected:
	ResourceMgr() = default;
	virtual ~ResourceMgr()
	{
		for (auto pair : resources)
		{
			delete pair.second;
		}
		resources.clear();
	}

	ResourceMgr(const ResourceMgr&) = delete;
	ResourceMgr* operator=(const ResourceMgr&) = delete;

	std::unordered_map<std::string, T*> resources;

	static T Empty;

public:



	bool Load(const std::string& path)
	{
		if (Exists(path))
			return false;

		T* res = new T();
		if (!res->loadFromFile(path))
		{
			delete res;
			return false;
		}

		resources.insert({ path, res });
		return true;
	}

	bool Unload(const std::string& id)
	{
		auto it = resources.find(id);
		if (it == resources.end())
		{
			return false;
		}

		delete it->second;
		resources.erase(it);
		return true;
	}

	void Load(const std::vector<std::string>& ids)
	{
		for (auto id : ids)
		{
			Load(id);
		}
	}
	/*void Load(const std::vector<std::pair<std::string, std::string>>& idPathPairs)
	{
		for (const auto& pair : idPathPairs)
		{
			Load(pair.first, pair.second);
		}
	}*/

	void Unload(const std::vector<std::string>& ids)
	{
		for (auto id : ids)
		{
			Unload(id);
		}
	}

	T& Get(const std::string& id) 
	{
		auto it = resources.find(id);
		if (it == resources.end())
		{
			std::cerr << "Resource not found: " << id << std::endl;
			return Empty;
		}
		return *(it->second);
	}
	
	bool Exists(const std::string& id) const
	{
		return resources.find(id) != resources.end();
	}
};

template<typename T>
T ResourceMgr<T>::Empty;

#define TEXTURE_MGR (ResourceMgr<sf::Texture>::Instance())
#define FONT_MGR (ResourceMgr<sf::Font>::Instance())
#define SOUNDBUFFER_MGR (ResourceMgr<sf::SoundBuffer>::Instance())
#define ANI_CLIP_MGR (ResourceMgr<AnimationClip>::Instance())