#ifndef SIM_SAVE_H
#define SIM_SAVE_H

#include <boost/crc.hpp>
#include <boost/bind.hpp>

#include <algorithm>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vector.h"

namespace Sim {
	/**
	 * @brief Holds a data stream akin to a file in memory.
	 * 
	 * This class is used to save and load serialized data.
	 */
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
			
			void append(const Save &other);
			
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
					void readIntRef(T &ref) { ref = readInt<T>(); }
					
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
					
					template<class T>
					void readFloatRef(T &ref) { ref = readFloat(); }
					
					void writeVec(const Vector &v)
					{ writeFloat(v.x); writeFloat(v.y); }
					
					Vector readVec()
					{
						Vector v;
						v.x = readFloat();
						v.y = readFloat();
						return v;
					}
					
					void writeSave(const Save &save)
					{	
						uint32_t svSize = save.data.size();
						writeInt<uint32_t>(svSize);
						nanoWrite(&(save.data[0]), svSize);
					}
					
					Save readSave()
					{
						Save tmp;
						uint32_t svSize = readInt<uint32_t>();
						
						tmp.data.resize(svSize);
						nanoRead(&(tmp.data[0]), svSize);
						
						return tmp;
					}
					
					/**
					 * Writes a vector or list container to the stream.
					 * 
					 * @param writeFunc A function object, takes two
					 * parameters: A reference to the object to be written,
					 * and a reference to this write pointer object.
					 */
					template<class C>
					void writeCtr(const C &data)
					{
						*this << uint32_t(data.size());
						for(typename C::const_iterator i=data.begin();
							i!=data.end(); i++)
							*this << *i;
					}
					
					/**
					 * Reads a vector or list container from the stream.
					 * 
					 * @param readFunc A function object taking the parameters:
					 * An input data object, and a referece to this read
					 * pointer object.
					 */
					template<class C>
					void readCtr(C &data)
					{
						data.clear();
						
						uint32_t count;
						*this >> count;
						
						data.resize(count);
						for(typename C::iterator i=data.begin();
							i!=data.end(); i++) {
							*this >> *i;
						}
					}
					
					template<class C>
					void readIntCtr(C &data)
					{
						readCtr(data,
							boost::bind(&BasePtr::readIntRef<typename C::value_type>,
							_2, _1)
						);
					}
					
					/// Writes a queue to the stream
					template<class C>
					void writeQueue(const C &data)
					{
						C queueCopy = C(data);
						*this << uint32_t(queueCopy.size());
						while(queueCopy.size()>0) {
							*this << queueCopy.top();
							queueCopy.pop();
						}
					}
					
					/// Reads a queue from the stream
					template<class C>
					void readQueue(C &data)
					{
						uint32_t loadCount;
						*this >> loadCount;
						while(loadCount>0) {
							typename C::value_type dataObj;
							*this >> dataObj;
							
							data.push(dataObj);
							loadCount--;
						}
					}
					
					/**
					 * Writes an unordered set to the stream.
					 * 
					 * The set is sorted before insertion to assure
					 * the checksum is deterministic.
					 */
					template<class C>
					void writeUnorderedSet(const C &data)
					{
						typedef std::vector<typename C::value_type> SetVector;
						
						SetVector result = SetVector(data.size());
						
						std::partial_sort_copy(data.begin(),data.end(),
							result.begin(), result.end());
						
						writeCtr(result);
					}
					
					/**
					 * Reads an unordered set from the stream.
					 */
					template<class C>
					void readUnorderedSet(C &data)
					{
						typedef std::vector<typename C::value_type> SetVector;
						
						SetVector input;
						readCtr(input);
						
						data.clear();
						for(typename SetVector::iterator i=input.begin();
							i!=input.end(); i++) {
							data.insert(*i);
						}
					}
					
					/// Writes a value into the stream
					template<class T>
					Save::BasePtr &operator<<(T val);
					
					/// Reads a value from the stream
					template<class T>
					Save::BasePtr &operator>>(T &val);
					
					virtual uint32_t debugReadPos() { return 0; }
					virtual uint32_t debugWritePos() { return 0; }
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
					
					uint32_t debugReadPos() { return readPtr; }
					uint32_t debugWritePos() { return file.data.size(); }
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
			
			/**
			 * Implements the operators necessary for
			 * for saving special types.
			 */
			template<class T>
			struct OperatorImpl {
				friend Save::BasePtr &operator<<(Save::BasePtr &fp, T const& ref)
				{ ref.save(fp); return fp; }
				
				friend Save::BasePtr &operator>>(Save::BasePtr &fp, T &ref)
				{ ref.load(fp); return fp; }
			};
	};
}

#endif
