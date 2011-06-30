#include "Save.h"

namespace Sim {
	const char *Save::FP_mix = "%g";
	int Save::FP_mix_bufSize = 32;
	
	uint32_t Save::checksum()
	{
		boost::crc_32_type checkSum;
		
		checkSum.process_bytes(&data[0], size());
		return checkSum.checksum();
	}
	
	void Save::FilePtr::nanoRead(uint8_t* ptr, uint32_t bytes)
	{
		uint32_t extraBytes = 0;
		if(readPtr+bytes > file.size()) {
			extraBytes = (readPtr+bytes) - file.size();
			memset(ptr+bytes-extraBytes, 0, extraBytes);
		}
		memcpy(ptr, &file.data[readPtr], bytes-extraBytes);
	}
	
	void Save::FilePtr::nanoWrite(const uint8_t* ptr, uint32_t bytes)
	{
		uint32_t oldSize = file.size();
		file.data.resize(oldSize+bytes);
		memcpy(&file.data[oldSize], ptr, bytes);
	}
}
