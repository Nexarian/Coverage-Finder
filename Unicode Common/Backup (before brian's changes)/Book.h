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
class Book : public PersistantObject
{
public:
	Book();
	Book(Book<DataType>&);
	virtual ~Book();

	virtual void DataTransaction(FileStream&);

	//Two ways to add an item to a book (either a direct pointer to the object (which is begin stored, or have the thing create the pointer, but either way it must be created by new

	//DataType* AddItem();
	//void AddItem(DataType*);

	DataType& AddItem();
	void AddItem(DataType);

	void DeleteItem(int ItemNumber);

	DataType& GetItem(int ItemNumber);
	bool SetItem(int ItemNumber, DataType& NewValue);


	DataType& operator[](int ItemNumber) {return GetItem(ItemNumber);}

	//DataType* GetItemPtr(int ItemNumber) {return ItemNumber < DataBag.size() ? DataBag[ItemNumber] : NULL;}
	

	Book<DataType>& operator=(const Book<DataType> &CreateBook);
	const bool operator==(const Book<DataType> &CompareBook) const;
	const bool operator!=(const Book<DataType> &CompareBook) const {return !(*this == CompareBook);}
	
	const int GetBookSize() const {return DataBag.size();}
	void Empty();

private:
	vector<DataType*> DataBag;

	void BasicConstruct();
};


template<class DataType>
Book<DataType>::Book() : PersistantObject()
{
	BasicConstruct();
}

template<class DataType>
Book<DataType>::Book(Book<DataType> &CreateBook) : PersistantObject()
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
	SetDerivedClassName(typeid(*this).name());
	FileStream::Classes().AddFactory(new Factory< Book<DataType> >);
}

template<class DataType>
DataType& Book<DataType>::AddItem() {
	DataType* Buffer = new DataType;
	DataBag.push_back(Buffer);
	return *Buffer;
}

template<class DataType> 
void Book<DataType>::AddItem(DataType NewItem) {
	DataType* Buffer = new DataType;
	*Buffer = NewItem;
	DataBag.push_back(Buffer);
}

template<class DataType> 
void Book<DataType>::DeleteItem(int ItemNumber) {
	delete DataBag[ItemNumber];
	DataBag.erase(DataBag.begin() + ItemNumber);	
}

template<class DataType>
DataType& Book<DataType>::GetItem(int ItemNumber) {
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
	int LoopMax = DataBag.size() - 1;
	for (int DeleteLoop = LoopMax; DeleteLoop >= 0; DeleteLoop--) {
		DeleteItem(DeleteLoop);
	}
}

template<class DataType>
const bool Book<DataType>::operator==(const Book<DataType>& CompareBook) const {
	if (GetBookSize() == CompareBook.GetBookSize()) { //the sizes must be the same
		for (int CompareLoop = 0; CompareLoop < CompareBook.DataBag.size(); CompareLoop++) {
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

		for (int CopyLoop = 0; CopyLoop < CopyBook.DataBag.size(); CopyLoop++) {
			AddItem() = *CopyBook.DataBag[CopyLoop];
		}
	}
	return *this;
}

template<class DataType>
void Book<DataType>::DataTransaction(FileStream& Stream) {
	if (Stream.IsSaving() == true) {
		Stream << (int)DataBag.size();
		for (int SaveLoop = 0; SaveLoop < DataBag.size(); SaveLoop++) {
			Stream << (*DataBag[SaveLoop]);
		}
	}
	else if (Stream.IsLoading() == true) {
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
	else {
		OutputDebugString("Error in File Stream configuration\n");
	}
}


#endif // !defined(AFX_BOOK_H__996CFF14_E510_4A23_95FD_F2CF8F6B674A__INCLUDED_)
