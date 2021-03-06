#include "CpuInput.h"

#include "../../param/IdParam.h"
#include "../../param/ValueParam.h"

namespace exts { namespace misc {
	typedef IdParam<Sim::Bot> BotId;
	typedef IdParam<Sim::Program> ProgramId;
	typedef ValueParam<uint32_t> Uint32Val;
	
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
		const IdParam<Sim::Bot> *bot = param->
			getParamT<BotId>(0);
		const IdParam<Sim::Program> *prog = param->
			getParamT<ProgramId>(1);
		const ValueParam<uint32_t> *delay = param->
			getParamT<Uint32Val>(2);
		
		Sim::Bot *botObj = mExtSim.getSim().getState().getBotFactory().getBot(bot->getId());
		if(botObj) {
			botObj->getState().mCpu.scheduleProgram(prog->getId(),delay->getVal());
		}
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
