#include <P3P/Dialog.hpp>
#include <P3P/objects/Npc.hpp>

Dialog::Dialog(std::string pText, Dialog * pDialogToGive, Quest* pQuestToGive)
{
	_text = pText;
	_dialogToGive = pDialogToGive;
	_questToGive = pQuestToGive;
}

//returns dialog message and adds a quest/dialog if possible
std::string Dialog::talk()
{
	if (_questToGive != nullptr)
	{
		Npc::givenQuests.push_back(_questToGive);
	}
	if (_dialogToGive != nullptr)
	{
		Npc::untalkedDialogs.push_back(_dialogToGive);
	}
	
	talked = true;

	return _text;
}