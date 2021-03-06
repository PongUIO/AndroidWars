#ifndef SOUND_H
#define SOUND_H

#include <al.h>
#include <string>

namespace Snd
{
	class Sound
	{
		public:
			Sound();
			~Sound();
			
			ALuint load(const std::string &filename);
			void instantPlay(ALuint id);
			
			void startup();
			void shutdown();

		private:
			
			

	};
}
#endif