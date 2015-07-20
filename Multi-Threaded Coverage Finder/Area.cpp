#include "StdAfx.h"
#include "area.h"

Area::Area(void)
{
	PointsProbed = false;
}

Area::~Area(void)
{
	ClearData(true, true, true, true, true);
}
void Area::ClearData(bool ClearObstacles, bool ClearVertices, bool ClearVertLines, bool ClearHorzLines, bool ClearRectDef) {

	if (ClearObstacles) {
		ObstacleRects.clear();
		ObstacleLines.clear();
		ObstaclePoints.clear();
	}

	if (ClearRectDef) {
		for (RectPtrArCItr DeleteLoop = Rectangles.begin(); DeleteLoop != Rectangles.end(); ++DeleteLoop) {
			delete *DeleteLoop;
		}
		Rectangles.clear();
	}
	if (ClearVertLines)	VerticalLines.clear();
	if (ClearHorzLines) HorizontalLines.clear();
	if (ClearVertices) Vertices.clear();
}

const bool Area::GetFirstInvalidLinePoint(const POINT StartPoint, const POINT EndPoint, POINT* ReturnPtr) {
	PointAr LinePoints;
	GeometricObject::LinePath(StartPoint, EndPoint, false, false, &LinePoints);

	for (PointArCItr VectIt = LinePoints.begin(); VectIt != LinePoints.end(); ++VectIt) {
		if (PointMap(VectIt->x, VectIt->y).Invalid) {
			if (ReturnPtr) *ReturnPtr = *VectIt; //as long as returnptr != 0
			return true;
		}
	}

	return false; //means that no invalid points were found in the line, the line is clear to go
}

const bool Area::GetFirstValidLinePoint(const POINT StartPoint, const POINT EndPoint, POINT* ReturnPtr) {
	PointAr LinePoints;
	GeometricObject::LinePath(StartPoint, EndPoint, false, false, &LinePoints);

	for (PointArCItr VectIt = LinePoints.begin(); VectIt != LinePoints.end(); ++VectIt) {
		if (!(PointMap(VectIt->x, VectIt->y).Invalid)) {
			if (ReturnPtr) *ReturnPtr = *VectIt; //as long as returnptr != 0
			return true;
		}
	}

	return false; //means that there is no valid point in the line, nearly impossible, but still possible
}

void Area::FillInLegalNodes(MappedPoint& Point, MappedPointPtrAr& NodeSeries, const bool SetToSelf, const bool UndoOperation, const bool MakeSingle) { //find all vertices that this point can legally move to (but only modify the point reference sent - any other behavior may be unsafe)

	MappedPointPtrArItr NodeItr;

	if (SetToSelf) {
		for (NodeItr = NodeSeries.begin(); NodeItr != NodeSeries.end(); ++NodeItr) {
			if (!GetFirstInvalidLinePoint(Point, *(*NodeItr))) { //then this node is valid, add it in
				Point.PossibleNodes().push_back(*NodeItr);
			}
		}
	}
	else { //fill all vertices with this point as a node

		if (UndoOperation) {
			for (NodeItr = NodeSeries.begin(); NodeItr != NodeSeries.end(); ++NodeItr) {
				if ((*NodeItr)->PossibleNodes().size() > 0) {
					if (*(*NodeItr)->PossibleNodes().back() == Point) {
						(*NodeItr)->PossibleNodes().pop_back(); //erase the last element
					}
				}
			}
		}
		else {
			for (NodeItr = NodeSeries.begin(); NodeItr != NodeSeries.end(); ++NodeItr) {
				if (!GetFirstInvalidLinePoint(Point, *(*NodeItr))) {
					if (MakeSingle) (*NodeItr)->PossibleNodes().clear(); //erase all other nodes...
					//add in the end as a valid node
					(*NodeItr)->PossibleNodes().push_back(&Point); //this is dangerous if this pointer is left in the vector - many null dereferences would occur, but it will be deleted before this function ends
				}
			}
		}
	}
}
void Area::AddObstacleRect(RECT Obstacle) {
	ObstacleRects.push_back(MappedRect(Obstacle));
}
void Area::AddObstacleRect(long ObX, long ObY, long ObWidth, long ObHeight) {
	RECT AddRect;

	AddRect.top = ObY;
	AddRect.left = ObX;
	AddRect.bottom = AddRect.top + ObHeight;
	AddRect.right = AddRect.left + ObWidth;

	AddObstacleRect(AddRect);

	PointsProbed = false;
}
void Area::AddObstacleLine(POINT Pt1, POINT Pt2) {	
	AddObstacleLine(Pt1.x, Pt1.y, Pt2.x, Pt2.y);

	PointsProbed = false;
}
void Area::AddObstacleLine(long Pt1X, long Pt1Y, long Pt2X, long Pt2Y) {
	ObstacleLines.push_back(MappedLine(Pt1X, Pt1Y, Pt2X, Pt2Y));

	PointsProbed = false;
}
void Area::AddObstaclePoint(POINT Point) {
	AddObstaclePoint(Point.x, Point.y);

	PointsProbed = false;
}
void Area::AddObstaclePoint(long PointX, long PointY) {
	ObstaclePoints.push_back(CPoint(PointX, PointY));

	PointsProbed = false;
}
void Area::SwapMappedPoints(MappedPoint& Pt1, MappedPoint& Pt2) {
	MappedPoint PointBuffer;

	PointBuffer = Pt1;

	Pt1 = Pt2;
	Pt2 = PointBuffer;
}
void Area::DrawProbe(HDC DrawDC, bool EraseScreen, bool DrawObstacles, bool DrawVertices, bool DrawVertLines, bool DrawHorzLines, bool DrawRectDefs) {

	HPEN DrawPen = 0; 
	HPEN OldPen;

	LOGBRUSH BrushLog;
	BrushLog.lbHatch = 0;
	BrushLog.lbStyle = BS_SOLID;

	HBRUSH DrawBrush;
	HBRUSH OldBrush;

	if (EraseScreen) {
		DrawPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	
		BrushLog.lbColor = RGB(255, 255, 255);

		DrawBrush = ::CreateBrushIndirect(&BrushLog);

		OldPen = SelectPen(DrawDC, DrawPen);
		OldBrush = SelectBrush(DrawDC, DrawBrush);

		Rectangle(DrawDC, 0, 0, GetWidth() + 1, GetHeight() + 1);

		SelectPen(DrawDC, OldPen);
		SelectBrush(DrawDC, (HGDIOBJ)OldBrush);

		DeletePen(DrawPen);
		DeleteBrush(DrawBrush);
	}

	//draw obstacle rectangles
	if (DrawObstacles) {
		DrawPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		BrushLog.lbColor = RGB(255, 0, 0);
		DrawBrush = ::CreateBrushIndirect(&BrushLog);

		//select the red
		OldPen = SelectPen(DrawDC, DrawPen);
		OldBrush = SelectBrush(DrawDC, DrawBrush);

		for (RectArCItr VectIt = ObstacleRects.begin(); VectIt != ObstacleRects.end(); ++VectIt) {
			Rectangle(DrawDC, VectIt->BasicRect.left, VectIt->BasicRect.top, VectIt->BasicRect.right + 1, VectIt->BasicRect.bottom + 1);
		}

		SelectPen(DrawDC, OldPen);
		SelectBrush(DrawDC, OldBrush);

		DeletePen(DrawPen);
		DeleteBrush(DrawBrush);
	}


	DrawPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	OldPen = SelectPen(DrawDC, DrawPen);

	LineArCItr LineLoop;

	//draw the rectangle lines
	COLORREF ColorBuffer = RGB(255, 106, 34);

	DrawPen = CreatePen(PS_SOLID, 1, ColorBuffer);
	OldPen = SelectPen(DrawDC, DrawPen);

	POINT First;
	POINT Second;

	if (DrawVertLines) {
		for (LineLoop = VerticalLines.begin(); LineLoop != VerticalLines.end(); ++LineLoop) {
			
			First = LineLoop->StartPoint;
			Second = LineLoop->EndPoint;

			MoveToEx(DrawDC, LineLoop->StartPoint.x, LineLoop->StartPoint.y, NULL);
			LineTo(DrawDC, LineLoop->EndPoint.x, LineLoop->EndPoint.y);
			
			SetPixel(DrawDC, LineLoop->StartPoint.x, LineLoop->StartPoint.y, ColorBuffer); //set the first pixel
			SetPixel(DrawDC, LineLoop->EndPoint.x, LineLoop->EndPoint.y, ColorBuffer); //since lineto doesn't finish the job
		}
	}

	if (DrawHorzLines) {
		for (LineLoop = HorizontalLines.begin(); LineLoop != HorizontalLines.end(); ++LineLoop) {

			First = LineLoop->StartPoint;
			Second = LineLoop->EndPoint;

			MoveToEx(DrawDC, LineLoop->StartPoint.x, LineLoop->StartPoint.y, NULL);
			LineTo(DrawDC, LineLoop->EndPoint.x, LineLoop->EndPoint.y);
		
			SetPixel(DrawDC, LineLoop->StartPoint.x, LineLoop->StartPoint.y, ColorBuffer); //set the first pixel
			SetPixel(DrawDC, LineLoop->EndPoint.x, LineLoop->EndPoint.y, ColorBuffer); //since lineto doesn't finish the job
		}
	}

	if (DrawRectDefs) {

		for (RectPtrArCItr RectLoop = Rectangles.begin(); RectLoop != Rectangles.end(); ++RectLoop) {
		
			MoveToEx(DrawDC, (*RectLoop)->BasicRect.left, (*RectLoop)->BasicRect.top, NULL);
			LineTo(DrawDC, (*RectLoop)->BasicRect.right, (*RectLoop)->BasicRect.bottom);

			SetPixel(DrawDC, (*RectLoop)->BasicRect.left, (*RectLoop)->BasicRect.top, ColorBuffer); //set the first pixel
			SetPixel(DrawDC, (*RectLoop)->BasicRect.right, (*RectLoop)->BasicRect.bottom, ColorBuffer); //since lineto doesn't finish the job
		}

		SelectPen(DrawDC, OldPen);
		DeletePen(DrawPen);

	}

	SelectPen(DrawDC, OldPen);

	DeletePen(DrawPen);

	if (DrawVertices) {
		for (MappedPointPtrArCItr VectIt = Vertices.begin(); VectIt != Vertices.end(); ++VectIt) {
			SetPixel(DrawDC, (*VectIt)->PointPart.x, (*VectIt)->PointPart.y, RGB(0, 0, 255));
		}
	}
}
void Area::MakeProbeOK(const bool VerticalProbe, const bool HorizontalProbe)  {
	if (!PointsProbed) {
		//ReProbe is safer - makes sure everything is right
		ReProbe(VerticalProbe, HorizontalProbe); //all this function needs are the vertices (no horizontal or vertical probe)
	}
}
const long Area::ReProbe(const bool VerticalProbe, const bool HorizontalProbe) {
	PointsProbed = false;

	ClearData(false, true, true, true, true);

	return ProbePoints(VerticalProbe, HorizontalProbe);
}
void Area::FindSiblings(const POINT DestPoint, PointAr& InvalidSiblings, PointAr& EmptySiblings) {

	int XMod = 0, YMod = 0;

	for (int SiblingLoop = 0; SiblingLoop <= 7; ++SiblingLoop) {
		switch (SiblingLoop) {
			case 0: //top left
				XMod = -1;
				YMod = -1;
				break;
			case 1: //top middle
				XMod = 0;
				YMod = -1;
				break;
			case 2: //top right
				XMod = 1;
				YMod = -1;
				break;
			case 3: //middle left
				XMod = 1;
				YMod = 0;
				break;
			case 4: //bottom right
				XMod = 1;
				YMod = 1;
				break;
			case 5: //bottom middle
				XMod = 0;
				YMod = 1;
				break;
			case 6: //bottom left
				XMod = -1;
				YMod = 1;
				break;
			case 7:
				XMod = -1;
				YMod = 0;
				break;
			default:
				assert(false); //really bad, this should NEVER EVER happen
		}

		POINT TestPoint = {DestPoint.x + XMod, DestPoint.y + YMod};

		ValidatePointLocation(TestPoint);

		if (PointMap(TestPoint.x, TestPoint.y).Invalid) {
			InvalidSiblings.push_back(TestPoint);
		}
		else {
			EmptySiblings.push_back(TestPoint);
		}
	}
}
void Area::ValidatePointLocation(POINT& Location) 
{
	if (Location.x < 0) Location.x = 0;
	if (Location.y < 0) Location.y = 0;
	if (Location.x > GetWidth()) Location.x = GetWidth();
	if (Location.y > GetHeight()) Location.y = GetHeight();
}
void Area::FindInvalidPoints() {

	MappedPoint* MappedPointPtr;
	long XLoop = 0;
	long YLoop = 0;

	//Initialize default values
	for (XLoop = 0; XLoop <= GetWidth(); ++XLoop) {
		for (YLoop = 0; YLoop <= GetHeight(); ++YLoop) {

			MappedPointPtr = &(PointMap(XLoop, YLoop));
		
			MappedPointPtr->PointPart.x = XLoop;
			MappedPointPtr->PointPart.y = YLoop;
			MappedPointPtr->Invalid = false;
			MappedPointPtr->Hit = false;
		}
	}

	//find all invalid points
	WhiteArea = 0;
	for (RectArCItr RectItr = ObstacleRects.begin(); RectItr != ObstacleRects.end(); ++RectItr) { //loop through the rectangles

		//loop through all points in the rectangle and make them invalid
		for (XLoop = RectItr->BasicRect.left; XLoop <= RectItr->BasicRect.right; ++XLoop) {
			for (YLoop = RectItr->BasicRect.top; YLoop <= RectItr->BasicRect.bottom; ++YLoop) {
				if (!PointMap(XLoop, YLoop).Invalid) ++WhiteArea;
				PointMap(XLoop, YLoop).Invalid = true;
			}
		}
	}
	//find the true number of valid points (which returns the whitespace area - not necessarily contiguous)
	WhiteArea = AreaArea - WhiteArea;
}
const long Area::ProbePoints(const bool VerticalProbe, const bool HorizontalProbe) {

	if (PointsProbed) return 0;

	PathAreaChange();

	long XLoop = 0;
	long YLoop = 0;
	MappedPoint* MappedPointPtr;
	MappedPoint CandidateVertex;

	POINT StartPoint;
	POINT EndPoint;
	POINT TestPoint;
	PointAr TheLine;

	RectArCItr RectItr;
	LineArCItr LineItr;
	PointArCItr PointItr;
	MappedPointPtrArCItr MappedItr;

	FindInvalidPoints();

	ClearData(false, true, true, true, true); //clear the vertices

	//now find the vertices (significant points)
	for (XLoop = 0; XLoop <= GetWidth(); ++XLoop) {
		for (YLoop = 0; YLoop <= GetHeight(); ++YLoop) {
			MappedPointPtr = &(PointMap(XLoop, YLoop));
			if (!MappedPointPtr->Invalid) continue;

			CandidateVertex.ResetData();

			//finds all empty and invalid siblings
			FindSiblings(MappedPointPtr->PointPart, MappedPointPtr->InvalidSiblings(), MappedPointPtr->EmptySiblings());

			size_t NumSiblings = MappedPointPtr->InvalidSiblings().size();

			if (MappedPointPtr->InvalidSiblings().size() >= 3 && MappedPointPtr->Invalid) { //check for adjacency (minimum of 3 required to even look at the point)

				if (NumSiblings == 7) { //this is a guarantee
					CandidateVertex.PointPart = MappedPointPtr->EmptySiblings().front();
					CandidateVertex.Invalid = false;

					CandidateVertex.IsVertex = true;
								
					CandidateVertex.IsRightVertex = CandidateVertex.PointPart.x > MappedPointPtr->PointPart.x;
					CandidateVertex.IsBottomVertex = CandidateVertex.PointPart.y > MappedPointPtr->PointPart.y;
					CandidateVertex.IsConcaveVertex = true;
				}
				else if ( //if this is true then the vertex is valid
					NumSiblings == 3 && 
					((Range(MappedPointPtr->InvalidSiblings().front(), *++MappedPointPtr->InvalidSiblings().begin()) == (double)1 && 
					Range(*++MappedPointPtr->InvalidSiblings().begin(), MappedPointPtr->InvalidSiblings().back()) == (double)1) ||

					(Range(MappedPointPtr->InvalidSiblings().front(), MappedPointPtr->InvalidSiblings().back()) == (double)1 &&
					Range(MappedPointPtr->InvalidSiblings().front(), *++MappedPointPtr->InvalidSiblings().begin()) == (double)1))
					)	{
		
					for (PointItr = MappedPointPtr->InvalidSiblings().begin(); PointItr != MappedPointPtr->InvalidSiblings().end(); ++PointItr) {
						if (Range(*MappedPointPtr, *PointItr) == (double)1) {

							long CoordMod;

							if (MappedPointPtr->PointPart.x - PointItr->x != 0) {
								CoordMod = MappedPointPtr->PointPart.x - PointItr->x;
								CandidateVertex.PointPart.x = MappedPointPtr->PointPart.x + CoordMod;
								CandidateVertex.IsRightVertex = (CoordMod > 0);
							}
							else if (MappedPointPtr->PointPart.y - PointItr->y != 0) {
								CoordMod = MappedPointPtr->PointPart.y - PointItr->y;
								CandidateVertex.PointPart.y = MappedPointPtr->PointPart.y + CoordMod;
								CandidateVertex.IsBottomVertex = (CoordMod > 0);
							}
						}
					}

					CandidateVertex.Invalid = false;
					CandidateVertex.IsVertex = true;
					CandidateVertex.IsConcaveVertex = false;
					
				}
				
				if (CandidateVertex.IsVertex) {
					//prevent invalid territory
					ValidatePointLocation(CandidateVertex.PointPart);
							
					PointMap(CandidateVertex.PointPart.x, CandidateVertex.PointPart.y) = CandidateVertex;
					
	 				Vertices.push_back(&PointMap(CandidateVertex.PointPart.x, CandidateVertex.PointPart.y));
				}

			}
			MappedPointPtr->ClearPointStorage(true, true, true, true, true); //sibling data does not need to be stored
		}
	}

	//Create the dividing lines and the resulting rectangles
	AddHorzVertDefaultLines();

	for (MappedItr = Vertices.begin(); MappedItr != Vertices.end(); ++MappedItr) {

		MappedPointPtr = &(PointMap((*MappedItr)->PointPart.x, (*MappedItr)->PointPart.y));

		//Do vertical lines
		if (!(MappedPointPtr->OnVertical) && VerticalProbe) {

			XLoop = (*MappedItr)->PointPart.x;
			YLoop = (*MappedItr)->PointPart.y;
			//Record the points

			StartPoint.x = XLoop;
			StartPoint.y = 0;

			EndPoint.x = XLoop;
			EndPoint.y = GetHeight();

			//Test the TopPoint to the current vertex
			
			if (GetFirstInvalidLinePoint(StartPoint, (*MappedItr)->PointPart, &TestPoint)) {
				if (TestPoint.y < YLoop) {

					GetFirstInvalidLinePoint((*MappedItr)->PointPart, StartPoint, &TestPoint);
					
					StartPoint.x = TestPoint.x;
					StartPoint.y = TestPoint.y + 1;
				}
			}

			//Test the EndPoint to the current vertex
		
			if (GetFirstInvalidLinePoint(EndPoint, (*MappedItr)->PointPart, &TestPoint)) {
				if (TestPoint.y > YLoop) {

					GetFirstInvalidLinePoint((*MappedItr)->PointPart, EndPoint, &TestPoint);
					
					EndPoint.x = TestPoint.x;
					EndPoint.y = TestPoint.y - 1;
				}
			}

			//now we know what the line is, so now we MUST update the status of each mapped point wrt/ Horizontal/Vertical presence

			ValidatePointLocation(StartPoint);
			ValidatePointLocation(EndPoint);

			UpdateHorzVertStatus(StartPoint, EndPoint, Vertical, true);
			VerticalLines.push_back(MappedLine(StartPoint, EndPoint));
		}

		//Do horizontal lines
		if (!(MappedPointPtr->OnHorizontal) && HorizontalProbe) {
			XLoop = (*MappedItr)->PointPart.x;
			YLoop = (*MappedItr)->PointPart.y;
			//Record the points

			StartPoint.x = 0;
			StartPoint.y = YLoop;
			
			EndPoint.x = GetWidth();
			EndPoint.y = YLoop;

			//Test the TopPoint to the current vertex
			
			if (GetFirstInvalidLinePoint(StartPoint, (*MappedItr)->PointPart, &TestPoint)) {
				if (TestPoint.x < XLoop) {

					GetFirstInvalidLinePoint((*MappedItr)->PointPart, StartPoint, &TestPoint);
					
					StartPoint.x = TestPoint.x + 1;
					StartPoint.y = TestPoint.y;
				}
			}

			//Test the EndPoint to the current vertex
			
			if (GetFirstInvalidLinePoint(EndPoint, (*MappedItr)->PointPart, &TestPoint)) {
				if (TestPoint.x > XLoop) {

					GetFirstInvalidLinePoint((*MappedItr)->PointPart, EndPoint, &TestPoint);

					EndPoint.x = TestPoint.x - 1;
					EndPoint.y = TestPoint.y;
				}
			}
	

			ValidatePointLocation(StartPoint);
			ValidatePointLocation(EndPoint);

			UpdateHorzVertStatus(StartPoint, EndPoint, Horizontal, true);
			HorizontalLines.push_back(MappedLine(StartPoint, EndPoint));
		}
	}

	//sort the vertical and horizontal lines by x/y position using a bubble sort
	LineArCItr LineLoop;

	//sort in ascending order
	HorizontalLines.sort();
	VerticalLines.sort();

	//update the status of each Mapped point

	for (LineArCItr VerticalUpdate = VerticalLines.begin(); VerticalUpdate != VerticalLines.end(); ++VerticalUpdate) {
	
		TheLine = LinePath(VerticalUpdate->StartPoint, VerticalUpdate->EndPoint, false, false);

		for (PointArCItr LineItr = TheLine.begin(); LineItr != TheLine.end(); ++LineItr) {
			MappedPointPtr = &(PointMap(LineItr->x, LineItr->y));

			MappedPointPtr->OnVertical = true;
			MappedPointPtr->VertLine = *VerticalUpdate;
		}

	}
	
	//Horizontal lines
	for (LineArCItr HorizontalUpdate = HorizontalLines.begin(); HorizontalUpdate != HorizontalLines.end(); ++HorizontalUpdate) {

		TheLine = LinePath(HorizontalUpdate->StartPoint, HorizontalUpdate->EndPoint, false, false);

		for (PointArCItr LineItr = TheLine.begin(); LineItr != TheLine.end(); ++LineItr) {
			MappedPointPtr = &(PointMap(LineItr->x, LineItr->y));

			MappedPointPtr->OnHorizontal = true;
			MappedPointPtr->HorzLine = *HorizontalUpdate;
		}
	}

	POINT TopLeft;
	POINT BottomRight;

	//using all the hoizontal lines will work for when the optimization is combined, or when the optimization is horizontal
	if ( (VerticalProbe && HorizontalProbe) || HorizontalProbe ) {
		for (LineArCItr HorzLoop = HorizontalLines.begin(); HorzLoop != HorizontalLines.end(); ++HorzLoop) {

			TopLeft = HorzLoop->StartPoint;

			PointAr TheLine ;
			LinePath(HorzLoop->StartPoint, HorzLoop->EndPoint, false, false, &TheLine);

			//loop through each point in the horizontal line
			for (PointArCItr LineLoop = TheLine.begin(); LineLoop != TheLine.end(); ++LineLoop) {

				MappedPointPtr = &(PointMap(LineLoop->x, LineLoop->y));
				
				BottomRight.x = BottomRight.y = -1; //BottomRight can never have this value

				//it's on the vertical line

				if (VerticalProbe && 
					HorizontalProbe && 
					MappedPointPtr->OnVertical && 
					(TopLeft != MappedPointPtr->PointPart) ) {

					//now travel down the vertical line until we encounter another horizontal line
					PointAr TheSecondLine;
					LinePath(MappedPointPtr->VertLine.StartPoint, MappedPointPtr->VertLine.EndPoint, false, false, &TheSecondLine);

					bool OK = false;

					for (PointArCItr Line2Loop = TheSecondLine.begin(); Line2Loop != TheSecondLine.end(); ++Line2Loop) {
						//This is complicated. Since we're scanning, and whenever we intersect a vertical line, we want to travel down it, we cannot start at the beginning of the line
						//This is because there may be vertical lines above where we are, and it will mess us up
						if (CPoint(*Line2Loop) == CPoint(MappedPointPtr->PointPart)) OK = true;

						if (PointMap(Line2Loop->x, Line2Loop->y).OnHorizontal && (*Line2Loop != MappedPointPtr->PointPart) && OK) {
							BottomRight = *Line2Loop;
							break;
						}
					}

				}	
			    else if (HorizontalProbe &&	!VerticalProbe && //must be horizontal probe only
					(TopLeft != MappedPointPtr->PointPart) && //the TopLeft of the rectangle cannot start off the line which descends
					((MappedPointPtr->OnVertical) || //now for specific conditions that can trigger it - in can be on a vertical line 
					  MappedPointPtr->Invalid ||  //it can be an invalid point (as long as it moves back one before adding in the point)
					  (MappedPointPtr->IsVertex && !(MappedPointPtr->IsBottomVertex)) || //it can be a vertex but not one on the bottom of a rectangle
					  ((*LineLoop == TheLine.back()) && (MappedPointPtr->PointPart.x != GetWidth()) ) ) ) { //it can be at the end of the currently searched line

					//since this is less well-defined we must just guess
					for (YLoop = MappedPointPtr->PointPart.y; YLoop <= GetHeight(); ++YLoop) {

						if ((PointMap(MappedPointPtr->PointPart.x - MappedPointPtr->Invalid, YLoop).OnHorizontal && YLoop != MappedPointPtr->PointPart.y)) {
							BottomRight.x = MappedPointPtr->PointPart.x - MappedPointPtr->Invalid;
							BottomRight.y = YLoop;
							break;
						}
					}
				}

				if (BottomRight.x != -1 && BottomRight.y != -1) {
					Rectangles.push_back(new MappedRect(TopLeft, BottomRight));
					TopLeft = MappedPointPtr->PointPart; //update the topleft coordinate
				}

			}
		}
	}
	else { //VerticalProbe == true 
		//we need a separate method for doing a vertical scan, it's fundamentally different.

		for (LineArCItr VertLoop = VerticalLines.begin(); VertLoop != VerticalLines.end(); ++VertLoop) {

			TopLeft = VertLoop->StartPoint;

			TheLine = LinePath(VertLoop->StartPoint, VertLoop->EndPoint, false, false);

			for (PointArCItr LineLoop = TheLine.begin(); LineLoop != TheLine.end(); ++LineLoop) {

				MappedPointPtr = &(PointMap(LineLoop->x, LineLoop->y));

				BottomRight.x = BottomRight.y = -1; //BottomRight can never have this value

				//if ( (MappedPointPtr->OnHorizontal || MappedPointPtr->Invalid || (MappedPointPtr->IsVertex && !MappedPointPtr->IsRightVertex) || ( ((LineLoop + 1) == TheLine.end()) && (MappedPointPtr->PointPart.y != GetHeight()) )) && (TopLeft != MappedPointPtr->PointPart) ) {
				if ( (MappedPointPtr->OnHorizontal || MappedPointPtr->Invalid || (MappedPointPtr->IsVertex && !MappedPointPtr->IsRightVertex) || ( (*LineLoop == TheLine.back()) && (MappedPointPtr->PointPart.y != GetHeight()) )) && (TopLeft != MappedPointPtr->PointPart) ) {
					for (XLoop = MappedPointPtr->PointPart.x; XLoop <= GetWidth(); ++XLoop) {
						if (PointMap(XLoop, MappedPointPtr->PointPart.y - MappedPointPtr->Invalid).OnVertical && (XLoop != MappedPointPtr->PointPart.x)) {
							BottomRight.x = XLoop;
							BottomRight.y = MappedPointPtr->PointPart.y - MappedPointPtr->Invalid;
							break;
						}
					}
				}

				if (BottomRight.x != -1 && BottomRight.y != -1) {
					Rectangles.push_back(new MappedRect(TopLeft, BottomRight));
					TopLeft = MappedPointPtr->PointPart; //update the topleft coordinate
				}
			}
		}
	}

	std::list< RectPtrArItr > RectEraser;

	//delete the invalid rectangles
	for (RectPtrArItr RectItr = Rectangles.begin(); RectItr != Rectangles.end(); ++RectItr) {
		if (GetFirstInvalidLinePoint(CPoint((*RectItr)->BasicRect.left, (*RectItr)->BasicRect.top), CPoint((*RectItr)->BasicRect.right, (*RectItr)->BasicRect.bottom))) {
			delete *RectItr;
			RectEraser.push_back(RectItr);
		}
	}
	EraseItems<MappedRect*>(Rectangles, RectEraser);

	std::list< MappedPointPtrArItr > MappedEraser;
	//now remove all concave vertices
	for (MappedPointPtrArItr ConcaveLoop = Vertices.begin(); ConcaveLoop != Vertices.end(); ++ConcaveLoop) {
		if ((*ConcaveLoop)->IsConcaveVertex) {

			MappedEraser.push_back(ConcaveLoop);

			//reset vertex data
			(*ConcaveLoop)->IsVertex = (*ConcaveLoop)->IsRightVertex = (*ConcaveLoop)->IsBottomVertex = (*ConcaveLoop)->IsConcaveVertex = false;
			(*ConcaveLoop)->ClearPointStorage(true, true, true, true, true);
			PointMap((*ConcaveLoop)->PointPart.x, (*ConcaveLoop)->PointPart.y) = *(*ConcaveLoop);
		}
	}
	EraseItems<MappedPoint*>(Vertices, MappedEraser);

	PointsProbed = true;

	VertexNodeFind();

	return WhiteArea;
}
void Area::VertexNodeFind() {

	MakeProbeOK();

	//now update the "nodes" of each vertex

	//loop through each vertex
	for (MappedPointPtrArItr VertexItr = Vertices.begin(); VertexItr != Vertices.end(); ++VertexItr) {
		//now loop through each vertex again
		(*VertexItr)->PossibleNodes().clear();

		for (MappedPointPtrArItr SubVertexItr = Vertices.begin(); SubVertexItr != Vertices.end(); ++SubVertexItr) {
			if (!GetFirstInvalidLinePoint(*(*VertexItr), *(*SubVertexItr)) && (*VertexItr)->PointPart != (*SubVertexItr)->PointPart) { //no invalid points were found, add into possible nodes
				(*VertexItr)->PossibleNodes().push_back(*SubVertexItr);
			}
		}
	}
}
template< typename DataType >
void Area::EraseItems(std::list< typename DataType >& DestVect, std::list< typename std::list< typename DataType>::iterator > EraseList) 
{
	for (std::list< std::list<DataType>::iterator >::const_iterator EraseLoop = EraseList.begin(); EraseLoop != EraseList.end(); ++EraseLoop) {
		DestVect.erase(*EraseLoop);	
	}
}
void Area::SetAreaHeight(const long NewHeight) {
	AreaHeight = NewHeight;
	PathAreaChange();
}
void Area::SetAreaWidth(const long NewWidth) {
	AreaWidth = NewWidth;
	PathAreaChange();
}
void Area::PathAreaChange() {
	PointMap.Redimension(2, false, AreaWidth, AreaHeight);
	AreaArea = (GetWidth() + 1) * (GetHeight() + 1);
}
void Area::UpdateHorzVertStatus(POINT StartLine, POINT EndLine, HV Type, bool Status) {

	PointAr TheLine;
	LinePath(StartLine, EndLine, false, false, &TheLine);
	MappedPoint* MappedPtr = 0;

	for (PointArCItr TheItr = TheLine.begin(); TheItr != TheLine.end(); ++TheItr) {
		MappedPtr = &(PointMap(TheItr->x, TheItr->y));
		if (Type == Horizontal) {
			MappedPtr->OnHorizontal = Status;
			MappedPtr->HorzLine.StartPoint = StartLine;
			MappedPtr->HorzLine.EndPoint = EndLine;
		}
		else if (Type == Vertical) {
			MappedPtr->OnVertical = Status;
			MappedPtr->VertLine.StartPoint = StartLine;
			MappedPtr->VertLine.EndPoint = EndLine;
		}
	}
}
void Area::AddHorzVertDefaultLines() {
	//for purposes of making this work the outside edges must have lines (regardless of whether there are obstacles)
	PointAr TestLine;
	PointArCItr LineItr;
	bool Previous = false, PrevBuffer = false;
	bool EndOfTheLine = false;
	POINT StartPoint = {0, 0};
	POINT EndPoint = {0, 0};
	POINT PrevPoint = {0, 0};

	for (int AddLoop = 0; AddLoop <= 3; ++AddLoop) {

		//define the lines like normal
		switch (AddLoop) {
			case 0:
				GetFirstValidLinePoint(CPoint(0, 0), CPoint(GetWidth(), 0), &StartPoint);
				GetFirstValidLinePoint(CPoint(GetWidth(), 0), CPoint(0, 0), &EndPoint);
				break;
			case 1:
				GetFirstValidLinePoint(CPoint(0, GetHeight()), CPoint(GetWidth(), GetHeight()), &StartPoint);
				GetFirstValidLinePoint(CPoint(GetWidth(), GetHeight()), CPoint(0, GetHeight()), &EndPoint);
				break;
			case 2:
				GetFirstValidLinePoint(CPoint(0, 0), CPoint(0, GetHeight()), &StartPoint);
				GetFirstValidLinePoint(CPoint(0, GetHeight()), CPoint(0, 0), &EndPoint);
				break;
			case 3:
				GetFirstValidLinePoint(CPoint(GetWidth(), 0), CPoint(GetWidth(), GetHeight()), &StartPoint);
				GetFirstValidLinePoint(CPoint(GetWidth(), GetHeight()), CPoint(GetWidth(), 0), &EndPoint);
				break;
		}

		//now figure out what's actually going on with the lines.
		Previous = false;
		HV HorzVertType;
		LineAr* HorzVertPtr;
		TestLine.clear();
		LinePath(StartPoint, EndPoint, false, false, &TestLine);

		if (AddLoop <= 1) { //horizontal
			HorzVertPtr = &HorizontalLines;
			HorzVertType = Area::Horizontal;
		}
		else {
			HorzVertPtr = &VerticalLines;
			HorzVertType = Area::Vertical;
		}

		for (LineItr = TestLine.begin(); LineItr != TestLine.end(); ++LineItr) {

			PrevBuffer = PointMap(LineItr->x, LineItr->y).Invalid;
			EndOfTheLine = (*LineItr == TestLine.back());

			if ( PrevBuffer || EndOfTheLine) { //if it's invalid then

				if ( !Previous || EndOfTheLine ) { //if the previous point was valid then we've hit an obstacle, this is the end
					if (EndOfTheLine) {
						EndPoint = *LineItr;
					}
					else {
						EndPoint = PrevPoint;
					}
					HorzVertPtr->push_back( MappedLine(StartPoint, EndPoint) );
					UpdateHorzVertStatus(StartPoint, EndPoint, HorzVertType, true);
				}

			}
			else {

				if (Previous) { //if the previous point was invalid, then this is the start
					StartPoint.x = LineItr->x;
					StartPoint.y = LineItr->y;
				}
			}

			Previous = PrevBuffer;
			PrevPoint.x = LineItr->x;
			PrevPoint.y = LineItr->y;

		}
	}
}
void Area::ReadData(FileStream& File) {
	ClearData(true, true, true, true, true);

	File >> AreaHeight; 
	File >> AreaWidth;
	File >> AreaArea;
	File >> WhiteArea;
	File >> PointsProbed;

	PathAreaChange();

	int ReadLoop = 0;
	int MaxRead = 0;

	File >> MaxRead;
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		MappedPoint ReadBuffer;
		File >> ReadBuffer;
		PointMap(ReadBuffer.PointPart.x, ReadBuffer.PointPart.y) = ReadBuffer;
		Vertices.push_back(&(PointMap(ReadBuffer.PointPart.x, ReadBuffer.PointPart.y)));
	}
	//now restore the possiblenodes and PathToEnd
	for (MappedPointPtrArItr RestoreLoop = Vertices.begin(); RestoreLoop != Vertices.end(); ++RestoreLoop) {

		for (int RestoreType = 0; RestoreType <= 1; ++RestoreType) {

			MappedPointPtrAr* ThingToRestore = 0;

			switch (RestoreType) {
				case 0:
					ThingToRestore = &((*RestoreLoop)->PossibleNodes());
					break;
				case 1:
					ThingToRestore = &((*RestoreLoop)->VertexFamily());
					break;
				default:
					//memory error, should never occur
					break;
			}


			for (MappedPointPtrArItr NodeRestore = ThingToRestore->begin(); NodeRestore != ThingToRestore->end(); ++NodeRestore) {
				//now obtain a point to the necessary mappedpoint
				int Counter = 0;
				MappedPoint* PtrBuffer = 0;
				for (MappedPointPtrArItr NodeFind = Vertices.begin(); NodeFind != Vertices.end(); ++NodeFind) {
					if (Counter == (*NodeRestore)->ArrayPosition) {
						PtrBuffer = *NodeFind;
						break;
					}
					++Counter;
				}
				delete *NodeRestore;
				*NodeRestore = PtrBuffer;
			}
		}
	}

	File >> MaxRead;
	Rectangles.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		Rectangles.push_back(dynamic_cast<MappedRect*>(File.ReadClass()));
	}

	File >> MaxRead;
	ObstacleRects.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		MappedRect ReadBuffer;
		File >> ReadBuffer;
		ObstacleRects.push_back(ReadBuffer);
	}

	File >> MaxRead;
	ObstacleLines.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		MappedLine ReadBuffer;
		File >> ReadBuffer;
		ObstacleLines.push_back(ReadBuffer);
	}

	File >> MaxRead;
	ObstaclePoints.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		POINT ReadBuffer;
		File >> ReadBuffer;
		ObstaclePoints.push_back(ReadBuffer);
	}

	File >> MaxRead;
	VerticalLines.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		MappedLine ReadBuffer;
		File >> ReadBuffer;
		VerticalLines.push_back(ReadBuffer);
	}

	File >> MaxRead;
	HorizontalLines.clear();
	for (ReadLoop = 0; ReadLoop < MaxRead; ++ReadLoop) {
		MappedLine ReadBuffer;
		File >> ReadBuffer;
		HorizontalLines.push_back(ReadBuffer);
	}

	FindInvalidPoints();

	//save/load Vertices, Rectangles, ObstacleRectangles, ObstacleLines, ObstaclePoints, VerticalLines, HorizontalLines
}
void Area::WriteData(FileStream& File) const {
	File << AreaHeight;
	File << AreaWidth;
	File << AreaArea;
	File << WhiteArea;
	File << PointsProbed;

	int PositionBuffer = 0;
	for (MappedPointPtrArItr VertLoop = Vertices.begin(); VertLoop != Vertices.end(); ++VertLoop) {
		(*VertLoop)->ArrayPosition = PositionBuffer++;
	}

	File << Vertices.size();
	for (MappedPointPtrArItr VertLoop = Vertices.begin(); VertLoop != Vertices.end(); ++VertLoop) {
		File << *(*VertLoop);
	}

	File << Rectangles.size();
	for (RectPtrArCItr RectLoop = Rectangles.begin(); RectLoop != Rectangles.end(); ++RectLoop) {
		File << **RectLoop;
	}

	File << ObstacleRects.size();
	for (RectArCItr RectLoop = ObstacleRects.begin(); RectLoop != ObstacleRects.end(); ++RectLoop) {
		File << *RectLoop;
	}

	File << ObstacleLines.size();
	for (LineArCItr LineLoop = ObstacleLines.begin(); LineLoop != ObstacleLines.end(); ++LineLoop) {
		File << *LineLoop;
	}

	File << ObstaclePoints.size();
	for (PointArCItr PointLoop = ObstaclePoints.begin(); PointLoop != ObstaclePoints.end(); ++PointLoop) {
		File << *PointLoop;
	}

	File << VerticalLines.size();
	for (LineArCItr LineLoop = VerticalLines.begin(); LineLoop != VerticalLines.end(); ++LineLoop) {
		File << *LineLoop;
	}

	File << HorizontalLines.size();
	for (LineArCItr LineLoop = HorizontalLines.begin(); LineLoop != HorizontalLines.end(); ++LineLoop) {
		File << *LineLoop;
	}
}