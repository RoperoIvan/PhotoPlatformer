#ifndef __J1FADETOBLACK_H__
#define  __J1FADETOBLACK_H__

#include "j1Module.h "
#include "SDL/include/SDL_rect.h "

class  j1FadetoBlack : public j1Module
{
public:
	j1FadetoBlack();
	~j1FadetoBlack();

	bool Start();
	bool PostUpdate(float id) override;
	void NewLevel();
	bool StartfadetoBlack(float time = 2.0f);
	bool IsFading() const;


private:

	enum class fade_step
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	};
	fade_step current_step = fade_step::NONE;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	float normalized = 0.0F;
};

#endif  