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

template<typename Interface>
class Industry  
{
	typedef std::vector<FactoryBase<Interface>*> FCollection;

public:
	Industry();
	Industry(Industry& CpyInd);
	virtual ~Industry();

	Industry& operator=(Industry<Interface> &CopyIndustry);
	const bool operator==(Industry<Interface> &TestIndustry) const;
	const bool operator!=(Industry<Interface> &TestIndustry) const {return !(*this == TestIndustry);}

	void AddFactory(FactoryBase<Interface>* Fact); //must be of type new Factory<DataType>
	Interface* CreateObject(const char* ClassName);
	void DeleteFactory(int FactIndex);

	FactoryBase<Interface>* GetFactory(int ItemNumber) {return (ItemNumber < Factories.size() ? Factories[ItemNumber] : NULL);}

	int GetTotalFactories() {return Factories.size();}
	
private:
	FCollection Factories;

	void EmptySelf(); //Erase everything in Factories
};


template<typename Interface>
Industry<Interface>::Industry()
{

}

template<typename Interface>
Industry<Interface>::Industry(Industry& CpyInd) {
	*this = CpyInd;
}

template<typename Interface>
Industry<Interface>::~Industry()
{
	//Delete any loose factories if need be
	EmptySelf();
}

template<typename Interface>
void Industry<Interface>::EmptySelf() {
	long DeleteLoop = (long)Factories.size() - 1;

	for (DeleteLoop; DeleteLoop >= 0; DeleteLoop--) {
		DeleteFactory(DeleteLoop);
	}
}

template<typename Interface>
Industry<Interface> &Industry<Interface>::operator=(Industry<Interface> &CopyIndustry) {
	OutputDebugString("Warning! The copy constructor was never properly written for the Industry class, undefined results will occur\n");

	if (*this != CopyIndustry) {
		EmptySelf();

		int LoopMax = CopyIndustry.Factories.size();
		int FactoryLoop = 0;

		for (FactoryLoop; FactoryLoop < LoopMax; FactoryLoop++) {
			AddFactory(new Factory<CopyIndustry.Factories[FactoryLoop]->Product>);
		}
	}
	return *this;
}

template<typename Interface>
const bool Industry<Interface>::operator==(Industry &CompareIndustry) const {
	if (Factories.size() == CompareIndustry.Factories.size()) {
		int CompareLoop = 0;
		int LoopMax = Factories.size();

		for (CompareLoop; CompareLoop < Factories.size(); CompareLoop++) {
			if (strcmp(Factories[CompareLoop]->GetClassName(), CompareIndustry.Factories[CompareLoop]->GetClassName()) != 0) {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;
}

template<typename Interface>
void Industry<Interface>::AddFactory(FactoryBase<Interface>* Fact) {
	//Make sure this factory doesn't already exist
	for (FCollection::iterator FactoryLoop = Factories.begin(); FactoryLoop != Factories.end(); ++FactoryLoop) {
		if ((*FactoryLoop)->ClassNamesEqual(*Fact)) {
			delete Fact;
			return;
		}
	}

	Factories.push_back(Fact);
}

template<typename Interface>
void Industry<Interface>::DeleteFactory(int FactIndex) {
	delete Factories[FactIndex];
	Factories.erase(Factories.begin() + FactIndex);
}

template<typename Interface>
Interface* Industry<Interface>::CreateObject(const char* ClassName) {

	for (FCollection::iterator FindLoop = Factories.begin(); FindLoop != Factories.end(); ++FindLoop) {
		if (strcmp((*FindLoop)->GetClassName(), ClassName) == 0) { //the name of the current class and the one specified match
			return (*FindLoop)->ManufactureObj(); //Create the match
		}
	}
	return 0; //No match was found
}

#endif // !defined(AFX_INDUSTRY_H__9CE6A693_65E1_4B4E_9807_F276B0A3C91F__INCLUDED_)