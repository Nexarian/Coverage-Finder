// FileStream.h: interface for the FileStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESTREAM_H__07E48B9B_5430_4630_9BD2_12FFB2CCE7F9__INCLUDED_)
#define AFX_FILESTREAM_H__07E48B9B_5430_4630_9BD2_12FFB2CCE7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <typeinfo.h>
#include <vector>
#include "Industry.h"
#include "PersistentObject.h"
#include "Main.h"

enum FSStream_FileOperation {
	Load,
	Save
};

class FileStream  
{
public:
	FileStream(FSStream_FileOperation Mode);
	virtual ~FileStream();

	bool OpenArchiveFile(const char* FileName);
	bool CloseArchiveFile();

	const char* GetFileName();

	FileStream& operator=(FileStream& LeftFile);
	const bool operator==(FileStream& LeftFile) const;
	const bool operator!=(FileStream& LeftFile) const {return !(*this == LeftFile);}

	//overloaded functions of the overloaded operator <<
	//built in C++ data types
	
	template<typename GenericType>
	FileStream& operator<<(GenericType Data) {

		const char* TypeIdName = typeid(GenericType).name();

		char* Buf = StrMid(TypeIdName, 0, 5, (long)strlen(TypeIdName));

		if ((Buf != 0) && ((strncmp(Buf, "class", 4) == 0) || (strncmp(Buf, "struct", 5) == 0))) { //Don't save unless we're sure it's a class
			WriteClass(reinterpret_cast<PersistentObject&>(Data));

			delete [] Buf;
			return *this;
		}
		
		fwrite(reinterpret_cast<const void*>(&Data), sizeof(GenericType), 1, FileHandle);

		delete [] Buf;
		return *this;
	}

	template<> FileStream& operator<<(char* Data) {
		//Strlen returns a 1-based value, we need to convert that to a zero based one
		WriteString(Data, (unsigned long)strlen(Data));

		return *this;
	}

	template<> FileStream& operator<<(const char* Data) {

		//Strlen returns a 1-based value, we need to convert that to a zero based one
		WriteString(Data, (long)strlen(Data));

		return *this;
	}
	
	//In case an object is going to be saved.the operator<< and the WriteClass functions are defined (WriteClass for parallelism)
	FileStream& WriteString(const char* Data, unsigned long StringSize); //WriteString has a special purpose because it allows a size to be specified, operator<< just uses strlen(String);

	//overloaded functions of the overloaded operator >>
	//built in C++ data types

	template<typename GenericType>
	FileStream& operator>>(GenericType& Data) {

		const wchar_t* TypeIdName = typeid(GenericType).name();

		wchar_t* Buf = StrMid(TypeIdName, 0, 4, (long)strlen(TypeIdName));

		if ((Buf != 0) && ((strncmp(Buf, "class", 4) == 0) || (strncmp(Buf, "struct", 5) == 0))) {

			PersistentObject& DataBuffer = reinterpret_cast<PersistentObject&>(Data);

			char* ClassName = NULL;
			(*this) >> ClassName;

			if ((strcmp(ClassName, DataBuffer.DerivedClassName()) == 0)) {
				DataBuffer.ReadData(*this);
			}
			else {
				OutputDebugString(L"Class Load Error - Class '");
				OutputDebugString(ClassName);
				OutputDebugString(L"' not found in FileStream.\n");
			}

			delete [] ClassName;
			
			delete [] Buf;
			return *this;
		}

		fread(reinterpret_cast<void*>(&Data), sizeof(GenericType), 1, FileHandle);

		delete [] Buf;
		return *this;
	}

	template<> FileStream& operator>>(char*& Data)	{

		unsigned long DataSize;
		fread(reinterpret_cast<void*>(&DataSize), sizeof(unsigned long), 1, FileHandle);

		char* StrBuffer = new char[DataSize];

		fread(reinterpret_cast<void*>(StrBuffer), DataSize, 1, FileHandle);

		Data = StrBuffer;
		return *this;
	}

	//In case an object is going to be loaded.
	void WriteClass(PersistentObject& Data);
	PersistentObject* ReadClass(); //ReadClass actually CREATES the class that was saved, using the bit operators mearly fills the current class

	static Industry<PersistentObject>& Classes();

	const bool IsLoading() const {return (FileMode == Load);}
	const bool IsSaving() const {return (FileMode == Save);}

private:
	FILE* FileHandle;
	FSStream_FileOperation FileMode;

	char* OpFileName;
};

#endif // !defined(AFX_FILESTREAM_H__07E48B9B_5430_4630_9BD2_12FFB2CCE7F9__INCLUDED_)
