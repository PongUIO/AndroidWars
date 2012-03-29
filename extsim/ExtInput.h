#ifndef EXTSIM_EXTINPUT_H
#define EXTSIM_EXTINPUT_H

#include "../simulation/Common.h"
#include "../simulation/Save.h"
#include "../simulation/Input.h"
#include "../simulation/utility/CallGroup.h"

#include "TypeRule.h"

#include "CommonTemplate.h"

namespace ExtS {
	class ExtSim;
	class ExtInput;
	
	class ExtProgramData;
	class ExtBotData;
	
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
			ExtInputObj(ExtInput &parent) : mParent(&parent) {}
			virtual ~ExtInputObj() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			/**
			 * Sends serialized input to this input object.
			 */
			virtual void loadInput(Sim::Save::BasePtr &fp)=0;
			
			/**
			 * Serializes input from this input object.
			 */
			virtual void saveInput(Sim::Save::BasePtr &fp)=0;
			
			/**
			 * Dispatch input by sending it to the simulation.
			 * 
			 * Input should be tested for constraints here.
			 */
			virtual void dispatchInput(InputConstraintMode mode)=0;
			
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
			ExtInput *mParent;
	};
	
	class InputData {
		public:
			typedef boost::shared_ptr<ParamList> ParamPtr;
			
			InputData(ParamList *param=0,
			Sim::IdType srcId=Sim::NoId) :
				mParam(param), mSrcId(srcId) {}
			
			ParamPtr getParamList() { return mParam; }
			const ParamPtr getParamList() const { return mParam; }
			Sim::IdType getSourceId() const { return mSrcId; }
			
		private:
			ParamPtr mParam;
			Sim::IdType mSrcId;
	};
	
	/**
	 * Generic input manager for factories.
	 */
	template<class DB>
	class ExtFactoryInput : public ExtInputObj {
		public:
			ExtFactoryInput(ExtInput& parent, ExtSim &extsim,
				Sim::Simulation *sim) : ExtInputObj(parent),
				mExtSim(&extsim), mViolationCounter(0), mBuffer() {}
			virtual ~ExtFactoryInput() {}
			
			void startup() {}
			void shutdown() {}
			
			InputData buildInput(Sim::IdType id) {
				DB &data = getExtDataComponent<DB>(*mExtSim);
				
				const typename DB::DataType *obj = data.getDataById(id);
				if(obj)
					return InputData(obj->getRule()->makeParam(), id);
				
				return InputData(0, Sim::NoId);
			}
			
			void registerInput(const Sim::Save &data)
			{ mBuffer.addInput(data); }
			
			void registerInput(const InputData &data) {
				Sim::Save saveObj;
				Sim::Save::FilePtr fp = Sim::Save::FilePtr(saveObj);
				
				fp << data.getSourceId();
				fp << *data.getParamList();
				
				registerInput(saveObj);
			}
			
			/**
			 * Loads input from a save pointer.
			 */
			void loadInput(Sim::Save::BasePtr& fp) {
				fp >> mBuffer;
			}
			
			void saveInput(Sim::Save::BasePtr& fp) {
				fp << mBuffer;
			}
			
			void dispatchInput(InputConstraintMode mode) {
				mViolationCounter = 0;
				
				// Check mode constraints
				if(mode!=IcmNone && !isValidMode(mode)) {
					mViolationCounter++;
					return;
				}
				
				DB &data = getExtDataComponent<DB>(*mExtSim);
				
				while(mBuffer.hasInput()) {
					Sim::Save saveObj = mBuffer.nextInput();
					Sim::Save::FilePtr fp = Sim::Save::FilePtr(saveObj);
					
					Sim::IdType id;
					fp >> id;
					
					// Read the source data object and typerule
 					const typename DB::DataType *dataObj = data.getDataById(id);
					const TypeRule *rule = dataObj ? dataObj->getRule() : 0;
					
					if(!(dataObj || rule)) {
						mViolationCounter++;
						continue;
					}
					
					// Read the parameter list
					ParamList *param = rule->makeParam();
					fp >> *param;
					
					if(!rule->checkConstrained(param,*mExtSim)) {
						mViolationCounter++;
						continue;
					}
					
					// Build simulation input
					rule->makeInput(*mExtSim, param);
				}
			}
			
			void discardInput() { mBuffer.clearInput();}

			uint32_t getLastConstraintViolationCount()
			{ return mViolationCounter; }

			virtual bool isValidMode(InputConstraintMode mode)=0;
			
		private:
			ExtSim *mExtSim;
			
			uint32_t mViolationCounter;
			Sim::InputBuffer<Sim::Save> mBuffer;
	};
	
	/**
	 * Manages input of program objects.
	 */
	class ExtProgramInput : public ExtFactoryInput<ExtProgramData> {
		public:
			ExtProgramInput(ExtInput& parent, ExtSim &extsim, Sim::Simulation *sim) :
				ExtFactoryInput<ExtProgramData>(parent,extsim,sim) {}
			virtual ~ExtProgramInput() {}
			
			bool isValidMode(InputConstraintMode mode)
			{ return mode==IcmPlayerInput; }
			
		private:
	};
	
	/**
	 * Manages input of abiltiy objects
	 */
	/*class ExtBotInput : public ExtFactoryInput<BotData> {
		public:
			ExtBotInput(ExtInput& parent, ExtSim& extsim, Sim::Simulation* sim) :
				ExtFactoryInput<BotData>(parent,extsim,sim) {}
			virtual ~ExtBotInput() {}
			
			bool isValidMode ( InputConstraintMode mode )
			{ return mode==IcmPlayerSetup || mode==IcmPlayerInput; }
	};*/
	
#define _EXTS_X_EXTINPUT_COMPONENTS \
	_EXTS_X(ExtProgramInput, Program)
	//_EXTS_X(ExtBotInput, Bot)
	
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
			
			ExtSim &getExtSim() { return *mExtSim; }
			
			/// @name Components
			//@{
				template<class T>
				T &getComponent();
				
#define _EXTS_X(type, name) type &get##name() { return m##name; }
				_EXTS_X_EXTINPUT_COMPONENTS
#undef _EXTS_X
			//@}
			
		private:
			ExtSim *mExtSim;
			
			/// @name Input managers
			//@{
#define _EXTS_X(type, name) type m##name;
				_EXTS_X_EXTINPUT_COMPONENTS
#undef _EXTS_X
			//@}
			
			/// @name Constraint mode
			//@{
				InputConstraintMode mConstraintMode;
				Sim::IdType mPlayerId;
			//@}
	};
}

#endif
