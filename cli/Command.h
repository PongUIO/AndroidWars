#ifndef COMMAND_H
#define COMMAND_H
#include <nepeta.h>

class Command {
	public:
		Command(){}
		~Command(){}
		
		virtual void run(const Nepeta::Node &node)=0;
		
	private:
};

#endif
