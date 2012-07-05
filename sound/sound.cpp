#include <alure.h>

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
		alureInitDevice(0, 0);

	}

	void Sound::shutdown()
	{
		alureShutdownDevice();

	}
	
	ALuint Sound::load(const std::string& filename)
	{
		return alureCreateBufferFromFile(filename.c_str());

	}
	
	void Sound::instantPlay(ALuint id)
	{
		ALuint source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, id);
		alSourcePlay(source);

	}
}