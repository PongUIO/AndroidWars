#ifndef EXTSIM_EXTINPUT_H
#define EXTSIM_EXTINPUT_H

#include "../simulation/Common.h"
#include "../simulation/Save.h"
#include "../simulation/Input.h"
#include "../simulation/utility/CallGroup.h"

namespace ExtS {
	class ExtSim;
	class ExtInput;
	
	enum InputConstraintMode {
		/**
		 * No constraints are enforced.
		 * Input should never be in this mode for any data
		 * sent by a client.
		 */
		IcmNone = 0,
		
		/**
		 * Input is constrained to only program allocation
		 * and cpu input, and cpu input may only be passed to
		 * the active player's bots.
		 * 
		 * All program allocations must be registered to a CPU to
		 * be valid according to the constraints.
		 */
		IcmPlayerInput,
		
		/**
		 * Input is constrained to bot creation, ability creation,
		 * weapon creation, and player setup input. But all are
		 * constrained in such a way that each creation takes up
		 * player "cost". Creation is only possible until all cost
		 * has expired.
		 */
		IcmPlayerSetup,
		
		IcmMax
	};
	
	class ExtInputObj {
		public:
			ExtInputObj(ExtInput &parent) : mParent(parent) {}
			virtual ~ExtInputObj() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			/**
			 * Sends serialized input to this input object.
			 */
			virtual void loadInput(Sim::Save::BasePtr &fp,
				InputConstraintMode mode)=0;
			
			/**
			 * Serializes input from this input object.
			 */
			virtual void saveInput(Sim::Save::BasePtr &fp)=0;
			
			/**
			 * Finalize input by sending it to the simulation.
			 */
			virtual void finalizeInput()=0;
			
			/**
			 * Discards all buffered input.
			 */
			virtual void discardInput()=0;
			
			/**
			 * Returns a value identifying the number of input
			 * elements that violated constraints. What this number counts
			 * is left up to the implementing class.
			 */
			virtual uint32_t getLastConstraintViolationCount()=0;
			
		protected:
			ExtInput &mParent;
	};
	
	/**
	 * Generic input manager for factories.
	 */
	template<class T>
	class ExtFactoryInput : public ExtInputObj {
		public:
			ExtFactoryInput(ExtInput& parent, Sim::Simulation *sim) :
				ExtInputObj(parent), mViolationCounter(0), mBuffer(sim) {}
			virtual ~ExtFactoryInput() {}
			
			void startup() { mBuffer.startup(); }
			void shutdown() { mBuffer.shutdown(); }

			void loadInput(Sim::Save::BasePtr& fp, InputConstraintMode mode) {
				// Input is always valid if no constraints are enforced.
				if(!(mode==IcmNone || isValidMode(mode)))
					return;
				
				uint32_t inputCount;
				fp >> inputCount;
				for(uint32_t i=0; i<inputCount; ++i) {
					Sim::Save tmp;
					fp >> tmp;
					
					// Build the input object from the source factory
					Sim::Save::FilePtr inptr = Sim::Save::FilePtr(tmp);
					typename T::Type *obj = buildObject(inptr);
					
					// Check that its constraints is valid
					bool isConstrained = isObjectConstrained(obj);
					
					if(isConstrained)
						mBuffer.registerInput(tmp);
					else
						mViolationCounter++;
					
					// Perform cleanup
					delete obj;
				}
			}
			void saveInput(Sim::Save::BasePtr& fp) {
			}
			
			void finalizeInput() {
				mBuffer.dispatchInput();
				mViolationCounter = 0;
			}
			void discardInput() {
			}

			uint32_t getLastConstraintViolationCount()
			{ return mViolationCounter; }

			virtual bool isObjectConstrained(typename T::Type *obj)=0;
			virtual bool isValidMode(InputConstraintMode mode)=0;
			virtual typename T::Type *buildObject(Sim::Save::BasePtr &fp)=0;
			
		private:
			uint32_t mViolationCounter;
			Sim::FactoryInput<T> mBuffer;
	};
	
	/**
	 * Manages input of program objects.
	 */
	class ExtProgramInput : public ExtFactoryInput<Sim::ProgramFactory> {
		public:
			ExtProgramInput(ExtInput& parent, Sim::Simulation *sim) :
				ExtFactoryInput<Sim::ProgramFactory>(parent,sim) {}
			virtual ~ExtProgramInput() {}
			
			bool isObjectConstrained(Sim::Program *prog);
			bool isValidMode(InputConstraintMode mode)
			{ return mode==IcmPlayerInput; }
			Sim::Program* buildObject(Sim::Save::BasePtr &fp);
			
		private:
	};
	
	/**
	 * @brief Manages extended behaviour for simulation input.
	 * 
	 * The basic goal of this class is to enforce any constraints that
	 * the extended simulation has.
	 * 
	 * Input is always constrained based on the active input mode.
	 * 
	 * Input is passed as serialized data. \c ExtInput mirrors some of the
	 * \c Simulation input systems and provides extended behaviour for them
	 * (primarily constraints-checking). For factory input this means making
	 * a proto-object from the input and checking that for constraints.
	 */
	class ExtInput {
		public:
			ExtInput(ExtSim &esim);
			~ExtInput();
			
			void setConstraintMode(InputConstraintMode mode,
				Sim::IdType playerId=Sim::NoId)
			{ mConstraintMode=mode; mPlayerId=playerId; }
			
			ExtSim &getExtSim() { return mExtSim; }
			
		private:
			ExtSim &mExtSim;
			
			/// @name Input managers
			//@{
				ExtProgramInput mProgram;
			//@}
			
			/// @name Constraint mode
			//@{
				InputConstraintMode mConstraintMode;
				Sim::IdType mPlayerId;
			//@}
	};
}

#endif
