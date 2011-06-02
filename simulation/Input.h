#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>

#include <vector>

namespace Sim {
	struct Command {
		uint32_t botId;
	};
	
	class InputBuffer {
		public:
			InputBuffer();
			~InputBuffer();
			
			void addCmd(const Command &cmd);
			void appendCmd(const InputBuffer &ref);
			void clear();
			
		private:
			typedef std::vector<Command> CmdVec;
			CmdVec mCommandList;
	};
}

#endif
