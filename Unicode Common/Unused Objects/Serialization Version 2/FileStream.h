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
#include "PersistantObject.h"

using namespace std;

class DataBase;

enum FileOperation {
	Load,
	Save
};

struct StringBuffer { //Used with the char* data type (an exception to the rule of standard data types)
	char* StringData;
	unsigned long Length;
};

class FileStream;

template<typename GenericType>
void WriteToFileStream(GenericType Data, FileStream* Target);

template<typename GenericType>
void LoadFromFileStream(GenericType& Data, FileStream* Target);

class FileStream  
{
public:
	FileStream(FileOperation Mode);
	virtual ~FileStream();

	template<typename GenericType>
	friend void WriteToFileStream(GenericType Data, FileStream* Target);

	template<typename GenericType>
	friend void LoadFromFileStream(GenericType& Data, FileStream* Target);


	void SaveArchiveToFile(const char* FileName);
	void LoadArchiveFromFile(const char* FileName);

	FileStream& operator=(FileStream& LeftFile);
	const bool operator==(FileStream& LeftFile) const;
	const bool operator!=(FileStream& LeftFile) const {return !(*this == LeftFile);}

	//overloaded functions of the overloaded operator <<
	//built in C++ data types

	//Integers
	FileStream& operator<<(const int Data);
	FileStream& operator<<(const unsigned int Data);
	//Longs
	FileStream& operator<<(const long Data);
	FileStream& operator<<(const unsigned long Data);
	//Shorts
	FileStream& operator<<(const short Data);
	FileStream& operator<<(const unsigned short Data);
	//Others
	FileStream& operator<<(const float Data);
	FileStream& operator<<(const double Data);
	FileStream& operator<<(const bool Data);
	FileStream& operator<<(const char Data);
	FileStream& operator<<(const char* Data);

	FileStream& WriteString(const char* Data, unsigned long StringSize);

	//In case an object is going to be saved.
	FileStream& operator<<(PersistantObject& Data);
	void WriteClass(PersistantObject& Data);

	//overloaded functions of the overloaded operator >>
	//built in C++ data types

	//Integers
	FileStream& operator>>(int& Data);
	FileStream& operator>>(unsigned int& Data);
	//Longs
	FileStream& operator>>(long& Data);
	FileStream& operator>>(unsigned long& Data);
	//Shorts
	FileStream& operator>>(short& Data);
	FileStream& operator>>(unsigned short& Data);
	//Others
	FileStream& operator>>(float& Data);
	FileStream& operator>>(double& Data);
	FileStream& operator>>(bool& Data);
	FileStream& operator>>(char& Data);
	FileStream& operator>>(char*& Data);

	//In case an object is going to be loaded.
	FileStream& operator>>(PersistantObject& DataClass);
	PersistantObject* ReadClass();

	static bool DataObjectsInitialized;
	static Industry& Classes();

	void ClearData();

	bool IsLoading() {return (FileMode == Load);}
	bool IsSaving() {return (FileMode == Save);}

	//Functions that should NOT be used unless you know what your doing
	int GetLoadPosition() {return LoadCounter;}
	void SetLoadPosition(int NewPosition) {LoadCounter = NewPosition;}

private:
	FILE* FileHandle;
	FileOperation FileMode;

	int LoadCounter; //A counter that is the load state

	vector<DataBase*> BasicData;

	const char* StringId;
};

#endif // !defined(AFX_FILESTREAM_H__07E48B9B_5430_4630_9BD2_12FFB2CCE7F9__INCLUDED_)
