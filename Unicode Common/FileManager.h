// FileManager.h: interface for the FileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_)
#define AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistentString.h"
#include <vector>

class FileManager : public PersistentObject
{
public:
	FileManager();
	FileManager(FileManager& CopyObject);
	virtual ~FileManager();

	FileManager& operator=(const FileManager& RightObject);
	const bool operator==(const FileManager& RightObject) const;
	const bool operator!=(const FileManager& RightObject) const {return !(*this == RightObject);}

	std::vector<char*> Data;
	std::vector<char*> Names;

	unsigned long NumberOfFiles() const {return (unsigned long)Data.size();}

	long SizeOfFile(const char* FileName);
	void AddFile(const char* FileName, const char* Name, unsigned long StringLength = 0);
	void DeleteFile(int FileIndex); 
	char* RetrieveFile(const char* FileName);
	void RenameFile(const int FileIndex, char* NewName, const unsigned long NameSize);
	void ReplaceEmbeddedFile(const int FileIndex, const char* NewFile);
	void ExtractFile(const int FileIndex, const char* DestFile);
	void Clear();
	bool SaveAll(const char* FileName);
	bool LoadAll(const char* FileName);

	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;

private:
	std::vector<unsigned long> DataSizes; //Sizes of the data in parallel to the actual data
	std::vector<unsigned long> NameSizes;
};

#endif // !defined(AFX_FILEMANAGER_H__FC58EFA1_3979_11D4_A81C_0050BAA28E02__INCLUDED_)
