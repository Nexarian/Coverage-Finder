/////////// Header for general graphics functions ///////////////////
#ifndef COMMON_GRAPHICS_INCLUDED
#define COMMON_GRAPHICS_INCLUDED

struct Color {
	int Red;
	int Green;
	int Blue;
};
extern const MaxColor;
Color RGBValues(long Color);
long InverseColor(long Color);
long GrayScaleColor(long Color);

enum BitmapRotation {
	VerticalFlip,
	HorizontalFlip,
	RotateLeft,
	RotateRight
};

HBITMAP RotateImage(HBITMAP SourceHandle, BitmapRotation Rotation);
HBITMAP CopyBitmap(HBITMAP BitmapToCopy);

#endif