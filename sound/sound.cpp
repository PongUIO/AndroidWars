#include "sound.h"

namespace Snd
{
	Sound::Sound()
	{
		

	}
	
	
	Sound::~Sound()
	{
		

	}
	
	void Sound::startup()
	{
		alutInit(0, 0);

	}

	void Sound::shutdown()
	{
		alutExit();

	}
	
	ALuint Sound::load(const std::string& filename)
	{
		return alutCreateBufferFromFile(filename.c_str());

	}
	
	void Sound::instantPlay(ALuint id)
	{
		ALuint source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, id);
		alSourcePlay(source);

	}
}