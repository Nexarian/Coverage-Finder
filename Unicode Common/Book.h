// Book.h: interface for the Book class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOK_H__996CFF14_E510_4A23_95FD_F2CF8F6B674A__INCLUDED_)
#define AFX_BOOK_H__996CFF14_E510_4A23_95FD_F2CF8F6B674A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileStream.h"

template<class DataType>
class Book : public PersistentObject
{
public:
	Book();
	Book(Book<DataType>&);
	virtual ~Book();

	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;

	//Two ways to add an item to a book (either a direct pointer to the object (which is begin stored, or have the thing create the pointer, but either way it must be created by new

	DataType& AddItem();
	void AddItem(DataType&);
	void InsertItem(unsigned long Position, DataType& NewItem);

	void DeleteItem(int ItemNumber);

	DataType& GetItem(int ItemNumber) const;
	bool SetItem(int ItemNumber, DataType& NewValue);

	DataType& operator[](int ItemNumber) const {return GetItem(ItemNumber);}
	
	Book<DataType>& operator=(const Book<DataType> &CreateBook);
	const bool operator==(const Book<DataType> &CompareBook) const;
	const bool operator!=(const Book<DataType> &CompareBook) const {return !(*this == CompareBook);}
	
	const long GetBookSize() const {return DataBag.size();}
	void Empty();

private:
	std::vector<DataType*> DataBag;

	void BasicConstruct();
};


template<class DataType>
Book<DataType>::Book() : PersistentObject()
{
	BasicConstruct();
}

template<class DataType>
Book<DataType>::Book(Book<DataType> &CreateBook) : PersistentObject()
{
	BasicConstruct();

	*this = CreateBook;
}

template<class DataType>
Book<DataType>::~Book()
{
	Empty();
}

template<class DataType>
void Book<DataType>::BasicConstruct() {
	
}

template<class DataType>
DataType& Book<DataType>::AddItem() {
	DataType* Buffer = new DataType;
	DataBag.push_back(Buffer);
	return *Buffer;
}

template<class DataType> 
void Book<DataType>::AddItem(DataType& NewItem) {
	DataType* Buffer = new DataType;
	*Buffer = NewItem;
	DataBag.push_back(Buffer);
}

template<class DataType>
void Book<DataType>::InsertItem(unsigned long Position, DataType& NewItem) {
	//allocate new memory for it
	DataType* Buffer = new DataType;
	*Buffer = NewItem;
	DataBag.insert(DataBag.begin() + Position, Buffer);
}

template<class DataType> 
void Book<DataType>::DeleteItem(int ItemNumber) {
	delete DataBag[ItemNumber];
	DataBag.erase(DataBag.begin() + ItemNumber);	
}

template<class DataType>
DataType& Book<DataType>::GetItem(int ItemNumber) const {
	return (*DataBag[ItemNumber]);
}
template<class DataType>
bool Book<DataType>::SetItem(int ItemNumber, DataType& NewValue) {
	if (ItemNumber >= 0 && ItemNumber < DataBag.size()) {
		delete DataBag[ItemNumber];
		*DataBag[ItemNumber] = NewValue;
		return true;
	}
	return false;
}

template<class DataType>
void Book<DataType>::Empty() {
	long LoopMax = DataBag.size() - 1;
	for (long DeleteLoop = LoopMax; DeleteLoop >= 0; DeleteLoop--) {
		DeleteItem(DeleteLoop);
	}
}

template<class DataType>
const bool Book<DataType>::operator==(const Book<DataType>& CompareBook) const {
	if (GetBookSize() == CompareBook.GetBookSize()) { //the sizes must be the same

		int SizeBuffer = (int)GetBookSize();

		for (int CompareLoop = 0; CompareLoop < SizeBuffer; CompareLoop++) {
			if (DataBag[CompareLoop] != CompareBook.DataBag[CompareLoop]) {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true; //Everything is the same
}

template<class DataType>
Book<DataType>& Book<DataType>::operator=(const Book<DataType> &CopyBook) {
	if (*this != CopyBook) {
		Empty();

		long BookSize = CopyBook.DataBag.size();

		for (long CopyLoop = 0; CopyLoop < BookSize; CopyLoop++) {
			AddItem() = *CopyBook.DataBag[CopyLoop];
		}
	}
	return *this;
}


template<class DataType>
void Book<DataType>::ReadData(FileStream& Stream) {

	Empty();

	int SizeBuffer = 0;
	Stream >> SizeBuffer; //Number of times to loop & load

	/*
	FileStream is designed to automatically create an object, but
	this class is designed to create it's own objects, all the FileStream needs to do
	is fill them with data, so therefore we can skip the class creation mechanism
	*/

	for (int LoadLoop = 0; LoadLoop < SizeBuffer; LoadLoop++) {
			
		Stream >> AddItem();

	}

}

template<class DataType>
void Book<DataType>::WriteData(FileStream& Stream) const {

	Stream << (int)DataBag.size();
	for (int SaveLoop = 0; SaveLoop < (int)DataBag.size(); SaveLoop++) {
		Stream << (*DataBag[SaveLoop]);
	}

}

#endif // !defined(AFX_BOOK_H__996CFF14_E510_4A23_95FD_F2CF8F6B674A__INCLUDED_)
