#include <P3P/SceneFader.hpp>
#include <JCPPEngine/TextureManager.hpp>

SceneFader* SceneFader::singletonInstance = nullptr;

SceneFader::SceneFader(sf::RenderWindow * pWindow) : GameObject()
{
	singletonInstance = this;
	sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture("images/NoSignal.jpg"));
	sprite->setColor(sf::Color(255, 255, 255, _currentAlpha));
	_window = pWindow;
}


SceneFader::~SceneFader()
{
	delete sprite;
}

void SceneFader::update(float pStep, bool pUpdateWorldTransform)
{
	//update time paramers(i had to use sfml time functions instead of pStep to avoid some behaviours)
	float timeStep;
	time = &clock.getElapsedTime();
	timeStep = time->asSeconds() - lastTime;
	lastTime = time->asSeconds();

	GameObject::update(pStep, pUpdateWorldTransform);

	//increase alpha(make more dark)
	if (_increaseAlphaFlag)
	{
		levelIsAllowedToReload = false;

		if (_currentAlpha < MAX_COLOR_VALUE)
		{
			_currentAlpha += MAX_COLOR_VALUE / (FADE_DURATION_TIME / timeStep);
			if (_currentAlpha > MAX_COLOR_VALUE)
			{
				_currentAlpha = MAX_COLOR_VALUE;
			}
			sprite->setColor(sf::Color(MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, _currentAlpha));
		}
		else
		{
			_increaseAlphaFlag = false;
			_stayMaxAlphaFlag = true;
			
		}
	}
	//stay black
	else if (_stayMaxAlphaFlag)
	{
		_blackFor += timeStep;
		if (_blackFor > STAY_BLACK_DURATION_TIME / 2)
		{
			levelIsAllowedToReload = true;
		}
		if (_blackFor > STAY_BLACK_DURATION_TIME)
		{
			_stayMaxAlphaFlag = false;
			_blackFor = 0;
		}
	}
	//decrease alpha(make more transparent)
	else if (_decreaseAlphaFlag)
	{
		if (timeStep <= 1)
		{
			if (_currentAlpha > 0.1f)
			{
				_currentAlpha -= MAX_COLOR_VALUE / (FADE_DURATION_TIME / timeStep);
				if (_currentAlpha < 0.1f)
				{
					_currentAlpha = 0.1f;
				}
				sprite->setColor(sf::Color(MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, _currentAlpha));
			}
			else
			{
				_decreaseAlphaFlag = false;
			}
		}
	}
}

void SceneFader::fade(bool state)
{
	if (state)
	{
		levelIsAllowedToReload = false;
		_increaseAlphaFlag = true;
	}
	else
	{
		_decreaseAlphaFlag = true;
	}
}