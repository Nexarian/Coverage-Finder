// FileStream.cpp: implementation of the FileStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileStream.h"
#include "DataObject.h"


template<typename GenericType>
void WriteToFileStream(GenericType Data, FileStream* Target) {
	DataObject<GenericType>* NewDatObj = new DataObject<GenericType>;
	NewDatObj->SetData(reinterpret_cast<void*>(&Data));
	Target->BasicData.push_back(NewDatObj);
}

template<typename GenericType>
void LoadFromFileStream(GenericType& Data, FileStream* Target) {
	Data = *reinterpret_cast<GenericType*>(Target->BasicData[(Target->LoadCounter)++]->GetData());
}


bool FileStream::DataObjectsInitialized = false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileStream::FileStream(FileOperation Mode) : StringId("class DataObject<struct StringBuffer>")
{
	FileMode = Mode;

	if (FileStream::DataObjectsInitialized == false) {

		FileStream::Classes().AddFactory(new Factory<DataObject<int> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<unsigned int> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<long> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<unsigned long> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<short> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<unsigned short> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<float> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<double> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<bool> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<char> >);
		FileStream::Classes().AddFactory(new Factory<DataObject<StringBuffer> >);

		FileStream::DataObjectsInitialized = true;
	}
}

FileStream::~FileStream()
{
	ClearData();
}

void FileStream::SaveArchiveToFile(const char* FileName) {

	OutputDebugString("Writing FileStream Data to ");
	OutputDebugString(FileName);
	OutputDebugString("\n");

	FileMode = Save;

	FileHandle = fopen(FileName, "wb");

	unsigned long DataSize = BasicData.size();

	//Write the total size of everything there is to save
	DebugAssert(fwrite(reinterpret_cast<const void*>(&DataSize), sizeof(unsigned int), 1, FileHandle) == 1);

	//Make sure that all the data types are put together

	DataBase* BaseBuffer;
	char* StrDataBuffer;
	
	for (unsigned long WriteLoop = 0; WriteLoop < BasicData.size(); WriteLoop++) {

		BaseBuffer = BasicData[WriteLoop];

		StrDataBuffer = const_cast<char*>(BaseBuffer->GetDerivedClassName());

		//Record the datatype of this dataobject, but first record the length of the string
		DataSize = strlen(StrDataBuffer) + 1;
		DebugAssert(fwrite(reinterpret_cast<void*>(&DataSize), sizeof(unsigned long), 1, FileHandle) == 1);

		DebugAssert(fwrite(StrDataBuffer, strlen(StrDataBuffer) + 1, 1, FileHandle) == 1); 

		if (strcmp(StrDataBuffer, StringId) == 0) { //Strings are a special case, everything else has already been handled (objects have been broken down into their more basic elements)
			StringBuffer CharArray;
			CharArray = *(reinterpret_cast<StringBuffer*>(BaseBuffer->GetData()));

			DataSize = ++CharArray.Length;

			DebugAssert(fwrite(reinterpret_cast<void*>(&DataSize), sizeof(unsigned long), 1, FileHandle) == 1);
			DebugAssert(fwrite(CharArray.StringData, CharArray.Length, 1, FileHandle) == 1);
		}
		else {
			DebugAssert(fwrite(BaseBuffer->GetData(), BaseBuffer->GetDataTypeSize(), 1, FileHandle) == 1);
		}
	}
	
	fclose(FileHandle);

	OutputDebugString("Done Saving Data!\n");
}

void FileStream::LoadArchiveFromFile(const char* FileName) {

	OutputDebugString("Loading FileStream Data from ");
	OutputDebugString(FileName);
	OutputDebugString("\n");

	ClearData(); //Erase everything currently in memory

	FileMode = Load;

	FileHandle = fopen(FileName, "rb");

	//Load the size of all the stored objects
	unsigned long SizeBuffer;
	DebugAssert(fread(reinterpret_cast<void*>(&SizeBuffer), sizeof(unsigned long), 1, FileHandle) == 1);

	DataBase* BaseBuffer;	//A buffer for each data object
	PersistantObject* PersistantBuffer;
	void* VoidBuffer;
	unsigned long SizeBufferRead;
	char* StrDataBuffer;

	for (int ReadLoop = 0; ReadLoop < SizeBuffer; ReadLoop++) {

		DebugAssert(fread(reinterpret_cast<void*>(&SizeBufferRead), sizeof(unsigned long), 1, FileHandle) == 1);
		
		StrDataBuffer = new char[SizeBufferRead];
		DebugAssert(fread(reinterpret_cast<void*>(StrDataBuffer), SizeBufferRead, 1, FileHandle) == 1);
		
		//re-create the data object
		VoidBuffer = FileStream::Classes().CreateObject(StrDataBuffer);
		delete [] StrDataBuffer;

		PersistantBuffer = reinterpret_cast<PersistantObject*>(VoidBuffer); //For some reason it crashes if we don't go to a persistantobject first, most likely because persistant object is concrete and database is virtual
		BaseBuffer = dynamic_cast<DataBase*>(PersistantBuffer);

		if (strcmp(BaseBuffer->GetDerivedClassName(), StringId) == 0) { //as always, strings are special
			StringBuffer StrBuf;
			
			DebugAssert(fread(reinterpret_cast<void*>(&SizeBufferRead), sizeof(unsigned long), 1, FileHandle) == 1);
			StrBuf.Length = SizeBufferRead;

			StrBuf.StringData = new char[StrBuf.Length];
			DebugAssert(fread(reinterpret_cast<void*>(StrBuf.StringData), StrBuf.Length, 1, FileHandle) == 1);

			BaseBuffer->SetData(reinterpret_cast<void*>(&StrBuf));
		}
		else {
			DebugAssert(fread(BaseBuffer->GetData(), BaseBuffer->GetDataTypeSize(), 1, FileHandle) == 1);
		}

		BasicData.push_back(BaseBuffer);
	}

	
	LoadCounter = 0; //Reset the load counter

	fclose(FileHandle);

	OutputDebugString("Done Loading Data!\n");
}

void FileStream::ClearData() {
	OutputDebugString("Clearing FileStream Data...\n");

	long DeleteMax = BasicData.size() - 1;

	vector<DataBase*>::iterator DatItr;

	unsigned long counter = 0;

	for (DatItr = BasicData.begin(); DatItr != BasicData.end(); DatItr++) {
		counter++;
		if (strcmp((*DatItr)->GetDerivedClassName(), StringId) == 0) {
			StringBuffer Buf = *(reinterpret_cast<StringBuffer*>((*DatItr)->GetData()));

			if (Buf.StringData != NULL) {
				delete [] Buf.StringData;
			}

			delete *DatItr;
		}
		else {
			delete *DatItr;
		}
	}

	BasicData.clear();

	OutputDebugString("Done Clearing Data!\n");
}

const bool FileStream::operator==(FileStream& CompareStream) const {
	if (BasicData.size() == CompareStream.BasicData.size()) {
		for (int CompareLoop = 0; CompareLoop < BasicData.size(); CompareLoop++) {
			if (BasicData[CompareLoop] != CompareStream.BasicData[CompareLoop]) {
				return false; //One element is not equal
			}
		}
	}
	else {
		return false;
	}
	return true;
}

FileStream& FileStream::operator=(FileStream& RightStream) {

	if (*this != RightStream) {
		ClearData();

		for (int CopyLoop = 0; CopyLoop < RightStream.BasicData.size(); CopyLoop++) {
			BasicData.push_back(RightStream.BasicData[CopyLoop]->MakeDataObjectLikeMe());
		}
	}
	return *this;
}

FileStream& FileStream::operator>>(PersistantObject& Data) {

	char* ClassName = NULL;
	(*this) >> ClassName;

	if (strcmp(ClassName, Data.DerivedClassName) == 0) {
		Data.DataTransaction(*this);
	}
	else {
		OutputDebugString("Class Load Error - Class not found in FileStream\n");
	}

	delete [] ClassName;

	return *this;
}
//To prevent initialization problems
Industry& FileStream::Classes() {
	static Industry StaticIndustry;

	return StaticIndustry;
}

FileStream& FileStream::operator<<(int Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(unsigned int Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(long Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(unsigned long Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(short Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(unsigned short Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(float Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(double Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(bool Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(char Data) {
	WriteToFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator<<(const char* Data) {

	//Strlen returns a 1-based value, we need to convert that to a zero based one
	WriteString(Data, strlen(Data));
	
	return *this;
}


FileStream& FileStream::WriteString(const char* Data, unsigned long StringSize) {

	StringBuffer Dat;

	Dat.StringData = new char[StringSize + 1];
	Dat.StringData[StringSize] = '\0';
	memcpy(Dat.StringData, Data, StringSize); //strcpy is not reliable, it uses strlen, which will not work in our case

	Dat.Length = StringSize;

	DataObject<StringBuffer>* DataObj = new DataObject<StringBuffer>;
	DataObj->SetData(reinterpret_cast<void*>(&Dat));
	BasicData.push_back(dynamic_cast<DataBase*>(DataObj));

	return *this;
}

//In case an object is going to be saved.
FileStream& FileStream::operator<<(PersistantObject& Data) {

	WriteClass(Data);

	return *this;
}


void FileStream::WriteClass(PersistantObject& Data) {
	
	(*this) << Data.DerivedClassName; //Add the class name to the file (this will notate that a class has been seralized

	Data.DataTransaction(*this); //save the rest of the class data
}

FileStream& FileStream::operator>>(int& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(unsigned int& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(long& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(unsigned long& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(short& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(unsigned short& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(float& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(double& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(bool& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(char& Data) {
	LoadFromFileStream(Data, this);
	return *this;
}

FileStream& FileStream::operator>>(char*& Data) {

	StringBuffer Dat = *(reinterpret_cast<StringBuffer*>(BasicData[LoadCounter++]->GetData()));

	char* StrBuffer = new char[Dat.Length];
	memcpy(StrBuffer, Dat.StringData, Dat.Length);

	Data = StrBuffer;
	return *this;
}

//In case an object is going to be saved.
PersistantObject* FileStream::ReadClass() {

	char* ClassName = NULL;
	(*this) >> ClassName;

	void* Buffer = FileStream::Classes().CreateObject(ClassName); //Use the loaded class name to create the object w/ the Industry

	delete [] ClassName; //Delete the name once used

	//All objects that are used in the filestream must inherit from filestream, so therefore it is possible to cast a void* to a FileStream* (safely, but dynamic cast will not work in this case)
	//However, if something goes wrong... There may be memory leaks, errors, system lockup, so don't ever try this with a non FileStream derived class
	PersistantObject* ObjectBuffer = reinterpret_cast<PersistantObject*>(Buffer); //this could potentially cause a problem (but it works in this case)

	ObjectBuffer->DataTransaction(*this); //Load the data from the class (this is a pure virtual function, so C++ will automatically resolve it to the class which implements it)

	return ObjectBuffer;
}