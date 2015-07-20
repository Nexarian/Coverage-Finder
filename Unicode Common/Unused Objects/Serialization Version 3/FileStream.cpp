// FileStream.cpp: implementation of the FileStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileStream::FileStream(FileOperation Mode)
{
	FileMode = Mode;
	FileHandle = NULL;
}

FileStream::~FileStream()
{
	if (FileHandle != NULL) CloseArchiveFile();
}

bool FileStream::OpenArchiveFile(const char* FileName) {

	if (FileHandle != NULL) {
		OutputDebugString("File Already Open: Close Current File Before Opening a New One\n");
		return false;
	}

	switch (FileMode) {
		case Load:
			OutputDebugString("Loading FileStream Data From ");
			break;
		case Save:
			OutputDebugString("Saving FileStream Data To ");
			break;
	}

	OutputDebugString(FileName);
	OutputDebugString("\n");

	FileHandle = fopen(FileName, (FileMode == Load ? "rb" : "wb"));

	if (FileHandle == NULL) {
		OutputDebugString("Open File Failed.\n");
		OpFileName = NULL;
	}
	else {
		long StrLen = strlen(FileName);

		OpFileName = new char[StrLen + 1];
		OpFileName[StrLen] = '\0';
		strcpy(OpFileName, FileName);

		OutputDebugString("File Opened Successfully!\n");
	}

	return !(FileHandle == NULL);
}

bool FileStream::CloseArchiveFile() {
	OutputDebugString("Closing FileStream\n");

	if (OpFileName != NULL) delete [] OpFileName;
	
	if (fclose(FileHandle) == 0) {
		FileHandle = NULL;
		OutputDebugString("File Closed Successfully!\n");
		return true;
	}
	else {
		OutputDebugString("File Close Failed!\n");
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
			OutputDebugString("FileStream Copy Successful!");
		}
		else {
			OutputDebugString("FileStream Copy Failed!");
		}
	}
	return *this;
}


//To prevent initialization problems
Industry& FileStream::Classes() {
	static Industry StaticIndustry;

	return StaticIndustry;
}

FileStream& FileStream::WriteString(const char* Data, unsigned long StringSize) {

	StringSize++;
	fwrite(reinterpret_cast<const void*>(&StringSize), sizeof(unsigned long), 1, FileHandle);
	fwrite(reinterpret_cast<const void*>(Data), StringSize, 1, FileHandle);

	return *this;
}

void FileStream::WriteClass(PersistantObject& Data) {
	
	(*this) << (const char*)Data.DerivedClassName; //Add the class name to the file (this will notate that a class has been seralized

	Data.DataTransaction(*this); //save the rest of the class data
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