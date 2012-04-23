#ifndef SIM_SAVE_H
#define SIM_SAVE_H

#include <boost/crc.hpp>

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <stdint.h>

#include "Vector.h"

namespace Sim {
	/**
	 * @brief Holds a data buffer akin to a file in memory.
	 * 
	 * This is used to save and load serialized data.
	 * 
	 * The subclass \c FilePtr allows manipulation of the data in a \c Save
	 * object, and \c SyncPtr computes a checksum of the data written to it,
	 * without altering any data.
	 */
	class Save {
		private:
			typedef std::vector<uint8_t> DataArray;
			DataArray mData;
			
		public:
			Save() {}
			~Save() {}
			
			/// Retrieves a pointer to the internal data array.
			uint8_t const *getData() const { return &mData[0]; }
			
			/// @copydoc getData
			uint8_t *getData() { return &mData[0]; }
			
			/**
			 * @brief Creates a CRC-32 checksum of the data.
			 * 
			 * Creates a CRC-32 checksum of the contained data and returns
			 * this value.
			 * 
			 * @return A CRC-32 checksum
			 */
			uint32_t checksum() const;
			
			/// Returns the size of the \c Save in bytes
			uint32_t size() const { return mData.size(); }
			
			/// Clears all data from the object, leaving it empty.
			void clear() { mData.clear(); }
			
			/**
			 * @brief Appends the data contained within another \c Save
			 * into this.
			 * 
			 * @note This is a convenience function to avoid having to create
			 * a \c FilePtr just to append a \c Save.
			 */
			void append(const Save &other);
			
			/**
			 * @brief Defines a common abstract interface for \c Save object
			 * manipulation.
			 * 
			 * These behave like file descriptors, and as such, depending
			 * on how the inheritor implements the core functions, may
			 * be used in many different ways. The default implementations
			 * are \c FilePtr, who is the normal read/write class, and
			 * \c SyncPtr, who directly computes a checksum without even
			 * needing to touch a file. A more extreme, but possible,
			 * implementation is to push the data through sockets, or
			 * adding an encryption or compression layer on top of \c FilePtr.
			 * 
			 * Only the \c nanoWrite() and \c nanoRead() functions
			 * need to be implemented by inheritors.
			 * 
			 * In addition to the "raw" read/write functions, this interface
			 * defines and implements a large array of function-pairs that
			 * are used to write or read specific data types and containers.
			 * 
			 * Lastly, a pair of stream operators are defined. These allows
			 * writing/reading to be performed in a very similar way to how
			 * std::iostream works, given that the operators are implemented
			 * for the relevant type.
			 */
			class BasePtr {
				protected:
					/**
					 * The caller assumes this writes the given number of
					 * \c bytes from the source \c ptr to a \c Save object.
					 */
					virtual void nanoWrite(const void *ptr, uint32_t bytes)=0;
					
					/**
					 * The caller assumes this reads the given number of
					 * \c bytes from a source (which is assumed to be a
					 * \c Save) into \c ptr.
					 * 
					 * @exception std::runtime_error Some implementations of
					 * \c BasePtr do not care about, or really permit reading
					 * from the descriptor. If an implementation does not
					 * specialize this function, it will throw an exception
					 * when called.
					 */
					virtual void nanoRead(void *ptr, uint32_t bytes)
					{ throw std::runtime_error(
						"Illegal call to BasePtr::nanoRead()"); }
					
					/**
					 * Converts the endianness of the input value to the
					 * format's standard little-endian.
					 */
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
					
					/**
					 * Converts the endianness of the input value
					 * from the format's standard to a system compatible value.
					 */
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
						nanoWrite(&tmp, sizeof(T));
					}
					
					template<class T>
					T readInt() {
						T tmp;
						nanoRead(&tmp, sizeof(T));
						return fromFormat<T>(tmp);
					}
					
					void writeFloat(double v) {
						nanoWrite(&v, sizeof(double));
					}
					
					double readFloat() {
						double v;
						nanoRead(&v, sizeof(double));
						return v;
					}
					
					void writeStr(const std::string &str)
					{
						writeInt<uint32_t>(str.size());
						nanoWrite(&str[0],str.size());
					}
					
					std::string readStr()
					{
						std::string tmp;
						uint32_t strSize;
						
						strSize = readInt<uint32_t>();
						tmp.resize(strSize);
						
						nanoRead(&tmp[0],strSize);
						return tmp;
					}
					
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
						uint32_t svSize = save.size();
						writeInt<uint32_t>(svSize);
						nanoWrite(save.getData(), svSize);
					}
					
					Save readSave()
					{
						Save tmp;
						uint32_t svSize = readInt<uint32_t>();
						
						tmp.mData.resize(svSize);
						nanoRead(tmp.getData(), svSize);
						
						return tmp;
					}
					
					/**
					 * Writes a sequence-type container
					 * (vector, deque, or list).
					 * 
					 * Each vector element is written using the stream
					 * operators.
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
					 * Reads a sequence-type container
					 * (vector, deque, or list).
					 * 
					 * Each vector element is read using the stream
					 * operators.
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
					
					/**
					 * Writes a queue-type container.
					 * 
					 * Each element is written using the stream operators.
					 */
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
					
					/**
					 * Reads a queue from the stream.
					 * 
					 * Each element is read using the stream operators.
					 */
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
					 * Writes a boost-type unordered set to the stream.
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
					 * Reads a boost-type unordered set from the stream.
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
					
					/// Writes a value of type T
					template<class T>
					Save::BasePtr &operator<<(T val);
					
					/// Reads a value of type T
					template<class T>
					Save::BasePtr &operator>>(T &val);
					
					/**
					 * Utility function used for reading the position of
					 * the read pointer.
					 */
					virtual uint32_t getReadPos() { return 0; }
					/**
					 * Utility function used for reading the position of
					 * the write pointer.
					 */
					virtual uint32_t getWritePos() { return 0; }
			};
			
			/**
			 * @brief Implements \c BasePtr functionality for reading and/or
			 * writing to a \c Save reference.
			 * 
			 * @note Both reading and writing is allowed at the same. Writing
			 * is always performed at the end of the \c Save reference, while
			 * reading is performed using an internal numerical pointer to
			 * a position in the \c Save reference.
			 */
			class FilePtr : public BasePtr {
				private:
					Save &mSaveRef;
					uint32_t mReadPtr;
					
					/**
					 * Writes \c bytes to the \c Save reference from the
					 * pointer \c ptr.
					 */
					void nanoWrite(const void *ptr, uint32_t bytes);
					
					/**
					 * Reads \c bytes from the \c Save reference and stores
					 * it in \c ptr.
					 * 
					 * @exception std::out_of_range If the \c bytes argument
					 * is such that mReadPtr+bytes > file.size(), this
					 * exception is thrown.
					 */
					void nanoRead(void *ptr, uint32_t bytes);
					
				public:
					FilePtr(Save &saveObj) :
						mSaveRef(saveObj), mReadPtr(0)
					{}
					
					uint32_t getReadPos() { return mReadPtr; }
					uint32_t getWritePos() { return mSaveRef.size(); }
					
					bool eof() const
					{ return mReadPtr >= mSaveRef.size(); }
					
					uint32_t bytesLeftToRead() const
					{ return mSaveRef.size()-mReadPtr; }
					
					void setReadPtr(uint32_t pos=0)
					{ mReadPtr = pos; }
			};
			
			/**
			 * @brief Checksum calculation descriptor.
			 * 
			 * Specialization of an ordinary file pointer,
			 * provides an equivalent interface to compute a
			 * checksum without writing any data.
			 */
			class SyncPtr : public BasePtr {
				private:
					void nanoWrite(const void *ptr, uint32_t bytes)
					{
						mCksum.process_bytes(ptr, bytes);
						mSize += bytes;
					}
					
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
			 * @brief Implements the operators necessary for
			 * for saving special types.
			 * 
			 * Any type that inherits from this class, and also implements
			 * the below function pair, supports the stream operators in
			 * \c BasePtr:
			 * - save(Save::BasePtr &fp);
			 * - load(Save::BasePtr &fp);
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
