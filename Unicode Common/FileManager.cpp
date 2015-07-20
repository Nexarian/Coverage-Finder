// FileManager.cpp: implementation of the FileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileManager.h"
#include "PersistentString.h"
#include <io.h>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
FileManager::FileManager()
{
}

FileManager::FileManager(FileManager& CopyObject) {
	*this = CopyObject;
}

FileManager::~FileManager()
{
	Clear();		
}

FileManager& FileManager::operator=(const FileManager& RightObject) {
	if (*this != RightObject) {

		Clear();

		std::vector<char*>::const_iterator RightDataItr = RightObject.Data.begin();
		std::vector<char*>::const_iterator RightNameItr = RightObject.Names.begin();

		std::vector<unsigned long>::const_iterator RightDatSizeItr = RightObject.DataSizes.begin();
		std::vector<unsigned long>::const_iterator RightNameSizeItr = RightObject.NameSizes.begin();

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
			strcpy_s(StringBuffer, *RightNameSizeItr, *RightNameItr);
			Names.push_back(StringBuffer);
			NameSizes.push_back(*RightNameSizeItr);

			++RightDatSizeItr;
			++RightNameItr;
			++RightNameSizeItr;
		}

	}

	return *this;
}

const bool FileManager::operator==(const FileManager& RightObject) const {

	if (Data.size() == RightObject.Data.size()) {

		std::vector<char*>::const_iterator ThisDataItr = Data.begin(), ThisNameItr = Names.begin();
		std::vector<unsigned long>::const_iterator ThisDatSizeItr = DataSizes.begin(), ThisNameSizeItr = NameSizes.begin();

		std::vector<char*>::const_iterator RightDataItr = RightObject.Data.begin(), RightNameItr = RightObject.Names.begin();
		std::vector<unsigned long>::const_iterator RightDatSizeItr = RightObject.DataSizes.begin(), RightNameSizeItr = RightObject.NameSizes.begin();

		while (RightDataItr != RightObject.Data.end()) {

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

			RightDataItr++;

			ThisDataItr++;
			ThisDatSizeItr++;
			RightDatSizeItr++;

			ThisNameItr++;
			ThisNameSizeItr++;
			RightNameItr++;
			RightNameSizeItr++;
		}
	}
	return false;
}


long FileManager::SizeOfFile(const char* FileName) {
	ifstream File(FileName, ios::in | ios::binary | ios::ate);
	long SizeBuffer = File.tellg();
	File.close();
	return SizeBuffer;
}

char* FileManager::RetrieveFile(const char* FileName) {

	ifstream File(FileName, ios::in | ios::binary | ios::ate);
	long FileSize = File.tellg();
	char* FileData = new char[FileSize]; //The data contained within the file (so the RAM must be large enough to hold it)

	File.seekg(0);
	File.read(FileData, FileSize);
	File.close();

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
	strcpy_s(Buffer, NameSize, Name);

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
	//Open the file (everything is always done in binary for this)
	ofstream File(DestFile, ios::out | ios::binary | ios::trunc);

	//Write the stored data to the file
	File.write(Data[FileIndex], DataSizes[FileIndex]);

	//Close the file
	File.close();
}

void FileManager::RenameFile(const int FileIndex, char* NewName, const unsigned long NewNameSize) {
	delete [] Names[FileIndex];
	Names[FileIndex] = NewName;
	NameSizes[FileIndex] = NewNameSize;
}

void FileManager::ReplaceEmbeddedFile(const int FileIndex, const char* NewFile) {
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

bool FileManager::SaveAll(const char* FileName) {
	//Save the sizes (so it knows the sizes to load later)
	ofstream File(FileName, ios::out | ios::binary | ios::trunc);
	unsigned int DataBuffer = Data.size();

	//Write the total number of items in the file
	File.write(reinterpret_cast<const char*>(&DataBuffer), sizeof(unsigned long));

	int FileLoop;
	//Write the sizes of the files
	for (FileLoop = 0; FileLoop < (int)DataSizes.size(); FileLoop++) {
		DataBuffer = DataSizes[FileLoop];
		File.write(reinterpret_cast<const char*>(&DataBuffer), sizeof(unsigned long));
	}

	//Now write the files
	for (FileLoop = 0; FileLoop < (int)Data.size(); FileLoop++) {
		File.write(Data[FileLoop], DataSizes[FileLoop]);
	}

	//Now write the sizes of the names of the files
	for (FileLoop = 0; FileLoop < (int)NameSizes.size(); FileLoop++) {
		DataBuffer = NameSizes[FileLoop];
		File.write(reinterpret_cast<const char*>(&DataBuffer), sizeof(unsigned long));
	}

	//Now write the names of the files
	for (FileLoop = 0; FileLoop < (int)Names.size(); FileLoop++) {

		File.write(Names[FileLoop], NameSizes[FileLoop]);

	}

	//Close the file
	File.close();

	return true;
}
bool FileManager::LoadAll(const char* FileName) {
	int SizeOfData = 0;
	Clear(); //erase everything
	//read the sizes of the data

	ifstream File(FileName, ios::in | ios::binary);

	unsigned long DataBuffer; //A buffer to store data read from the file until it can be added to storage
	char* StringBuffer; //StringBuffer is not deleted b/c it is a pointer to data that will be deleted with the clear function

	//Read the total number of items in the file
	File.read(reinterpret_cast<char*>(&SizeOfData), sizeof(unsigned long));

	int FileLoop;

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) { 
		File.read(reinterpret_cast<char*>(&DataBuffer), sizeof(unsigned long));
		DataSizes.push_back(DataBuffer);
	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {
		StringBuffer = new char[DataSizes[FileLoop]];
		File.read(StringBuffer, DataSizes[FileLoop]);
		Data.push_back(StringBuffer);
	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {
		File.read(reinterpret_cast<char*>(&DataBuffer), sizeof(unsigned long));
		NameSizes.push_back(DataBuffer);
	}

	for (FileLoop = 0; FileLoop < SizeOfData; FileLoop++) {
		StringBuffer = new char[NameSizes[FileLoop]];
		File.read(StringBuffer, NameSizes[FileLoop]);
		Names.push_back(StringBuffer);
	}

	File.close();

	return true;
}

void FileManager::ReadData(FileStream& File) {

	int IntBuffer, SizeBuffer = NumberOfFiles(), FileLoop = 0;
	PersistentString StringBuffer;

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

void FileManager::WriteData(FileStream& File) const {

	int IntBuffer, SizeBuffer = NumberOfFiles(), FileLoop = 0;
	PersistentString StringBuffer;

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