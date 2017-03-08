#include "SoundManager.hpp"
#include <iostream>


namespace JCPPEngine
{

	//Static fields
	static std::map <std::string, sf::SoundBuffer> _buffers;
	static std::vector <sf::Sound*> _sounds;
	static std::vector <sf::Music*> _music;
	static const int MAXLOOPING = 5;
	static sf::Sound* _loopingSounds [MAXLOOPING] = { nullptr, nullptr, nullptr, nullptr, nullptr };
	static sf::Music* _loopingMusic [MAXLOOPING] = { nullptr, nullptr, nullptr, nullptr, nullptr};


	//If necessary, creates requested soundbuffer and then returns it
	sf::SoundBuffer* SoundManager::GetBuffer (std::string pSoundFile)
	{
		if (_buffers.count (pSoundFile) == 0)
		{
			_buffers [pSoundFile] = sf::SoundBuffer ();
			if (!_buffers [pSoundFile].loadFromFile (pSoundFile))
			{
				std::cout<<"[ERROR]: could not load the sound '"<<pSoundFile<<"'!"<<std::endl;
				return nullptr;
			}
		}
		return &_buffers [pSoundFile];
	}


	//////////////////////////////|	UPDATING FUNCTIONS
	//Start playing a new sound
	void SoundManager::PlaySound (sf::Sound* pSound)
	{
		pSound->setLoop (false);
		_sounds.push_back (pSound);
		pSound->play ();
	}
	//Start playing a new music
	void SoundManager::PlayMusic (std::string pMusicFile)
	{
		sf::Music* music = new sf::Music ();
		music->openFromFile (pMusicFile);
		music->setLoop (false);
		_music.push_back (music);
		music->play ();
	}
	//Start playing a looping sound
	int SoundManager::PlaySoundLoop (sf::Sound* pSound)
	{
		pSound->setLoop (true);
		for (int i = 0, size = MAXLOOPING; i < size; i ++)
		{
			if (_loopingSounds [i] == nullptr)
			{
				_loopingSounds [i] = pSound;
				pSound->play ();
				return i;
			}
		}
		//If we got here, all looping-sound-slots are occupied.
		std::cout << "[ERROR]: All Sound loop-slots are occupied." << std::endl;
		return -1;
	}
	//Start playing a looping music
	int SoundManager::PlayMusicLoop (std::string pMusicFile)
	{
		for (int i = 0, size = MAXLOOPING; i < size; i ++)
		{
			if (_loopingMusic [i] == nullptr)
			{
				_loopingMusic [i] = new sf::Music ();
				if (!_loopingMusic [i]->openFromFile (pMusicFile))
				{
					return -1;
				}
				_loopingMusic [i]->setLoop (true);
				_loopingMusic [i]->setVolume (150);
				_loopingMusic [i]->play ();
				return i;
			}
		}
		//If we got here, all looping-music-slots are occupied.
		std::cout << "[ERROR]: All Music loop-slots are occupied." << std::endl;
		return -1;
	}
	//Get how many sounds are looping
	int SoundManager::GetSoundLoopCount ()
	{
		for (int i = 0, size = MAXLOOPING; i < size; i ++)
		{
			if (_loopingSounds [i] == nullptr)
			{
				return i;
			}
		}
		return MAXLOOPING;
	}
	//Get how many music's are looping
	int SoundManager::GetMusicLoopCount ()
	{
		for (int i = 0, size = MAXLOOPING; i < size; i ++)
		{
			if (_loopingMusic [i] == nullptr)
			{
				return i;
			}
		}
		return MAXLOOPING;
	}
	//Stop playing a looping sound
	void SoundManager::StopSoundLoop (int pSoundIndex)
	{
		if (_loopingSounds [pSoundIndex] == nullptr)
		{
			std::cout << "<warning>: attempting to stop a non-existant/already stopped soundloop." << std::endl;
			return;
		}
		_loopingSounds [pSoundIndex]->stop ();
		delete _loopingSounds [pSoundIndex];
		_loopingSounds [pSoundIndex] = nullptr;
	}
	//Stop playing a looping music
	void SoundManager::StopMusicLoop (int pMusicIndex)
	{
		if (_loopingMusic [pMusicIndex] == nullptr)
		{
			std::cout << "<warning>: attempting to stop a non-existant/already stopped musicloop." << std::endl;
			return;
		}
		_loopingMusic [pMusicIndex]->stop ();
		delete _loopingMusic [pMusicIndex];
		_loopingMusic [pMusicIndex] = nullptr;
	}
	//Keep track of sounds that are playing
	void SoundManager::Update ()
	{
		if (_sounds.size () > 0)
		{
			for (int i = _sounds.size () - 1; i >= 0; i--)
			{
				if (_sounds [i]->getStatus () != sf::Sound::Status::Playing)
				{
					delete _sounds [i];
					_sounds [i] = nullptr;
					_sounds.erase (_sounds.begin () + i);
				}
			}
		}
		if (_music.size () > 0)
		{
			for (int i = _music.size () - 1; i >= 0; i--)
			{
				if (_music [i]->getStatus () != sf::Music::Status::Playing)
				{
					delete _music [i];
					_music [i] = nullptr;
					_music.erase (_music.begin () + i);
				}
			}
		}
	}


	//Clean out all used memory
	void SoundManager::Clean ()
	{
		//Clear all non-looping audio
		if (_sounds.size () > 0)
		{
			for (int i = _sounds.size () - 1; i >= 0; i --)
			{
				_sounds [i]->stop ();
				delete _sounds [i];
				_sounds [i] = nullptr;
			}
			_sounds.clear ();
		}
		if (_music.size () > 0)
		{
			for (int i = _music.size () - 1; i >= 0; i --)
			{
				_music [i]->stop ();
				delete _music [i];
				_music [i] = nullptr;
			}
			_music.clear ();
		}
		//Clear all looping audio
		if (MAXLOOPING > 0)
		{
			for (int i = MAXLOOPING - 1; i >= 0; i --)
			{
				if (_loopingSounds [i] != nullptr)
				{
					_loopingSounds [i]->stop ();
					delete _loopingSounds [i];
					_loopingSounds [i] = nullptr;
				}
				if (_loopingMusic [i] != nullptr)
				{
					_loopingMusic [i]->stop ();
					delete _loopingMusic [i];
					_loopingMusic [i] = nullptr;
				}
			}
		}
	}

}