// Factory.h: interface for the Factory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACTORY_H__6C730C00_0813_4664_ADC4_2B12C275B5D1__INCLUDED_)
#define AFX_FACTORY_H__6C730C00_0813_4664_ADC4_2B12C275B5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FactoryBase.h"
#include <vector>

using namespace std;

template<class Product>
class Factory : public FactoryBase
{
public:
	Factory();
	virtual ~Factory();

	void* ManufactureObj();

	const char* GetClassName();
	bool ClassNamesEqual(FactoryBase& Fact) {return (strcmp(GetClassName(), Fact.GetClassName()) == 0);}
};

template< class Product >
Factory<Product>::Factory() {
	
}

template< class Product >
Factory<Product>::~Factory() {

}

template< class Product >
void* Factory<Product>::ManufactureObj() { //Make a new object
	Product* Buffer;
	Buffer = new Product;
	return dynamic_cast<void*>(Buffer); //return a pointer to the newly created object
}
//Return the name of the class
template< class Product >
const char* Factory<Product>::GetClassName() {
	return typeid(Product).name();
}


#endif // !defined(AFX_FACTORY_H__6C730C00_0813_4664_ADC4_2B12C275B5D1__INCLUDED_)
