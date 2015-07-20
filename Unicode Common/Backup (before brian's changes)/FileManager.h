// FileManager.h: interface for the FileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_)
#define AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistantString.h"

#include <vector>
using namespace std;

class FileManager : public PersistantObject
{
public:
	FileManager();
	FileManager(FileManager& CopyObject);
	virtual ~FileManager();


	FileManager& operator=(const FileManager& RightObject);
	const bool operator==(const FileManager& RightObject) const;
	const bool operator!=(const FileManager& RightObject) const {return !(*this == RightObject);}

	vector<char*> Data;
	vector<char*> Names;

	unsigned long NumberOfFiles() {return Data.size();}

	long SizeOfFile(const char* FileName);
	void AddFile(const char* FileName, const char* Name, unsigned long StringLength = 0);
	void DeleteFile(int FileIndex); 
	char* RetrieveFile(const char* FileName);
	void RenameFile(const int FileIndex, char* NewName, const unsigned long NameSize);
	void ReplaceFile(const int FileIndex, const char* NewFile);
	void ExtractFile(const int FileIndex, const char* DestFile);
	void Clear();
	void SaveAll(const char* FileName);
	void LoadAll(const char* FileName);

	void DataTransaction(FileStream& File);

private:
	vector<unsigned long> DataSizes; //Sizes of the data in parallel to the actual data
	vector<unsigned long> NameSizes;

	void BasicConstruct();
};

#endif // !defined(AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_)
