// FileStream.cpp: implementation of the FileStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileStream::FileStream(FSStream_FileOperation Mode)
{
	FileMode = Mode;
	FileHandle = NULL;
}

FileStream::~FileStream()
{
	if (FileHandle != NULL) CloseArchiveFile();
}

bool FileStream::OpenArchiveFile(const wchar_t* FileName) {

	if (FileHandle != NULL) {
		OutputDebugString(L"File Already Open: Close Current File Before Opening a New One\n");
		return false;
	}

	switch (FileMode) {
		case Load:
			OutputDebugString(L"Loading FileStream Data From ");
			break;
		case Save:
			OutputDebugString(L"Saving FileStream Data To ");
			break;
	}

	OutputDebugString(FileName);
	OutputDebugString(L"\n");

	if (fopen_s(&FileHandle, FileName, (FileMode == Load ? "rb" : "wb")) != 0) {
		OutputDebugString(L"Open File Failed.\n");
		OpFileName = NULL;
	}
	else {
		long StrLen = wcslen(FileName) + 1;

		OpFileName = new char[StrLen];
		OpFileName[StrLen - 1] = '\0';
		strcpy_s(OpFileName, StrLen, FileName);

		OutputDebugString(L"File Opened Successfully!\n");
	}

	return !(FileHandle == NULL);
}

bool FileStream::CloseArchiveFile() {
	OutputDebugString(L"Closing FileStream\n");

	if (OpFileName != NULL) delete [] OpFileName;
	
	if (fclose(FileHandle) == 0) {
		FileHandle = NULL;
		OutputDebugString(L"File Closed Successfully!\n");
		return true;
	}
	else {
		OutputDebugString(L"File Close Failed!\n");
		return false;
	}
}

const char* FileStream::GetFileName() {
	return OpFileName;
}

const bool FileStream::operator==(FileStream& CompareStream) const {
	if (FileMode == CompareStream.FileMode) {
		return (FileHandle == CompareStream.FileHandle);
	}
	else {
		return false;
	}
}

FileStream& FileStream::operator=(FileStream& RightStream) {

	if (*this != RightStream) {
		bool SuccessIndicator = false;

		FileMode = RightStream.FileMode;
		if (CloseArchiveFile()) {
			if (OpenArchiveFile(RightStream.FileHandle->_tmpfname)) {
				SuccessIndicator = true;
			}
		}
		if (SuccessIndicator)  {
			OutputDebugString(L"FileStream Copy Successful!\n");
		}
		else {
			OutputDebugString(L"FileStream Copy Failed!\n");
		}
	}
	return *this;
}

//To prevent initialization problems
Industry<PersistentObject>& FileStream::Classes() {
	static Industry<PersistentObject> StaticIndustry;

	return StaticIndustry;
}

FileStream& FileStream::WriteString(const char* Data, unsigned long StringSize) {

	StringSize++;
	fwrite(reinterpret_cast<const void*>(&StringSize), sizeof(unsigned long), 1, FileHandle);
	fwrite(reinterpret_cast<const void*>(Data), StringSize, 1, FileHandle);

	return *this;
}

void FileStream::WriteClass(PersistentObject& Data) {
	(*this) << (const char*)Data.DerivedClassName(); //Add the class name to the file (this will notate that a class has been seralized
	Data.WriteData(*this);
}

//In case an object is going to be saved.
PersistentObject* FileStream::ReadClass() {
	char* ClassName = 0;
	(*this) >> ClassName;

	PersistentObject* Buffer = FileStream::Classes().CreateObject(ClassName); //Use the loaded class name to create the object w/ the Industry

	delete [] ClassName; //Delete the name once used

	Buffer->ReadData(*this); //Load the data from the class (this is a pure virtual function, so C++ will automatically resolve it to the class which implements it)

	return Buffer;
}