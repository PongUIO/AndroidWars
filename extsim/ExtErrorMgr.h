#ifndef EXTS_EXTERRORMGR_H
#define EXTS_EXTERRORMGR_H

#include <string>
#include <vector>
#include <boost/current_function.hpp>
#include <boost/format.hpp>

#include "../simulation/Common.h"

#include "ExtModule.h"

#define EXTS_FUNC BOOST_CURRENT_FUNCTION

namespace exts {
	class ExtSim;
	
	typedef boost::format msgformat;
	
	/**
	 * @brief Stores a list of all messages that have occured in ExtSim
	 */
	class ExtErrorMgr : public ExtModule {
		public:
			enum Severity {
				/// Intended for minor notifications that strictly speaking
				/// are not errors, such as when constraints fail.
				Note = 0,
				
				/// Intended for minor issues catched, such as invalid
				/// identifiers or bad parameters to functions.
				Warning,
				
				/// Intended for severe internal issues that risk compromizing
				/// ExtSim's stability.
				Error
			};
			
			struct Message {
				Message(Severity nlevel,
					const std::string &nwhere, const std::string &nwhat) :
					level(nlevel), where(nwhere), what(nwhat)
				{}
				
				Severity level;
				std::string where;
				std::string what;
			};
			
			typedef std::vector<Message> MessageVec;
			
			/// @name Initialization
			//@{
				ExtErrorMgr(ExtSim &esim);
				~ExtErrorMgr();
				
				void startup() { clearMessages(); }
				void shutdown() { clearMessages(); }
			//@}
			
			/// @name Interface
 			//@{
				void message(
					Severity level,
					const std::string &func,
					const std::string &msg
				);
				
				const MessageVec &getMessages() const { return mMessage; }
				void clearMessages() { mMessage.clear(); }
				
				// Common messages
				void msgId(const std::string &func, Sim::IdType id);
 			//@}
			
		private:
			ExtSim &mExtSim;
			MessageVec mMessage;
			
	};
}

#endif
