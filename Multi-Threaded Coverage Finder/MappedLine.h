#pragma once

#include "GeometricObject.h"

class MappedLine : public GeometricObject 
{

public:

	explicit MappedLine();
	MappedLine(const MappedLine& CopyLine);
	explicit MappedLine(const POINT SP, const POINT EP);
	explicit MappedLine(const LONG Sx, const LONG Sy, const LONG Ex, const LONG Ey);

	virtual ~MappedLine();
	
	POINT StartPoint;
	POINT EndPoint;

	const bool IsVertical() const;
	const bool IsHorizontal() const;

	const POINT Midpoint() const;

	//returns true if it is vertical
	const double Slope() const;

	const double PerpDist(const POINT Perp) const;

	const MappedLine& operator=(const MappedLine& CopyData);
	const bool operator==(const MappedLine& CompData) const;
	const bool operator!=(const MappedLine& CompData) const;

	//greater than or less than only applies when the lines are horizontal or vertical

	const bool operator<(const MappedLine& CompData) const;
	const bool operator<=(const MappedLine& CompData) const;
	const bool operator>(const MappedLine& CompData) const;
	const bool operator>=(const MappedLine& CompData) const;

protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};

typedef std::list< MappedLine >          LineAr;
typedef LineAr::iterator                 LineArItr;
typedef LineAr::const_iterator           LineArCItr;