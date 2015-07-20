// Industry.cpp: implementation of the Industry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Industry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Industry::Industry()
{

}
Industry::Industry(Industry& CpyInd) {
	*this = CpyInd;
}

Industry::~Industry()
{
	//Delete any loose factories if need be
	EmptySelf();
}

void Industry::EmptySelf() {
	int DeleteLoop = Factories.size() - 1;

	for (DeleteLoop; DeleteLoop >= 0; DeleteLoop--) {
		DeleteFactory(DeleteLoop);
	}
}

Industry &Industry::operator=(Industry &CopyIndustry) {
	OutputDebugString("Waring! The copy constructor was never properly written for the Industry class, undefined results will occur\n");

	if (*this != CopyIndustry) {
		EmptySelf();

		int LoopMax = CopyIndustry.Factories.size();
		int FactoryLoop = 0;

		for (FactoryLoop; FactoryLoop < LoopMax; FactoryLoop++) {
			//Errors are generated if I enabled this line, currently a copy constructor for an industry object is not necessary
			//AddFactory(new Factory<CopyIndustry.Factories[FactoryLoop]->Product>);
		}
	}
	return *this;
}
const bool Industry::operator==(Industry &CompareIndustry) const {
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

void Industry::AddFactory(FactoryBase* Fact) {
	//Make sure this factory doesn't already exist
	int LoopMax = Factories.size();
	int SearchLoop = 0;

	for (SearchLoop = 0; SearchLoop < LoopMax; SearchLoop++) {
		if (strcmp(Factories[SearchLoop]->GetClassName(), Fact->GetClassName()) == 0) {
			delete Fact; //It is required to be created by new
			return;
		}
	}
	Factories.push_back(Fact);
}
void Industry::DeleteFactory(int FactIndex) {
	delete Factories[FactIndex];
	Factories.erase(Factories.begin() + FactIndex);
}
void* Industry::CreateObject(const char* ClassName) {
	for (int FindLoop = 0; FindLoop < Factories.size(); FindLoop++) {
		if (strcmp(Factories[FindLoop]->GetClassName(), ClassName) == 0) { //the name of the current class and the one specified match
			return Factories[FindLoop]->ManufactureObj(); //Create the match
		}
	}
	return NULL; //No match was found
}