#include "../sound.h"

int main()
{
	Snd::Sound sound;
	sound.startup();
	ALuint id = sound.load("../../testmod/sound/hill.ogg");
	sound.instantPlay(id);
	sleep(5);
	return 0;
}