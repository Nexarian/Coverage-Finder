// MultiArray.h: interface for the MultiArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_)
#define AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileStream.h"
#include "PersistentObject.h"

//*********** Version 2.5 ***********

typedef long DimensionType;

template< class ArrayType >
class MultiArray : public PersistentObject
{
public:

	//NOTE: If you are receiving errors about the contructor below, it might be because DimensionType is not a number (ie. long, int, double, etc) - This class is not designed for DimensionType to be non-numeric
	//MultiArray(const DimensionType XSize = -1, const DimensionType YSize = -1, const DimensionType ZSize = -1, const ArrayDimensions NewDimension = OneD);
	MultiArray();
	MultiArray(const MultiArray &OldArray);
	virtual ~MultiArray();

	//NOTE: Redimension is the equivlent of AddItem/DeleteItem in a Book<typename> object (it creates and deletes items by resizing)
	void Redimension(const unsigned short NewDimension = 1, bool Preserve = true, ...);

	//NOTE: If you are receiving errors about these two functions, it might be because DimensionType is not a number (ie. long, int, double, etc) - This class is not designed for DimensionType to be non-numeric

	DimensionType ElementIndex(DimensionType FirstElement, ...);

	ArrayType& GetElement(DimensionType FirstElement, ...) const;
	ArrayType& operator()(DimensionType FirstElement, ...) const;

	MultiArray<ArrayType>& operator=(const MultiArray<ArrayType> &SrcArray);
	bool operator==(const MultiArray<ArrayType> &SrcArray) const;
	bool operator!=(const MultiArray<ArrayType> &SrcArray) const {return !(*this == SrcArray);}

	DimensionType GetDimensionSize(unsigned short DimNumber);

	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;

private:

	//This macro is critical to this class, do not modify it unless you know what you are doing
	#define ArrayArg(FirstElement, I) \
		va_start(I, FirstElement); \
		if (ParamArrayInput != NULL) { \
			delete [] ParamArrayInput; \
			ParamArrayInput = NULL; \
		} \
		ParamArrayInput = new DimensionType[Dimensions]; \
		if (Dimensions > 0) { \
			ParamArrayInput[0] = FirstElement; \
			for (int DimensionLoop = 1; DimensionLoop < Dimensions; ++DimensionLoop) { \
				ParamArrayInput[DimensionLoop] = va_arg(I, DimensionType); \
			} \
		} \
		va_end(I);

	ArrayType* Data; //The array which stores the data

	//ArrayDimensions CurrentDimension;
	unsigned short Dimensions; //The array dimensions (for example, 1-D, 2-D, 3-D, 4-D, etc)

	DimensionType* ArraySize; //This is the pointer to the array which has the sizes of all the dimensions
	int TotalArraySize; //The length of the actual 1-D array

	void DeleteMemoryData();

	void BasicConstruct();

	mutable DimensionType* ParamArrayInput;
	DimensionType GetElementIndexBasic() const; //Retrieves the element index from the above array of unsigned longs above

	DimensionType* MaxLoops;
	MultiArray<ArrayType>* PreserveArray;
	void FillParamArrayRecursive(unsigned short DimensionNumber); //Utility function to be used for redimensioning only
};

template< class ArrayType  >
MultiArray<ArrayType>::MultiArray()
{
	BasicConstruct();
}
template< class ArrayType >
MultiArray<ArrayType>::MultiArray(const MultiArray &OldArray) 
{
	BasicConstruct();

	*this = OldArray;
}

template< class ArrayType  >
void MultiArray<ArrayType>::BasicConstruct() {
	TotalArraySize = 0;
	Data = 0;
	
	Dimensions = 0;
	ArraySize = 0;

	ParamArrayInput = 0;
}

template< class ArrayType >
MultiArray<ArrayType>::~MultiArray()
{
	DeleteMemoryData();
	if (ParamArrayInput != NULL)  {
		delete [] ParamArrayInput;
		ParamArrayInput = NULL;
	}
}

template< class ArrayType >
void MultiArray<ArrayType>::Redimension(const unsigned short NewDimension, bool Preserve, ...)  //Element dimensions are 1-based
{
	if (TotalArraySize <= 0 || NewDimension != Dimensions) Preserve = false;

	/*
	Copy the old array into the new one.

	Here's how the preservation works
	Since this class works by translating an X-Dimensional array into a 1-Dimensional array, copying the 1-Dimensional array over will not work, since the translations may be different for different size arrays (NOTE that preservation will only work when the dimensions stay the same)
	What we have to do is make an exact copy now, and then copy it back later... Allowing each to use their custom translations while preserving the same amount of data
	*/

	if (Preserve) {
		PreserveArray = new MultiArray<ArrayType>;

		*PreserveArray = *this;
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

		unsigned short CopyLoop = 0;
		DimensionType DimSize1, DimSize2;

		//Determine the sizes to loop to for each dimension
		MaxLoops = new DimensionType[Dimensions];

		for (CopyLoop = 0; CopyLoop < Dimensions; ++CopyLoop) {

			DimSize1 = PreserveArray->GetDimensionSize(CopyLoop + 1);
			DimSize2 = GetDimensionSize(CopyLoop + 1);

			if (DimSize2 < DimSize1) {
				MaxLoops[CopyLoop] = DimSize2;
			}
			else {
				MaxLoops[CopyLoop] = DimSize1;
			}
			
		}

		//Initialize the loop counters to zero
		if (ParamArrayInput!= NULL) {
			delete [] ParamArrayInput;
			ParamArrayInput = NULL;
		}

		ParamArrayInput = new DimensionType[Dimensions];
		PreserveArray->ParamArrayInput = new DimensionType[Dimensions];

		FillParamArrayRecursive(Dimensions - 1);

		delete [] MaxLoops;
		delete [] ParamArrayInput;
		ParamArrayInput = 0;
		delete PreserveArray;
	}
}

template< class ArrayType >
void MultiArray<ArrayType>::FillParamArrayRecursive(unsigned short DimensionNumber) {

	ParamArrayInput[DimensionNumber] = 0;
	PreserveArray->ParamArrayInput[DimensionNumber] = 0;

	long AdvLoop = 0;
	for (AdvLoop; AdvLoop <= MaxLoops[DimensionNumber]; ++AdvLoop) {

		if (DimensionNumber == 0) {
			Data[GetElementIndexBasic()] = PreserveArray->Data[PreserveArray->GetElementIndexBasic()];
		}
		else {
			FillParamArrayRecursive(DimensionNumber - 1);
		}

		ParamArrayInput[DimensionNumber] = AdvLoop;
		PreserveArray->ParamArrayInput[DimensionNumber] = AdvLoop;
	}
}

template< class ArrayType >
DimensionType MultiArray<ArrayType>::GetElementIndexBasic() const {
	
	//This function assumes that ParamArrayInput as already been filled by an instance of the arrayarg macro

	DimensionType nIndex = 0;
	DimensionType MultiplyIndex = 1; //Multiplying by 0 is no good.

	//This is probably the most complex part of this class (and the most fundamental)
	//Here the x-dimensional array is converted to a 1-D array.
	//A quick example: (3, 2) on a 3x3 grid = 3 + (2 (y-coord) * 3 (map width)) = 3 + 6 = 9

	for (int DimensionLoop = 0; DimensionLoop < Dimensions; ++DimensionLoop) {
		//The ? : statement is to prevent multiplication with the first x-coordinate (a special case that only occurs once)
		MultiplyIndex *= (DimensionLoop == 0 ? 1 : ArraySize[DimensionLoop - 1]);
		nIndex += (ParamArrayInput[DimensionLoop] * MultiplyIndex);
	}

	if (nIndex >= TotalArraySize) {
		OutputDebugString("Error - MultiArray Boundary Exceeded!\n");
		return 0;
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
ArrayType& MultiArray<ArrayType>::GetElement(DimensionType FirstElement, ...) const
{
	va_list Coords;

	ArrayArg(FirstElement, Coords)

	return Data[GetElementIndexBasic()];
}

template< class ArrayType >
ArrayType& MultiArray<ArrayType>::operator()(DimensionType FirstElement, ...) const
{
	va_list Coords;

	ArrayArg(FirstElement, Coords)

	return Data[GetElementIndexBasic()];
}

template< class ArrayType >
MultiArray<ArrayType>& MultiArray<ArrayType>::operator=(const MultiArray<ArrayType> &SrcArray) {

	//Nothing that exists in the array is relevent now.
	if (*this != SrcArray) {
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

	return *this;
}
template< class ArrayType >
bool MultiArray<ArrayType>::operator==(const MultiArray<ArrayType> &SrcArray) const {
	return ((SrcArray.Dimensions == Dimensions) &&
		(SrcArray.TotalArraySize == TotalArraySize));
}

template< class ArrayType >
void MultiArray<ArrayType>::ReadData(FileStream& File) {
	DeleteMemoryData(); //Get rid of the old stuff

	//Load the array data

	//Load the statistics about the array (total size and number of dimensions)
	File >> TotalArraySize;
	File >> Dimensions;

	//Load the sizes of each dimension
	ArraySize = new DimensionType[Dimensions];

	unsigned short DataLoop = 0;

	for (DataLoop = 0; DataLoop < Dimensions; DataLoop++) {
		File >> ArraySize[DataLoop];
	}

	//Now load the data
	Data = new ArrayType[TotalArraySize];
	
	for (DataLoop = 0; DataLoop < TotalArraySize; DataLoop++) {
		File >> Data[DataLoop];
	}
}

template< class ArrayType >
void MultiArray<ArrayType>::WriteData(FileStream& File) const {

	File << TotalArraySize;
	File << Dimensions;

	unsigned short DataLoop = 0;
		
	for (DataLoop = 0; DataLoop < Dimensions; DataLoop++) {
		File << ArraySize[DataLoop];
	}
		
	for (DataLoop = 0; DataLoop < TotalArraySize; DataLoop++) {
		File << Data[DataLoop];
	}
}

//The following functions work with the converted 1-D array ONLY (DO NOT attempt to use them with a multi-array) - utility only
template< class ArrayType >
void MultiArray<ArrayType>::DeleteMemoryData() {
	if (Data != NULL) {
		delete [] Data;
		Data = NULL;
	}
	if (ArraySize != NULL) {
		delete [] ArraySize;
		ArraySize = NULL;
	}
}

#endif // !defined(AFX_MULTIARRAY_H__3D8B8E46_93DE_4E14_9BAA_DFA1068EB9F1__INCLUDED_)
