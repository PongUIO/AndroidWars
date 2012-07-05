#include "../sound.h"

int main()
{
	Snd::Sound sound;
	sound.startup();
	ALuint id = sound.load("../../testmod/sound/hill.ogg");
	sound.instantPlay(id);
	return 0;
}