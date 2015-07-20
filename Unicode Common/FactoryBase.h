// FactoryBase.h: interface for the FactoryBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACTORYBASE_H__9085C04F_B3D9_4EEF_830F_CCFDAE879865__INCLUDED_)
#define AFX_FACTORYBASE_H__9085C04F_B3D9_4EEF_830F_CCFDAE879865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<typename I>
class FactoryBase  
{
public:
	FactoryBase() {}
	virtual ~FactoryBase() {}

	bool ClassNamesEqual(FactoryBase<I>& Fact) {return (strcmp(GetClassName(), Fact.GetClassName()) == 0);};
	virtual I* ManufactureObj() = 0;
	virtual const char* GetClassName() = 0;
};

#endif // !defined(AFX_FACTORYBASE_H__9085C04F_B3D9_4EEF_830F_CCFDAE879865__INCLUDED_)
