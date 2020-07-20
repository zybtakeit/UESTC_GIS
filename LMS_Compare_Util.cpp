#include "LMS_Compare_Util.h"

bool LMS_Compare_Util::checkGeographicInformation(const char* Image_File_Name_1, const char* Image_File_Name_2, const char* Tfw_File_Name_1, const char* Tfw_File_Name_2) {
	return true;
}

bool LMS_Compare_Util::readTiffInformation(const char* fileName, LMS_DataInformation* imageDataInformation,bool readGeoInformation) {
	//========��ȡͼ������=======================================
	GDALAllRegister();//ע��GDAL
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
	GDALDataset* pDataset = (GDALDataset*)GDALOpen(fileName, GA_ReadOnly);//����GDALOopen���ļ�
	if (pDataset == nullptr) //����ļ��������򷵻�1
	{
		return false;
	}
	imageDataInformation->bandCount = pDataset->GetRasterCount();//��ȡ��������
	GDALRasterBand* TempBand = pDataset->GetRasterBand(1);
	imageDataInformation->DataType = TempBand->GetRasterDataType();//��ȡ���ݴ洢��ʽ
	imageDataInformation->imageWidth = pDataset->GetRasterXSize();//��ȡͼ����
	imageDataInformation->imageHeight = pDataset->GetRasterYSize();//��ȡͼ��߶�
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
	GDALAllRegister();//ע��GDAL
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
	GDALDataset* pDataset = (GDALDataset*)GDALOpen(fileName, GA_ReadOnly);//����GDALOopen���ļ�
	if (pDataset == nullptr)
	{
		return nullptr;
	}
	int pBandMap[3] = { 1,2,3 }; //��ȡ�Ĳ���λ��
	int bandCount = pDataset->GetRasterCount();//��ȡ��������
	GDALRasterBand* TempBand = pDataset->GetRasterBand(1);
	GDALDataType DataType = TempBand->GetRasterDataType();//��ȡ���ݴ洢��ʽ
	unsigned char* AllBand = new unsigned char[PictureWidth * PictureHeight * 3]; //�����������ڴ洢Ҫ��ȡ��ͼ����Ϣ
	pDataset->RasterIO(GF_Read, range.topLeft().x(), range.topLeft().y(), range.width(), range.height(), AllBand, PictureWidth, PictureHeight, DataType, 3, pBandMap, 1 * 3, 1 * 3 * PictureWidth , 1);
	//����GDALDataset::RasterIO��ȡͼ������貨�ε���Ϣ������������
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