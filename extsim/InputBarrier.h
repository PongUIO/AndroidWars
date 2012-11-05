#ifndef EXTSIM_INPUTBARRIER_H
#define EXTSIM_INPUTBARRIER_H

#include <vector>

#include "../simulation/Save.h"

#include "ExtModule.h"

namespace exts {
	class ExtSim;
	class ParamList;
	
	/**
	 * @brief Stores a list of \c ParamList objects.
	 */
	class InputList : public Sim::Save::OperatorImpl<InputList> {
		public:
			typedef std::vector<ParamList*> ParamListVec;
			
			InputList(ExtSim &esim);
			~InputList();
			
			bool registerInput(ParamList *param);
			void inheritList(InputList &source);
			void copyList(const InputList &source);
			
			void discardInput();
			
			const ParamListVec &getParamList() const
			{ return mInput; }
			
			void save(Sim::Save::BasePtr &fp) const;
			void load(Sim::Save::BasePtr &fp);
			
		private:
			ExtSim &mExtSim;
			ParamListVec mInput;
	};
	
	/**
	 * @brief Filters input from external sources and sends it to the
	 * simulation.
	 * 
	 * This class is designed to be the middle ground where external sources
	 * place their input, before it is distributed and managed further by
	 * \c ExtSim and the \c Simulation itself.
	 * 
	 * Firstly, all input passed through here is constrained based on their
	 * \c TypeRule object. If the input passes the constraint, it is
	 * temporarily stored in the \c InputBarrier until input is finalized.
	 * 
	 * When input is finalized, the input is first copied to the
	 * \c ReplayManager, then each \c ParamList object calls its corresponding
	 * \c TypeRule object to construct input in the \c Simulation.
	 */
	class InputBarrier : public ExtModule {
		public:
			/// @name Initialization
			//@{
				InputBarrier(ExtSim &esim);
				~InputBarrier();
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Interface
			//@{
				// Access
				InputList &getInputList() { return mInput; }
				const InputList &getInputList() const { return mInput; }
				
				// Utility functions
				void copyInput(const InputList &il);
				void applyInput(InputList &il);
				void dispatchInput(bool saveReplay=true);
				
				// Basic input interface
				bool registerInput(ParamList *param);
				
				void feedInput();
				void commitReplay();
				void postProcessInput();
				void discardInput();
				
				// Save/load
				void saveInput(Sim::Save::BasePtr &fp) const;
				void loadInput(Sim::Save::BasePtr &fp);
				
				void loadInput(const Sim::Save &save);
			//@}
			
		private:
			ExtSim &mExtSim;
			InputList mInput;
	};
}

#endif
