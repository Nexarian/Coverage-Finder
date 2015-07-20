/////////// Header for general graphics functions ///////////////////
#ifndef COMMON_GRAPHICS_INCLUDED
#define COMMON_GRAPHICS_INCLUDED

#include <vector>

struct Color {
	int Red;
	int Green;
	int Blue;
};
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

double GetTrigXCoordDeg(double Degree, double Radius, double Offset);
double GetTrigYCoordDeg(double Degree, double Radius, double Offset);
double GetTrigXCoordRad(double Radian, double Radius, double Offset);
double GetTrigYCoordRad(double Radian, double Radius, double Offset);

std::vector<POINT> GetRegularPolygonPoints(unsigned int NumSides, double Radius, const POINT& Center);

double Range2D(const POINT& Point1, const POINT& Point2);

class CommonGDIGraphics {
	public:
		CommonGDIGraphics();
		virtual ~CommonGDIGraphics();

		static const HBRUSH GetBlueBrush();
		static const HBRUSH GetGreenBrush();
		static const HBRUSH GetRedBrush();
		static const HBRUSH GetYellowBrush();
		static const HBRUSH GetWhiteBrush();
		static const HBRUSH GetBlackBrush();

		static const HPEN GetBluePen();
		static const HPEN GetGreenPen();
		static const HPEN GetRedPen();
		static const HPEN GetYellowPen();
		static const HPEN GetWhitePen();
		static const HPEN GetBlackPen();

	private:

		static HBRUSH BlueBrush;
		static HBRUSH GreenBrush;
		static HBRUSH RedBrush;
		static HBRUSH YellowBrush;
		static HBRUSH WhiteBrush;
		static HBRUSH BlackBrush;

		static HPEN BluePen;
		static HPEN GreenPen;
		static HPEN RedPen;
		static HPEN YellowPen;
		static HPEN WhitePen;
		static HPEN BlackPen;

		static CommonGDIGraphics ObjectAllocator;
};

#endif