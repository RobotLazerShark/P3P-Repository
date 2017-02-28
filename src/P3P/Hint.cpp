#include <P3P/Hint.hpp>
#include <P3P/Level.hpp>

Hint::Hint(int pX, int pZ, std::string pInfo) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->setMaterial(new LitMaterial("Hint.jpg"));
	_model->setParent(this);
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
		translate(glm::vec3(0, -10, 0));
	}
	else
	{
		translate(glm::vec3(0, 10, 0));
	}
}