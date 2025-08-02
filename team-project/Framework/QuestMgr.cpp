#include "stdafx.h"
#include "QuestMgr.h"
void QuestMgr::AddQuest(const std::string& id) 
{
	quests.emplace(id, Quest(id));
}
void QuestMgr::StartQuest(const std::string& id)
{
	quests[id].status = QuestStatus::InProgress;
	quests[id].step = 1;
}
void  QuestMgr::CompleteQuest(const std::string& id)
{
	quests[id].status = QuestStatus::Completed;

}
QuestStatus  QuestMgr::GetStatus(const std::string& id) 
{
	return quests.count(id) ? quests[id].status : QuestStatus::NotStarted;
}
void QuestMgr::Save(const std::string& fileName)
{
	
}
void QuestMgr::Load(const std::string& fileName)
{

}