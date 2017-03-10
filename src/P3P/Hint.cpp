#include <P3P/Hint.hpp>
#include <P3P/Level.hpp>
#include <vector>

Hint::Hint(int pX, int pZ, std::string pInfo) : GameObject()
{
	//Set up model
	_model = new GameObject ();
	_model->setParent (Level::singletonInstance->transparencyLayer4);
	_model->translate (glm::vec3 (pX, 0, pZ));
	DoomSprite* sprite = new DoomSprite ("Hint.png");
	sprite->setParent (_model);
	sprite->translate (glm::vec3 (pX, 1, pZ));
	translate(glm::vec3(pX * Level::TILESIZE, 2, pZ * Level::TILESIZE));

	sf::Font * font = new sf::Font();
	font->loadFromFile("fonts/Font1.ttf");
	_text = new sf::Text(pInfo, *font);
	_text->setPosition(500, 500);

	_animator = new AnimationBehaviour({ "Hint.txt" }, false);
	_model->setBehaviour(_animator);
	_animator->playAnimation(0, true);
	setActive(false);
}


Hint::~Hint()
{
	delete _text;
	GameObject::~GameObject();
}

void Hint::setActive(bool active)
{
	if (active)
	{
		_model->setParent (Level::singletonInstance->transparencyLayer4);
	}
	else
	{
		_model->setParent (nullptr);
	}
}