#ifndef EXTSIM_EXTDATALISTENER_H
#define EXTSIM_EXTDATALISTENER_H

#include <boost/unordered_map.hpp>

#include "nepeta.h"

namespace exts {
	struct DataListener {
		virtual void loadNode(const Nepeta::Node &node)=0;
		virtual void postProcess()=0;
	};
	
	class ListenerEntry {
		public:
			ListenerEntry() {}
			~ListenerEntry() {}
			
			void loadNode(const Nepeta::Node &node);
			void postProcess();
			
			void registerListener(DataListener *listener)
			{ mListeners.push_back(listener); }
		
		private:
			typedef std::vector<DataListener*> ListenerVec;
			ListenerVec mListeners;
	};
	
	class ListenerGroup {
		public:
			ListenerGroup() {}
			~ListenerGroup();
			
			void loadNode(const Nepeta::Node &node);
			void postProcess();
			
			void registerListener(const std::string &str, DataListener *L);
			
		private:
			typedef boost::unordered_map<std::string, ListenerEntry*> ListenerMap;
			ListenerMap mListenerMap;
	};
}

#endif
