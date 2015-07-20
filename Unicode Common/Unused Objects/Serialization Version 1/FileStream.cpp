// FileStream.cpp: implementation of the FileStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileStream.h"
#include "Industry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileStream::FileStream(FileOperation Mode)
{
	FileMode = Mode;
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

	unsigned int DataSize = BasicData.size();

	DebugAssert(fwrite(reinterpret_cast<const void*>(&DataSize), sizeof(unsigned int), 1, FileHandle) == 1);

	//Make sure that all the data types are put together

	DataObject* Buffer;

	TypeList TypeBuf;

	for (unsigned long WriteLoop = 0; WriteLoop < BasicData.size(); WriteLoop++) {

		Buffer = BasicData[WriteLoop];

		TypeBuf = Buffer->GetActiveType();

		DebugAssert(fwrite(reinterpret_cast<const void*>((unsigned int*)&TypeBuf), sizeof(unsigned int), 1, FileHandle) == 1);

		switch(Buffer->GetActiveType()) {
			case Int: 

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetIntAddr()), sizeof(int), 1, FileHandle) == 1);
				break;

			case UnInt:

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetUnsignedIntAddr()), sizeof(unsigned int), 1, FileHandle) == 1);
				break;

			case Long:

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetLongAddr()), sizeof(long), 1, FileHandle) == 1);
				break;

			case UnLong:

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetUnsignedLongAddr()), sizeof(unsigned long), 1, FileHandle) == 1);
				break;

			case Short:

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetShortAddr()), sizeof(short), 1, FileHandle) == 1);
				break;

			case UnShort:

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetUnsignedShortAddr()), sizeof(unsigned short), 1, FileHandle) == 1);
				break;

			case Float: 
								
				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetFloatAddr()), sizeof(float), 1, FileHandle) == 1);
				break;

			case Double: 
				
				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetDoubleAddr()), sizeof(double), 1, FileHandle) == 1);
				break;

			case Boolean: 
				
				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetBoolAddr()), sizeof(bool), 1, FileHandle) == 1);
				break;
						  
			case Character: 

				DebugAssert(fwrite(reinterpret_cast<const void*>(&Buffer->GetCharAddr()), sizeof(char), 1, FileHandle) == 1);
				break;
							
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
	unsigned int SizeBuffer;
	DebugAssert(fread(reinterpret_cast<void*>(&SizeBuffer), sizeof(unsigned int), 1, FileHandle) == 1);

	DataObject* Buffer;	//A buffer for each data object

	//An integer buffer for loading the type for each data object
	unsigned int TypeBuffer = 0;

	for (int ReadLoop = 0; ReadLoop < SizeBuffer; ReadLoop++) {

		//re-create the data object
		Buffer = new DataObject;
		
		DebugAssert(fread(reinterpret_cast<void*>(&TypeBuffer), sizeof(unsigned int), 1, FileHandle) == 1);

		Buffer->SetActiveType((TypeList)TypeBuffer);

		switch (TypeBuffer) {
			case Int: 

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetIntAddr()), sizeof(int), 1, FileHandle) == 1);
				break;

			case UnInt:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetUnsignedIntAddr()), sizeof(unsigned int), 1, FileHandle) == 1);
				break;

			case Long:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetLongAddr()), sizeof(long), 1, FileHandle) == 1);
				break;

			case UnLong:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetUnsignedLongAddr()), sizeof(unsigned long), 1, FileHandle) == 1);
				break;

			case Short:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetShortAddr()), sizeof(short), 1, FileHandle) == 1);
				break;

			case UnShort:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetUnsignedShortAddr()), sizeof(unsigned short), 1, FileHandle) == 1);
				break;

			case Float:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetFloatAddr()), sizeof(float), 1, FileHandle) == 1);
				break;

			case Double:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetDoubleAddr()), sizeof(double), 1, FileHandle) == 1);
				break;

			case Boolean:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetBoolAddr()), sizeof(bool), 1, FileHandle) == 1);
				break;

			case Character:

				DebugAssert(fread(reinterpret_cast<void*>(&Buffer->GetCharAddr()), sizeof(char), 1, FileHandle) == 1);
				break;

		}

		BasicData.push_back(Buffer);
	}

	
	LoadCounter = 0; //Reset the load counter

	fclose(FileHandle);

	OutputDebugString("Done Loading Data!\n");
}

void FileStream::ClearData() {
	OutputDebugString("Clearing FileStream Data...\n");

	long DeleteMax = BasicData.size() - 1;

	vector<DataObject*>::iterator DatItr;

	for (DatItr = BasicData.begin(); DatItr != BasicData.end(); DatItr++) {
		delete *DatItr;
	}

	BasicData.clear();

	OutputDebugString("Done Clearing Data!\n");

	//for (long DeleteLoop = DeleteMax; DeleteLoop >= 0; DeleteLoop--) {
		//delete BasicData[DeleteLoop];
		//BasicData.erase(BasicData.begin() + DeleteLoop);
	//}
	//BasicData.clear();
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

		DataObject* ObjPtr;

		for (int CopyLoop = 0; CopyLoop < RightStream.BasicData.size(); CopyLoop++) {
			ObjPtr = new DataObject;
			*ObjPtr = *RightStream.BasicData[CopyLoop];
			BasicData.push_back(ObjPtr);
		}
	}
	return *this;
}


FileStream& FileStream::operator<<(int Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Int);
	NewDatObj->SetInt(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(unsigned int Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(UnInt);
	NewDatObj->SetUnsignedInt(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(long Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Long);
	NewDatObj->SetLong(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(unsigned long Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(UnLong);
	NewDatObj->SetUnsignedLong(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(short Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Short);
	NewDatObj->SetShort(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(unsigned short Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(UnShort);
	NewDatObj->SetUnsignedShort(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(float Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Float);
	NewDatObj->SetFloat(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(double Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Double);
	NewDatObj->SetDouble(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(bool Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Boolean);
	NewDatObj->SetBool(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(char Data) {
	DataObject* NewDatObj = new DataObject;
	NewDatObj->SetActiveType(Character);
	NewDatObj->SetChar(Data);
	BasicData.push_back(NewDatObj);
	return *this;
}

FileStream& FileStream::operator<<(const char* Data) {

	//Strlen returns a 1-based value, we need to convert that to a zero based one
	WriteString(Data, strlen(Data));
	
	return *this;
}

FileStream& FileStream::WriteString(const char* Data, unsigned long StringSize) {
	(*this) << StringSize;

	for (int StringLoop = 0; StringLoop < StringSize; StringLoop++) {
		(*this) << Data[StringLoop];
	}

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


//overloaded functions of the overloaded operator >>
//built in C++ data types
FileStream& FileStream::operator>>(int& Data) {
	Data = BasicData[LoadCounter++]->GetInt();
	return *this;
}

FileStream& FileStream::operator>>(unsigned int& Data) {
	Data = BasicData[LoadCounter++]->GetUnsignedInt();
	return *this;
}

FileStream& FileStream::operator>>(long& Data) {
	Data = BasicData[LoadCounter++]->GetLong();
	return *this;
}

FileStream& FileStream::operator>>(unsigned long& Data) {
	Data = BasicData[LoadCounter++]->GetUnsignedLong();
	return *this;
}

FileStream& FileStream::operator>>(short& Data) {
	Data = BasicData[LoadCounter++]->GetShort();
	return *this;
}

FileStream& FileStream::operator>>(unsigned short& Data) {
	Data = BasicData[LoadCounter++]->GetUnsignedShort();
	return *this;
}

FileStream& FileStream::operator>>(float& Data) {
	Data = BasicData[LoadCounter++]->GetFloat();
	return *this;
}

FileStream& FileStream::operator>>(double& Data) {
	Data = BasicData[LoadCounter++]->GetDouble();
	return *this;
}

FileStream& FileStream::operator>>(bool& Data) {
	Data = BasicData[LoadCounter++]->GetBool();
	return *this;
}

FileStream& FileStream::operator>>(char& Data) {
	Data = BasicData[LoadCounter++]->GetChar();
	return *this;
}

FileStream& FileStream::operator>>(char*& Data) {

	unsigned long CharLength = 0;
	(*this) >> CharLength; //CharLength is 1-based

	char* StringBuffer = new char[CharLength + 1];
	StringBuffer[CharLength] = '\0';

	char CharBuffer; //A character buffer for loading from file

	for (int GetLoop = 0; GetLoop < CharLength; GetLoop++) {
		(*this) >> CharBuffer;
		StringBuffer[GetLoop] = CharBuffer;
	}

	Data = StringBuffer;
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

FileStream& FileStream::operator>>(PersistantObject& DataObject) {

	char* ClassName = NULL;
	(*this) >> ClassName;

	if (strcmp(ClassName, DataObject.DerivedClassName) == 0) {
		DataObject.DataTransaction(*this);
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
