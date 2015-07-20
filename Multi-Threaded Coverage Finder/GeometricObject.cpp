#include "StdAfx.h"
#include "GeometricObject.h"
#include "Area.h"

Area GeometricObject::PrvArea;
long GeometricObject::DrawSpeed;
long GeometricObject::DrawCounter;

const bool operator==(const POINT& Point1, const POINT& Point2) {
	return ((Point1.x == Point2.x) && (Point1.y == Point2.y));
}
const bool operator!=(const POINT& Point1, const POINT& Point2) {
	return !(Point1 == Point2);
}
const bool operator==(const RECT& Rect1, const RECT& Rect2) {
	return ((Rect1.top == Rect2.top) &&
			(Rect1.left == Rect2.left) &&
			(Rect1.right == Rect2.right) &&
			(Rect1.bottom == Rect2.bottom));
}
const bool operator!=(const RECT& Rect1, const RECT& Rect2) {
	return !(Rect1 == Rect2);
}

FileStream& operator<<(FileStream& LeftSide, const POINT& Pt) {
	return LeftSide << Pt.x << Pt.y;
}
FileStream& operator>>(FileStream& LeftSide, POINT& Pt) {
	LeftSide >> Pt.x;
	LeftSide >> Pt.y;
	return LeftSide;
}

GeometricObject::GeometricObject(void)
{
}

GeometricObject::~GeometricObject(void)
{
}

void GeometricObject::Draw(HDC DrawDC, bool EraseScreen, bool DrawObstacles, bool DrawVertices, bool DrawVertLines, bool DrawHorzLines, bool DrawRectDefs) {
	GetArea()->DrawProbe(DrawDC, EraseScreen, DrawObstacles, DrawVertices, DrawVertLines, DrawHorzLines, DrawRectDefs);
}

Area* GeometricObject::GetArea() const {
	return &PrvArea;
}

void GeometricObject::DrawFilibuster() {
	if (!GeometricObject::DrawSpeed) return;
	if (GeometricObject::DrawCounter == GeometricObject::DrawSpeed) {
		Sleep(1);
		GeometricObject::ResetDrawCounter();
	}
	++GeometricObject::DrawCounter;
}

PointAr GeometricObject::LinePath(const POINT StartPoint, const POINT EndPoint, const bool RegisterHit, const bool ObeyRegister, PointAr* VectToAppend) {

	//NOTE: This function ASSUMES that all points are valid. It simply turns an algebraic linear line graph into distict pixel coordinates

	PointAr ReturnPath;

	if (!(ObeyRegister && PrvArea.PointMap(StartPoint.x, StartPoint.y).Hit)) {
		ReturnPath.push_back(StartPoint);
		if (VectToAppend != 0) VectToAppend->push_back(StartPoint);
		if (RegisterHit) PrvArea.PointMap(StartPoint.x, StartPoint.y).Hit = true;
	}

	if (StartPoint == EndPoint) return ReturnPath;

	POINT CurrentLocation = StartPoint;
	POINT LocationBuffer;
	bool Vertical;
	double Slope;

	{
		MappedLine TheLine(StartPoint, EndPoint);
		Vertical = TheLine.IsVertical();
		Slope = (!Vertical ? TheLine.Slope() : 0);
	}
	
	double XCoordBuffer = CurrentLocation.x;
	double YCoordBuffer = CurrentLocation.y;

	do {
		LocationBuffer = CurrentLocation;

		if (Vertical) {
			Approach(CurrentLocation.y, 1, CurrentLocation.y < EndPoint.y, CurrentLocation.y > EndPoint.y);
		}
		else {

			
#define LT (CurrentLocation.x < EndPoint.x)
#define GT (CurrentLocation.x > EndPoint.x)

			//move one closer to the goal

			Approach(XCoordBuffer, 1, LT, GT);
			Approach(YCoordBuffer, Slope, LT, GT);

			CurrentLocation.x = (LONG)(XCoordBuffer + (XCoordBuffer < 0 ? -0.5 : 0.5));
			CurrentLocation.y = (LONG)(YCoordBuffer + (YCoordBuffer < 0 ? -0.5 : 0.5));
		}

		long RangeStorage = (long)(Range(LocationBuffer, CurrentLocation) + 0.5);

		while (RangeStorage != 1 && RangeStorage != 0) { //recalibrate

			Approach(LocationBuffer.y, 1, LocationBuffer.y < CurrentLocation.y, LocationBuffer.y > CurrentLocation.y);

			RangeStorage = (long)(Range(LocationBuffer, CurrentLocation) + 0.5);

			if (ObeyRegister && PrvArea.PointMap(CurrentLocation.x, CurrentLocation.y).Hit) continue;
			if (RegisterHit) PrvArea.PointMap(CurrentLocation.x, CurrentLocation.y).Hit = true;

			if (VectToAppend != 0) VectToAppend->push_back(LocationBuffer);
			ReturnPath.push_back(LocationBuffer);
		}

		if (ObeyRegister && PrvArea.PointMap(CurrentLocation.x, CurrentLocation.y).Hit) continue;
		if (RegisterHit) PrvArea.PointMap(CurrentLocation.x, CurrentLocation.y).Hit = true;

		if (VectToAppend != 0) VectToAppend->push_back(CurrentLocation);
 		ReturnPath.push_back(CurrentLocation);

	} while (CurrentLocation != EndPoint);

	return ReturnPath;
}
const double GeometricObject::Range(const POINT Point1, const POINT Point2) {
	return sqrt(pow((double)Point1.x - Point2.x, 2) + pow((double)Point1.y - Point2.y, 2));
}
