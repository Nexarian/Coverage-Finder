#include "StdAfx.h"
#include "Graphics.h"
#include "Main.h"
#include <limits.h>

//splits a RGB color into the Red, Green, and Blue color components
Color RGBValues(long DestColor) {
    Color ReturnColor;
    ReturnColor.Red = DestColor & UCHAR_MAX;
    ReturnColor.Green = (DestColor & USHRT_MAX) / (UCHAR_MAX + 1);
    ReturnColor.Blue = DestColor / (USHRT_MAX + 1);
    return ReturnColor;
}
//returns the inverse of a standard RGB color
long InverseColor(long DestColor) {
	Color ColorValues = RGBValues(DestColor);
	ColorValues.Red = UCHAR_MAX - ColorValues.Red;
	ColorValues.Green = UCHAR_MAX - ColorValues.Green;
	ColorValues.Blue = UCHAR_MAX - ColorValues.Blue;
	return RGB(ColorValues.Red, ColorValues.Green, ColorValues.Blue);
}
//returns the Gray scale color of a RGB color
long GrayScaleColor(long DestColor) {
	Color ColorValues = RGBValues(DestColor);
	int ColorAverage = (ColorValues.Red + ColorValues.Green + ColorValues.Blue) / 3;
	ColorValues.Red = ColorAverage;
	ColorValues.Green = ColorAverage;
	ColorValues.Blue = ColorAverage;
	return RGB(ColorValues.Red, ColorValues.Green, ColorValues.Blue);
}

HBITMAP RotateImage(HBITMAP SourceHandle, BitmapRotation Rotation) {
	//Load the source bitmap into the DC
	HDC SourceDC = CreateCompatibleDC(NULL);
	HBITMAP OldSource = (HBITMAP)SelectObject(SourceDC, (HGDIOBJ)SourceHandle);
	//Get the description of the source bitmap
	BITMAP SourceDescription;
	GetObject((HGDIOBJ)SourceHandle, sizeof(SourceDescription), &SourceDescription);
	
	//Create the destination bitmap
	HBITMAP DestBitmap;

	if (Rotation == VerticalFlip || Rotation == HorizontalFlip) { //Create the dest bitmap with the same dimensions (vertical or horizontal flip)
		DestBitmap = CreateBitmap(SourceDescription.bmWidth, SourceDescription.bmHeight, 1, SourceDescription.bmBitsPixel, NULL); 
	}
	else if (Rotation == RotateLeft || Rotation == RotateRight) { //Create the source bitmap with flipped dimensions
		DestBitmap = CreateBitmap(SourceDescription.bmHeight, SourceDescription.bmWidth, 1, SourceDescription.bmBitsPixel, NULL);
	}
	//Create the destination DC
	HDC DestDC = CreateCompatibleDC(NULL);
	HBITMAP OldDest = (HBITMAP)SelectObject(DestDC, (HGDIOBJ)DestBitmap);
	
	//Loop through and rotate the bitmap
	COLORREF SourceColor; //Color buffer

	for (int XLoop = 0; XLoop <= SourceDescription.bmWidth; ++XLoop) {
		for (int YLoop = 0; YLoop <= SourceDescription.bmHeight; ++YLoop) {
			SourceColor = GetPixel(SourceDC, XLoop, YLoop);
			switch (Rotation) {
				case VerticalFlip:
					SetPixel(DestDC, XLoop, (SourceDescription.bmHeight - YLoop) - 1, SourceColor);
					break;
				case HorizontalFlip:
					SetPixel(DestDC, (SourceDescription.bmWidth - XLoop) - 1, YLoop, SourceColor);
					break;
				case RotateLeft:
					SetPixel(DestDC, YLoop, (SourceDescription.bmWidth - XLoop) - 1, SourceColor);
					break;
				case RotateRight:
					SetPixel(DestDC, (SourceDescription.bmHeight - YLoop) - 1, XLoop, SourceColor);
					break;
			}
		}
	}
	//Re-select the old DCs
	SelectObject(SourceDC, (HGDIOBJ)OldSource);
	SelectObject(DestDC, (HGDIOBJ)OldDest);
	//Delete the DCs
	DeleteDC(SourceDC);
	DeleteDC(DestDC);

	return DestBitmap;
}

HBITMAP CopyBitmap(HBITMAP BitmapToCopy) {

	HDC SourceDC = CreateCompatibleDC(NULL);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(SourceDC, (HGDIOBJ)BitmapToCopy);

	BITMAP SourceDescrip;
	GetObject((HGDIOBJ)BitmapToCopy, sizeof(SourceDescrip), &SourceDescrip);

	HDC ReturnDC = CreateCompatibleDC(NULL);
	HBITMAP ReturnBitmap = CreateBitmap(SourceDescrip.bmWidth, SourceDescrip.bmHeight, 1, SourceDescrip.bmBitsPixel, NULL);

	HBITMAP OldReturnBitmap = (HBITMAP)SelectObject(ReturnDC, (HGDIOBJ)ReturnBitmap);

	BitBlt(ReturnDC, 0, 0, SourceDescrip.bmWidth, SourceDescrip.bmHeight, SourceDC, 0, 0, SRCCOPY);

	SelectObject(SourceDC, (HGDIOBJ)OldBitmap);
	SelectObject(ReturnDC, (HGDIOBJ)OldReturnBitmap);

	DeleteDC(SourceDC);
	DeleteDC(ReturnDC);

	return ReturnBitmap;
}


double GetTrigXCoordDeg(double Degree, double Radius, double Offset) {
	return GetTrigXCoordRad(DegreesToRadians(Degree), Radius, Offset);
}
double GetTrigYCoordDeg(double Degree, double Radius, double Offset) {
	return GetTrigYCoordRad(DegreesToRadians(Degree), Radius, Offset);
}
double GetTrigXCoordRad(double Radian, double Radius, double Offset) {
	return cos(Radian) * Radius + Offset;
}
double GetTrigYCoordRad(double Radian, double Radius, double Offset) {
	return sin(Radian) * Radius + Offset;
}

std::vector<POINT> GetRegularPolygonPoints(unsigned int NumSides, double Radius, const POINT& Center) {
	/*Here's the explaination of (720 - (180 * (NumSides% - 2)) / (NumSides% * 2))
    This procedure was written to draw a regular hexagon with a base parallel to the base of the screen
    Adding the above sum to the degree measure does this for all regular polygons
    720 is the sum of the degree measures of any hexagon
    180 * (NumSides% - 2) is the sum of the degree measures for any polygon
    Taking the difference of these two determines how much of an offset needs to be performed
    Then divide it by the number of sides so we rotate it just enough for one side (multiply by 2 to chop this in half so we don't get a swing effect)

    (720 - (180 * (NumSides% - 2)) / (NumSides% * 2))
    This uses the simplified version to expedite the calculations*/
	
	std::vector<POINT> ReturnBuffer;
	if (Radius < 0) return ReturnBuffer; //it makes no sense to have a radius of less than 0!

	double FudgeConstant = ((90 * (6 - (double)NumSides)) / (double)NumSides);
	double DegreeMeasure;
	double CentralDegree = 360.0 / NumSides;
	POINT PointBuffer;

	for (unsigned int PolygonLoop = 0; PolygonLoop < NumSides; ++PolygonLoop) {

		//Get the degree where the point is actually supposed to be
		DegreeMeasure = PolygonLoop * -CentralDegree - FudgeConstant;

		PointBuffer.x = int(ceil(GetTrigXCoordDeg(DegreeMeasure, Radius, Center.x)));
		PointBuffer.y = int(ceil(GetTrigYCoordDeg(DegreeMeasure, Radius, Center.y)));

		ReturnBuffer.push_back(PointBuffer);
	}

	return ReturnBuffer;
}

double Range2D(const POINT& Point1, const POINT& Point2) {
	return sqrt(pow(double(Point1.x - Point2.x), 2.0) + pow(double(Point1.y - Point2.y), 2.0));
}

HBRUSH CommonGDIGraphics::BlueBrush = 0;
HBRUSH CommonGDIGraphics::GreenBrush = 0;
HBRUSH CommonGDIGraphics::RedBrush = 0;
HBRUSH CommonGDIGraphics::YellowBrush = 0;
HBRUSH CommonGDIGraphics::WhiteBrush = 0;
HBRUSH CommonGDIGraphics::BlackBrush = 0;
HPEN CommonGDIGraphics::BluePen = 0;
HPEN CommonGDIGraphics::GreenPen = 0;
HPEN CommonGDIGraphics::RedPen = 0;
HPEN CommonGDIGraphics::YellowPen = 0;
HPEN CommonGDIGraphics::WhitePen = 0;
HPEN CommonGDIGraphics::BlackPen = 0;
#define CommonGDIGraphicsObjs 12

CommonGDIGraphics::CommonGDIGraphics() {

}
CommonGDIGraphics::~CommonGDIGraphics() {
	HGDIOBJ ObjPtr = 0;
	for (int DeallocateLoop = 0; DeallocateLoop < CommonGDIGraphicsObjs; ++DeallocateLoop) {
		switch (DeallocateLoop) {
			case 0:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::BlueBrush);
				break;
			case 1:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::GreenBrush);
				break;
			case 2:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::RedBrush);
				break;
			case 3:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::YellowBrush);
				break;
			case 4:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::WhiteBrush);
				break;
			case 5:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::BlackBrush);
				break;
			case 6:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::BluePen);
				break;
			case 7:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::GreenPen);
				break;
			case 8:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::RedPen);
				break;
			case 9:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::YellowPen);
				break;
			case 10:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::WhitePen);
				break;
			case 11:
				ObjPtr = ((HGDIOBJ)CommonGDIGraphics::BlackPen);
				break;
			default:
				continue;
		}
		if (ObjPtr != 0) {
			DeleteObject(ObjPtr);
			ObjPtr = 0;
		}
	}
}

const HBRUSH CommonGDIGraphics::GetBlueBrush() {
	if (CommonGDIGraphics::BlueBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(0, 0, UCHAR_MAX), 0};
		CommonGDIGraphics::BlueBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::BlueBrush;
}
const HBRUSH CommonGDIGraphics::GetGreenBrush() {
	if (CommonGDIGraphics::GreenBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(0, UCHAR_MAX, 0), 0};
		CommonGDIGraphics::GreenBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::GreenBrush;
}
const HBRUSH CommonGDIGraphics::GetRedBrush() {
	if (CommonGDIGraphics::RedBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(UCHAR_MAX, 0, 0), 0};
		CommonGDIGraphics::RedBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::RedBrush;
}
const HBRUSH CommonGDIGraphics::GetYellowBrush() {
	if (CommonGDIGraphics::YellowBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(UCHAR_MAX, UCHAR_MAX, 0), 0};
		CommonGDIGraphics::YellowBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::YellowBrush;
}
const HBRUSH CommonGDIGraphics::GetWhiteBrush() {
	if (CommonGDIGraphics::WhiteBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX), 0};
		CommonGDIGraphics::WhiteBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::WhiteBrush;
}
const HBRUSH CommonGDIGraphics::GetBlackBrush() {
	if (CommonGDIGraphics::BlackBrush == 0) {
		LOGBRUSH BrushStruct = {BS_SOLID, RGB(0, 0, 0), 0};
		CommonGDIGraphics::BlackBrush = ::CreateBrushIndirect(&BrushStruct);
	}
	return CommonGDIGraphics::BlackBrush;
}

const HPEN CommonGDIGraphics::GetBluePen() {
	if (CommonGDIGraphics::BluePen == 0) {
		CommonGDIGraphics::BluePen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, UCHAR_MAX));
	}
	return CommonGDIGraphics::BluePen;
}

const HPEN CommonGDIGraphics::GetGreenPen() {
	if (CommonGDIGraphics::GreenPen == 0) {
		CommonGDIGraphics::GreenPen = ::CreatePen(PS_SOLID, 1, RGB(0, UCHAR_MAX, 0));
	}
	return CommonGDIGraphics::GreenPen;
}

const HPEN CommonGDIGraphics::GetRedPen() {
	if (CommonGDIGraphics::RedPen == 0) {
		CommonGDIGraphics::RedPen = ::CreatePen(PS_SOLID, 1, RGB(UCHAR_MAX, 0, 0));
	}
	return CommonGDIGraphics::RedPen;
}

const HPEN CommonGDIGraphics::GetYellowPen() {
	if (CommonGDIGraphics::YellowPen == 0) {
		CommonGDIGraphics::YellowPen = ::CreatePen(PS_SOLID, 1, RGB(UCHAR_MAX, UCHAR_MAX, 0));
	}
	return CommonGDIGraphics::YellowPen;
}

const HPEN CommonGDIGraphics::GetWhitePen() {
	if (CommonGDIGraphics::WhitePen == 0) {
		CommonGDIGraphics::WhitePen = ::CreatePen(PS_SOLID, 1, RGB(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX));
	}
	return CommonGDIGraphics::WhitePen;
}

const HPEN CommonGDIGraphics::GetBlackPen() {
	if (CommonGDIGraphics::BlackPen == 0) {
		CommonGDIGraphics::BlackPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	}
	return CommonGDIGraphics::BlackPen;
}