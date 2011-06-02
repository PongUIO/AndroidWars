#include "Input.h"

namespace Sim {

InputBuffer::InputBuffer()
{}

InputBuffer::~InputBuffer()
{}

void InputBuffer::addCmd(const Command& cmd)
{
	mCommandList.push_back(cmd);
}

void InputBuffer::appendCmd(const Sim::InputBuffer& ref)
{
	mCommandList.insert(
		mCommandList.end(),
		ref.mCommandList.begin(),
		ref.mCommandList.end()
	);
}

void InputBuffer::clear()
{
	mCommandList.clear();
}

}