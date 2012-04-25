#ifndef SIM_COMMONTEMPLATE_H
#define SIM_COMMONTEMPLATE_H

#include "data/BaseData.h"

namespace Sim {
	class Simulation;
	class State;
	
	/// Allows generic retrieval of \c Simulation subsystems.
	template<class T>
	T &getSimulationComponent(Simulation &sim);
	
	/// Allows for generic retrieval of \c State components.
	template<class T>
	T &getStateComponent(Simulation &sim);
	
	/// Allows for generic retrieval of \c Data components.
	template<class T>
	T &getDataComponent(Simulation &sim);
	
	/// Allows for generic retrieval of \c Input components.
	template<class T>
	T &getInputComponent(Simulation &sim);
	
	/**
	 * Allows generic retrieval of database objects based on a type.
	 * 
	 * @param V Either a \c std::string or \c IdType value to lookup.
	 */
	template<class T, class V>
	const typename T::TypeDatabase::Type *
	getDataObjFromType(Simulation &sim, V val)
	{ return getDataComponent<typename T::TypeDatabase>(sim).getType(val); }
}

#endif
