#pragma once

#include "GeometricObject.h"
#include "MappedPoint.h"
#include "MappedRect.h"

#define Approach(ValToChange, Increment, Condition1, Condition2) \
			if (Condition1) {             \
				ValToChange += Increment; \
			}                             \
			else if (Condition2) {        \
				ValToChange -= Increment; \
			}           

class Area : public GeometricObject
{
public:
	Area(void);
	virtual ~Area(void);

	void ClearData(const bool ClearObstacles, const bool ClearVertices, const bool ClearVertLines, const bool ClearHorzLines, const bool ClearRectDef);

	const long ReProbe(const bool VerticalProbe, const bool HorizontalProbe);
	const long ProbePoints(const bool VerticalProbe, const bool HorizontalProbe); //main function - does all the probing and calls probe rectangles
	void MakeProbeOK(const bool VerticalProbe = false, const bool HorizontalProbe = false); //makes sure the points are correctly probed for any function needing it
	void FindInvalidPoints();

	void DrawProbe(const HDC DrawDC, const bool EraseScreen, const bool DrawObstacles, const bool DrawVertices, const bool DrawVertLines, const bool DrawHorzLines, bool DrawRectDefs); //simply draw the obstacles
	void VertexNodeFind(); //updates the possiblepoints vector of each vertex

	void FillInLegalNodes(MappedPoint& Point, MappedPointPtrAr& NodeSeries, const bool SetToSelf = true, const bool UndoOperation = false, const bool MakeSingle = false);
	//SetToSelf == true means that the point will be filled with all legal vertices
	//when false SetToSelf does the opposite, it sets all vertices to add Point as a node

	const bool GetFirstInvalidLinePoint(const POINT StartPoint, const POINT EndPoint, POINT* ReturnPtr = 0);
	const bool GetFirstValidLinePoint(const POINT StartPoint, const POINT EndPoint, POINT* ReturnPtr = 0);

	void SwapMappedPoints(MappedPoint& Pt1, MappedPoint& Pt2);

	void AddObstacleRect(RECT Obstacle);
	void AddObstacleRect(long ObX, long ObY, long ObWidth, long ObHeight);

	void AddObstacleLine(POINT Pt1, POINT Pt2);
	void AddObstacleLine(long Pt1X, long Pt1Y, long Pt2X, long Pt2Y);

	void AddObstaclePoint(POINT Point);
	void AddObstaclePoint(long PointX, long PointY);
	
	const bool IsPointInvalid(const long X, const long Y) const {return PointMap(X, Y).Invalid;}

	void SetAreaHeight(const long NewHeight);
	void SetAreaWidth(const long NewWidth);

	//minus 1 b/c it starts at 0, and the numbers are 1-based
	const long GetHeight() {return AreaHeight - 1;}
	const long GetWidth() {return AreaWidth - 1;}
	const long GetTotalArea() {return AreaArea;}
	const long GetWhiteArea() {return WhiteArea;}

	mutable MappedPointPtrAr Vertices;
	RectPtrAr Rectangles;
	MultiArray<MappedPoint> PointMap; //responsible for storage and retrieval of most of the program's data

private:
	void PathAreaChange();

	long AreaHeight, AreaWidth, AreaArea, WhiteArea;

	bool PointsProbed;

	RectAr ObstacleRects; //A collection of rectangles that indicate where the path cannot be
	LineAr ObstacleLines;
	PointAr ObstaclePoints;
	
	LineAr VerticalLines;
	LineAr HorizontalLines;

	void FindSiblings(const POINT DestPoint, PointAr& InvalidSiblings, PointAr& EmptySiblings);
	void ValidatePointLocation(POINT& Location);

	enum HV {
		Horizontal = 0,
		Vertical = 1
	};

	void UpdateHorzVertStatus(POINT StartLine, POINT EndLine, HV Type, bool Status);
	void AddHorzVertDefaultLines();

	template< typename DataType> void EraseItems(std::list< typename DataType>& DestVect, std::list< typename std::list< typename DataType>::iterator > EraseList);

protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};