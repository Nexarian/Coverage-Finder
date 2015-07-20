// Industry.h: interface for the Industry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDUSTRY_H__9CE6A693_65E1_4B4E_9807_F276B0A3C91F__INCLUDED_)
#define AFX_INDUSTRY_H__9CE6A693_65E1_4B4E_9807_F276B0A3C91F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Factory.h"
#include <vector>

using namespace std;


class Industry  
{
public:
	Industry();
	Industry(Industry& CpyInd);
	virtual ~Industry();

	Industry& operator=(Industry &CopyIndustry);
	const bool operator==(Industry &TestIndustry) const;
	const bool operator!=(Industry &TestIndustry) const {return !(*this == TestIndustry);}

	void AddFactory(FactoryBase* Fact); //must be of type new Factory<DataType>
	void* CreateObject(const char* ClassName);
	void DeleteFactory(int FactIndex);

	FactoryBase* GetFactory(int ItemNumber) {return (ItemNumber < Factories.size() ? Factories[ItemNumber] : NULL);}

	int GetTotalFactories() {return Factories.size();}
	

private:
	vector<FactoryBase*> Factories;

	void EmptySelf(); //Erase everything in Factories
};

#endif // !defined(AFX_INDUSTRY_H__9CE6A693_65E1_4B4E_9807_F276B0A3C91F__INCLUDED_)
