// FileManager.cpp: implementation of the FileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileManager.h"
#include <io.h>
#include "PersistantString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileManager::FileManager()
{
	BasicConstruct();
}

FileManager::FileManager(FileManager& CopyObject) {
	BasicConstruct();
	*this = CopyObject;
}

FileManager::~FileManager()
{
	Clear();		
}

void FileManager::BasicConstruct() {
	SetDerivedClassName(typeid(*this).name());
	FileStream::Classes().AddFactory(new Factory<FileManager>);

}

FileManager& FileManager::operator=(const FileManager& RightObject) {
	if (*this != RightObject) {

		Clear();

		vector<char*>::const_iterator RightDataItr = RightObject.Data.begin();
		vector<char*>::const_iterator RightNameItr = RightObject.Names.begin();

		vector<unsigned long>::const_iterator RightDatSizeItr = RightObject.DataSizes.begin();
		vector<unsigned long>::const_iterator RightNameSizeItr = RightObject.NameSizes.begin();

		char* StringBuffer;

		for (RightDataItr; RightDataItr != RightObject.Data.end(); ++RightDataItr) {

			//Copy the data
			StringBuffer = new char[*RightDatSizeItr];
			memcpy(StringBuffer, *RightDataItr, *RightDatSizeItr); //Use memcpy b/c we want to specify the size
			Data.push_back(StringBuffer);
			DataSizes.push_back(*RightDatSizeItr);

			//Copy the names
			StringBuffer = new char[*RightNameSizeItr + 1];
			StringBuffer[*RightNameSizeItr] = '\0';
			strcpy(StringBuffer, *RightNameItr);
			Names.push_back(StringBuffer);
			NameSizes.push_back(*RightNameSizeItr);

			RightDatSizeItr++;
			RightNameItr++;
			RightNameSizeItr++;
		}

	}

	return *this;
}

const bool FileManager::operator==(const FileManager& RightObject) const {

	if (Data.size() == RightObject.Data.size()) {

		vector<char*>::const_iterator ThisDataItr = Data.begin(), ThisNameItr = Names.begin();
		vector<unsigned long>::const_iterator ThisDatSizeItr = DataSizes.begin(), ThisNameSizeItr = NameSizes.begin();

		vector<char*>::const_iterator RightDataItr = RightObject.Data.begin(), RightNameItr = RightObject.Names.begin();
		vector<unsigned long>::const_iterator RightDatSizeItr = RightObject.DataSizes.begin(), RightNameSizeItr = RightObject.NameSizes.begin();

		for (RightDataItr; RightDataItr != RightObject.Data.end(); ++RightDataItr) {

			ThisDataItr++;
			ThisDatSizeItr++;
			RightDatSizeItr++;

			ThisNameItr++;
			ThisNameSizeItr++;
			RightNameItr++;
			RightNameSizeItr++;

			//Copy the data
			if (*ThisDatSizeItr == *RightDatSizeItr) {
				if (memcmp(*ThisDataItr, *RightDataItr, *RightDatSizeItr) != 0) return false;
			}
			else {
				return false;
			}

			//Copy the names
			if (*ThisNameSizeItr == *RightNameSizeItr) {
				if (memcmp(*ThisNameItr, *RightNameItr, *RightNameSizeItr) != 0) return false;
			}
			else {
				return false;
			}

		}
	}
	return false;
}


long FileManager::SizeOfFile(const char* FileName) {
	long FileSize;
	FILE* File;
	
	File = fopen(FileName,"rb");
	//Use a unix function to find the file size
	FileSize = _filelength(File->_file);
	fclose(File);

	return FileSize;
}

char* FileManager::RetrieveFile(const char* FileName) {
	
	int FileSize = SizeOfFile(FileName); //The size of the file to be retreived

	FILE* File = fopen(FileName, "rb"); //Pointer to the file

	char* FileData = new char[FileSize]; //The data contained within the file (so the RAM must be large enough to hold it)

	fread(FileData, FileSize, 1, File);

	fclose(File);

	return FileData;
}

void FileManager::AddFile(const char* FileName, const char* Name, unsigned long StringLength) {

	unsigned long NameSize;// = strlen(Name);
	if (StringLength == 0) {
		NameSize = strlen(Name);
	}
	else {
		NameSize = StringLength;
	}

	char* Buffer = new char[NameSize + 1];
	Buffer[NameSize] = '\0';
	strcpy(Buffer, Name);

	Names.push_back(Buffer); //Add to the names

	NameSizes.push_back(NameSize);

	Data.push_back(RetrieveFile(FileName)); //Add to the File collection

	DataSizes.push_back(SizeOfFile(FileName));
}

void FileManager::DeleteFile(const int FileIndex) {

	//Delete the file contents
	delete [] Data[FileIndex];
	Data.erase(Data.begin() + FileIndex);

	//Delete the name contents
	delete [] Names[FileIndex];
	Names.erase(Names.begin() + FileIndex);

	//Erase the sizes (of the file and the names)
	NameSizes.erase(NameSizes.begin() + FileIndex);
	DataSizes.erase(DataSizes.begin() + FileIndex);
}

void FileManager::ExtractFile(const int FileIndex, const char* DestFile) {
	FILE* File;

	//Open the file (everything is always done in binary for this)
	File = fopen(DestFile, "wb");

	//Write the stored data to the file
	fwrite(Data[FileIndex], DataSizes[FileIndex], 1, File);

	//Close the file
	fclose(File);
}

void FileManager::RenameFile(const int FileIndex, char* NewName, const unsigned long NewNameSize) {
	delete [] Names[FileIndex];
	Names[FileIndex] = NewName;
	NameSizes[FileIndex] = NewNameSize;
}

void FileManager::ReplaceFile(const int FileIndex, const char* NewFile) {
	delete [] Data[FileIndex];
	Data[FileIndex] = RetrieveFile(NewFile);
	DataSizes[FileIndex] = SizeOfFile(NewFile);
}

void FileManager::Clear() {
	//Clear all data storage
	char* DataBuffer;
	char* NameBuffer;

	for (int FileLoop = (Data.size() - 1); FileLoop >= 0; FileLoop--) {
		//A buffer to delete the data later
		DataBuffer = Data[FileLoop];
		NameBuffer = Names[FileLoop];

		//Pop the data
		Data.pop_back();
		Names.pop_back();

		//Delete the data
		delete [] DataBuffer;
		delete [] NameBuffer;
	}
	//Now just erase the sizes
	DataSizes.clear(); //Erase the sizes of the files
	NameSizes.clear();
}

void FileManager::SaveAll(const char* FileName) {
	//Save the sizes (so it knows the sizes to load later)
	FILE* File = fopen(FileName, "wb");

	unsigned int DataBuffer;
	DataBuffer = Data.size();

	//Write the total number of items in the file
	fwrite(reinterpret_cast<const void*>(&DataBuffer), sizeof(unsigned long), 1, File);

	int FileLoop;
	//Write the sizes of the files
	for (FileLoop = 0; FileLoop < DataSizes.size(); FileLoop++) {

		DataBuffer = DataSizes[FileLoop];

		fwrite(reinterpret_cast<const void*>(&DataBuffer), sizeof(unsigned long), 1, File);

	}

	//Now write the files
	for (FileLoop = 0; FileLoop < Data.size(); FileLoop++) {

		fwrite(Data[FileLoop], DataSizes[FileLoop], 1, File);

	}

	//Now write the sizes of the names of the files
	for (FileLoop = 0; FileLoop < NameSizes.size(); FileLoop++) {

		DataBuffer = NameSizes[FileLoop];

		fwrite(reinterpret_cast<const void*>(&DataBuffer), sizeof(unsigned long), 1, File);
	}

	//Now write the names of the files
	for (FileLoop = 0; FileLoop < Names.size(); FileLoop++) {

		fwrite(Names[FileLoop], NameSizes[FileLoop], 1, File);

	}

	//Close the file
	fclose(File);
}
void FileManager::LoadAll(const char* FileName) {
	int SizeOfData = 0;
	Clear(); //erase everything
	//read the sizes of the data
	FILE* File;

	File = fopen(FileName, "rb");

	unsigned long DataBuffer; //A buffer to store data read from the file until it can be added to storage

	char* StringBuffer; //StringBuffer is not deleted b/c it is a pointer to data that will be deleted with the clear function

	//Read the total number of items in the file
	fread(reinterpret_cast<void*>(&SizeOfData), sizeof(unsigned long), 1, File);

	int FileLoop;

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) { 

		fread(reinterpret_cast<void*>(&DataBuffer), sizeof(unsigned long), 1, File);

		DataSizes.push_back(DataBuffer);

	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {

		StringBuffer = new char[DataSizes[FileLoop]];

		fread(StringBuffer, DataSizes[FileLoop], 1, File);

		Data.push_back(StringBuffer);

	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {

		fread(reinterpret_cast<void*>(&DataBuffer), sizeof(unsigned long), 1, File);

		NameSizes.push_back(DataBuffer);

	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {

		StringBuffer = new char[NameSizes[FileLoop]];

		fread(StringBuffer, NameSizes[FileLoop], 1, File);

		Names.push_back(StringBuffer);

	}

	fclose(File);
}

void FileManager::DataTransaction(FileStream &File) {

	int IntBuffer, SizeBuffer = NumberOfFiles(), FileLoop = 0;
	PersistantString StringBuffer;

	if (File.IsSaving() == true) {

		File << SizeBuffer;

		for (FileLoop = 0; FileLoop < SizeBuffer; FileLoop++) {

			IntBuffer = DataSizes[FileLoop];
			File << IntBuffer;

			File.WriteString(Data[FileLoop], IntBuffer);

			IntBuffer = NameSizes[FileLoop];
			File << IntBuffer;

			File.WriteString(Names[FileLoop], IntBuffer);

		}

	}
	else { //IsLoading() == true

		Clear();

		char* PrimitiveBuffer;

		File >> SizeBuffer;

		for (FileLoop = 0; FileLoop < SizeBuffer; FileLoop++) {

			File >> IntBuffer;
			DataSizes.push_back(IntBuffer);

			File >> PrimitiveBuffer;
			Data.push_back(PrimitiveBuffer);

			File >> IntBuffer;
			NameSizes.push_back(IntBuffer);

			File >> PrimitiveBuffer;
			Names.push_back(PrimitiveBuffer);
		}
	}

}