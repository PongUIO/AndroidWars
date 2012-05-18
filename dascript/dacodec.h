#ifndef CODEC_H
#define CODEC_H

#include <stdint.h>
#include <string.h>
#include "dascript.h"

class DaText : public DaCodec {
	public:
		void encode(std::string&) {}
		void decode(std::string&) {}
		
	private:
		
};

class DaBase64 : public DaCodec {
	public:
		void encode(std::string& dst) {
			std::string src = dst;
			dst.clear();
			uint32_t encPtr=0;
			unsigned char encBuf[3];
			uint32_t n = 0;
			
			// Encode bytes
			size_t i;
			for(i=0; i<src.size(); i++) {
				encBuf[encPtr++] = src[i];
				
				if(encPtr >= 3) {
					encodeTri(n, encBuf);
					writeEnc(dst,n);
					encPtr=0;
				}
			}
			
			// Encode padding
			if(encPtr > 0) {
				switch(encPtr) {
					case 1: encBuf[1] = 0;
					case 2: encBuf[2] = 0;
				}
				
				encodeTri(n, encBuf);
				dst += base64chars[(n >> 18) & 63];
				dst += base64chars[(n >> 12) & 63];
				dst += (encPtr>=2) ? '=' : base64chars[(n >> 6) & 63];
				dst += '=';
			}
		}
		
		/**
		 * Decode a base64 string.
		 * The decoding can be done in-place, as the
		 * decoded string will be shorter than the
		 * encoded string.
		 */
		void decode(std::string& dst) {
			// Assure the string is a power of 4
			dst.resize(dst.size()+dst.size()%4,'=');
			size_t decPtr=0;
			size_t outPtr=0;
			unsigned char decBuf[4];
			
			for(size_t i=0; i<dst.size(); i++) {
				char res = base64dchars[(unsigned char)dst[i]];
				if(res != -1)
					decBuf[decPtr++] = res;
				
				if(decPtr >= 4) {
					decPtr = 0;
					decodeQuad(&dst[outPtr], decBuf);
					outPtr += 3;
				}
			}
			
			if(decPtr != 0) {
				memset(decBuf+decPtr,0,4-decPtr);
				uint32_t n = (
					decBuf[0]<<18 |
					decBuf[1]<<12 |
					decBuf[2]<< 6 |
					decBuf[3]
				);
				
				switch(decPtr) {
					case 3:
					case 2: dst[outPtr++] = (n >> 16) & 0xFF;
					case 1: dst[outPtr++] = (n >> 8) & 0xFF;
				}
			}
			
			dst.resize(outPtr);
		}
		
	private:
		void writeEnc(std::string &dst, uint32_t n) {
			dst += base64chars[(n >> 18) & 63];
			dst += base64chars[(n >> 12) & 63];
			dst += base64chars[(n >> 6)  & 63];
			dst += base64chars[(n >> 0)  & 63];
		}
		
		void encodeTri(uint32_t &out, const unsigned char c[3]) {
			out = (
				(c[0] << 16) |
				(c[1] << 8 ) |
				(c[2])
			);
		}
		
		void decodeQuad(char out[3], const unsigned char c[4]) {
			uint32_t n = (
				(c[0]<<18) |
				(c[1]<<12) |
				(c[2]<< 6) |
				(c[3]<< 0)
			);
			
			out[0] = (n >> 16) & 0xFF;
			out[1] = (n >>  8) & 0xFF;
			out[2] = (n >>  0) & 0xFF;
		}
		
		static const char *base64chars;
		static char base64dchars[];
};

#endif
