#include <P3P/Quest.hpp>

Quest::Quest (std::string pInfo, std::string pGoalItem, float pFixIntensity)
{
	_info = pInfo;
	_goalItem = pGoalItem;
	fixIntensity = pFixIntensity;
}
