#pragma once

#include <assert.h>
#include <list>
#include <MultiArray.h>

//equality operators for the point structure
const bool operator==(const POINT& Point1, const POINT& Point2);
const bool operator!=(const POINT& Point1, const POINT& Point2);
//C++ will not allow overloading of = operators for non members, I already tried

const bool operator==(const RECT& Rect1, const RECT& Rect2);
const bool operator!=(const RECT& Rect1, const RECT& Rect2);
//C++ will not allow overloading of = operators for non members, I already tried

FileStream& operator<<(FileStream& LeftSide, const POINT& Pt);
FileStream& operator>>(FileStream& LeftSide, POINT& Pt);

//Ar = "Array"

typedef std::list< POINT >               PointAr;
typedef PointAr::iterator                PointArItr;
typedef PointAr::const_iterator          PointArCItr;

class Area;

class GeometricObject : public PersistentObject
{
public:
	GeometricObject(void);
	virtual ~GeometricObject(void);

	//this has been tested with Lists and Vectors, nothing else
	template< typename ArType >
		ArType AddSTLArrays(ArType& V1, const ArType& V2, const bool ModifyFirst) {
			ArType ReturnVect;
			ArType::iterator VectItr;
			ArType::const_iterator VectCItr;

			if (!ModifyFirst) {
				for (VectCItr = V1.begin(); VectCItr != V1.end(); ++VectCItr) {
					ReturnVect.push_back(*VectCItr);
				}
			}
			for (VectCItr = V2.begin(); VectCItr != V2.end(); ++VectCItr) {
				if (ModifyFirst) {
					V1.push_back(*VectCItr);
				}
				else {
					ReturnVect.push_back(*VectCItr);
				}
			}

			if (ModifyFirst) {
				ArType Nothing;
				return Nothing;
			}
			else {
				return ReturnVect;
			}
		}

	virtual void Draw(HDC DrawDC, bool EraseScreen, bool DrawObstacles, bool DrawVertices, bool DrawVertLines, bool DrawHorzLines, bool DrawRectDefs);

	//StartPoint and EndPoint are obvious... RegisterHit means that it "hits" the points on the PointMap... ObeyRegister means "should I ignore points that are already hit?", VectToAppend means an array that it adds into to save computational speed
	static PointAr LinePath(const POINT StartPoint, const POINT EndPoint, const bool RegisterHit, const bool ObeyRegister, PointAr* VectToAppend = 0);

	static const double Range(const POINT Point1, const POINT Point2);

	static void SetDrawSpeed(long DS) {GeometricObject::DrawSpeed = DS;}
	static void DrawFilibuster();
	static void ResetDrawCounter() {GeometricObject::DrawCounter = 0;}

	Area* GetArea() const;

private:

	static Area PrvArea;
	static long DrawSpeed;
	static long DrawCounter;

};

typedef std::list< unsigned long >       ULongAr;
typedef ULongAr::iterator                ULongArItr;
typedef ULongAr::const_iterator          ULongArCItr;