#include <string.h>

#include "Save.h"

#include "Common.h"

namespace Sim {
	uint32_t Save::checksum() const
	{
		boost::crc_32_type checkSum;
		
		checkSum.process_bytes(getData(), size());
		return checkSum.checksum();
	}
	
	void Save::append(const Save& other)
	{
		mData.insert(mData.end(), other.mData.begin(), other.mData.end());
	}
	
	void Save::FilePtr::nanoRead(void* vptr, uint32_t bytes)
	{
		uint8_t *ptr = static_cast<uint8_t*>(vptr);
		
		if(mReadPtr > mSaveRef.size())
			throw std::out_of_range(
				"FilePtr::nanoRead() : Attempt to read beyond size");
		
		memcpy(ptr, (mSaveRef.getData()+mReadPtr), bytes);
		mReadPtr += bytes;
	}
	
	void Save::FilePtr::nanoWrite(const void* vptr, uint32_t bytes)
	{
		const uint8_t *ptr = static_cast<const uint8_t*>(vptr);
		
		uint32_t oldSize = mSaveRef.size();
		mSaveRef.mData.resize(oldSize+bytes);
		memcpy(mSaveRef.getData()+oldSize, ptr, bytes);
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
	STREAM_FUNC(std::string, writeStr(val), val=readStr())
}
