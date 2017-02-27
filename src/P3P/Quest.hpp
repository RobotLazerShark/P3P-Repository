#ifndef QUEST_Def
#define QUEST_Def

#include <string>

class Quest
{
public:
	Quest (std::string pInfo, std::string pGoalItem, float pFixIntensity);
	std::string _info;
	std::string _goalItem;
	float fixIntensity = 0;
};

#endif