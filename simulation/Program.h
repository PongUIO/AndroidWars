#ifndef SIM_PROGRAM_H
#define SIM_PROGRAM_H

#include <stdint.h>
#include <limits>

#include "Factory.h"
#include "Save.h"

#include "data/ProgramD.h"
#include "program/Sensor.h"

namespace Sim {
	// Forward declarations
	class ProgramFactory;
	class Simulation;
	class Bot;
	class BotCpu;
	
	class Program {
		public:
			typedef ProgramDatabase TypeDatabase;
			
			virtual uint32_t getCycleCost()=0;
			
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			
			Prog::Sensor &getEndSensor() { return mEndSensor; }
			
			void setForceFinished() { mFinished=true; }
			void setRunningTime(uint32_t steps) { mRunningTime=steps; }
			
			Program(Simulation *sim, IdType id, IdType typeId) :
				mId(id), mTypeId(typeId), mSim(sim),
				mEndSensor(), mRunningTime(-1), mFinished(false) {}
			virtual ~Program() {}
			
		protected:
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp);
				virtual void load(Save::BasePtr &fp);
				
				virtual void process(Bot *bot, BotCpu *cpu)=0;
				virtual bool isFinished(Bot *bot, BotCpu *cpu)=0;
				
				virtual void start(Bot *bot, BotCpu *cpu);
				virtual void end(Bot *bot, BotCpu *cpu);
			//@}
				
			/// @name System data
			//@{
				IdType mId;
				IdType mTypeId;
				Simulation *mSim;
			//@}
			
		private:
			/// @name Core program
			//@{
				Prog::Sensor mEndSensor;
				uint32_t mRunningTime;
				bool mFinished;
				
				void tickRunningTime()
				{	if(mRunningTime != uint32_t(-1) && mRunningTime>0)
						mRunningTime--; }
				
				/**
				 * Returns true when the program is finished executing.
				 * 
				 * This may be due to one of three events:
				 * - The program has been forced to quit, as due to a Kill
				 * 		program.
				 * - The running time has reached zero
				 * - The actual program code has decided it is finished.
				 * 
				 * A program will never execute if this returns true.
				 */
				bool isCompletelyFinished(Bot *bot, BotCpu *cpu)
				{ return mFinished || mRunningTime<=0 || isFinished(bot,cpu); }
			//@}
			
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
			
			Program *createFromSerialized(Save::BasePtr &fp, Sim::IdType id);
			
			void destroyProgram(IdType id) { removeObj(id); }
			Program *getProgram(IdType id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Program* obj) { delete obj; }
			//@}
	};
}

#endif
