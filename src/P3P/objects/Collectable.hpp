#ifndef COLLECTIBLE_Def
#define COLLECTIBLE_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <JCPPEngine/AbstractListener.hpp>

//We can extend this class for different types of collectables if neccesary
class Collectable : public GameObject, public JCPPEngine::AbstractListener
{
public:
	Collectable(int pX, int pZ, std::string pName);
	Collectable (int pX, int pZ, std::string pName, std::string pDialog, bool pCopyCollect);
	~Collectable ();
	std::string getName ();
	virtual bool collect (int pOldX, int pOldZ);
	virtual void update (float pStep, bool pUpdateWorldTransform = false) override;
	void ProcessEvent (JCPPEngine::Event* pEvent) override;
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
};

#endif