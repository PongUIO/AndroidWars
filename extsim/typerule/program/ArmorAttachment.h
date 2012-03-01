#ifndef EXTSIM_PROGRAM_ARMORATTACHMENT_H
#define EXTSIM_PROGRAM_ARMORATTACHMENT_H

#include "../../TypeRule.h"

namespace ExtS { namespace Prog {
		class ArmorAttachmentRule : public TypeRule {
			public:
				ArmorAttachmentRule();
				~ArmorAttachmentRule() {}
				
				virtual TypeRule* clone()
				{ return new ArmorAttachmentRule(*this); }
				
				virtual bool makeInput(
					Sim::Save::BasePtr& fp, const ParamList* param) const;
				
			private:
		};
} }

#endif
