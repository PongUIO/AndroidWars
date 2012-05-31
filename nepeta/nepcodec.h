/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#ifndef _NEPCODEC_H
#define _NEPCODEC_H

#include <sstream>
#include <stdint.h>
#include <string.h>
#include "nepeta.h"

/**
 * @brief Interface for argument block codecs.
 * 
 * Codecs are primarily used to read block-type arguments by decoding the
 * value into another format. The two basic codecs implemented are "text"
 * (\c NepText) and "base64" (\c NepBase64).
 * 
 * While Nepeta can read binary data in a block-type argument, not all text
 * editors handle binary data as well. Codecs are implemented to add support
 * for alternate representations of the binary data.
 * 
 * The \c NepParser support adding additional codecs to the parsing process.
 * This may be useful to implement more advanced behaviour (for example
 * autocompiling embedded scripts).
 */
class NepCodec {
	public:
		typedef std::map<std::string, NepCodec*> CodecPack;
		
		NepCodec() {}
		virtual ~NepCodec() {}
		
		/**
		 * @brief Encodes a data string into the codec's format.
		 * 
		 * This is never used by \c NepParser, and for most implementations
		 * this may be ignored.
		 * 
		 * @param dst The output stringstream to write the encoded data to.
		 * @param ind The indentation level to prefix each line with.
		 * @param str The argument used as a source
		 */
		virtual void encode(std::ostringstream &dst,
			size_t ind, const std::string &str) {}
		
		/**
		 * @brief Decodes a string read from a block argument into
		 * @brief the codec's format.
		 * 
		 * @param node The node in which the argument originated
		 * @param argi The index of the argument in the node
		 * @param str The actual argument string.
		 */
		virtual void decode(Nepeta::Node &node, size_t argi,
			std::string &str)=0;
		
	private:
};

/// @internal
class NepText : public NepCodec {
	public:
		void decode(Nepeta::Node& node, size_t argi, std::string& str) {}
		
	private:
		
};

/// @internal
class NepBase64 : public NepCodec {
	public:
		void encode(std::ostringstream &out, size_t ind, const std::string &str)
		{
			std::string src = str;
			std::string dst = "";
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
			
			// Write this to the output stream
			out << "\n" << std::string(ind,'\t');
			for(size_t c=0; c<dst.size(); c++) {
				if(c % (70-ind*6) == 0 && c!=0)
					out << "\n" << std::string(ind,'\t');
				out << dst[c];
			}
		}
		
		/**
		 * Decode a base64 string.
		 * The decoding can be done in-place, as the
		 * decoded string will be shorter than the
		 * encoded string.
		 */
		void decode(Nepeta::Node &, size_t, std::string& dst) {
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
