#include "StdAfx.h"
#include "MappedLine.h"

MappedLine::MappedLine() {
	StartPoint.x = StartPoint.y = 0;
	EndPoint.x = EndPoint.y = 0;
}

MappedLine::MappedLine(const MappedLine& CopyLine) {
	*this = CopyLine;
}
MappedLine::MappedLine(const POINT SP, const POINT EP) {
	StartPoint = SP;
	EndPoint = EP;
}
MappedLine::MappedLine(const LONG Sx, const LONG Sy, const LONG Ex, const LONG Ey) {
	StartPoint.x = Sx;
	StartPoint.y = Sy;

	EndPoint.x = Ex;
	EndPoint.y = Ey;
}

MappedLine::~MappedLine() {
}

const bool MappedLine::IsVertical() const {
	return StartPoint.x == EndPoint.x;
}
const bool MappedLine::IsHorizontal() const {
	return StartPoint.y == EndPoint.y;
}

const POINT MappedLine::Midpoint() const {
	POINT ReturnPoint;
	ReturnPoint.x = (StartPoint.x + EndPoint.x) / 2;
	ReturnPoint.y = (StartPoint.y + EndPoint.y) / 2;
	return ReturnPoint;
}

//returns true if it is vertical
const double MappedLine::Slope() const {
	if (IsVertical()) return 0; //don't even waste time calculating
	return (double)(EndPoint.y - StartPoint.y) / (double)(EndPoint.x - StartPoint.x);
}

const double MappedLine::PerpDist(const POINT Perp) const {
	if (IsVertical()) { //the line is vertical
		return (double)abs(Perp.x - StartPoint.x);
	}
	else if (IsHorizontal()) { //the line is horizontal
		return (double)abs(Perp.y - StartPoint.y);
	}
	else { //the line is normal
		double S = Slope();

		double NormalB = EndPoint.y - (S * EndPoint.x);
		double PerpendicularSlope = ((double)-1) / S;
		double PerpendicularB = (double)Perp.y - (double)((double)PerpendicularSlope * (double)Perp.x);

		POINT IntersectPoint;
		IntersectPoint.x = (LONG)((double)(PerpendicularB - NormalB) / (double)(S - PerpendicularSlope));
		IntersectPoint.y = (LONG)(S * IntersectPoint.x + NormalB);

		return GeometricObject::Range(IntersectPoint, Perp);
	}
}

const MappedLine& MappedLine::operator=(const MappedLine& CopyData) {
	if (*this != CopyData) {
		StartPoint = CopyData.StartPoint;
		EndPoint = CopyData.EndPoint;
	}
	return *this;
}

const bool MappedLine::operator==(const MappedLine& CompData) const {
	return (StartPoint == CompData.StartPoint) &&
		(EndPoint == CompData.EndPoint);
}

const bool MappedLine::operator!=(const MappedLine& CompData) const {
	return !(*this == CompData);
}

//greater than or less than only applies when the lines are horizontal or vertical

const bool MappedLine::operator<(const MappedLine& CompData) const {
	if (IsVertical() && CompData.IsVertical()) { //x coordinates are equal
		return StartPoint.y < CompData.StartPoint.y;
	}
	else if (IsHorizontal() && CompData.IsHorizontal()) { //y coordinates are equal
		return StartPoint.x < CompData.StartPoint.x;
	}
	return false;
}
const bool MappedLine::operator<=(const MappedLine& CompData) const {
	return (*this < CompData) || (*this == CompData);
}

const bool MappedLine::operator>(const MappedLine& CompData) const {
	if (IsVertical() && CompData.IsVertical()) { //x coordinates are equal
		return StartPoint.y > CompData.StartPoint.y;
	}
	else if (IsHorizontal() && CompData.IsHorizontal()) { //y coordinates are equal
		return StartPoint.x > CompData.StartPoint.x;
	}
	return false;
}
const bool MappedLine::operator>=(const MappedLine& CompData) const {
	return (*this > CompData) || (*this == CompData);
}

void MappedLine::ReadData(FileStream& File) {
	File >> StartPoint;
	File >> EndPoint;
}
void MappedLine::WriteData(FileStream& File) const {
	File << StartPoint;
	File << EndPoint;
}