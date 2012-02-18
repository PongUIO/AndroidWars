#ifndef DASCRIPT_H
#define DASCRIPT_H

#include <string>
#include <map>
#include <vector>

class Script {
	public:
		/**
		 * Data object.
		 * Contains a data identifier along
		 * with any number of string arguments.
		 */
		class Data {
			public:
				explicit Data(const std::string &id="") : mId(id)
					{}
				~Data() {}
				
				void setLineData(size_t line, size_t col)
				{	mLine=line; mCol=col; }
				size_t getLine() const { return mLine; }
				size_t getCol() const { return mCol; }
				
				void addArg(const std::string &arg)
					{ mArgs.push_back(arg); }
				
				std::string &createArg()
					{ mArgs.push_back(std::string()); return mArgs.back(); }
				
				size_t argCount() const { return mArgs.size(); }
				bool isDefined() const { return !mId.empty(); }
				const std::string &getId() const { return mId; }
				
				const std::string &firstArg() const
				{
					if(argCount()>0)
						return mArgs[0];
					else return sEmpty;
				}
				
				const std::string &getArg(size_t index) const
				{
					if(index<argCount())
						return mArgs[index];
					else return sEmpty;
				}
				
				void dump(int indent=0);
				
				static Data &emptyData();
				
			private:
				typedef std::vector<std::string> ArgVec;
				
				size_t mLine, mCol;
				std::string mId;
				ArgVec mArgs;
				
				static std::string sEmpty;
		};
		
		enum ObjectType {
			OtInvalid=0,
			OtData,
			OtBlock
		};
		
		/**
		 * Block object.
		 * Contains an identifier and
		 * any number of child blocks or
		 * data objects.
		 */
		class Block {
			private:
				struct ObjectData {
					ObjectData() : mType(OtInvalid) {}
					
					explicit ObjectData(Block *block) :
						mType(OtBlock) { u.block=block; }
					explicit ObjectData(Data *data) :
						mType(OtData) { u.data=data; }
					
					ObjectType mType;
					
					union {
						Block *block;
						Data *data;
					} u;
				};
				typedef std::vector<ObjectData> ObjectVector;
			
			public:
				typedef std::pair<std::string,Data> DataPair;
				typedef std::pair<std::string,Block*> BlockPair;
				
				typedef std::multimap<std::string, Data> DataMap;
				typedef std::multimap<std::string, Block*> BlockMap;
				
				typedef std::pair<DataMap::iterator, DataMap::iterator> DataRange;
				
				Block(Block *p=0, const std::string &id="") :
					mId(id), mParent(p)
					{}
				~Block();
				
				void setLineData(size_t line, size_t col)
				{	mLine=line; mCol=col; }
				size_t getLine() const { return mLine; }
				size_t getCol() const { return mCol; }
				
				Block *getBlock(const std::string &id) const {
					BlockMap::const_iterator i = mBlock.find(id);
					if(i == mBlock.end()) return 0;
					else return i->second;
				}
				
				Block *createBlock(
					const std::string &id,
					size_t line=0, size_t col=0)
				{
					Block *block = new Block(this,id);
					mBlock.insert( BlockPair(id, block) );
					block->setLineData(line,col);
					
					mObjectData.push_back( ObjectData(block) );
					return block;
				}
				
				DataRange getData(const std::string &id)
				{	return mData.equal_range(id); }
				
				Data &getDataSimple(const std::string &id)
				{	DataMap::iterator i = mData.find(id);
					if(i == mData.end())
						i=mData.insert( DataPair(id, Data()) );
					return i->second;
				}
				
				const std::string &getDataFirst(const std::string &id)
				{	return getDataSimple(id).firstArg(); }
				
				Data &createData(
					const std::string &id,
					size_t line=0, size_t col=0)
				{
					DataMap::iterator i = mData.insert( DataPair(id,Data(id)) );
					i->second.setLineData(line,col);
					
					mObjectData.push_back( ObjectData(&i->second) );
					return i->second;
				}
				
				BlockMap &getBlockMap()
				{	return mBlock; }
				
				size_t getSize() const
				{	return mObjectData.size(); }
				
				ObjectType getTypeOf(size_t index)
				{	return index<getSize() ?
					mObjectData[index].mType : OtInvalid; }
				
				Block *getBlockIndex(size_t index)
				{	return index<getSize() &&
						mObjectData[index].mType==OtBlock ?
						mObjectData[index].u.block : 0;
				}
				
				Data *getDataIndex(size_t index)
				{	return index<getSize() &&
						mObjectData[index].mType==OtData ?
						mObjectData[index].u.data : 0;
				}
					
				
				const std::string &getId() const
				{	return mId; }
				
				Block *getParent() { return mParent; }
				
			private:
				void setParent(Block *parent) { mParent = parent; }
				
				size_t mLine, mCol;
				
				DataMap mData;
				BlockMap mBlock;
				ObjectVector mObjectData;
				
				std::string mId;
				Block *mParent;
		};
		
		enum ErrorType {
			ErrNoCloseString = 0,
			ErrBlockNoClosing,
			ErrIllegalCharacter,
			ErrPrematureEnd,
			
			WarnReqNewline,
			
			WarningStart = WarnReqNewline
		};
		
		/**
		 * Contains information about errors
		 * that occured during compilation.
		 */
		struct Error {
			Error(ErrorType type) : type(type)
				{}
			
			ErrorType type;
			size_t stLine,stCol;
			size_t enLine,enCol;
		};
		typedef std::vector<Error> ErrorVec;
		
		void compile(const std::string &str);
		const ErrorVec &getError() const
		{	return mErrors; }
		
		static std::string errorStr(const Error &err);
		
		Block &getRoot()
		{	return mRoot; }
		
	private:
		Block mRoot;
		ErrorVec mErrors;
		
		void addError(ErrorType type,
			size_t sL, size_t sC, size_t eL=0, size_t eC=0)
		{
			Error err = Error(type);
			err.stLine = sL;
			err.stCol = sC;
			err.enLine = eL;
			err.enCol = eC;
			mErrors.push_back( err );
		}
		
		friend class TokenData;
};

#endif
