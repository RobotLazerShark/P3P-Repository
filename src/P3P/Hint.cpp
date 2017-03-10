#include <P3P/Hint.hpp>
#include <P3P/Level.hpp>

Hint::Hint(int pX, int pZ, std::string pInfo) : GameObject()
{
	//Set up model
	_model = new DoomSprite ("Hint.png");
	GameObject* offset = new GameObject ();
	offset->translate (glm::vec3 (pX, 2, pZ));
	_model->setParent (offset);
//	_model->setParent(Level::singletonInstance->transparencyLayer4);
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