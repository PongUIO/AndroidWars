#include "CpuInput.h"

#include "../../param/IdList.h"
#include "../../param/ValRange.h"

namespace exts { namespace misc {
	typedef IdList<Sim::Bot> BotId;
	typedef IdList<Sim::Program> ProgramId;
	typedef ValRange<uint32_t> Uint32Val;
	
	CpuInput::CpuInput(ExtSim& esim): TypeRule(esim)
	{
		registerParam(new BotId("Bot",false));
		registerParam(new ProgramId("Program",false));
		registerParam(new Uint32Val("Delay",false));
	}
	
	CpuInput::~CpuInput()
	{}
	
	void CpuInput::makeInput(const ParamList* param) const
	{
		const IdList<Sim::Bot> *bot = getRefParam()->
			getParamT<BotId>(0);
		const IdList<Sim::Program> *prog = getRefParam()->
			getParamT<ProgramId>(1);
		const ValRange<uint32_t> *delay = getRefParam()->
			getParamT<Uint32Val>(2);
		
		mExtSim.getSim().getInput().getCpuInput().registerInput(
		bot->getId(), prog->getId(), delay->getVal());
	}
	
	ParamList* CpuInput::setupParam(Sim::IdType bot, Sim::IdType prog,
	uint32_t delay)
	{
		ParamList *param = makeParam();
		
		BotId *botId = param->getParamT<BotId>(0);
		ProgramId *progId = param->getParamT<ProgramId>(1);
		Uint32Val *delayParam = param->getParamT<Uint32Val>(2);
		
		botId->setId(bot);
		progId->setId(prog);
		delayParam->setVal(delay);
		
		return param;
	}

	
	void CpuInput::buildTimelineData(const ParamList* param) const
	{}
	
	Sim::IdType CpuInput::registerSimData(const std::string& name) const
	{	return Sim::NoId; }
} }
