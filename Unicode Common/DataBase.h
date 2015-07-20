// DataBase.h: interface for the DataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__26C55FC6_9C05_4C3A_B6CA_71B5ECCB2752__INCLUDED_)
#define AFX_DATABASE_H__26C55FC6_9C05_4C3A_B6CA_71B5ECCB2752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DataBase  
{
public:
	DataBase();
	virtual ~DataBase();

	virtual void* GetData() = 0;
	virtual void SetData(void* NewData) = 0;

	virtual unsigned int GetDataTypeSize() = 0;
	virtual const char* GetDataTypeName() = 0;

	virtual const char* GetDerivedClassName() = 0;

	virtual DataBase* MakeDataObjectLikeMe() = 0;
};

#endif // !defined(AFX_DATABASE_H__26C55FC6_9C05_4C3A_B6CA_71B5ECCB2752__INCLUDED_)
