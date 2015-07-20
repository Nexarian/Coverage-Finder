#include "StdAfx.h"
#include "Path.h"
#include <fstream>

Path::Path(void)
{
}

Path::~Path(void)
{
	ClearData();
}

void Path::SavePathVectors(const char* FileName) const {
	std::ofstream OutputFile(FileName);

	//each of these is a vector
	for (std::list< std::pair<POINT, POINT> >::const_iterator VectIt = LastPathVectors.begin(); VectIt != LastPathVectors.end(); ++VectIt) {
		OutputFile << GeometricObject::Range(VectIt->first, VectIt->second) << " " << atan2((double)VectIt->first.y - VectIt->second.y, (double)VectIt->second.x - VectIt->first.x) << std::endl;
	}

	OutputFile.close();
}

void Path::ReadData(FileStream& File) {
	ClearData();
}

void Path::WriteData(FileStream& File) const {
	
}

bool Path::PointIntersect(POINT ThePoint, RECT TheRect) {
	return (ThePoint.x >= TheRect.left && ThePoint.x <= TheRect.right) && (ThePoint.y >= TheRect.top && ThePoint.y <= TheRect.bottom);
}

void Path::DrawPath(const PointAr& PathToDraw, const HDC DestDC, const COLORREF Color, const int DrawSpeed) const {

	GeometricObject::ResetDrawCounter();

	for (PointArCItr VectIt = PathToDraw.begin(); VectIt != PathToDraw.end(); ++VectIt) {
		GeometricObject::DrawFilibuster();
		SetPixel(DestDC, VectIt->x, VectIt->y, Color);
	}
}

PointAr Path::GeneratePath(POINT StartLocation, POINT EndLocation) {

	PointAr ReturnPath;
	ReturnPath.clear();
	LastPathVectors.clear();
	ShortestPath.ResetData();

	//prevent a 0-length path from being returned
	if (StartLocation == EndLocation) {
		ReturnPath.push_back(StartLocation);
		PathsGenerated.push_back(ReturnPath);
		return ReturnPath;
	}

	GetArea()->MakeProbeOK();

	//update the class records
	StartPoint = StartLocation;
	EndPoint = EndLocation;

	MappedLine OrgLine(StartLocation, EndLocation);

	for (MappedPointPtrArItr VertexLoop = GetArea()->Vertices.begin(); VertexLoop != GetArea()->Vertices.end(); ++VertexLoop) {
		(*VertexLoop)->Range = OrgLine.PerpDist((*VertexLoop)->PointPart);
	}

	//the dummy path
	PathDescriptor DummyPath;
	DummyPath.TotalDistance = 0;

	//add in the ending point
	MappedPoint DummyEnd;
	DummyEnd.PointPart = EndLocation;
	DummyEnd.IsVertex = true;
	//this mutilates the vertices for path-finding purposes
	GetArea()->FillInLegalNodes(DummyEnd, GetArea()->Vertices, false, false, true);

	//add in the starting point
	MappedPoint DummyStart;
	DummyStart.PointPart = StartLocation;
	DummyStart.IsVertex = true; //hack-ish
	GetArea()->FillInLegalNodes(DummyStart, GetArea()->Vertices, true, false, false);

	DummyPath.Points.push_back(&DummyStart); //add it to the dummy path

	//now add the ending point as a node to the starting point (if it's legal)
	if (!GetArea()->GetFirstInvalidLinePoint(StartLocation, EndLocation)) {
		DummyPath.Points.front()->PossibleNodes().push_back(&DummyEnd);

		DummyPath.Points.clear();
		DummyPath.Points.push_back(&DummyStart);
		DummyPath.Points.push_back(&DummyEnd);
		DummyPath.TotalDistance = Range(DummyStart, DummyEnd);
		ShortestPath = DummyPath;
	}
	else {
		for (MappedPointPtrArItr VertexLoop = GetArea()->Vertices.begin(); VertexLoop != GetArea()->Vertices.end(); ++VertexLoop) {
			(*VertexLoop)->PossibleNodes().sort();
		}

		DevelopPathsRecursive(&DummyPath);
	}

	//now search for the shortest path and return it
	if (ShortestPath.TotalDistance) {
		MappedPointPtrArCItr cur_it = ShortestPath.Points.begin();
		for (MappedPointPtrArCItr prev_it = cur_it++; cur_it != ShortestPath.Points.end(); prev_it = cur_it++) {
			GetArea()->AddSTLArrays(ReturnPath, LinePath((*prev_it)->PointPart, (*cur_it)->PointPart, false, false), true);
			LastPathVectors.push_back( std::pair<POINT, POINT>((*prev_it)->PointPart, (*cur_it)->PointPart));
		}

		PathsGenerated.push_back(ReturnPath);
	}
	else {
		OutputDebugString(L"Pathfinding has failed.\n");
	}

	for (MappedPointPtrArCItr VertLoop = GetArea()->Vertices.begin(); VertLoop != GetArea()->Vertices.end(); ++VertLoop) {
		(*VertLoop)->ClearPointStorage(true, true, true, true, true);
	}

	GetArea()->VertexNodeFind(); //restore all vertices nodes

	return ReturnPath;
}

void Path::DevelopPathsRecursive(PathDescriptor* PathToDevelop) {

	//NOTE: This procedure uses MappedPoint::IsConcaveVertex as a "hit" indicator
	MappedPoint* DataBuffer;

	for (MappedPointPtrArCItr PathsItr = PathToDevelop->Points.back()->PossibleNodes().begin(); PathsItr != PathToDevelop->Points.back()->PossibleNodes().end(); ++PathsItr) {

		DataBuffer = *PathsItr;

		//look for the current point in the current path - if found, do nothing (continue the loop), otherwise continue the operation
		if (!DataBuffer->IsConcaveVertex) {

			//change the transferbuffer, then undo the changes to speed up the procedure
			double RangeBuffer = Range(*PathToDevelop->Points.back(), DataBuffer->PointPart);
			PathToDevelop->TotalDistance += RangeBuffer;			

			if (!(PathToDevelop->TotalDistance > ShortestPath.TotalDistance && ShortestPath.TotalDistance)) {

				PathToDevelop->Points.push_back(*PathsItr);
				DataBuffer->IsConcaveVertex = true;

				if (**PathsItr == EndPoint) {

					if (!ShortestPath.TotalDistance || (ShortestPath.TotalDistance > PathToDevelop->TotalDistance)) {
						ShortestPath = *PathToDevelop;
					}

				}
				else {
					DevelopPathsRecursive(PathToDevelop);
				}

				PathToDevelop->Points.pop_back();
				DataBuffer->IsConcaveVertex = false;
			}

			PathToDevelop->TotalDistance -= RangeBuffer;
		}	
	}
}

void Path::Draw(HDC DrawDC) {

	HPEN DrawPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN OldPen = SelectPen(DrawDC, DrawPen);

	for (std::list< PointAr >::const_iterator VectIt = PathsGenerated.begin(); VectIt != PathsGenerated.end(); ++VectIt) {
		::MoveToEx(DrawDC, VectIt->front().x, VectIt->front().y, NULL);
		::LineTo(DrawDC, VectIt->back().x, VectIt->back().y);

		DrawPath(*VectIt, DrawDC, RGB(0, 0, 255));
	}

	SelectPen(DrawDC, OldPen);
	DeletePen(DrawPen);	
}

void Path::ClearData() { //does not clear obstacles
	PathsGenerated.clear();
	LastPathVectors.clear();
	ShortestPath.ResetData();
}