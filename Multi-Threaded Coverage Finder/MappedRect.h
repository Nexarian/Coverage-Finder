#pragma once

enum ScanType {
	Vertical = 0,
	Horizontal = 1,
	DiagUp = 2,
	DiagDown = 3
};

enum ScanLocation {
	Beginning = 0,
	Ending = 1
};

enum VerticalStart {
	Top = 0,
	Bottom = 1
};

enum HorizontalStart {
	Left = 0,
	Right = 1
};

#include "GeometricObject.h"
#include "MappedLine.h"

class MappedRect : public GeometricObject 
{

public: 

	RECT BasicRect;
	double Rg; //variable used for sorting the rectangles

	//variables to describe how the rectangles are to be covered
	bool Scanned;
	//store the details of how a rectangle is scanned in the rectangle
	VerticalStart ScanY;
	HorizontalStart ScanX;
	ScanType ScanUsed;

	//operators
	operator RECT() {return BasicRect;}

	const bool operator==(const MappedRect& CompareRect) const;
	const bool operator!=(const MappedRect& CompareRect) const;
	const MappedRect& operator=(const MappedRect& CompareRect);
	const MappedRect& operator=(const RECT& CompareRect);
	const bool operator<(const MappedRect& CompareRect) const;
	const bool operator<=(const MappedRect& CompareRect) const;
	const bool operator>(const MappedRect& CompareRect) const;
	const bool operator>=(const MappedRect& CompareRect) const;

	//initialization
	explicit MappedRect();
	explicit MappedRect(const MappedRect& CopyRect);
	explicit MappedRect(RECT CopyRect);
	explicit MappedRect(const POINT TopLeft, const POINT BottomRight);
	explicit MappedRect(const LONG Tx, const LONG Ty, const LONG Bx, const LONG By);
	virtual ~MappedRect();

	//Geometric properties of the rectangle
	const double Height() const {return BasicRect.bottom - BasicRect.top;}
	const double Width() const {return BasicRect.right - BasicRect.left;}
	const double RectArea() const {return (Height() + 1) * (Width() + 1);}

	const POINT TopLeft() const;
	const POINT TopRight() const;
	const POINT BottomLeft() const;
	const POINT BottomRight() const;

	const MappedLine GetDiagonal() const;
	const bool IsSquare() const;
	const POINT RectCenter() const;

	//rectangle analysis function
	const POINT FindClosestVertex(POINT FindPoint) const;

	const POINT GetCoordinateFromLocation(HorizontalStart HorzStart, VerticalStart VertStart) const;

protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};

typedef std::list< MappedRect >          RectAr;
typedef RectAr::iterator                 RectArItr;
typedef RectAr::const_iterator           RectArCItr;

typedef std::list< MappedRect* >         RectPtrAr;
typedef RectPtrAr::iterator              RectPtrArItr;
typedef RectPtrAr::const_iterator        RectPtrArCItr;