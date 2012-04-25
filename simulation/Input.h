#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <deque>

#include "CommonTemplate.h"
#include "Save.h"
#include "Common.h"
#include "StateObj.h"

#include "Bot.h"
#include "Program.h"
#include "Ability.h"
#include "Bullet.h"
#include "Weapon.h"

#include "utility/CallGroup.h"

namespace Sim {
	
	class InputObj {
		public:
			InputObj() {}
			virtual ~InputObj() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void finalizeInput()=0;
			virtual void dispatchInput()=0;
			
			virtual void save(Save::BasePtr &fp) const=0;
			virtual void load(Save::BasePtr &fp)=0;
	};
	
	/**
	 * This class handles a buffer of input commands.
	 */
	template<class T>
	class InputBuffer : private Save::OperatorImpl< InputBuffer<T> > {
		public:
			InputBuffer() {}
			~InputBuffer() {}
			
			void clearInput() { mInputQueue.clear(); }
			
			bool hasInput() {
				return !mInputQueue.empty();
			}
			
			T nextInput() {
				T tmp = mInputQueue.front();
				mInputQueue.pop_front();
				return tmp;
			}
			
			void addInput(const T &i) {
				mInputQueue.push_back(i);
			}
			
			void addInput(const InputBuffer<T> &buf) {
				for(typename InputQueue::const_iterator i=
					buf.mInputQueue.begin(); i!=buf.mInputQueue.end(); i++) {
					mInputQueue.push_back(*i);
				}
			}
			
			size_t getSize() const { return mInputQueue.size(); }
			
			void save(Save::BasePtr &fp) const {
				fp.writeCtr(mInputQueue);
			}
			void load(Save::BasePtr &fp) {
				fp.readCtr(mInputQueue);
			}
			
		private:
			typedef std::deque<T> InputQueue;
			InputQueue mInputQueue;
	};
	
	/**
	 * Handles input related to creating new objects in factories.
	 */
	template<class T>
	class FactoryInput : public InputObj,
	public Save::OperatorImpl<FactoryInput<T> > {
		public:
			FactoryInput(Simulation *sim) : mIdCounter(0), mSim(sim) {}
			
			void startup() {}
			void shutdown() {}
			
			void registerInput(const Save &data) { mBuffer.addInput(data); }
			void registerInput(typename T::Type *obj) {
				T &fact = getStateComponent<T>(*mSim);
				
				Save data;
				Save::FilePtr fp = Save::FilePtr(data);
				fact.saveObj(obj, fp);
				registerInput(data);
			}
			
			/**
			 * Builds an object and registers it as input.
			 * 
			 * The object is given an automatic id which should correspond to
			 * its real id when dispatched.
			 * 
			 * \c Impl must have the following constructor format:
			 * - <em>Impl(Simulation *sim, IdType id, IdType typeId,
			 * 		const Impl::Config &cfg);</em>
			 * 
			 * @param typeId The identifier of the database object that is
			 * associated with this particular \c Impl.
			 * 
			 * @warning \c typeId is \b strictly required to be an identifer
			 * to a valid database object. The caller \b must know that
			 * they're creating a valid \c Impl for the given \c typeId.
			 */
			template<class Impl>
			Impl *buildInputImpl(const typename Impl::Config &cfg,
			IdType typeId) {
				T &fact = getStateComponent<T>(*mSim);
				
				IdType id = allocateId();
				Impl *obj = new Impl(mSim,id,typeId,cfg);
				
				if(obj)
					mTmpBuffer.addInput(obj);
				
				return obj;
			}
			
			/**
			 * Convenience implementation to allow the creation of input
			 * using a name rather than a type identifier.
			 * 
			 * @see buildInputImpl
			 */
			template<class Impl>
			Impl *buildInputImplStr(const typename Impl::Config &cfg,
			const std::string &name) {
				return buildInputImpl<Impl>(cfg,
					getDataComponent<typename Impl::TypeDatabase>(*mSim).
					getIdOf(name));
			}
			
			/**
			 * Similar to \c buildInputImpl, but creating an object of the
			 * default generic type.
			 */
			/*template<class Arg>
			typename T::Type *buildInput(const Arg &cfg) {
				T &fact = getStateComponent<T>(*mSim);
				
				IdType id = allocateId();
				typename T::Type *obj = fact.template create<Arg>(cfg, id);
				if(obj)
					mTmpBuffer.addInput(obj);
				
				return obj;
			}*/
			
			IdType allocateId() {
				T &fact = getStateComponent<T>(*mSim);
				return fact.getCurrentUniqueId() + mIdCounter++;
			}
			
			void finalizeInput() {
				while(mTmpBuffer.hasInput()) {
					typename T::Type *obj = mTmpBuffer.nextInput();
					registerInput(obj);
					delete obj;
				}
			}
			
			void dispatchInput() {
				T &fact = getStateComponent<T>(*mSim);
				
				while(mBuffer.hasInput()) {
					Save save = mBuffer.nextInput();
					Save::FilePtr fp = Save::FilePtr(save);
					fact.createSerialized(fp);
				}
				
				mIdCounter=0;
			}
			
			void save(Save::BasePtr &fp) const
			{ fp << mBuffer; }
			void load(Save::BasePtr &fp)
			{ fp >> mBuffer; }
			
		private:
			InputBuffer<typename T::Type*> mTmpBuffer;
			InputBuffer<Save> mBuffer;
			
			IdType mIdCounter;
			
			Simulation *mSim;
	};
	
	/**
	 * Handles input specifically related to scheduling to a bot's cpu.
	 */
	class BotCpuInput : public InputObj, 
	public Save::OperatorImpl<BotCpuInput> {
		public:
			struct CpuRef : public Save::OperatorImpl<CpuRef> {
				CpuRef(IdType botId=0, IdType progId=0, uint32_t delay=0) :
					mTargetBot(botId), mProgramId(progId), mDelay(delay)
					{}
				
				IdType mTargetBot;
				IdType mProgramId;
				uint32_t mDelay;
				
				void save(Save::BasePtr &fp) const
				{ fp << mTargetBot << mProgramId << mDelay; }
				void load(Save::BasePtr &fp)
				{ fp >> mTargetBot >> mProgramId >> mDelay; }
			};
			
			BotCpuInput(Simulation *sim) : mSim(sim) {}
			~BotCpuInput() {}
			
			void registerInput(IdType botId, IdType progId, uint32_t delay)
			{ mBuffer.addInput(CpuRef(botId,progId,delay)); }
			
			void startup();
			void shutdown();
			
			void finalizeInput() {}
			void dispatchInput();
			
			void save(Save::BasePtr &fp) const
			{ fp << mBuffer; }
			void load(Save::BasePtr &fp)
			{ fp >> mBuffer; }
		
		private:
			InputBuffer<CpuRef> mBuffer;
			
			Simulation *mSim;
	};
	
#define _SIM_X_INPUT_COMPONENTS \
	_SIM_X(FactoryInput<BotFactory>, BotInput) \
	_SIM_X(FactoryInput<ProgramFactory>, ProgramInput) \
	_SIM_X(FactoryInput<AbilityFactory>, AbilityInput) \
	_SIM_X(FactoryInput<BulletFactory>, BulletInput) \
	_SIM_X(FactoryInput<WeaponFactory>, WeaponInput) \
	_SIM_X(BotCpuInput, CpuInput)
	
	/**
	 * @brief Main simulation input manager.
	 */
	class Input : public CallGroup<InputObj> {
		public:
			Input(Simulation *sim);
			~Input();
			
			void startup();
			void shutdown();
			
			void finalizeInput();
			void dispatchInput();
				
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
			template<class T>
			T &getComponent();
			
#define _SIM_X(type, name) type &get##name() { return m##name; }
			_SIM_X_INPUT_COMPONENTS
#undef _SIM_X
			
		private:
			Simulation *mSim;
			
#define _SIM_X(type, name) type m##name;
			_SIM_X_INPUT_COMPONENTS
#undef _SIM_X
	};
}

#endif
