#ifndef EXTSIM_PROGRAM_ARMORATTACHMENT_H
#define EXTSIM_PROGRAM_ARMORATTACHMENT_H

#include "../../TypeRule.h"

namespace ExtS { namespace Prog {
		class ArmorAttachmentRule : public TypeRule {
			public:
				ArmorAttachmentRule();
				~ArmorAttachmentRule() {}
				
				void saveInput(ParamList* , Sim::Save::BasePtr& ) const;
				
			private:
		};
} }

#endif
