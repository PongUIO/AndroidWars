#include "ExtErrorMgr.h"

namespace exts {
	ExtErrorMgr::ExtErrorMgr(ExtSim &esim) : ExtModule(), mExtSim(esim)
	{}
	
	ExtErrorMgr::~ExtErrorMgr()
	{}
	
	void ExtErrorMgr::message(
		Severity level,
		const std::string& func,
		const std::string& msg
	) {
		mMessage.push_back( Message(level, func, msg) );
	}
	
	void ExtErrorMgr::msgId(const std::string& func, Sim::IdType id)
	{
		if(id == Sim::NoId) {
			message(Warning, func,
			"The invalid identifier NoId was detected");
		} else {
			message(Warning, func,
			(msgformat("%1% is not a valid identifier to any object here") % id)
			.str());
		}
	}

}
