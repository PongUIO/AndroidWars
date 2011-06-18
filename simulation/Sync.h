#ifndef SYNC_H
#define SYNC_H

#include <boost/crc.hpp>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Vector.h"

namespace Sim {
	/**
	 * Implements a synchronization object.
	 * 
	 */
	class Sync {
		public:
			static const char *FP_mix;
			static int FP_mix_bufSize;
			
			/**
			 * Mixes an integer datatype into the checksum.
			 * Handles automatic endian conversion.
			 */
			template<class T>
			void mixInt(const T &v)
			{
				T tmp = convertEndian<T>(v);
				checkSum.process_bytes(&tmp, sizeof(T));
			}
			
			/**
			 * Mixes a floating point datatype into the checksum.
			 * Handles automatic string conversion.
			 * 
			 * @warning The limits of this string conversion is
			 * set in \c FP_mix and \c FP_mix_bufSize.
			 */
			template<class T>
			void mixFloat(const T &v)
			{
				char byteStr[FP_mix_bufSize];
				memset(byteStr,0, FP_mix_bufSize);
				
				int bytes = snprintf(byteStr, FP_mix_bufSize, FP_mix, v);
				checkSum.process_bytes(byteStr, bytes);
			}
			
			void mixVec(const Vector &v)
			{
				mixFloat(v.x); mixFloat(v.y);
			}
			
			/**
			 * Converts a basic data type into
			 * a compatible format.
			 */
			template<class T>
			static T convertEndian(const T &val)
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
			
			uint32_t sum() { return checkSum.checksum(); }
		private:
			boost::crc_32_type checkSum;
	};

}

#endif
