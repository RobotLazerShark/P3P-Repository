#ifndef COLLECTIBLE_Def
#define COLLECTIBLE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <JCPPEngine/AbstractListener.hpp>
#include <P3P/Stats.hpp>

//We can extend this class for different types of collectables if neccesary
class Collectable : public GameObject, public JCPPEngine::AbstractListener
{
public:
	Collectable(int pX, int pZ, std::string pName, bool pMark = false);
	Collectable (int pX, int pZ, std::string pName, std::string pDialog, bool pCopyCollect, bool pMark = false);
	~Collectable ();
	std::string getName ();
	virtual bool collect (int pOldX, int pOldZ);
	virtual void update (float pStep, bool pUpdateWorldTransform = false) override;
	void ProcessEvent (JCPPEngine::Event* pEvent) override;
	bool isQuestItem ();
protected:
	std::string _name;
	bool _collected = false;
	bool _copyCollect = false;
	bool _hasDialog = false;
	bool _showingDialog = false;
	int _position [2] = { 0, 0 };
	int _playerPosition [2] = { -1, -1 };
	sf::Sprite* _textBox = nullptr;
	sf::Text* _text = nullptr;
	GameObject* _model = nullptr;
	GameObject* _mark = nullptr;
};

#endif