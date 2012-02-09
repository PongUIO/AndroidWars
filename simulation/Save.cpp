#include "Save.h"

#include "Common.h"

namespace Sim {
	const char *Save::FP_mix = "%g";
	int Save::FP_mix_bufSize = 32;
	
	uint32_t Save::checksum()
	{
		boost::crc_32_type checkSum;
		
		checkSum.process_bytes(&data[0], size());
		return checkSum.checksum();
	}
	
	void Save::append(const Sim::Save& other)
	{
		data.insert(data.end(), other.data.begin(), other.data.end());
	}
	
	void Save::FilePtr::nanoRead(uint8_t* ptr, uint32_t bytes)
	{
		uint32_t extraBytes = 0;
		if(readPtr+bytes > file.size()) {
			extraBytes = (readPtr+bytes) - file.size();
			if(extraBytes > bytes)
				extraBytes = bytes;
			memset(ptr+bytes-extraBytes, 0, extraBytes);
		}
		memcpy(ptr, &file.data[readPtr], bytes-extraBytes);
		readPtr += bytes;
	}
	
	void Save::FilePtr::nanoWrite(const uint8_t* ptr, uint32_t bytes)
	{
		uint32_t oldSize = file.size();
		file.data.resize(oldSize+bytes);
		memcpy(&file.data[oldSize], ptr, bytes);
		readPtr += bytes;
	}
	
	// Save::BasePtr stream operators
	//
	//
	
	// Base macros
#define WRITE_FUNC(Type, contents) \
	template<> Save::BasePtr& Save::BasePtr::operator<<(Type val) \
	{ contents; return *this; }
#define READ_FUNC(Type, contents) \
	template<> Save::BasePtr& Save::BasePtr::operator>>(Type &val) \
	{ contents; return *this; }

#define STREAM_FUNC(Type, writeCts, readCts) \
	WRITE_FUNC(Type, writeCts) \
	READ_FUNC(Type, readCts)
	
	// Base types
#define STREAM_INT(Type) \
	STREAM_FUNC(Type, writeInt<Type>(val), val=readInt<Type>())
#define STREAM_FLOAT(Type) \
	STREAM_FUNC(Type, writeFloat(val), val=readFloat())
	
	STREAM_INT(int8_t)
	STREAM_INT(int16_t)
	STREAM_INT(int32_t)
	STREAM_INT(int64_t)
	
	STREAM_INT(uint8_t)
	STREAM_INT(uint16_t)
	STREAM_INT(uint32_t)
	STREAM_INT(uint64_t)
	
	STREAM_FLOAT(double)
	
	// Special types
	STREAM_FUNC(Vector, writeVec(val), val=readVec())
	STREAM_FUNC(Save, writeSave(val), val=readSave())
	STREAM_FUNC(bool, writeInt<uint8_t>(val), val=readInt<uint8_t>())
}
