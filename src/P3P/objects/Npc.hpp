#ifndef NPC_Def
#define NPC_Def

//Include files
#include <mge/core/GameObject.hpp>
#include <mge/materials/GlitchMaterial.hpp>
#include <P3P/Quest.hpp>
#include <SFML\Graphics.hpp>
#include <P3P/Stats.hpp>

class Npc : public GameObject
{
public:
	Npc(int pX, int pZ);
	~Npc ();
	void talk();
	void displayDialog (std::string pText, bool pNpcTalking = true);
	void updateDialog (std::string pText);
	void stopDialog ();
	void addQuest (Quest * newQuest);
	void update (float pStep, bool pUpdateWorldTransform = false) override;
	std::vector<Quest*> activeQuests;
	static Npc* singletonInstance;
	bool talking = false;
	int questTalks = 0;
	int completedQuests = 0;
	int position [2] = { 0, 0 };
private:
	GameObject* _model = nullptr;
	GlitchMaterial* _material = nullptr;
	float _glitchIntensity = 1;
	int _playerPosition [2] = { -1, -1 };
	sf::Sprite* _textBox = nullptr;
	sf::Text* _text = nullptr;
	int _soundIndex = -1;
};

#endif