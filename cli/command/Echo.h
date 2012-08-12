#ifndef ECHO_H
#define ECHO_H
#include <nepeta.h>
#include <iostream>
#include "../Command.h"

class Echo : public Command {
	public:
		void run(const Nepeta::Node& node)
		{
			std::cout << node.getArg(0) << std::endl;
		}
};

#endif
