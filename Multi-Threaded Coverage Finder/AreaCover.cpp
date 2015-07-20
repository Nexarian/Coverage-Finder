#include "StdAfx.h"
#include "areacover.h"

AreaCover::AreaCover(void)
{
	PathCreator = 0;
	FinalCandidate.clear();
	FinalValue = 0;
	CandidateValue = 0;
	OptimalCandidate.clear();
	CurrentHeuristic = OrderBased;
	CurrentOptimization = Complete;
	RectangleListPreserve.clear();
}
AreaCover::~AreaCover(void)
{
	FinalCandidate.clear();
	OptimalCandidate.clear();
	RectangleListPreserve.clear();
}
const POINT AreaCover::GetCoordinateFromScanLocation(const MappedRect &Dimensions, const HorizontalStart HorzCoord, const VerticalStart VertCoord, const ScanType S, const ScanLocation L) {
	switch (S) {
		case Vertical:
			switch (L) {
				case Beginning:
					return VerticalCoverBegin(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				case Ending:
					return VerticalCoverEnd(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				default:
					VERIFY(FALSE);
			}
			break;
		case Horizontal:
			switch (L) {
				case Beginning:
					return HorizontalCoverBegin(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				case Ending:
					return HorizontalCoverEnd(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				default:
					VERIFY(FALSE);
					break;
			}
			break;
		case DiagUp:
			switch (L) {
				case Beginning:
					return DiagonalUpCoverBegin(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				case Ending:
					return DiagonalUpCoverEnd(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				default:
					VERIFY(FALSE);
					break;
			}
			break;
		case DiagDown:
			switch (L) {
				case Beginning:
					return DiagonalDownCoverBegin(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				case Ending:
					return DiagonalDownCoverEnd(Dimensions.TopLeft(), Dimensions.BottomRight(), VertCoord, HorzCoord);
				default:
					VERIFY(FALSE);
					break;
			}
			break;
		default:
			VERIFY(FALSE);
			break;
	}
	POINT DummyPoint;
	DummyPoint.x = DummyPoint.y = 0;
	return DummyPoint;
}
void AreaCover::SortAreaRectangles(RectPtrAr &RectangleArray, bool Ascending) {

	RectangleListPreserve = RectangleArray;

	bool Sorted = true;

	do {
		Sorted = true;

		RectPtrArItr SortLoop1 = RectangleArray.begin();
		for (RectPtrArItr SortLoop2 = ++RectangleArray.begin(); SortLoop2 != RectangleArray.end(); ++SortLoop2) {
			if (Ascending ? (**SortLoop1 > **SortLoop2) : (**SortLoop1 < **SortLoop2)) {
				MappedRect* Buffer = *SortLoop1;
				*SortLoop1 = *SortLoop2;
				*SortLoop2 = Buffer;
				Sorted = false;
				break;
			}
			++SortLoop1;
		}
	} while (!Sorted);
}
void AreaCover::UndoSort(RectPtrAr &RectangleArray) {
	RectangleArray = RectangleListPreserve;
}
void AreaCover::PerformBlindCoverage(ScanType Scan, VerticalStart VertStart, HorizontalStart HorzStart) {

	{
	CoverRect DummyBuffer;
	GeneratedCoverages.push_back(DummyBuffer);
	}

	for (RectPtrArItr RectItr = GetArea()->Rectangles.begin(); RectItr != GetArea()->Rectangles.end(); ++RectItr) {
		(*RectItr)->ScanX = HorzStart;
		(*RectItr)->ScanY = VertStart;
		(*RectItr)->ScanUsed = Scan;
		GeneratedCoverages.back().RectList.push_back(**RectItr);
	}
}
void AreaCover::PerformBasicCoverage(ScanType S, MovementHeuristic CAlgorithm)
{
	POINT EndCurrent;
	POINT ClosestPoint;
	long RectOrder = 0;
	RectPtrArItr RectItr;
	VerticalStart VertStart;
	HorizontalStart HorzStart;
	RectPtrArItr TheRect;

	EndCurrent.x = EndCurrent.y = 0;

	{
	CoverRect DummyBuffer;
	GeneratedCoverages.push_back(DummyBuffer);
	}

	for (RectItr = GetArea()->Rectangles.begin(); RectItr != GetArea()->Rectangles.end(); ++RectItr) {
		(*RectItr)->Scanned = false;
	}
	
	do {

		for (RectPtrArItr RectItr = GetArea()->Rectangles.begin(); RectItr != GetArea()->Rectangles.end(); ++RectItr) {
			switch (CAlgorithm) {
				case OrderBased:
					(*RectItr)->Rg = ++RectOrder;
					break;
				case VertexBased:
					(*RectItr)->Rg = GeometricObject::Range(EndCurrent, (*RectItr)->FindClosestVertex(EndCurrent));
					break;
				case CenterBased:
					//uses the center of the rectangle (or the midpoint of the diagonal)
					(*RectItr)->Rg = GeometricObject::Range(EndCurrent, (*RectItr)->GetDiagonal().Midpoint());
					break;
				default:
					//should never happen
					break;
			}
		}


		//implement bubble sort of rectangles, the built in list system is not sophisticated enough
		//to do the correct memory addressing
		SortAreaRectangles(GetArea()->Rectangles, true);

		if (MoreToScan(GetArea()->Rectangles, TheRect)) {
			ClosestPoint = (*TheRect)->FindClosestVertex(EndCurrent);

			if (ClosestPoint.y == (*TheRect)->TopLeft().y) {
				VertStart = Top;
			}
			else {
				VertStart = Bottom;
			}
			if (ClosestPoint.x == (*TheRect)->TopLeft().x) {
				HorzStart = Left;
			}
			else {
				HorzStart = Right;
			}

			(*TheRect)->ScanUsed = S;
			(*TheRect)->ScanX = HorzStart;
			(*TheRect)->ScanY = VertStart;
			(*TheRect)->Scanned = true;

			switch (S) {
				case Vertical:
					GeneratedCoverages.back().RectList.push_back(**TheRect);
					EndCurrent = VerticalCoverEnd((*TheRect)->TopLeft(), (*TheRect)->BottomRight(), VertStart, HorzStart);
					break;
				case Horizontal:
					GeneratedCoverages.back().RectList.push_back(**TheRect);
					EndCurrent = HorizontalCoverEnd((*TheRect)->TopLeft(), (*TheRect)->BottomRight(), VertStart, HorzStart);
					break;
				case DiagUp:
					GeneratedCoverages.back().RectList.push_back(**TheRect);
					EndCurrent = DiagonalUpCoverEnd((*TheRect)->TopLeft(), (*TheRect)->BottomRight(), VertStart, HorzStart);
					break;
				case DiagDown:
					GeneratedCoverages.back().RectList.push_back(**TheRect);
					EndCurrent = DiagonalDownCoverEnd((*TheRect)->TopLeft(), (*TheRect)->BottomRight(), VertStart, HorzStart);
					break;
			}
		}

		UndoSort(GetArea()->Rectangles);

	} while (MoreToScan(GetArea()->Rectangles, TheRect));
}
void AreaCover::PerformCoverage(OptimalType Completeness, MovementHeuristic CAlgorithm) {
	//reset values
	FinalValue = 0;
	CandidateValue = 0;
	FinalCandidate.clear();
	ReturnVector.clear();
	OptimalCandidate.clear();
	//OptimalCandidate is always a constant size
	OptimalCandidate.resize(GetArea()->Rectangles.size());
	CurrentRect = OptimalCandidate.begin();
	PreviousRect = OptimalCandidate.begin();
	for (RectPtrArItr RectItr = GetArea()->Rectangles.begin(); RectItr != GetArea()->Rectangles.end(); ++RectItr) {
		(*RectItr)->Scanned = false;
	}
	CurrentOptimization = Completeness;
	CurrentHeuristic = CAlgorithm;
	//begin recursive coverage
	OptimalCover();

	//assemble the final results into something the rest of the class can understand
	CoverRect ReturnBuffer;
	ReturnBuffer.RectList = FinalCandidate;
	ReturnBuffer.EquivalentPoints.clear();

	GeneratedCoverages.push_back(ReturnBuffer);
}
bool AreaCover::MoreToScan(RectPtrAr &RectangleArray, RectPtrArItr &AvaliableRect) {
	for (RectPtrArItr RectItr = RectangleArray.begin(); RectItr != RectangleArray.end(); ++RectItr) {
		if (!((*RectItr)->Scanned)) {
			AvaliableRect = RectItr;
			return true;
		}
	}
	return false;
}
void AreaCover::OptimalCover() {

	//short-circuit the procedure if the working scan is worse than the best known scan
	if ((FinalValue != 0) && CandidateValue >= FinalValue) return;
	
	double RgStorage = 0;
	RectPtrArItr RectLoop = GetArea()->Rectangles.begin();
	MappedRect* EditRect = 0; //actual pointer used to edit things
	bool DoScan = true;
	bool ExitLoop = true;

	do {

		//short-circuit the procedure to prevent rectangles from being scanned twice (consequence of recursiveness)
		if (CurrentOptimization == Complete) {
			ExitLoop = (RectLoop == GetArea()->Rectangles.end()); //exit the loop if the end has been reached
			if (!ExitLoop) {
				//it's ok to proceed, iterators are not equal
				DoScan = !(*RectLoop)->Scanned; //don't do a scan if the rectangle has already been scanned
				if (DoScan) EditRect = *RectLoop; //the rectangle is not scanned, scan it
				++RectLoop; //increment the loop for the next iteration
			}
			else {
				DoScan = false;
			}
		}
		else if (CurrentOptimization == Semi) {
			long RectOrder = 0;

			//figure out what the last coordinate was (otherwise assume (0,0))
			POINT EndCurrent;
			if (CurrentRect == OptimalCandidate.begin()) {
				EndCurrent.x = EndCurrent.y = 0;
			}
			else {
				EndCurrent = GetCoordinateFromScanLocation(**PreviousRect, (*PreviousRect)->ScanX, (*PreviousRect)->ScanY, (*PreviousRect)->ScanUsed, Ending);
			}

			for (RectPtrArCItr RectItr = GetArea()->Rectangles.begin(); RectItr != GetArea()->Rectangles.end(); ++RectItr) {
				switch (CurrentHeuristic) {
					case OrderBased:
						(*RectItr)->Rg = ++RectOrder;
						break;
					case VertexBased:
						(*RectItr)->Rg = GeometricObject::Range(EndCurrent, (*RectItr)->FindClosestVertex(EndCurrent));
						break;
					case CenterBased:
						//uses the center of the rectangle (or the midpoint of the diagonal)
						(*RectItr)->Rg = GeometricObject::Range(EndCurrent, (*RectItr)->GetDiagonal().Midpoint());
						break;
					default:
						VERIFY(FALSE);
						break;
				}
			}
			SortAreaRectangles(GetArea()->Rectangles, true); //sort the rectangles based the the heuristic value that was previously stored
			DoScan = MoreToScan(GetArea()->Rectangles, RectLoop); //find out if we need to do a scan and at the same time find the "current" rectangle
			if (DoScan) EditRect = *RectLoop; //store the current rectangle if it exists
			UndoSort(GetArea()->Rectangles); //undo the sort so the original order is not messed up
			ExitLoop = true; //this system does not loop; that's why it is "semi"
		}
		else {
			VERIFY(FALSE);
		}

		if (DoScan) {

			EditRect->Scanned = true; //we are going to scan the rectangle now
			
			for (int VertexLoop = 0; VertexLoop <= 3; ++VertexLoop) {

				VerticalStart PosVBuffer;
				HorizontalStart PosHBuffer;
				ScanType ScanBuffer;

				switch (VertexLoop) {
					case 0:
						PosVBuffer = Top;
						PosHBuffer = Left;
						break;
					case 1:
						PosVBuffer = Top;
						PosHBuffer = Right;
						break;
					case 2:
						PosVBuffer = Bottom;
						PosHBuffer = Left;
						break;
					case 3:
						PosVBuffer = Bottom;
						PosHBuffer = Right;
						break;
					default:
						VERIFY(FALSE);
						break;
				}

				EditRect->ScanY = PosVBuffer;
				EditRect->ScanX = PosHBuffer;

				for (int ScanLoop = 0; ScanLoop <= 3; ++ScanLoop) {

					switch (ScanLoop) {
						case 0:
							ScanBuffer = Vertical;
							break;
						case 1:
							ScanBuffer = Horizontal;
							break;
						case 2:
						case 3:
							if ((PosVBuffer == Bottom && PosHBuffer == Left) || (PosVBuffer == Top && PosHBuffer == Right)) {
								ScanBuffer = DiagUp;
							}
							else {
								ScanBuffer = DiagDown;
							}
							break;
					}

					EditRect->ScanUsed = ScanBuffer;

					//fill in information about how to scan the rectangle
					RgStorage = 0;
					if (CurrentRect != OptimalCandidate.begin()) {
						RgStorage = Range(GetCoordinateFromScanLocation(**PreviousRect, (*PreviousRect)->ScanX, (*PreviousRect)->ScanY, (*PreviousRect)->ScanUsed, Ending),
										  GetCoordinateFromScanLocation(*EditRect, EditRect->ScanX, EditRect->ScanY, EditRect->ScanUsed, Beginning));

						CandidateValue += RgStorage;
					}

					if (CurrentRect != OptimalCandidate.end()) {
						*CurrentRect = EditRect; //set the iterators equal
						if (CurrentRect != OptimalCandidate.begin()) PreviousRect = CurrentRect; //preserve the current rectangle for pathfinding/editing
						++CurrentRect; //increment the current rectangle in preparation for more calls to OptimalCover
					}

					OptimalCover();

					CandidateValue -= RgStorage;
					if (CurrentRect != OptimalCandidate.begin()) --CurrentRect;
					if (PreviousRect != OptimalCandidate.begin()) --PreviousRect;
				}
			}

			EditRect->Scanned = false;
		}

	} while (!ExitLoop); //if you want it to exit the loop, ExitLoop must be false, so reverse for logic's sake

	if (((CandidateValue < FinalValue) || (FinalValue == 0)) && (CurrentRect == OptimalCandidate.end())) {
		//copy the optimalcandidate into the final candidate

		FinalCandidate.clear();

		for (RectPtrArItr CopyLoop = OptimalCandidate.begin(); CopyLoop != OptimalCandidate.end(); ++CopyLoop) {
			FinalCandidate.push_back(**CopyLoop);
		}

		FinalValue = CandidateValue;
	}	
}
void AreaCover::RectanglesToPoints(CRArItr &Target, bool BlackAndWhite) {

	RectArItr CoverLoop = Target->RectList.begin();
	if (CoverLoop == Target->RectList.end()) return;

	Target->EquivalentPoints.clear();

	for (long XLoop = 0; XLoop <= GetArea()->GetWidth(); ++XLoop) {
		for (long YLoop = 0; YLoop <= GetArea()->GetHeight(); ++YLoop) {
			GetArea()->PointMap(XLoop, YLoop).Hit = false;
		}
	}

	PointAr PointList;

	for (RectArItr CoverPrevLoop = CoverLoop++; CoverPrevLoop != Target->RectList.end(); (CoverLoop == Target->RectList.end()) ? ++CoverPrevLoop : (CoverPrevLoop = CoverLoop++)) {

		COLORREF ColorBuffer = BlackAndWhite ? 0 : RGB(RandomNumber(1, 254), RandomNumber(1, 254), RandomNumber(1, 254));

		PointList.clear();

		switch (CoverPrevLoop->ScanUsed) {
			case Vertical:
				VerticalCover(CoverPrevLoop->TopLeft(), CoverPrevLoop->BottomRight(), &PointList, CoverPrevLoop->ScanY, CoverPrevLoop->ScanX);

				PointListToColorList(&PointList, &(Target->EquivalentPoints), ColorBuffer, false);
				PointList.clear();
				break;
			case Horizontal:
				HorizontalCover(CoverPrevLoop->TopLeft(), CoverPrevLoop->BottomRight(), &PointList, CoverPrevLoop->ScanY, CoverPrevLoop->ScanX);

				PointListToColorList(&PointList, &(Target->EquivalentPoints), ColorBuffer, false);
				PointList.clear();
				break;
			case DiagUp:
				DiagonalUpCover(CoverPrevLoop->TopLeft(), CoverPrevLoop->BottomRight(), &PointList, CoverPrevLoop->ScanY, CoverPrevLoop->ScanX);

				PointListToColorList(&PointList, &(Target->EquivalentPoints), ColorBuffer, false);
				PointList.clear();
				break;
			case DiagDown:
				DiagonalDownCover(CoverPrevLoop->TopLeft(), CoverPrevLoop->BottomRight(), &PointList, CoverPrevLoop->ScanY, CoverPrevLoop->ScanX);

				PointListToColorList(&PointList, &(Target->EquivalentPoints), ColorBuffer, false);
				PointList.clear();
				break;
			default:
				VERIFY(FALSE);
				break;
		}


		if (CoverLoop != Target->RectList.end()) {
			GetArea()->AddSTLArrays(PointList, 
				PathCreator->GeneratePath(
					GetCoordinateFromScanLocation(*CoverPrevLoop, CoverPrevLoop->ScanX, CoverPrevLoop->ScanY, CoverPrevLoop->ScanUsed, Ending), 
					GetCoordinateFromScanLocation(*CoverLoop, CoverLoop->ScanX, CoverLoop->ScanY, CoverLoop->ScanUsed, Beginning)), true);

			PointListToColorList(&PointList, &(Target->EquivalentPoints), 0, true);
		}
	}
}
void AreaCover::Draw(HDC DrawDC, CRArItr DrawDescription, bool BlackAndWhite, bool ForceRecreate, bool DrawPath, bool DrawNumbers) {

	if (DrawDescription->EquivalentPoints.size() == 0 || ForceRecreate) RectanglesToPoints(DrawDescription, BlackAndWhite);

	GeometricObject::ResetDrawCounter();

	//draw the colored points

	for (CPArCItr DrawLoop = DrawDescription->EquivalentPoints.begin(); DrawLoop != DrawDescription->EquivalentPoints.end(); ++DrawLoop) {
		GeometricObject::DrawFilibuster();
		if ((DrawLoop->PathCoord && DrawPath) || !DrawLoop->PathCoord) { 
			SetPixel(DrawDC, DrawLoop->Coords.x, DrawLoop->Coords.y, BlackAndWhite ? 0 : DrawLoop->Color);
		}
	}
	if (DrawNumbers) DrawRectangleNumbers(DrawDC, DrawDescription);
}
void AreaCover::DrawRectangleNumbers(HDC DrawDC, CRArItr DrawDescription) {
	long RectNumber = 1;
	SIZE TextSize;
	long Digits = 0;

	//draw the text labels
	for (RectArCItr RectLoop = DrawDescription->RectList.begin(); RectLoop != DrawDescription->RectList.end(); ++RectLoop) {
		Digits = NumberDigits(RectNumber);
		wchar_t* TextBuffer = new wchar_t[Digits + 1];
		TextBuffer[Digits] = '\0';
		_ltow(RectNumber, TextBuffer, 10);

		GetTextExtentPoint32(DrawDC, TextBuffer, Digits, &TextSize);
		ExtTextOut(DrawDC, (int)((RectLoop->Width() / 2) - (TextSize.cx / 2) + RectLoop->BasicRect.left), (int)((RectLoop->Height() / 2) - (TextSize.cy / 2) + RectLoop->BasicRect.top), 0, 0, TextBuffer, Digits, 0);
		delete [] TextBuffer;
		++RectNumber;
	}
}
void AreaCover::PointListToColorList(PointAr* PtList, CPAr* CList, COLORREF C, bool PathMarker) {

	CoverPoint PointBuffer;
	PointBuffer.Color = C;
	PointBuffer.PathCoord = PathMarker;

	for (PointArCItr PointLoop = PtList->begin(); PointLoop != PtList->end(); ++PointLoop) {
		PointBuffer.Coords = *PointLoop;					
		CList->push_back(PointBuffer);
	}
}
PointAr AreaCover::VerticalCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart) {

	ReturnVector.clear();

	long XAdvance = 0;
	bool NormalDirection = true;

	if (VertStart == Top && HorzStart == Left) {
		for (XAdvance = TopLeft.x; XAdvance <= BottomRight.x; ++XAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(XAdvance, TopLeft.y), CPoint(XAdvance, BottomRight.y), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(XAdvance, BottomRight.y), CPoint(XAdvance, TopLeft.y), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Top && HorzStart == Right) {
		for (XAdvance = BottomRight.x; XAdvance >= TopLeft.x; --XAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(XAdvance, TopLeft.y), CPoint(XAdvance, BottomRight.y), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(XAdvance, BottomRight.y), CPoint(XAdvance, TopLeft.y), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Bottom && HorzStart == Left) {
		for (XAdvance = TopLeft.x; XAdvance <= BottomRight.x; ++XAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(XAdvance, BottomRight.y), CPoint(XAdvance, TopLeft.y), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(XAdvance, TopLeft.y), CPoint(XAdvance, BottomRight.y), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Bottom && HorzStart == Right) {
		for (XAdvance = BottomRight.x; XAdvance >= TopLeft.x; --XAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(XAdvance, BottomRight.y), CPoint(XAdvance, TopLeft.y), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(XAdvance, TopLeft.y), CPoint(XAdvance, BottomRight.y), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}

	if (VectToFill != 0) {
		AddSTLArrays(*VectToFill, ReturnVector, true);
	}

	return ReturnVector;
}
const POINT AreaCover::VerticalCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	MappedRect DataRect(TopLeft, BottomRight);
	return DataRect.GetCoordinateFromLocation(HorzStart, VertStart);
}
const POINT AreaCover::VerticalCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;

	div_t DivisionInfo = div((int)BottomRight.x - (int)TopLeft.x, (int)2);

	if (DivisionInfo.rem != 0) { //the number is odd, use standard configuration
		if (VertStart == Top && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Top && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Bottom && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = TopLeft.y;
		}
		else if (VertStart == Bottom && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = TopLeft.y;
		}
	}
	else {
		if (VertStart == Top && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = TopLeft.y;
		}
		else if (VertStart == Top && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = TopLeft.y;
		}
		else if (VertStart == Bottom && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Bottom && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = BottomRight.y;
		}
	}

	return PointBuffer;
}
PointAr AreaCover::HorizontalCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart) {

	ReturnVector.clear();

	long YAdvance = 0;
	bool NormalDirection = true;

	if (VertStart == Top && HorzStart == Left) {
		for (YAdvance = TopLeft.y; YAdvance <= BottomRight.y; ++YAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(TopLeft.x, YAdvance), CPoint(BottomRight.x, YAdvance), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(BottomRight.x, YAdvance), CPoint(TopLeft.x, YAdvance), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Top && HorzStart == Right) {
		for (YAdvance = TopLeft.y; YAdvance <= BottomRight.y; ++YAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(BottomRight.x, YAdvance), CPoint(TopLeft.x, YAdvance), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(TopLeft.x, YAdvance), CPoint(BottomRight.x, YAdvance), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Bottom && HorzStart == Left) {
		for (YAdvance = BottomRight.y; YAdvance >= TopLeft.y; --YAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(TopLeft.x, YAdvance), CPoint(BottomRight.x, YAdvance), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(BottomRight.x, YAdvance), CPoint(TopLeft.x, YAdvance), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}
	else if (VertStart == Bottom && HorzStart == Right) {
		for (YAdvance = BottomRight.y; YAdvance >= TopLeft.y; --YAdvance) {
			if (NormalDirection) {
				LinePath(CPoint(BottomRight.x, YAdvance), CPoint(TopLeft.x, YAdvance), true, true, &ReturnVector);
			}
			else {
				LinePath(CPoint(TopLeft.x, YAdvance), CPoint(BottomRight.x, YAdvance), true, true, &ReturnVector);
			}
			NormalDirection = !NormalDirection;
		}
	}

	if (VectToFill != 0) {
		AddSTLArrays(*VectToFill, ReturnVector, true);
	}

	return ReturnVector;

}
const POINT AreaCover::HorizontalCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;

	if (VertStart == Top && HorzStart == Left) {
		PointBuffer = TopLeft;
	}
	else if (VertStart == Top && HorzStart == Right) {
		PointBuffer.x = BottomRight.x;
		PointBuffer.y = TopLeft.y;
	}
	else if (VertStart == Bottom && HorzStart == Left) {
		PointBuffer.x = TopLeft.x;
		PointBuffer.y = BottomRight.y;
	}
	else if (VertStart == Bottom && HorzStart == Right) {
		PointBuffer = BottomRight;
	}
	
	return PointBuffer;
}
const POINT AreaCover::HorizontalCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;

	div_t DivisionInfo = div((int)BottomRight.y - (int)TopLeft.y, (int)2);

	if (DivisionInfo.rem != 0) { //a remainder, number is odd, use intuitive configuration
		if (VertStart == Top && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Top && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Bottom && HorzStart == Left) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = TopLeft.y;
		}
		else if (VertStart == Bottom && HorzStart == Right) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = TopLeft.y;
		}
	}
	else if (DivisionInfo.rem == 0) { //a remainder, number is even, use non-intuitive configuration (changed X coordinates)
		if (VertStart == Top && HorzStart == Left) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Top && HorzStart == Right) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = BottomRight.y;
		}
		else if (VertStart == Bottom && HorzStart == Left) {
			PointBuffer.x = TopLeft.x;
			PointBuffer.y = TopLeft.y;
		}
		else if (VertStart == Bottom && HorzStart == Right) {
			PointBuffer.x = BottomRight.x;
			PointBuffer.y = TopLeft.y;
		}
	}

	return PointBuffer;
}
PointAr AreaCover::DiagonalUpCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart) {

	//Scan from top left to bottom right
	ReturnVector.clear();

	bool XMax = false;
	bool YMax = false;
	bool ExitLoop = false;
	bool NormalDirection = true;

	long XAdvance = 0;
	long YAdvance = 0;

	POINT TopPtBuf = {0, 0};
	POINT BottomPtBuf = {0, 0};


	do {

		//Make sure directions have changed AND that they've reached the end of those changed directions
		ExitLoop = YMax && XMax && (XAdvance == (abs(BottomRight.y - TopLeft.y) + 1)) && (YAdvance == (abs(BottomRight.x - TopLeft.x) + 1));

		if (!ExitLoop) {

			if (VertStart == Top) {

				if (!YMax) {
					TopPtBuf.x = TopLeft.x;
					TopPtBuf.y = BottomRight.y - YAdvance;
				}
				else {
					TopPtBuf.x = TopLeft.x + YAdvance;
					TopPtBuf.y = TopLeft.y;
				}

				if (!XMax) {
					BottomPtBuf.x = TopLeft.x + XAdvance;
					BottomPtBuf.y = BottomRight.y;
				}
				else {
					BottomPtBuf.x = BottomRight.x;
					BottomPtBuf.y = BottomRight.y - XAdvance;
				}

			}
			else if (VertStart == Bottom) {

				if (!XMax) {
					TopPtBuf.x = BottomRight.x - XAdvance;
					TopPtBuf.y = TopLeft.y;
				}
				else {
					TopPtBuf.x = TopLeft.x;
					TopPtBuf.y = TopLeft.y + XAdvance;
				}

				if (!YMax) {
					BottomPtBuf.x = BottomRight.x;
					BottomPtBuf.y = TopLeft.y + YAdvance;
				}
				else {
					BottomPtBuf.x = BottomRight.x - YAdvance;
					BottomPtBuf.y = BottomRight.y;
				}

			}

			if (NormalDirection) {
				LinePath(TopPtBuf, BottomPtBuf, true, true, &ReturnVector);
			}
			else {
				LinePath(BottomPtBuf, TopPtBuf, true, true, &ReturnVector);
			}

			NormalDirection = !NormalDirection;

			++XAdvance;
			++YAdvance;
		}	

		if (VertStart == Top) {

			if ((XAdvance + TopLeft.x) > BottomRight.x && !XMax) {
				XMax = true;
				XAdvance = 1;
			}
			if ((YAdvance + TopLeft.y) > BottomRight.y && !YMax) {
				YMax = true;
				YAdvance = 1;
			}

		}
		else if (VertStart == Bottom) {
			
			if ((BottomRight.x - XAdvance) < TopLeft.x && !XMax) {
				XMax = true;
				XAdvance = 1;
			}
			if ((TopLeft.y + YAdvance) > BottomRight.y && !YMax) {
				YMax = true;
				YAdvance = 1;
			}
		}

	} while (!ExitLoop);

	if (VectToFill != 0) {
		AddSTLArrays(*VectToFill, ReturnVector, true);
	}

	return ReturnVector;
}

//the diagonal scans always start and end at the same positions, it only matters for the horizontal/vertical scans

const POINT AreaCover::DiagonalUpCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;
	if (VertStart == Top) {
		PointBuffer.x = TopLeft.x;
		PointBuffer.y = BottomRight.y;
	}
	else if (VertStart == Bottom) {
		PointBuffer.x = BottomRight.x;
		PointBuffer.y = TopLeft.y;
	}

	return PointBuffer;
}
const POINT AreaCover::DiagonalUpCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;
	if (VertStart == Top) {
		PointBuffer.x = BottomRight.x;
		PointBuffer.y = TopLeft.y;
	}
	else if (VertStart == Bottom) {
		PointBuffer.x = TopLeft.x;
		PointBuffer.y = BottomRight.y;
	}

	return PointBuffer;
}
PointAr AreaCover::DiagonalDownCover(POINT TopLeft, POINT BottomRight, PointAr* VectToFill, VerticalStart VertStart, HorizontalStart HorzStart) {

	//scan from bottom left to top right
	ReturnVector.clear();
	bool XMax = false;
	bool YMax = false;
	bool ExitLoop = false;
	bool NormalDirection = true;

	long XAdvance = 0;
	long YAdvance = 0;

	POINT TopPtBuf = {0, 0};
	POINT BottomPtBuf = {0, 0};

	do {

		//Make sure directions have changed AND that they've reached the end of those changed directions
		ExitLoop = YMax && XMax && (XAdvance == (abs(BottomRight.y - TopLeft.y) + 1)) && (YAdvance == (abs(BottomRight.x - TopLeft.x) + 1));

		if (!ExitLoop) {

			if (VertStart == Top) {
				if (!XMax) {
					TopPtBuf.x = TopLeft.x + XAdvance;
					TopPtBuf.y = TopLeft.y;
				}
				else {
					TopPtBuf.x = BottomRight.x;
					TopPtBuf.y = TopLeft.y + XAdvance;
				}

				if (!YMax) {
					BottomPtBuf.x = TopLeft.x;
					BottomPtBuf.y = TopLeft.y + YAdvance;
				}
				else {
					BottomPtBuf.x = TopLeft.x + YAdvance;
					BottomPtBuf.y = BottomRight.y;
				}

			}
			else if (VertStart == Bottom) {

				if (!YMax) {
					TopPtBuf.x = BottomRight.x;
					TopPtBuf.y = BottomRight.y - YAdvance;
				}
				else {
					TopPtBuf.x = BottomRight.x - YAdvance;
					TopPtBuf.y = TopLeft.y;
				}

				if (!XMax) {
					BottomPtBuf.x = BottomRight.x - XAdvance;
					BottomPtBuf.y = BottomRight.y;
				}
				else {
					BottomPtBuf.x = TopLeft.x;
					BottomPtBuf.y = BottomRight.y - XAdvance;
				}

			}

			if (NormalDirection) {
				LinePath(TopPtBuf, BottomPtBuf, true, true, &ReturnVector);
			}
			else {
				LinePath(BottomPtBuf, TopPtBuf, true, true, &ReturnVector);
			}

			NormalDirection = !NormalDirection;

			++XAdvance;
			++YAdvance;
		}	

		if (VertStart == Top) {
			if ((XAdvance + TopLeft.x) > BottomRight.x && !XMax) {
				XMax = true;
				XAdvance = 1;
			}
			if ((YAdvance + TopLeft.y) > BottomRight.y && !YMax) {
				YMax = true;
				YAdvance = 1;
			}
		}
		else if (VertStart == Bottom) {
			if ((BottomRight.x - XAdvance) < TopLeft.x && !XMax) {
				XMax = true;
				XAdvance = 1;
			}
			if ((BottomRight.y - YAdvance) < TopLeft.y && !YMax) {
				YMax = true;
				YAdvance = 1;
			}
		}

	} while (!ExitLoop);

	if (VectToFill != 0) {
		AddSTLArrays(*VectToFill, ReturnVector, true);
	}

	return ReturnVector;
}
const POINT AreaCover::DiagonalDownCoverBegin(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;
	if (VertStart == Top) {
		PointBuffer.x = TopLeft.x;
		PointBuffer.y = TopLeft.y;
	}
	else if (VertStart == Bottom) {
		PointBuffer.x = BottomRight.x;
		PointBuffer.y = BottomRight.y;
	}

	return PointBuffer;
}
const POINT AreaCover::DiagonalDownCoverEnd(POINT TopLeft, POINT BottomRight, VerticalStart VertStart, HorizontalStart HorzStart) const {
	POINT PointBuffer;
	if (VertStart == Top) {
		PointBuffer.x = BottomRight.x;
		PointBuffer.y = BottomRight.y;
	}
	else if (VertStart == Bottom) {
		PointBuffer.x = TopLeft.x;
		PointBuffer.y = TopLeft.y;
	}

	return PointBuffer;
}
void AreaCover::ReadData(FileStream& File) {

}
void AreaCover::WriteData(FileStream& File) const {

}