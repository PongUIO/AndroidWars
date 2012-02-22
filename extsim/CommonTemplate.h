#ifndef EXTSIM_COMMONTEMPLATE_H
#define EXTSIM_COMMONTEMPLATE_H

namespace ExtS {
	class ExtSim;
	
	/// Allows generic retrieval of \c ExtSim subsystems
	template<class T>
	T &getExtSimComponent(ExtSim &extsim);
	
	/// Allows generic retrieval of \c ExtData databases
	template<class T>
	T &getExtDataComponent(ExtSim &extsim);
	
	/// Allows generic retrieval of \c ExtInput components
	template<class T>
	T &getExtInputComponent(ExtSim &extsim);
}

#endif
