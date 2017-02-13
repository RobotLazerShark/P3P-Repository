#ifndef DIALOG_Def
#define DIALOG_Def

#include <string>
#include <P3P/Quest.hpp>


class Dialog
{
public:
	Dialog(std::string pText, Dialog * pDialogToGive, Quest* pQuestToGive);
	std::string talk();
	bool talked = false;
private:
	std::string _text;
	Dialog* _dialogToGive;
	Quest* _questToGive;
};

#endif