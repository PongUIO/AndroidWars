#ifndef SIM_ABILITY_H
#define SIM_ABILITY_H

#include <stdint.h>

#include "Factory.h"
#include "Save.h"

namespace Sim {
	class Bot;
	
	#define _SIM_ABILITY_HEADER static const std::string &getTypeName();
	
	/**
	 * Base class for passive abilities.
	 */
	class Ability {
		public:
			uint32_t getId() const { return mId; }
			uint32_t getTypeId() const { return mTypeId; }
			
			bool isDead() { return false; }
			bool isActive() { return true; }
			
		protected:
			Ability(Simulation *sim, uint32_t id, uint32_t typeId) :
				mId(id), mTypeId(typeId), mSim(sim) {}
			virtual ~Ability() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				virtual void prepareStep(double delta, Bot *bot)=0;
				virtual void updateCpu(double delta, Bot *bot)=0;
				virtual void step(double delta, Bot *bot)=0;
			//@}
			
			/// @name System data
			//@{
				uint32_t mId;
				uint32_t mTypeId;
				Simulation *mSim;
			//@}
			
		private:
			friend class AbilityFactory;
			friend class BotAbility;
	};
	
	class AbilityFactory : private UidFactory<Ability>, public StateObj {
		public:
			AbilityFactory(Simulation *sim);
			virtual ~AbilityFactory();
			
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
			 * Creates an ability and inserts it into the factory.
			 * May create any valid inheritor of \c Program that is registered
			 * as an ability in the simulation data.
			 * 
			 * @return A pointer to the new object if successful, or NULL
			 * if the ability type is not registered.
			 */
			template<class T>
			T *createAbility(const typename T::Config &cfg) {
				uint32_t typeId = getAbilityTypeId(T::getTypeName());
				if(typeId == getAbilityTypeNoId())
					return 0;
				
				InsertData insData = insertObject();
				T *tmp = new T(mSim, insData.first, typeId, cfg);
				*insData.second = tmp;
				
				return tmp;
			}
			
			Ability *getAbility(uint32_t id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Ability *obj) { delete obj; }
				void saveObj(Ability* , Save::BasePtr &fp);
				Ability* loadObj(uint32_t internalId, Save::BasePtr &fp);
			//@}
			
			uint32_t getAbilityTypeId(const std::string &name);
			uint32_t getAbilityTypeNoId();
			
			Simulation *mSim;
	};
}

#endif
