#include "StdAfx.h"
#include "MappedPoint.h"
#include "Area.h"

MappedPoint::MappedPoint(void) {
	BasicConstruct();
}
MappedPoint::MappedPoint(const MappedPoint& CopyPoint) {
	BasicConstruct();
	*this = CopyPoint;
}
MappedPoint::~MappedPoint(void) {
	ClearPointStorage(true, true, true, true, true);
}

void MappedPoint::ResetData(void) {
	BasicReset();
	ClearPointStorage(true, true, true, true, true);
}

const MappedPoint& MappedPoint::operator=(const MappedPoint& CopyPoint) {
	if (*this != CopyPoint) {
		PointPart = CopyPoint.PointPart;

		Invalid = CopyPoint.Invalid;

		OnVertical = CopyPoint.OnVertical;
		OnHorizontal = CopyPoint.OnHorizontal;

		VertLine = CopyPoint.VertLine;
		HorzLine = CopyPoint.HorzLine;

		IsVertex = CopyPoint.IsVertex;
		IsBottomVertex = CopyPoint.IsBottomVertex;
		IsRightVertex = CopyPoint.IsRightVertex;
		IsConcaveVertex = CopyPoint.IsConcaveVertex;
		ArrayPosition = CopyPoint.ArrayPosition;
		Range = CopyPoint.Range;
		Hit = CopyPoint.Hit;

		ClearPointStorage(!CopyPoint.PN, !CopyPoint.VF, !CopyPoint.ES, !CopyPoint.IS, !CopyPoint.PE);

		if (CopyPoint.PN) {
			PossibleNodes().clear();
			*PN = *(CopyPoint.PN);
		}
		if (CopyPoint.ES) {
			EmptySiblings().clear();
			*ES = *(CopyPoint.ES);
		}
		if (CopyPoint.VF) {
			VertexFamily().clear();
			*VF = *(CopyPoint.VF);
		}
		if (CopyPoint.IS) {
			InvalidSiblings().clear();
			*IS = *(CopyPoint.IS);
		}
		if (CopyPoint.PE) {
			PathToEnd().clear();
			*PE = *(CopyPoint.PE);
		}
				
	}

	return *this;
}
const bool MappedPoint::operator==(const MappedPoint& ComparePoint) const {

	bool PnEq = false, IsEq = false, VfEq = false, EsEq = false, PeEq = false, *BoolBuf;

	//this first one is done seperately because it uses a different data type
	for (int RepeatLoop = 0; RepeatLoop <= 2; ++RepeatLoop) {

		BoolBuf = 0;
		std::list<MappedPoint*>** PtrBuf = 0;
		std::list<MappedPoint*>** PtrBufComp = 0;

		switch (RepeatLoop) {
			case 0:
				BoolBuf = &PnEq;
				PtrBuf = &PN;
				PtrBufComp = &(ComparePoint.PN);
				break;
			case 1:
				BoolBuf = &PeEq;
				PtrBuf = &PE;
				PtrBufComp = &(ComparePoint.PE);
				break;
			case 2:
				BoolBuf = &VfEq;
				PtrBuf = &VF;
				PtrBufComp = &(ComparePoint.VF);
				break;
			default:
				//should never occur, memory error
				break;
		}

		if (*PtrBuf == 0 && *PtrBufComp == 0) {
			*BoolBuf = true;
		}
		else {
			if (*PtrBuf == 0 || *PtrBufComp == 0) {
				*BoolBuf = false;
			}
			else {
				*BoolBuf = (**PtrBuf == **PtrBufComp);
			}
		}
	}

	for (int RepeatLoop = 0; RepeatLoop <= 1; ++RepeatLoop) {

		BoolBuf = 0;
		PointAr** PtrBuf = 0;
		PointAr** PtrBufComp = 0;

		switch (RepeatLoop) {
			case 0:
				BoolBuf = &IsEq;
				PtrBuf = &IS;
				PtrBufComp = &(ComparePoint.IS);
				break;
			case 1:
				BoolBuf = &EsEq;
				PtrBuf = &ES;
				PtrBufComp = &(ComparePoint.ES);
				break;
			default:
				//should never occur - memory error
				break;
		}

		if (*PtrBuf == 0 && *PtrBufComp == 0) {
			*BoolBuf = true;
		}
		else {
			if (*PtrBuf == 0 || *PtrBufComp == 0) {
				*BoolBuf = false;
			}
			else {
				*BoolBuf = (**PtrBuf == **PtrBufComp);
			}
		}
	}

	return (PointPart == ComparePoint.PointPart) &&
		(Invalid == ComparePoint.Invalid) &&
		(OnVertical == ComparePoint.OnVertical) &&
		(OnHorizontal == ComparePoint.OnHorizontal) &&
		(VertLine == ComparePoint.VertLine) &&
		(HorzLine == ComparePoint.HorzLine) &&
		(IsVertex == ComparePoint.IsVertex) &&
		(IsBottomVertex == ComparePoint.IsBottomVertex) &&
		(IsRightVertex == ComparePoint.IsRightVertex) &&
		(IsConcaveVertex == ComparePoint.IsConcaveVertex) &&
		(PnEq) &&
		(IsEq) &&
		(VfEq) &&
		(EsEq) &&
		(PeEq);
}
const bool MappedPoint::operator!=(const MappedPoint& ComparePoint) const {
	return !(*this == ComparePoint);
}

//WARNING! These comparison operators are specialized to the range number, do not use for general purpose
const bool MappedPoint::operator>(const MappedPoint& ComparePoint) const {
	return (Range > ComparePoint.Range);
}
const bool MappedPoint::operator>=(const MappedPoint& ComparePoint) const {
	return (*this > ComparePoint || Range == ComparePoint.Range);
}
const bool MappedPoint::operator<(const MappedPoint& ComparePoint) const {
	return (Range < ComparePoint.Range);
}
const bool MappedPoint::operator<=(const MappedPoint& ComparePoint) const {
	return (*this > ComparePoint || Range == ComparePoint.Range);
}

//accessor functions
std::list<MappedPoint*>& MappedPoint::PossibleNodes() {
	if (!PN) PN = new std::list<MappedPoint*>;
	return *PN;
}
std::list<MappedPoint*>& MappedPoint::PathToEnd() {
	if (!PE) PE = new std::list<MappedPoint*>;
	return *PE;
}
std::list<MappedPoint*>& MappedPoint::VertexFamily() {
	if (!VF) VF = new std::list<MappedPoint*>;
	return *VF;
}
PointAr& MappedPoint::EmptySiblings() {
	if (!ES) ES = new PointAr;
	return *ES;
}
PointAr& MappedPoint::InvalidSiblings() {
	if (!IS) IS = new PointAr;
	return *IS;
}
//maintainence function
void MappedPoint::ClearPointStorage(bool PossibleN, bool EmptyS, bool VertexF, bool InvalidS, bool PathE) const {
	if (PossibleN) ClearPtr(PN);
	if (EmptyS) ClearPtr(ES);
	if (VertexF) ClearPtr(VF);
	if (InvalidS) ClearPtr(IS);
	if (PathE) ClearPtr(PE);
}
void MappedPoint::BasicConstruct(void) {
	BasicReset();
	PN = 0;
	ES = 0;
	VF = 0;
	IS = 0;
	PE = 0;
}
void MappedPoint::BasicReset(void) {
	Invalid = 
	OnVertical =
	OnHorizontal =
	IsVertex = 
	IsBottomVertex =
	IsRightVertex =
	IsConcaveVertex = false;
}
void MappedPoint::ReadData(FileStream& File) {
	File >> Invalid;
	File >> OnVertical;
	File >> OnHorizontal;
	File >> IsBottomVertex;
	File >> IsRightVertex;
	File >> IsConcaveVertex;
	File >> VertLine ;
	File >> HorzLine;
	File >> PointPart;
	File >> Range;

	ClearPointStorage(true, true, true, true, true);

	LoadLinkedPoints(File, PN);
	LoadLinkedPoints(File, VF);
	LoadNormalPoints(File, ES);
	LoadNormalPoints(File, IS);
}
void MappedPoint::WriteData(FileStream& File) const {
	File << Invalid << OnVertical << OnHorizontal << IsBottomVertex << IsRightVertex << IsConcaveVertex;
	File << VertLine << HorzLine;
	File << PointPart;
	File << Range;

	SaveLinkedPoints(File, PN);
	SaveLinkedPoints(File, VF);
	SaveNormalPoints(File, ES);
	SaveNormalPoints(File, IS);
}

void MappedPoint::SaveLinkedPoints(FileStream& File, std::list<MappedPoint*>*& DataPtr) const {
	if (DataPtr == 0) {
		File << 0; //record "false" because the data does not exist
	}
	else {
		File << 1;
		File << DataPtr->size();
		for (MappedPointPtrArCItr NodeItr = DataPtr->begin(); NodeItr != DataPtr->end(); ++NodeItr) {
			File << (*NodeItr)->ArrayPosition;
		}
	}
}

void MappedPoint::LoadLinkedPoints(FileStream& File, std::list<MappedPoint*>*& DataPtr) {

	ClearPtr(DataPtr);

	int ReadBuffer = 0, ReadLoop = 0;

	//now read back the array properties
	File >> ReadBuffer;
	if (ReadBuffer != 0) { //did the array exist?

		int NumBuffer;
		MappedPoint* DummyBuffer;

		File >> ReadBuffer; //re-read the number of objects in the array
		//reading back possible nodes
		PN = new MappedPointPtrAr; 
		for (ReadLoop = 0; ReadLoop < ReadBuffer; ++ReadLoop) {
			File >> NumBuffer;
			DummyBuffer = new MappedPoint;
			DummyBuffer->ArrayPosition = NumBuffer;
			DataPtr->push_back(DummyBuffer);
		}
	}
}

void MappedPoint::SaveNormalPoints(FileStream& File, PointAr*& DataPtr) const {
	if (DataPtr == 0) {
		File << 0;
	}
	else {
		File << 1;
		File << DataPtr->size();
		for (PointArCItr NodeItr = DataPtr->begin(); NodeItr != DataPtr->end(); ++NodeItr) {
			File << *NodeItr;
		}
	}
}

void MappedPoint::LoadNormalPoints(FileStream& File, PointAr*& DataPtr) {
	int ReadBuffer, ReadLoop;

	ClearPtr(DataPtr);

	File >> ReadBuffer;
	if (ReadBuffer != 0) {

		POINT Buffer;

		File >> ReadBuffer; 
	
		DataPtr = new PointAr;
		for (ReadLoop = 0; ReadLoop < ReadBuffer; ++ReadLoop) {
			File >> Buffer;
			DataPtr->push_back(Buffer);
		}
	}
}