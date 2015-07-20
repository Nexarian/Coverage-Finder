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
#include <typeinfo>

template<class Interface, class Product>
class Factory : public FactoryBase<Interface>
{
public:
	Factory();
	virtual ~Factory();

	Interface* ManufactureObj();

	const char* GetClassName();
};

template<class Interface, class Product>
Factory<Interface, Product>::Factory() {
	
}

template<class Interface, class Product>
Factory<Interface, Product>::~Factory() {

}

template<class Interface, class Product>
Interface* Factory<Interface, Product>::ManufactureObj() { //Make a new object
	return new Product;
}
//Return the name of the class
template<class Interface, class Product>
const char* Factory<Interface, Product>::GetClassName() {
	return typeid(Product).name();
}

#endif // !defined(AFX_FACTORY_H__6C730C00_0813_4664_ADC4_2B12C275B5D1__INCLUDED_)
