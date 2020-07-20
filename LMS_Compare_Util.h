#pragma once
#include <gdal_priv.h>
#include <qrect.h>
#include <qstring>
struct LMS_DataInformation {
	QString imageFileName;
	QString tfwFileName;
	double deltaX;
	double deltaY;
	double rotationX;
	double rotationY;
	double _UpperLeftPointX;
	double _UpperLeftPointY;
	int DataType = 0;
	int imageWidth = 0;
	int imageHeight = 0;
	int bandCount = 0;

	int sceneStartX;
	int sceneStartY;
	int sceneWidth;
	int sceneHeight;
};

struct LMS_PaintInformation {
	int startX;
	int startY;
	int PictureWidth;
	int PictureHeight;
};

struct crossArea {
	double leftPos;
	double rightPos;
};

struct readArea {
	int _UpperLeftX = 0;
	int _UpperLeftY = 0;
	int _BottomRightX = 0;
	int _BottomRightY = 0;
	bool exist = false;
};

class LMS_Compare_Util
{
public:
	void setArea(double _UpperLeftX, double _UpperLeftY, double _BottomRightX, double _BottomRightY, double scale, readArea* readarea);
	void getImageArea(QRectF sceneRect, LMS_DataInformation* imageDataInformation_1, readArea* readarea, double scale, int value);
	bool readTiffInformation(const char* fileName, LMS_DataInformation* imageDataInformation, bool readGeoInformation);
	bool checkGeographicInformation(const char* Image_File_Name_1, const char* Image_File_Name_2, const char* Tfw_File_Name_1, const char* Tfw_File_Name_2);
	unsigned char* readTiffImage(const char* fileName, QRect range, int width, int height);
};

