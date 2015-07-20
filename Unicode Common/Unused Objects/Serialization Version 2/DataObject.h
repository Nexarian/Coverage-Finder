// DataObject.h: interface for the DataObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_)
#define AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Main.h"
#include "PersistantObject.h"
#include "DataBase.h"
#include "FileStream.h"

template<class DataObjectType>
class DataObject : public PersistantObject, public DataBase
{
public:
	DataObject();
	DataObject(DataObject<DataObjectType>& CopyData);
	virtual ~DataObject();

	void* GetData() {return &MainData;}
	void SetData(void* NewData) {MainData = *reinterpret_cast<DataObjectType*>(NewData);}

	unsigned int GetDataTypeSize() {return sizeof(DataObjectType);}
	const char* GetDataTypeName() {return typeid(DataObjectType).name();}
	const char* GetDerivedClassName() {return DerivedClassName;}


	DataObject<DataObjectType>& operator=(DataObject<DataObjectType>& RightObject);
	const bool operator==(DataObject<DataObjectType>& RightObject) const {return MainData == RightObject.MainData}
	const bool operator!=(DataObject<DataObjectType>& RightObject) const {return !(*this == RightObject);}

	DataBase* MakeDataObjectLikeMe();

	void DataTransaction(FileStream& Data) {};

private:
	
	BasicConstruct();

	DataObjectType MainData;
};

template<class DataObjectType>
DataObject<DataObjectType>::DataObject() {
	BasicConstruct();
}

template<class DataObjectType>
DataObject<DataObjectType>::DataObject(DataObject<DataObjectType>& CopyData) {
	BasicConstruct();
	*this = CopyData;
}


template<class DataObjectType>
DataObject<DataObjectType>::BasicConstruct() {
	SetDerivedClassName(typeid(*this).name());
	FileStream::Classes().AddFactory(new Factory<DataObject<DataObjectType> >);	
}

template<class DataObjectType>
DataObject<DataObjectType>::~DataObject()
{

}

template<class DataObjectType>
DataBase* DataObject<DataObjectType>::MakeDataObjectLikeMe() {
	DataObject<DataObjectType>* Buffer = new DataObject<DataObjectType>;
	Buffer->operator=(*this);
	return dynamic_cast<DataBase*>(Buffer);
}

template<class DataObjectType>
DataObject<DataObjectType>& DataObject<DataObjectType>::operator=(DataObject<DataObjectType>& RightObject) {
	MainData = RightObject.MainData;
	return *this;
}

#endif // !defined(AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_)
