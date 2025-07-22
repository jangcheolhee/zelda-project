#pragma once
#include "Singleton.h"
enum class QuestStatus
{
	NotStarted,
	InProgress,
	Completed,
};

class Quest
{
public:
	std::string id;
	QuestStatus status;
	int step;
	Quest(const std::string& id = "")
		:id(id),status(QuestStatus::NotStarted), step (0) {}

};

class QuestMgr
{
public:
	std::unordered_map<std::string, Quest> quests;
	void AddQuest(const std::string& id);
	void StartQuest(const std::string& id);
	void CompleteQuest(const std::string& id);
	QuestStatus GetStatus(const std::string& id);

	void Save(const std::string& fileName);
	void Load(const std::string& fileName);
};
