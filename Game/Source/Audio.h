#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define MAX_VOLUME 128

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	//Unload FX 
	void UnloadFx();

	//Unload Music 
	void UnloadMusic();

	//Music Volume Control
	void MusicVolume(int vol);

	//FX Volume Control
	void FxVolume(int vol);

	int GetMusicVolume();

	int GetFxVolume();

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
	int currentMusicVolume;
	int currentFxVolume;
};

#endif  __AUDIO_H__