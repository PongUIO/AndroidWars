#ifndef SIM_PROGRAMD_H
#define SIM_PROGRAMD_H

#include <stdint.h>
#include <string>
#include <boost/unordered_map.hpp>

#include "BaseData.h"

namespace Sim {
	class Program;
	
	/**
	 * \short Program data block.
	 * 
	 * The rationale for this class is to allow a dynamic implementation of
	 * program types. This allows the simulation to support programs not
	 * implemented by the simulation itself.
	 */
	class ProgramD {
		public:
			ProgramD() : mId(-1) {}
			virtual ~ProgramD() {}
			
			/// @name Basic program management behaviour
			//@{
				virtual const std::string &getTypeNameVirt() const=0;
				virtual Program *createProgram(
					Simulation *sim, uint32_t id) const=0;
			//@}
		
		
			uint32_t mId;
	};
	
	class ProgramDatabase : public DataT<ProgramD> {
		public:
			ProgramDatabase() {}
			virtual ~ProgramDatabase() {}
			
			const ProgramD *getProgramType(uint32_t type) const
			{ return getType(type); }
			
			void registerBase();
			void registerAbility();
			void registerAllDefault();
			
			uint32_t registerCustom(ProgramD *progType);
			
			static const uint32_t NoId;
			uint32_t getTypeIdOf(const std::string &type) const;
			
		private:
			typedef boost::unordered_map<std::string,uint32_t> TypeMap;
			TypeMap mTypeMap;
	};
}

#endif
