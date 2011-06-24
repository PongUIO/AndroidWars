#ifndef SIM_SAVE_H
#define SIM_SAVE_H

#include <boost/crc.hpp>

#include <vector>
#include <stdint.h>
#include <stdio.h>
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
			uint32_t size() const { return data.size(); }
			void clear() { data.clear(); }
			uint8_t const *getData() const { return &data[0]; }
			
			class Ptr {
				private:
					Save &file;
					uint32_t readPtr;
					
					virtual void nanoWrite(const uint8_t *ptr, uint32_t bytes);
					virtual void nanoRead(uint8_t *ptr, uint32_t bytes);
					
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
					Ptr(Save &file) :
						file(file)
					{}
					
					bool eof() const
					{ return readPtr >= file.data.size(); }
					
					void rewind()
					{ readPtr = 0; }
					
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
			};
			
			/**
			 * Specialization of an ordinary file pointer,
			 * provides an equivalent interface to compute a
			 * checksum without writing any data.
			 */
			class SyncPtr : public Ptr {
				private:
					void nanoWrite(const uint8_t *ptr, uint32_t bytes)
					{
						cksum.process_bytes(ptr, bytes);
					}
					
					void nanoRead(uint8_t *ptr, uint32_t bytes)
						{}
					
					boost::crc_32_type cksum;
				public:
					SyncPtr(Save &file) : Ptr(file)
						{}
					
					uint32_t checksum()
					{ return cksum.checksum(); }
			};
		
	};
}

#endif
