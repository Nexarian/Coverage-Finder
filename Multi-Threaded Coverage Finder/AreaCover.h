#pragma once

#include "Path.h"

struct CoverPoint {
	CoverPoint() {
		Coords.x = Coords.y = 0;
		Color = 0;
		PathCoord = false;
	}
	POINT Coords; //what are the euclidean coordinates of the unit?
	COLORREF Color; //what color is the unit?
	bool PathCoord; //is it a unit in a connecting path?
};
typedef std::list<CoverPoint> CPAr;
typedef CPAr::iterator CPArItr;
typedef CPAr::const_iterator CPArCItr;

struct CoverRect {
	CoverRect() {
		RectList.clear();
		EquivalentPoints.clear();
	}
	~CoverRect() {
		RectList.clear();
		EquivalentPoints.clear();
	}
	std::list<MappedRect> RectList;
	CPAr EquivalentPoints;
};
typedef std::list<CoverRect> CRAr;
typedef CRAr::iterator CRArItr;
typedef CRAr::const_iterator CRArCItr;

enum MovementHeuristic {
	OrderBased, //blindly based on the order in which the rectangles are discovered
	VertexBased, //based on which vertex is closest to move to
	CenterBased //based on which rectangles center is closest to move to
};

enum OptimalType {
	Complete,
	Semi
};

class AreaCover : public GeometricObject
{
public:
	AreaCover(void);
	virtual ~AreaCover(void);

	const POINT GetCoordinateFromScanLocation(const MappedRect &Dimensions, const HorizontalStart HorzCoord, const VerticalStart VertCoord, const ScanType S, const ScanLocation L);

	void PerformCoverage(OptimalType Completeness, MovementHeuristic CAlgorithm);
	void PerformBasicCoverage(ScanType Scan, MovementHeuristic CAlgorithm); //performs estimated coverage value
	void PerformBlindCoverage(ScanType Scan, VerticalStart VertStart, HorizontalStart HorzStart);

	PointAr VerticalCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart);
	const POINT VerticalCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;
	const POINT VerticalCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;

	PointAr HorizontalCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart);
	const POINT HorizontalCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;
	const POINT HorizontalCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;

	PointAr DiagonalUpCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart);
	const POINT DiagonalUpCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;
	const POINT DiagonalUpCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;

	PointAr DiagonalDownCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart);
	const POINT DiagonalDownCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;
	const POINT DiagonalDownCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const;

	void RectanglesToPoints(CRArItr &Target, bool BlackAndWhite); //critical procedure
	void SetPathCreator(Path* PathFinder) {PathCreator = PathFinder;}

	virtual void Draw(HDC DrawDC, CRArItr DrawDescription, bool BlackAndWhite, bool ForceRecreate, bool DrawPath, bool DrawNumbers); //returns the length of the drawn system
	void DrawRectangleNumbers(HDC DrawDC, CRArItr DrawDescription);
	
	CRAr GeneratedCoverages;

private:

	Path* PathCreator;

	RectAr FinalCandidate;
	double FinalValue;
	RectPtrAr OptimalCandidate;
	double CandidateValue;
	MovementHeuristic CurrentHeuristic;
	OptimalType CurrentOptimization;
	RectPtrAr RectangleListPreserve; //to prevent data loss of the original order of probe

	void OptimalCover(); //recursive coverage function

	void PointListToColorList(PointAr* PtList, CPAr* CList, COLORREF C, bool PathMarker);
	void SortAreaRectangles(RectPtrAr &RectangleArray, bool Ascending = true);
	void UndoSort(RectPtrAr &RectangleArray); //restore to configuration saved before the sort
	bool MoreToScan(RectPtrAr &RectangleArray, RectPtrArItr &AvaliableRect);

	RectPtrArItr CurrentRect;
	RectPtrArItr PreviousRect;

	PointAr ReturnVector;

protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};