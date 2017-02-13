#ifndef QUEST_Def
#define QUEST_Def

#include <string>

class Quest
{
public:
	Quest(std::string pInfo, std::string pGoalItem);
	std::string _info;
	std::string _goalItem;
};

#endif