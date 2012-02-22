#ifndef SIM_ABILITY_H
#define SIM_ABILITY_H

#include <stdint.h>

#include "Factory.h"
#include "Save.h"

namespace Sim {
	class Bot;
	class AbilityDatabase;
	
	#define _SIM_ABILITY_HEADER static const std::string &getTypeName();
	
	/**
	 * Base class for passive abilities.
	 */
	class Ability {
		public:
			typedef AbilityDatabase TypeDatabase;
			
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			
			bool isFinished() { return mIsFinished || mDuration==0; }
			bool isActive() { return mIsActive && mDelay==0; }
			
			/// Returns true when this Ability is ready for complete removal
			bool isDead() { return mReferences==0; }
			
			virtual void abort(bool force=false);
			virtual void toggleSuspend(bool isSuspended, bool force=false);
			virtual void injectDelay(int32_t delay, bool force=false);
			virtual void setDuration(uint32_t duration, bool force=false);
			
			Ability(Simulation *sim, IdType id, IdType typeId) :
				mId(id), mTypeId(typeId), mReferences(0), mSim(sim),
				mIsActive(true), mIsFinished(false), mDelay(0), mDuration(-1)
				{}
			virtual ~Ability() {}
			
		protected:
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				void updateInternal();
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
				
				// State flags
				bool mIsActive;
				bool mIsFinished;
				uint32_t mDelay;
				uint32_t mDuration;
				
				void saveSys(Save::BasePtr &fp) const
				{ fp << mReferences << mIsActive << mIsFinished << mDelay << mDuration; }
				void loadSys(Save::BasePtr &fp)
				{ fp >> mReferences >> mIsActive >> mIsFinished >> mDelay >> mDuration; }
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
			
			Ability *getAbility(IdType id) { return getObject(id); }
			
			void saveObj(Ability* obj, Save::BasePtr& fp);
			Ability* loadObj(IdType internalId, Save::BasePtr& fp);
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Ability *obj) { delete obj; }
			//@}
	};
}

#endif
