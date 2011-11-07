#ifndef SIM_SAVE_H
#define SIM_SAVE_H

#include <boost/crc.hpp>

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vector.h"

namespace Sim {
	class Save {
		private:
			typedef std::vector<uint8_t> DataArray;
			DataArray data;
			
			static const char *FP_mix;
			static int FP_mix_bufSize;
		
		public:
			Save() {}
			~Save() {}
			
			uint32_t checksum();
			size_t size() const { return data.size(); }
			void clear() { data.clear(); }
			uint8_t const *getData() const { return &data[0]; }
			
			class BasePtr {
				private:
					virtual void nanoWrite(const uint8_t *ptr, uint32_t bytes)=0;
					virtual void nanoRead(uint8_t *ptr, uint32_t bytes)=0;
					
					template<class T>
					static T toFormat(const T &val)
					{
						union {
							T v;
							uint8_t bytes[sizeof(T)];
						} u;
						
						u.v = 0;
						for(uint32_t i=0; i<sizeof(T); i++)
							u.bytes[i] = (val >> (8*i)) & 0xFF;
						
						return u.v;
					}
					
					template<class T>
					static T fromFormat(const T &val)
					{
						union {
							T v;
							uint8_t bytes[sizeof(T)];
						} u;
						T res = 0;
						
						u.v = val;
						for(uint32_t i=0; i<sizeof(T); i++)
							res |= u.bytes[i] << (8*i);
						
						return res;
					}
				
				public:
					template<class T>
					void writeInt(const T &v) {
						T tmp = toFormat<T>(v);
						nanoWrite((uint8_t*)&tmp, sizeof(T));
					}
					
					template<class T>
					T readInt() {
						T tmp;
						nanoRead((uint8_t*)&tmp, sizeof(T));
						return fromFormat<T>(tmp);
					}
					
					template<class T>
					void writeFloat(const T &v) {
						char byteStr[FP_mix_bufSize];
						memset(byteStr,0, FP_mix_bufSize);
						
						uint8_t bytes = snprintf(byteStr, FP_mix_bufSize, FP_mix, v);
						writeInt<uint8_t>(bytes);
						nanoWrite((uint8_t*)byteStr, bytes);
					}
					
					double readFloat() {
						char byteStr[FP_mix_bufSize];
						memset(byteStr,0, FP_mix_bufSize);
						
						uint8_t bytes = readInt<uint8_t>();
						nanoRead( (uint8_t*)byteStr, bytes);
						return atof(byteStr);
					}
					
					void writeVec(const Vector &v)
					{ writeFloat(v.x); writeFloat(v.y); }
					
					Vector readVec()
					{ return Vector(readFloat(),readFloat()); }
			};
			
			class FilePtr : public BasePtr {
				private:
					Save &file;
					uint32_t readPtr;
					
					void nanoWrite(const uint8_t *ptr, uint32_t bytes);
					void nanoRead(uint8_t *ptr, uint32_t bytes);
					
				public:
					FilePtr(Save &file) :
						file(file), readPtr(0)
					{}
					
					bool eof() const
					{ return readPtr >= file.data.size(); }
					
					void rewind()
					{ readPtr = 0; }
					
					uint32_t getPtrPos() const { return readPtr; }
			};
			
			/**
			 * Specialization of an ordinary file pointer,
			 * provides an equivalent interface to compute a
			 * checksum without writing any data.
			 */
			class SyncPtr : public BasePtr {
				private:
					void nanoWrite(const uint8_t *ptr, uint32_t bytes)
					{
						mCksum.process_bytes(ptr, bytes);
						mSize += bytes;
					}
					
					void nanoRead(uint8_t *ptr, uint32_t bytes)
						{ (void)ptr; (void)bytes; }
					
					boost::crc_32_type mCksum;
					uint32_t mSize;
				public:
					SyncPtr() : mSize(0)
						{}
					
					uint32_t checksum()
					{ return mCksum.checksum(); }
					
					uint32_t size()
					{ return mSize; }
			};
		
	};
}

#endif
