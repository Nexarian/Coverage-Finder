#pragma once

#include <algorithm>
#include <MultiArray.h>
#include "GeometricObject.h"
#include "Area.h"

class Path : public GeometricObject
{
public:
	Path(void);
	virtual ~Path(void);

	PointAr GeneratePath(POINT StartLocation, POINT EndLocation);
	std::list< PointAr > PathsGenerated;
	std::list< std::pair<POINT, POINT> > LastPathVectors;
	
	void ClearData();

	virtual void Draw(HDC DrawDC);

	void DrawPath(const PointAr& PathToDraw, const HDC DestDC, const COLORREF Color, const int DrawSpeed = 100) const;

	void SavePathVectors(const char* FileName) const;
	
private:
	//used for internal path calculations
	struct PathDescriptor { 
		MappedPointPtrAr Points;
		double TotalDistance;

		PathDescriptor() {
			ResetData();
		}

		void ResetData() {
			Points.clear();
			TotalDistance = 0;
		}

		PathDescriptor& operator=(const PathDescriptor CopyDesc) {
			if (*this != CopyDesc) {
				TotalDistance = CopyDesc.TotalDistance;
				Points = CopyDesc.Points;
			}
			return *this;
		}

		const bool operator==(const PathDescriptor CompareDesc) const {
			return (TotalDistance == CompareDesc.TotalDistance) &&
				(Points == CompareDesc.Points);
		}

		const bool operator!=(const PathDescriptor CompareDesc) const {
			return !(*this == CompareDesc);
		}
	};

	void DevelopPathsRecursive(PathDescriptor* PathToDevelop); //a recursive function that performs a tree-search to find all possible paths between vertices
	
	PathDescriptor ShortestPath;

	bool PointIntersect(POINT ThePoint, RECT TheRect);

	//this class is meant for only one purpose - to find a path - store the beginning and ending points.
	POINT StartPoint;
	POINT EndPoint;

protected:
	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;
};
