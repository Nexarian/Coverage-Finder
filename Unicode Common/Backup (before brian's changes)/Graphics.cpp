#include "StdAfx.h"
#include "Graphics.h"

const int MaxColor = 255;
//splits a RGB color into the Red, Green, and Blue color components
Color RGBValues(long DestColor) {
    Color ReturnColor;
    ReturnColor.Red = DestColor & 255;
    ReturnColor.Green = (DestColor & 65535) / 256;
    ReturnColor.Blue = DestColor / 65536;
    return ReturnColor;
}
//returns the inverse of a standard RGB color
long InverseColor(long DestColor) {
	Color ColorValues = RGBValues(DestColor);
	ColorValues.Red = MaxColor - ColorValues.Red;
	ColorValues.Green = MaxColor - ColorValues.Green;
	ColorValues.Blue = MaxColor - ColorValues.Blue;
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