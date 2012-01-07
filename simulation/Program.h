#ifndef SIM_PROGRAM_H
#define SIM_PROGRAM_H

#include <stdint.h>

#include "Factory.h"
#include "Save.h"

#include "data/ProgramD.h"

namespace Sim {
	// Forward declarations
	class ProgramFactory;
	class Simulation;
	class Bot;
	class BotCpu;
	
	/**
	 * Required for any program implementation.
	 * 
	 * This declares a function for the typename of the program.
	 */
#define _SIM_PROGRAM_HEADER static const std::string &getTypeName();
	
	class Program {
		public:
			virtual uint32_t getCycleCost()=0;
			
			uint32_t getId() const { return mId; }
			uint32_t getTypeId() const { return mTypeId; }
			
		protected:
			Program(Simulation *sim, uint32_t id, uint32_t typeId) :
				mId(id), mTypeId(typeId), mSim(sim) {}
			virtual ~Program() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				virtual void process(Bot *bot, BotCpu *cpu)=0;
				virtual bool isFinished(Bot *bot, BotCpu *cpu)=0;
			//@}
				
			/// @name System data
			//@{
				uint32_t mId;
				uint32_t mTypeId;
				Simulation *mSim;
			//@}
			
		private:
			friend class ProgramFactory;
			friend class DefaultUidFactory<Program>;
			
			friend class BotCpu;
			friend class BotInput;
	};
	
	/**
	 * Factory manager for programs.
	 * 
	 * Programs behave slightly different from ordinary types in terms
	 * of IDs. Program IDs are always unique through the runtime of the
	 * simulation.
	 * 
	 * This is because bot input require exact and persistent identifiers to
	 * preserve the concept that it should not matter to determinism whether
	 * input is fed per-phase or in batch at the beginning of the simulation.
	 * 
	 * As a result of this, IDs used internally by the factory are hidden from
	 * view as best as possible.
	 * 
	 * Unfortunately, the issue where cheksums are different whether
	 * input is given per-phase or all at once is unavoidable.
	 */
	class ProgramFactory : public DefaultUidFactory<Program> {
		public:
			ProgramFactory(Simulation* sim);
			virtual ~ProgramFactory();
			
			/// @name StateObj functions
			//@{
				void startup();
				void shutdown();
				
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);
				
				void startPhase();
				void endPhase();
				
				void step(double stepTime);
			//@}
			
			/**
			 * Creates a program and inserts it into the factory.
			 * May create any valid inheritor of \c Program.
			 */
			template<class T>
			T *createProgram(const typename T::Config &cfg)
			{	return createType<T>(cfg); }
			
			Program *createFromSerialized(Save::BasePtr &fp);
			
			void destroyProgram(uint32_t id) { removeObj(id); }
			Program *getProgram(uint32_t id) { return getObject(id); }
			
			uint32_t getLastPhaseId() { return mLastPhaseInputId; }
			uint32_t getCurrentUniqueId()
			{ return UidFactory<Program>::getCurrentUniqueId(); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Program* obj) { delete obj; }
				
				const DataBehaviourT<Program>::Behaviour* getBehaviourFromName(const std::string& name) const;
				const DataBehaviourT<Program>::Behaviour* getBehaviourFromId(uint32_t id) const;
			//@}
			
			uint32_t mLastPhaseInputId;
			
			friend class ReplayManager;
	};
}

#endif
