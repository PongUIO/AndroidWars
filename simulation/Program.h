#ifndef SIM_PROGRAM_H
#define SIM_PROGRAM_H

#include <stdint.h>
#include <boost/unordered_map.hpp>

#include "Factory.h"
#include "Save.h"

namespace Sim {
	// Forward declarations
	class ProgramFactory;
	class Simulation;
	class Bot;
	class BotCpu;
	
	class Program {
		public:
			enum ProgramTypeId {
#define _SIM_PROGRAM_DEF(clsType) Pti##clsType,
				#include "program/ProgramDef.def"
#undef _SIM_PROGRAM_DEF

				PtiMax
			};
			class SaveSystem {
				public:
					virtual Program *createProgram(Simulation *sim,
						uint32_t id)=0;
			};
			
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
			//@}
				
			/// @name System data
			//@{
				uint32_t mId;
				uint32_t mTypeId;
				Simulation *mSim;
			//@}
			
			friend class ProgramFactory;
			friend class Factory<Program>;
			
			friend class BotCpu;
	};
	
	class ProgramFactory : public Factory<Program>, public StateObj {
		public:
			ProgramFactory(Simulation* sim);
			virtual ~ProgramFactory();
			
			void startup();
			void shutdown();
			
			void save(Save::BasePtr& fp);
			void load(Save::BasePtr& fp);
			
			template<class T>
			T *createProgram(const typename T::Config &cfg) {
				uint32_t id = Factory<Program>::newId();
				T *tmp = new T(mSim,id,cfg);
				Factory<Program>::addObj(tmp);
				return tmp;
			}
			
			Program *getProgram(uint32_t id) { return getObject(id); }
			const Program *getProgram(uint32_t id) const
				{ return getObject(id); }
			
			void startPhase();
			void endPhase();
			
			void step(double stepTime);
			
		private:
			typedef boost::unordered_map<uint32_t, Program::SaveSystem*>
				ProgramTypeMap;
			ProgramTypeMap mTypeMap;
			
			void deleteInstance(Program* obj) { delete obj; }
			void saveObj(Program *obj , Save::BasePtr &fp);
			Program* loadObj(uint32_t id, Save::BasePtr &fp);
			
			Simulation *mSim;
	};
}

#endif
