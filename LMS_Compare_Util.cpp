#include "LMS_Compare_Util.h"

bool LMS_Compare_Util::checkGeographicInformation(const char* Image_File_Name_1, const char* Image_File_Name_2, const char* Tfw_File_Name_1, const char* Tfw_File_Name_2) {
	return true;
}

bool LMS_Compare_Util::readTiffInformation(const char* fileName, LMS_DataInformation* imageDataInformation,bool readGeoInformation) {
	//========获取图像数据=======================================
	GDALAllRegister();//注册GDAL
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
	GDALDataset* pDataset = (GDALDataset*)GDALOpen(fileName, GA_ReadOnly);//利用GDALOopen打开文件
	if (pDataset == nullptr) //如果文件不存在则返回1
	{
		return false;
	}
	imageDataInformation->bandCount = pDataset->GetRasterCount();//获取波段数量
	GDALRasterBand* TempBand = pDataset->GetRasterBand(1);
	imageDataInformation->DataType = TempBand->GetRasterDataType();//获取数据存储格式
	imageDataInformation->imageWidth = pDataset->GetRasterXSize();//获取图像宽度
	imageDataInformation->imageHeight = pDataset->GetRasterYSize();//获取图像高度
	if (readGeoInformation) {
		double transformParamter[6];
		if (pDataset->GetGeoTransform(transformParamter) == CE_Failure) {
			return false;
		}
		imageDataInformation->_UpperLeftPointX = transformParamter[0];
		imageDataInformation->deltaX = transformParamter[0];
		imageDataInformation->rotationX = transformParamter[0];
		imageDataInformation->_UpperLeftPointY = transformParamter[0];
		imageDataInformation->rotationY = transformParamter[0];
		imageDataInformation->deltaY = transformParamter[0];
	}
	return true;
}

unsigned char* LMS_Compare_Util::readTiffImage(const char* fileName, QRect range, int PictureWidth, int PictureHeight) {
	GDALAllRegister();//注册GDAL
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
	GDALDataset* pDataset = (GDALDataset*)GDALOpen(fileName, GA_ReadOnly);//利用GDALOopen打开文件
	if (pDataset == nullptr)
	{
		return nullptr;
	}
	int pBandMap[3] = { 1,2,3 }; //读取的波段位置
	int bandCount = pDataset->GetRasterCount();//获取波段数量
	GDALRasterBand* TempBand = pDataset->GetRasterBand(1);
	GDALDataType DataType = TempBand->GetRasterDataType();//获取数据存储格式
	unsigned char* AllBand = new unsigned char[PictureWidth * PictureHeight * 3]; //设置数组用于存储要读取的图像信息
	pDataset->RasterIO(GF_Read, range.topLeft().x(), range.topLeft().y(), range.width(), range.height(), AllBand, PictureWidth, PictureHeight, DataType, 3, pBandMap, 1 * 3, 1 * 3 * PictureWidth , 1);
	//利用GDALDataset::RasterIO读取图像的所需波段的信息并存入数组中
	return AllBand;
}

void LMS_Compare_Util::getImageArea(QRectF sceneRect, LMS_DataInformation* imageDataInformation_1, readArea* readarea, double scale, int value) {
	double imageUpperLeftX = imageDataInformation_1->sceneStartX + value * 0.01 * imageDataInformation_1->sceneWidth;
	double imageUpperLeftY = imageDataInformation_1->sceneStartY;
	double imageBottomRightX = imageDataInformation_1->sceneStartX + imageDataInformation_1->sceneWidth;
	double imageBottomRightY = imageDataInformation_1->sceneStartY + imageDataInformation_1->sceneHeight;
	double sceneUpperLeftX = sceneRect.topLeft().x();
	double sceneUpperLeftY = sceneRect.topLeft().y();
	double sceneWidth = sceneRect.width();
	double sceneHeight = sceneRect.height();
	double sceneBottomRightX = sceneUpperLeftX + sceneWidth;
	double sceneBottomRightY = sceneUpperLeftY + sceneHeight;
	if (imageBottomRightX <= sceneUpperLeftX || imageBottomRightY <= sceneUpperLeftY || sceneBottomRightX <= imageUpperLeftX || sceneBottomRightY <= imageUpperLeftY) {
		readarea->exist = false;
		return;
	}
	if (imageUpperLeftX < sceneUpperLeftX)
	{
		if (imageUpperLeftY < sceneUpperLeftY)
		{
			if (imageBottomRightX < sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, sceneUpperLeftY, imageBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, sceneUpperLeftY, imageBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
			else if (imageBottomRightX >= sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, sceneUpperLeftY, sceneBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, sceneUpperLeftY, sceneBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
		}
		else if (imageUpperLeftY > sceneUpperLeftY)
		{
			if (imageBottomRightX < sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, imageUpperLeftY, imageBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, imageUpperLeftY, imageBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
			else if (imageBottomRightX >= sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, imageUpperLeftY, sceneBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(sceneUpperLeftX, imageUpperLeftY, sceneBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
		}
	}
	else if (imageUpperLeftX >= sceneUpperLeftX)
	{
		if (imageUpperLeftY < sceneUpperLeftY)
		{
			if (imageBottomRightX < sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(imageUpperLeftX, sceneUpperLeftY, imageBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(imageUpperLeftX, sceneUpperLeftY, imageBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
			else if (imageBottomRightX >= sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(imageUpperLeftX, sceneUpperLeftY, sceneBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(imageUpperLeftX, sceneUpperLeftY, sceneBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
		}
		else if (imageUpperLeftY >= sceneUpperLeftY)
		{
			if (imageBottomRightX < sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(imageUpperLeftX, imageUpperLeftY, imageBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(imageUpperLeftX, imageUpperLeftY, imageBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
			else if (imageBottomRightX >= sceneBottomRightX)
			{
				if (imageBottomRightY < sceneBottomRightY)
				{
					setArea(imageUpperLeftX, imageUpperLeftY, sceneBottomRightX, imageBottomRightY, scale, readarea);
				}
				else if (imageBottomRightY >= sceneBottomRightY)
				{
					setArea(imageUpperLeftX, imageUpperLeftY, sceneBottomRightX, sceneBottomRightY, scale, readarea);
				}
			}
		}
	}
}

void LMS_Compare_Util::setArea(double _UpperLeftX, double _UpperLeftY, double _BottomRightX, double _BottomRightY, double scale, readArea* readarea) {
	readarea->_UpperLeftX = _UpperLeftX;
	readarea->_UpperLeftY = _UpperLeftY;
	readarea->_BottomRightX = _BottomRightX;
	readarea->_BottomRightY = _BottomRightY;
	readarea->exist = true;
}