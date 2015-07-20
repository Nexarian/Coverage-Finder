#include "StdAfx.h"
#include "MappedRect.h"

const bool MappedRect::operator==(const MappedRect& CompareRect) const {
	return (BasicRect == CompareRect.BasicRect) &&
		(Scanned == CompareRect.Scanned) &&
		(ScanUsed == CompareRect.ScanUsed) &&
		(ScanY == CompareRect.ScanY) &&
		(ScanX == CompareRect.ScanX) &&
		(Rg == CompareRect.Rg);
}
const bool MappedRect::operator!=(const MappedRect& CompareRect) const {
	return !(*this == CompareRect);
}
const MappedRect& MappedRect::operator=(const MappedRect& CompareRect) {
	if (*this != CompareRect) {
		BasicRect = CompareRect.BasicRect;
		Scanned = CompareRect.Scanned;
		ScanUsed = CompareRect.ScanUsed;
		ScanY = CompareRect.ScanY;
		ScanX = CompareRect.ScanX;
		Rg = CompareRect.Rg;
	}
	return *this;
}
const MappedRect& MappedRect::operator=(const RECT& CompareRect) {
	if (BasicRect != CompareRect) {
		BasicRect = CompareRect;
	}
	return *this;
}
const bool MappedRect::operator<(const MappedRect& CompareRect) const {
	return Rg < CompareRect.Rg;
}
const bool MappedRect::operator<=(const MappedRect& CompareRect) const {
	return (*this == CompareRect) || (*this < CompareRect);
}
const bool MappedRect::operator>(const MappedRect& CompareRect) const {
	return Rg > CompareRect.Rg;
}
const bool MappedRect::operator>=(const MappedRect& CompareRect) const {
	return (*this == CompareRect) || (*this > CompareRect);
}
//initialization
MappedRect::MappedRect() {
	BasicRect.top = BasicRect.right = BasicRect.left = BasicRect.bottom = 0;
	ScanX = Left;
	ScanY = Top;
	ScanUsed = Vertical;
	Rg = 0;
	Scanned = false;
}
MappedRect::MappedRect(const MappedRect& CopyRect) {
	ScanX = Left;
	ScanY = Top;
	ScanUsed = Vertical;
	Rg = 0;
	*this = CopyRect;
}
MappedRect::MappedRect(RECT CopyRect) {
	ScanX = Left;
	ScanY = Top;
	ScanUsed = Vertical;
	Rg = 0;
	*this = CopyRect;
}
MappedRect::MappedRect(const POINT TopLeft, const POINT BottomRight) {
	BasicRect.top = TopLeft.y;
	BasicRect.left = TopLeft.x;
	BasicRect.bottom = BottomRight.y;
	BasicRect.right = BottomRight.x;
	ScanX = Left;
	ScanY = Top;
	ScanUsed = Vertical;
	Rg = 0;
	Scanned = false;
}
MappedRect::MappedRect(const LONG Tx, const LONG Ty, const LONG Bx, const LONG By) {
	BasicRect.top = Ty;
	BasicRect.left = Tx;
	BasicRect.bottom = By;
	BasicRect.right = Bx;

	ScanX = Left;
	ScanY = Top;
	ScanUsed = Vertical;
	Rg = 0;
	Scanned = false;
}
MappedRect::~MappedRect() {

}
const POINT MappedRect::TopLeft() const {
	POINT ReturnPoint;
	ReturnPoint.x = BasicRect.left;
	ReturnPoint.y = BasicRect.top;
	return ReturnPoint;
}
const POINT MappedRect::TopRight() const {
	POINT ReturnPoint;
	ReturnPoint.x = BasicRect.right;
	ReturnPoint.y = BasicRect.top;
	return ReturnPoint;
}
const POINT MappedRect::BottomLeft() const {
	POINT ReturnPoint;
	ReturnPoint.x = BasicRect.left;
	ReturnPoint.y = BasicRect.bottom;
	return ReturnPoint;
}
const POINT MappedRect::BottomRight() const {
	POINT ReturnPoint;
	ReturnPoint.x = BasicRect.right;
	ReturnPoint.y = BasicRect.bottom;
	return ReturnPoint;
}
const POINT MappedRect::GetCoordinateFromLocation(HorizontalStart HorzStart, VerticalStart VertStart) const {
	if (HorzStart == Left && VertStart == Top) {
		return TopLeft();
	}
	else if (HorzStart == Right && VertStart == Top) {
		return TopRight();
	}
	else if (HorzStart == Left && VertStart == Bottom) {
		return BottomLeft();
	}
	else if (HorzStart == Right && VertStart == Bottom) {
		return BottomRight();
	}
	POINT DummyPoint;
	DummyPoint.x = DummyPoint.y = 0;
	return DummyPoint;
}
const MappedLine MappedRect::GetDiagonal() const {
	return MappedLine(BasicRect.left, BasicRect.top, BasicRect.right, BasicRect.bottom);
}
const bool MappedRect::IsSquare() const {
	return GetDiagonal().Slope() == (double)1.0;
}
const POINT MappedRect::RectCenter() const {
	return GetDiagonal().Midpoint();
}
//rectangle analysis function
const POINT MappedRect::FindClosestVertex(POINT FindPoint) const {
	POINT ReturnPoint = FindPoint, ComparePoint = FindPoint;
	double ReturnDist = 0, CompareBuffer = 0;

	for (int FindLoop = 0; FindLoop <= 3; ++FindLoop) {
		switch (FindLoop) {
			case 0:
				ComparePoint = TopLeft();
				break;
			case 1:
				ComparePoint = TopRight();
				break;
			case 2:
				ComparePoint = BottomLeft();
				break;
			case 3:
				ComparePoint = BottomRight();
				break;
		}
		CompareBuffer = GeometricObject::Range(ComparePoint, FindPoint);
		if (FindLoop == 0 || CompareBuffer < ReturnDist) { //for calibration
			ReturnPoint = ComparePoint;
			ReturnDist = CompareBuffer;
		}
	}

	return ReturnPoint;
}

void MappedRect::ReadData(FileStream& File) {
	File >> BasicRect.top;
	File >> BasicRect.bottom;
	File >> BasicRect.left;
	File >> BasicRect.right;
	File >> Rg;
	File >> Scanned;
}
void MappedRect::WriteData(FileStream& File) const {
	File << BasicRect.top;
	File << BasicRect.bottom;
	File << BasicRect.left;
	File << BasicRect.right;
	File << Rg;
	File << Scanned;
}