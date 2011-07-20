#include <stdio.h>
#include "../statesys/StateSys.h"

class Test : public Sim::StateSys::State {
	public:
		Test() {}
		
		void exec(Sim::StateSys::Reference::Thread &t) {
			printf("Hello at %p (from %p)!\n", t.mActive, &t);
			printf("Our argument is %d\n", boost::any_cast<uint32_t>(t.mArg));
			t.mActive = nextState();
		}
};

int main(void)
{
	Sim::StateSys sys;
	
	// Setup state system
	sys.registerState(new Test());
	
	uint32_t baseThread = sys.registerState(new Sim::StdState::Delay(5.0) );
	sys.registerState(new Test());
	
	sys.getState(0)->insertChild(sys.getState(baseThread));
	sys.getState(baseThread)->insertChild(sys.getState(baseThread+1));
	sys.getState(baseThread+1)->insertChild(sys.getState(baseThread));
	
	sys.registerEntryPoint(0);
	
	sys.finalize();
	
	// Setup a system reference
	Sim::StateSys::Reference ref = Sim::StateSys::Reference(&sys);
	
	ref.startThread(0, uint32_t(42));
	for(int i=0; i<10; i++) {
		if(i==5)
			ref.startThread(0, uint32_t(55));
		ref.exec(1);
	}
	
	return 0;
}
