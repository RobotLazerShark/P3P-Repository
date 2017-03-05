#ifndef SoundManager_Def
#define SoundManager_Def

//Includes
#include <map>
#include <SFML/Audio.hpp>


namespace JCPPEngine
{
	class SoundManager
	{
		public:
			static sf::SoundBuffer* GetBuffer (std::string pSoundFile);
			static void PlaySound (sf::Sound* pSound);
			static void PlayMusic (std::string pMusicFile);
			static int PlaySoundLoop (sf::Sound* pSound);
			static int PlayMusicLoop (std::string pMusicFile);
			static int GetSoundLoopCount ();
			static int GetMusicLoopCount ();
			static void StopSoundLoop (int pSoundIndex);
			static void StopMusicLoop (int pMusicIndex);
			static void Update ();
			static void Clean ();
	};
}

#endif