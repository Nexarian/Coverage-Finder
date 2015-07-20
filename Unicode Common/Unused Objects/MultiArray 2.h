// MultiArray.h: interface for the MultiArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_)
#define AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileStream.h"
#include "PersistantObject.h"

//This version currently supports a 3-D matrix at most

/*enum ArrayDimensions { //Enum containing the dimensions currently supported by the multiarray class
	//Numbered for easy conversion
	OneD = 1,
	TwoD = 2,
	ThreeD = 3
};*/

typedef unsigned long DimensionType;

//This macro is critical to this class, do not modify it unless you know what you are doing
#define ArrayArg(FirstElement, Indexes) \
	va_start(Indexes, FirstElement); \
	if (ParamArrayInput != NULL) delete [] ParamArrayInput; \
	ParamArrayInput = new DimensionType[Dimensions]; \
	ParamArrayInput[0] = FirstElement; \
	for (int DimensionLoop = 1; DimensionLoop < Dimensions; DimensionLoop++) { \
		ParamArrayInput[DimensionLoop] = va_arg(Indexes, DimensionType); \
	} \
	va_end(Indexes);


template< class ArrayType >
class MultiArray : public PersistantObject
{
public:

	//NOTE: If you are receiving errors about the contructor below, it might be because DimensionType is not a number (ie. long, int, double, etc) - This class is not designed for DimensionType to be non-numeric
	//MultiArray(const DimensionType XSize = -1, const DimensionType YSize = -1, const DimensionType ZSize = -1, const ArrayDimensions NewDimension = OneD);
	MultiArray();
	MultiArray(const MultiArray &OldArray);
	virtual ~MultiArray();

	//NOTE: Redimension is the equivlent of AddItem/DeleteItem in a Book<typename> object (it creates and deletes items by resizing)
	//void Redimension(const ArrayDimensions NewDimension = OneD, const bool Preserve = true, const DimensionType XSize = -1, const DimensionType YSize = -1, const DimensionType ZSize = -1);
	void Redimension(const unsigned short NewDimension = 1, bool Preserve = true, ...);

	//NOTE: If you are receiving errors about these two functions, it might be because DimensionType is not a number (ie. long, int, double, etc) - This class is not designed for DimensionType to be non-numeric

	DimensionType ElementIndex(DimensionType FirstElement, ...);

	ArrayType& GetElement(DimensionType FirstElement, ...);
	ArrayType& operator()(DimensionType FirstElement, ...);

	void operator=(const MultiArray<ArrayType> &SrcArray);
	bool operator==(const MultiArray<ArrayType> &SrcArray) const;
	bool operator!=(const MultiArray<ArrayType> &SrcArray) const {return !(*this == SrcArray);}

	DimensionType GetDimensionSize(unsigned short DimNumber);

	void DataTransaction(FileStream &File);

private:
	ArrayType* Data; //The array which stores the data

	//ArrayDimensions CurrentDimension;
	unsigned short Dimensions; //The array dimensions (for example, 1-D, 2-D, 3-D, 4-D, etc)

	DimensionType* ArraySize; //This is the pointer to the array which has the sizes of all the dimensions
	int TotalArraySize; //The length of the actual 1-D array

	void DeleteMemoryData();

	void BasicConstruct();

	DimensionType* ParamArrayInput;
	DimensionType GetElementIndexBasic(); //Retrieves the element index from the above array of unsigned longs above
};

template< class ArrayType  >
MultiArray<ArrayType>::MultiArray()
{
	BasicConstruct();

	Data = NULL;
	ArraySize = NULL;

	Redimension(0, false, 0);
}
template< class ArrayType >
MultiArray<ArrayType>::MultiArray(const MultiArray &OldArray) 
{
	BasicConstruct();

	*this = OldArray;
}

template< class ArrayType  >
void MultiArray<ArrayType>::BasicConstruct() {
	SetDerivedClassName(typeid(*this).name());
	FileStream::Classes().AddFactory(new Factory< MultiArray<ArrayType> >);
	ParamArrayInput = NULL;
}

template< class ArrayType >
MultiArray<ArrayType>::~MultiArray()
{
	if (Data != NULL) delete [] Data;
	if (ArraySize != NULL) delete [] ArraySize;
}
template< class ArrayType >
void MultiArray<ArrayType>::Redimension(const unsigned short NewDimension, bool Preserve, ...) 
{
	ArrayType* PreserveArray; //Only used if Preserve == true
	int OldArraySize = 0;

	if (TotalArraySize <= 0) Preserve = false;

	if (Preserve == true) {
		//Copy the old array into the new one
		OldArraySize = TotalArraySize;
		PreserveArray = new ArrayType[TotalArraySize];

		//CopyMemory(PreserveArray, Data, TotalArraySize * sizeof(ArrayType*));
		for (int ArrayCopyLoop = 0; ArrayCopyLoop < TotalArraySize; ArrayCopyLoop++) {
			PreserveArray[ArrayCopyLoop] = Data[ArrayCopyLoop];
		}
	}

	Dimensions = NewDimension;

	///////////////Delete and redefine all the important variables///////////////

	DeleteMemoryData();
	ArraySize = new DimensionType[Dimensions];

	//Figure out the size of the array
	TotalArraySize = 1; //mulitplying by 0 is no good

	va_list DimSizes;
	va_start(DimSizes, Preserve);

	DimensionType NumberBuffer;

	for (int DimLoop = 0; DimLoop < Dimensions; DimLoop++) {

		NumberBuffer = va_arg(DimSizes, DimensionType);

		TotalArraySize *= NumberBuffer;
		ArraySize[DimLoop] = NumberBuffer;
	}

	va_end(DimSizes);

	if (TotalArraySize < 0) {
		return; //Error protection (just in case something goes horribly wrong)
	}
	else {
		//Create the new array
		Data = new ArrayType[TotalArraySize];
	}

	//Now copy the old data back into the array (if it needs to be preserved)
	if (Preserve == true) {

		long CopyLoop = 0;

		for (CopyLoop = 0; CopyLoop < (OldArraySize > TotalArraySize ? TotalArraySize : OldArraySize); CopyLoop++) {
			Data[CopyLoop] = PreserveArray[CopyLoop];
		}
		delete [] PreserveArray;
	}
}

template< class ArrayType >
DimensionType MultiArray<ArrayType>::GetElementIndexBasic() {
	
	//This function assumes that ParamArrayInput as already been filled by an instance of the arrayarg macro

	DimensionType nIndex = 0;
	DimensionType MultiplyIndex = 0;

	for (int DimensionLoop = 0; DimensionLoop < Dimensions; DimensionLoop++) {
		MultiplyIndex *= ArraySize[DimensionLoop];
		nIndex += (ParamArrayInput[DimensionLoop] * MultiplyIndex);
	}

	return nIndex;
}

template< class ArrayType >
DimensionType MultiArray<ArrayType>::ElementIndex(DimensionType FirstElement, ...) {
	//Retrieve the parameter array
	va_list Indexes;

	ArrayArg(FirstElement, Indexes)

	return GetElementIndexBasic(); //Retrieve the element index using the utility function
}

template< class ArrayType >
DimensionType MultiArray<ArrayType>::GetDimensionSize(unsigned short DimNumber) {
	if (ArraySize == NULL) return 0;
	return ArraySize[((DimensionType)DimNumber) - 1];
}

template< class ArrayType >
ArrayType& MultiArray<ArrayType>::GetElement(DimensionType FirstElement, ...) 
{
	va_list Coords;
	ArrayArg(FirstElement, Coords)

	return Data[GetElementIndexBasic()];
}

template< class ArrayType >
ArrayType& MultiArray<ArrayType>::operator()(DimensionType FirstElement, ...) 
{
	va_list Coords;

	ArrayArg(FirstElement, Coords)

	return Data[GetElementIndexBasic()];
}

template< class ArrayType >
void MultiArray<ArrayType>::operator=(const MultiArray<ArrayType> &SrcArray) {

	DeleteMemoryData();
	
	Dimensions = SrcArray.Dimensions;
	TotalArraySize = SrcArray.TotalArraySize;

	ArraySize = new DimensionType[Dimensions];
	Data = new ArrayType[TotalArraySize];

	int DataLoop = 0;

	for (DataLoop = 0; DataLoop < Dimensions; ++DataLoop) {
		ArraySize[DataLoop] = SrcArray.ArraySize[DataLoop];
	}

	for (DataLoop = 0; DataLoop < TotalArraySize; DataLoop++) {
		Data[DataLoop] = SrcArray.Data[DataLoop];
	}
}
template< class ArrayType >
bool MultiArray<ArrayType>::operator==(const MultiArray<ArrayType> &SrcArray) const {
	return ((SrcArray.Dimensions == Dimensions) &&
		(SrcArray.TotalArraySize == TotalArraySize));
}

template< class ArrayType >
void MultiArray<ArrayType>::DataTransaction(FileStream& File) {
	/*int DataLoop = 0;

	if (File.IsSaving()) { //It's saving
		File << TotalArraySize;
		
		for (DataLoop = 0; DataLoop < TotalArraySize; DataLoop++) {
			File << *(Data[DataLoop]);
		}

		File << (int)CurrentDimension;
		
		for (DataLoop = 0; DataLoop < CurrentDimension; DataLoop++) {
			File << ArraySize[DataLoop];
		}
	}
	else { //It's loading
		DeleteMemoryData(Data, TotalArraySize - 1);
		if (ArraySize != NULL) delete [] ArraySize;

		//Load the array data

		File >> TotalArraySize;
		Data = new ArrayType*[TotalArraySize];
		
		for (DataLoop = 0; DataLoop < TotalArraySize; DataLoop++) {
			File >> *(Data[DataLoop]);
		}

		//Load the sizes of each dimension (no matter how many dimensions, it will always be converted to a 1-D array
		int Buffer = 0;
		File >> Buffer;
		CurrentDimension = (ArrayDimensions)Buffer;

		ArraySize = new DimensionType[CurrentDimension];

		for (DataLoop = 0; DataLoop < CurrentDimension; DataLoop++) {
			File >> ArraySize[DataLoop];
		}
	}*/
}

//The following functions work with the converted 1-D array ONLY (DO NOT attempt to use them with a multi-array) - utility only
template< class ArrayType >
void MultiArray<ArrayType>::DeleteMemoryData() {
	delete [] Data;
	delete [] ArraySize;
}

/*template< class ArrayType, class DimensionType >
void MultiArray<ArrayType>::Serialize(CArchive &ar) {


	CObject::Serialize(ar); //call the base class

	if (ar.IsStoring() == TRUE) {
		//Write the main data;
		ar << TotalArraySize;
		ar.Write(Data, TotalArraySize * sizeof(ArrayType));
		//Write the sizes
		ar << (int)CurrentDimension;
		ar.Write(ArraySize, (int)CurrentDimension * sizeof(DimensionType));
	}
	else {
		if (Data != NULL) delete [] Data;
		if (ArraySize != NULL) delete [] ArraySize;

		ar >> TotalArraySize;
		Data = new ArrayType[TotalArraySize];
		ar.Read(Data, TotalArraySize * sizeof(ArrayType));
		
		ar >> CurrentDimension;
		ar.Read(ArraySize, (int)CurrentDimension * sizeof(DimensionType));
	}
}*/

#endif // !defined(AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_)
