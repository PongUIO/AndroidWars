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
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			
			bool isFinished() { return false; }
			bool isActive() { return true; }
			
			/// Returns true when this Ability is ready for complete removal
			bool isDead() { return mReferences==0; }
			
		protected:
			Ability(Simulation *sim, IdType id, IdType typeId) :
				mId(id), mTypeId(typeId), mReferences(0), mSim(sim) {}
			virtual ~Ability() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				virtual void prepareStep(double delta, Bot *bot)=0;
				virtual void updateCpu(double delta, Bot *bot)=0;
				virtual void step(double delta, Bot *bot)=0;
				
				virtual void start(Bot *) {}
				virtual void end(Bot *) {}
			//@}
			
			/// @name System data
			//@{
				void reference() { mReferences++; }
				void dereference() { mReferences--; }
				
				IdType mId;
				IdType mTypeId;
				uint32_t mReferences;
				Simulation *mSim;
			//@}
			
		private:
			friend class AbilityFactory;
			friend class DefaultUidFactory<Ability>;
			friend class BotAbility;
	};
	
	class AbilityFactory : public DefaultUidFactory<Ability> {
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
			T *createAbility(const typename T::Config &cfg)
			{	return createType<T>(cfg); }
			
			Ability *getAbility(IdType id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Ability *obj) { delete obj; }
				
				const DataBehaviourT<Ability>::Behaviour* getBehaviourFromId(IdType id) const;
				const DataBehaviourT<Ability>::Behaviour* getBehaviourFromName(const std::string& name) const;
				
				void saveObj(Ability* obj, Save::BasePtr& fp);
				Ability* loadObj(IdType internalId, Save::BasePtr& fp);
			//@}
	};
}

#endif
