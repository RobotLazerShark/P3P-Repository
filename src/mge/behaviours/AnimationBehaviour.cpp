#include <mge/behaviours/AnimationBehaviour.hpp>
#include <mge/core/GameObject.hpp>


//Constructor that reads keyframe animations from the given files
AnimationBehaviour::AnimationBehaviour (std::vector <std::string> pAnimFiles) : AbstractBehaviour ()
{
	for (int i = 0, size = pAnimFiles.size (); i < size; i ++)
	{
		std::string filepath = "mge/animations/" + pAnimFiles [i];
		std::vector <KeyFrame> animation;
		//Import animation data
		std::ifstream file (filepath, std::ios::in);
		if (file.is_open ())
		{
			std::string line;
			std::string readString;
			int strPos;
			KeyFrame frame;
			bool reading = true;
			while (reading)
			{
				getline (file, line);
				if (line.find ("time") != std::string::npos)//Read frame time
				{
					int stringSize = line.length ();
					strPos = (int)line.find ("=");
					strPos ++;
					readString = "";
					while (line [strPos] != ':' && strPos < stringSize)//Read seconds
					{
						readString += line [strPos];
						strPos ++;
					}
					frame.time = std::stof (readString);
					strPos ++;
					readString = "";
					while (strPos < stringSize)//Read milliseconds
					{
						readString += line [strPos];
						strPos ++;
					}
					if (readString.length () > 0)
					{
						frame.time += std::stof (readString) / 60.0f;
					}
				}
				else if (line.find ("position") != std::string::npos)
				{
					glm::vec3 pos;
					strPos = (int)line.find ("=");
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read x value
					{
						readString += line [strPos];
						strPos ++;
					}
					pos.x = -1 * std::stof (readString);//Convert from Unity's coöridnate system
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read y value
					{
						readString += line [strPos];
						strPos ++;
					}
					pos.y = std::stof (readString);
					strPos ++;
					readString = "";
					while (strPos < line.length ())//Read z value
					{
						readString += line [strPos];
						strPos ++;
					}
					pos.z = -1 * std::stof (readString);//Convert from Unity's coöridnate system
					frame.position = pos;
				}
				else if (line.find ("rotation") != std::string::npos)
				{
					glm::vec3 angles;
					strPos = (int)line.find ("=");
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read x value
					{
						readString += line [strPos];
						strPos ++;
					}
					angles.x = -1 * std::stof (readString);//Convert from Unity's coördinate system
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read y value
					{
						readString += line [strPos];
						strPos ++;
					}
					angles.y = -1 * std::stof (readString);//Convert from Unity's coördinate system
					strPos ++;
					readString = "";
					while (strPos < line.length ())//Read z value
					{
						readString += line [strPos];
						strPos ++;
					}
					angles.z = std::stof (readString);
					frame.rotation = angles;
				}
				else if (line.find ("scale") != std::string::npos)
				{
					glm::vec3 scale;
					strPos = (int)line.find ("=");
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read x value
					{
						readString += line [strPos];
						strPos ++;
					}
					scale.x = std::stof (readString);
					strPos ++;
					readString = "";
					while (line [strPos] != ',')//Read y value
					{
						readString += line [strPos];
						strPos ++;
					}
					scale.y = std::stof (readString);
					strPos ++;
					readString = "";
					while (strPos < line.length ())//Read z value
					{
						readString += line [strPos];
						strPos ++;
					}
					scale.z = std::stof (readString);
					frame.scale = scale;
				}
				else if (line [0] == '-')//Check for end of file mark
				{
					animation.push_back (frame);
					if (line [1] == '-')
					{
						reading = false;
						_animations.push_back (animation);
						KeyFrame newFrame;
						frame = newFrame;
					}
				}
			}
		}
		else
		{
			std::cout << "[ERROR]: could not import animation from file '" << filepath << "'!" << std::endl;
		}
	}
}


//Update animation (if one is playing)
void AnimationBehaviour::update (float pStep)
{
	if (_playing)
	{
		KeyFrame prevFrame = _animations [_currentAnimation] [_lastFrame];
		KeyFrame nextFrame = _animations [_currentAnimation] [(_lastFrame + 1) % _animations [_currentAnimation].size ()];

		float interpolation = (_time - prevFrame.time) / (nextFrame.time - prevFrame.time);
		//Build a matrix by interpolating values from keyframes
		glm::vec3 interpVec = interpolation * nextFrame.position + (1 - interpolation) * prevFrame.position;
		glm::mat4 newTransform = glm::translate (interpVec);
		interpVec = interpolation * nextFrame.rotation + (1 - interpolation) * prevFrame.rotation;
		newTransform *= glm::rotate (glm::radians (interpVec.y), glm::vec3 (0,1,0)) * glm::rotate (glm::radians (interpVec.x), glm::vec3 (1,0,0)) * glm::rotate (glm::radians (interpVec.z), glm::vec3 (0,0,1));
		interpVec = interpolation * nextFrame.scale + (1 - interpolation) * prevFrame.scale;
		newTransform = glm::scale (newTransform, interpVec);
		//Apply calculated transform
		_owner->setTransform (newTransform);
		
		_time += pStep;
		if (_time >= nextFrame.time)
		{
			int frameCount = _animations [_currentAnimation].size ();
			_lastFrame = (_lastFrame + 1) % frameCount;
			if (_lastFrame == (frameCount - 1))
			{
				_time = 0;
				if (!_loop)
				{
					stopAnimation ();
					return;
				}
			}
		}
	}
}


//////////////////////////////|	SETTERS
//Start playing an animation. If one is already playing, it will be stopped.
void AnimationBehaviour::playAnimation (int pAnimIndex, bool pLoop, void(*pFuncPtr) (int))
{
	if (_playing)
	{
		stopAnimation ();
	}
	_stopFunction = pFuncPtr;
	_normalTransform = _owner->getTransform ();
	KeyFrame start = _animations [pAnimIndex] [0];
	glm::mat4 newTransform = glm::translate (start.position);
	newTransform *= glm::rotate (glm::radians (start.rotation.y), glm::vec3 (0,1,0)) * glm::rotate (glm::radians (start.rotation.x), glm::vec3 (1,0,0)) * glm::rotate (glm::radians (start.rotation.z), glm::vec3 (0,0,1));
	newTransform = glm::scale (newTransform, start.scale);
	_owner->setTransform (newTransform);
	_playing = true;
	_currentAnimation = pAnimIndex;
	_lastFrame = 0;
	_loop = pLoop;
}

//Stop playing the animation, if one is playing.
void AnimationBehaviour::stopAnimation ()
{
	_playing = false;
	//Reset transform
	_owner->setTransform (_normalTransform);
	_normalTransform = glm::mat4 ();
	//Call stopfunction
	if (_stopFunction != nullptr)
	{
		_stopFunction (_currentAnimation);
	}
	_currentAnimation = -1;
	_stopFunction = nullptr;
}


//////////////////////////////|	GETTERS
//Get whether or not there is currently an animation playing
bool AnimationBehaviour::isPlaying ()
{
	return _playing;
}

//Get whether or not a specific animation is playing
bool AnimationBehaviour::isPlaying (int pAnimIndex)
{
	return (_playing && (_currentAnimation == pAnimIndex));
}

//Get the index of the currently playing animation (-1 if no animation is playing).
int AnimationBehaviour::currentAnimation ()
{
	return _currentAnimation;
}

//Get whether or not the current animation is looped
bool AnimationBehaviour::looping ()
{
	return _loop;
}

//Get the duration in seconds of a given animation
float AnimationBehaviour::getDuration (int pAnimIndex)
{
	return _animations [pAnimIndex] [_animations [pAnimIndex].size () - 1].time;
}