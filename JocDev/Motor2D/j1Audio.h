#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"
#include "Slider.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void UnLoadFx();

	int SetVolume(float vol);
	int SetFXVolume(float vol);
	int RaiseVolume();
	int DecreaseVolume();
	int GetVolume() const
	{
		return music_volume;
	}
	int GetVolumeFX() const
	{
		return fx_volume;
	}
	void AdjustSliderVolume(Slider* sld);
	void AdjustFXSliderVolume(Slider* sld);
private:

	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
	int music_volume = 100;
	int fx_volume = 80;
};

#endif // __j1AUDIO_H__