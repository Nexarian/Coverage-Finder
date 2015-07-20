#pragma once

#include "MappedLine.h"

class MappedPoint : public GeometricObject 
{
public:

	void ResetData(void);

	//these are constructors, and SHOULD ONLY be called with a constructor
	explicit MappedPoint(void);
	explicit MappedPoint(const MappedPoint& CopyPoint);
	virtual ~MappedPoint(void);

	POINT PointPart; //Point coordinates

	//Is this point legal to travel through?
	bool Invalid;
	//Is the point used?
	bool Hit;
	//Is it on a horizontal/vertical divide line?
	bool OnVertical;
	bool OnHorizontal;
	//variables for if OnVertical or OnHorizontal is true
	MappedLine VertLine;
	MappedLine HorzLine;

	double Range;
	int ArrayPosition; //position of element in an array

	//Vertex status
	bool IsVertex;
	bool IsBottomVertex;
	bool IsRightVertex;
	bool IsConcaveVertex; //there is a legitimate purpose for a concave vertex

	const MappedPoint& operator=(const MappedPoint& CopyPoint);
	const bool operator==(const MappedPoint& ComparePoint) const;
	const bool operator!=(const MappedPoint& ComparePoint) const;

	//WARNING! These comparison operators are specialized to the range number, do not use for general purpose
	const bool operator>(const MappedPoint& ComparePoint) const; 
	const bool operator>=(const MappedPoint& ComparePoint) const;
	const bool operator<(const MappedPoint& ComparePoint) const;
	const bool operator<=(const MappedPoint& ComparePoint) const;

	operator POINT() {return PointPart;}

	//accessor functions
	std::list<MappedPoint*>& PossibleNodes();
	std::list<MappedPoint*>& PathToEnd();
	std::list<MappedPoint*>& VertexFamily();
	PointAr& EmptySiblings();
	PointAr& InvalidSiblings();
	
	//maintainence function
	void ClearPointStorage(bool PossibleN, bool EmptyS, bool VertexF, bool InvalidS, bool PathE) const;

private:

	void BasicConstruct(void);
	void BasicReset(void);

	//must do manual define because otherwise a recursive situation occurs
	//these are pointers to save memory
	mutable std::list<MappedPoint*>* PN; //points that are legal to travel to (only used for vertices)
	mutable std::list<MappedPoint*>* PE; //path to ending point (only used for vertices)
	mutable std::list<MappedPoint*>* VF;
	mutable PointAr* ES; //empty siblings
	mutable PointAr* IS; //invalid siblings

	void SaveLinkedPoints(FileStream& File, std::list<MappedPoint*>*& DataPtr) const;
	void LoadLinkedPoints(FileStream& File, std::list<MappedPoint*>*& DataPtr); 

	void SaveNormalPoints(FileStream& File, PointAr*& DataPtr) const;
	void LoadNormalPoints(FileStream& File, PointAr*& DataPtr);

	template< typename ClType > //this assumes that the basic type is a pointer
		void ClearPtr(ClType& Dat) const {
			delete Dat;
			Dat = 0;
		}
protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};

typedef std::list< MappedPoint >         MappedPointAr;
typedef MappedPointAr::iterator          MappedPointArItr;
typedef MappedPointAr::const_iterator    MappedPointArCItr;

typedef std::list< MappedPoint* >        MappedPointPtrAr;
typedef MappedPointPtrAr::iterator       MappedPointPtrArItr;
typedef MappedPointPtrAr::const_iterator MappedPointPtrArCItr;