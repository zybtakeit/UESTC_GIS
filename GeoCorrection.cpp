#include "GeoCorrection.h"

//int GeoCorrection::RPC_Correction(const char* pszSrcFile, const char* pszDstFile, GDALRPCInfo* psRPC, double dfPixErrThreshold, char** papszOptions, GDALRIOResampleAlg eResampleMethod, const char* pszFormat, GDALProgressFunc* pProcess) {
//	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
//	if (pProcess != NULL) {
//		process
//			pProcess->
//	}
//	GDALAllRegister();
//	//��ԭʼͼ�񲢻�ȡͼ�������Ϣ
//	GDALDatasetH hSrcDS = GDALOpen(pszSrcFile, GA_ReadOnly);
//	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1));
//	int iBandCount = GDALGetRasterCount(hSrcDS);
//	//����RPC����ת����ϵ
//	void* hTransformArg = GDALCreateRPCTransformer(psRPC, FALSE, dfPixErrThreshold, papszOptions);
//	if (hTransformArg == NULL) {
//		GDALClose(hSrcDS);
//		return PARAM_ERROR;
//	}
//	//�������ͼ���������Χ����С��������
//	double adfGeoTransform[6];
//	double adfExtent[4];
//	int nPixels, nLines;
//
//	if (GDALSuggestedWarpOutput2(hSrcDS, GDALRPCTransform, hTransformArg, adfGeoTransform, &nPixels, &nLines, adfExtent, 0) != CE_None) {
//		GDALClose(hSrcDS);
//		return PARAM_ERROR;
//	}
//	//�������ͼ��
//	GDALDataType pszFormat =  GDALGetRasterDataType(hSrcDS);
//	GDALDriverH hDriver = GDALGetDriverByName(pszFormat);
//	GDALDatasetH hDstDS = GDALCreate(hDriver, pszDstFile, nPixels, nLines, iBandCount, eDT, NULL);//�������ͼ�����Ϣ
//	GDALSetProjection(hDstDS, SRS_WKT_WGS84_LAT_LONG);//���òο�ϵ
//	GDALSetGeoTransform(hSrcDS, adfGeoTransform);
//	//����GDALWarp�任ѡ��
//	GDALWarpOptions* psWarpOptions = GDALCreateWarpOptions();
//	psWarpOptions->papszWarpOptions = CSLDuplicate(NULL);
//	psWarpOptions->eWorkingDataType = eDT; //��������
//	psWarpOptions->eResampleAlg = GRA_NearestNeighbour; //��ֵ��ʽ
//	psWarpOptions->hSrcDS = hSrcDS; //ԭʼͼ����Ϣ
//	psWarpOptions->hDstDS = hDstDS; //Ŀ��ͼ����Ϣ
//
//	psWarpOptions->pfnTransformer = GDALGenImgProjTransform;// GDALCreateGCPTransformer;
//	psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer(hSrcDS, NULL, hDstDS, NULL, true, 0.0, 0);
//
//	psWarpOptions->pfnProgress = GDALTermProgress;
//	psWarpOptions->pProgressArg = pProcess;
//	psWarpOptions->nBandCount = iBandCount;
//	psWarpOptions->panSrcBands = (int*)CPLMalloc(psWarpOptions->nBandCount * sizeof(int));
//	psWarpOptions->panDstBands = (int*)CPLMalloc(psWarpOptions->nBandCount * sizeof(int));
//	for (int i = 0; i < iBandCount; i++) {
//		psWarpOptions->panSrcBands[i] = i + 1;
//		psWarpOptions->panDstBands[i] = i + 1;
//	}
//	//����GDALWarpִ�ж��󣬲�ʹ��GDALWarpOptions���г�ʼ��
//	GDALWarpOperation outputWarpOperation;
//	outputWarpOperation.Initialize(psWarpOptions);
//	//ִ�д���
//	outputWarpOperation.ChunkAndWarpImage(0, 0, nPixels, nLines);
//	//�ͷź͹ر���Դ
//	GDALDestroyRPCTransformer(psWarpOptions->pTransformerArg);
//	GDALDestroyWarpOptions(psWarpOptions);
//	GDALDestroyRPCTransformer(hTransformArg);
//	GDALClose(hSrcDS);
//	GDALClose(hDstDS);
//	if (pProcess != NULL) {
//	}
//	return SUCCESS;
//}

int GeoCorrection::GCP_Correction() {
	return 0;
}

GDALRPCInfo GeoCorrection::CreateRPCInfo(const char* pszSrcFile) {
	GDALAllRegister();
	GDALDataset* pSrcDS = (GDALDataset*)GDALOpen(pszSrcFile, GA_ReadOnly);
	char** papszRPCInfo = pSrcDS->GetMetadata("RPC");
	GDALRPCInfo sRPC;
	GDALExtractRPCInfo(papszRPCInfo, &sRPC);
	GDALClose((GDALDatasetH)pSrcDS);
	return sRPC;
}

GDALRPCInfo GeoCorrection::LoadRPBInfo(const char* pszSrcFile) {
	//char** RPCInfo = GDALLoadRPBFile(pszSrcFile);
	GDALRPCInfo sRPC;
	//GDALExtractRPCInfo(RPCInfo, &sRPC);
	return sRPC;
}

GDALRPCInfo GeoCorrection::LoadRPCInfo(const char* pszSrcFile) {
	//char** RPCInfo = GDALLoadRPCFile(pszSrcFile);
	GDALRPCInfo sRPC;
	//GDALExtractRPCInfo(RPCInfo, &sRPC);
	return sRPC;
}

int GeoCorrection::RPCCorrect(const char* InputImageFilePath, const char* RPCInfoFilePath, const char* DEMInfoFilePath, const char* OutputImageFilePath, int RPC_Mode, int DEM_Mode) {
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
	GDALRPCInfo RPCInfo;
	switch (RPC_Mode)
	{
	case 2:
		RPCInfo = CreateRPCInfo(InputImageFilePath);
		break;
	case 1:
		RPCInfo = LoadRPCInfo(InputImageFilePath);
		break;
	case 0:
		RPCInfo = CreateRPCInfo(InputImageFilePath);
		break;
	default:
		break;
	}
	char** RPCOptions = NULL;
	if (DEM_Mode == 0) {

	}
	GDALAllRegister();
	//��ԭʼͼ�񲢻�ȡͼ�������Ϣ
	GDALDatasetH hSrcDS = GDALOpen(InputImageFilePath, GA_ReadOnly);
	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1));
	int iBandCount = GDALGetRasterCount(hSrcDS);
	//����RPC����ת����ϵ
	void* hTransformArg = GDALCreateRPCTransformer(&RPCInfo, FALSE, 0, RPCOptions);
	if (hTransformArg == NULL) {
		GDALClose(hSrcDS);
		return PARAM_ERROR;
	}
	//�������ͼ���������Χ����С��������
	double adfGeoTransform[6];
	double adfExtent[4];
	int nPixels, nLines;

	if (GDALSuggestedWarpOutput2(hSrcDS, GDALRPCTransform, hTransformArg, adfGeoTransform, &nPixels, &nLines, adfExtent, 0) != CE_None) {
		GDALClose(hSrcDS);
		return PARAM_ERROR;
	}
	//�������ͼ��
	GDALDataType pszFormat = GDALGetRasterDataType(hSrcDS);
	GDALDriverH hDriver = GDALGetDriver(pszFormat);
	GDALDatasetH hDstDS = GDALCreate(hDriver, OutputImageFilePath, nPixels, nLines, iBandCount, eDT, NULL);//�������ͼ�����Ϣ
	GDALSetProjection(hDstDS, SRS_WKT_WGS84_LAT_LONG);//���òο�ϵ
	GDALSetGeoTransform(hSrcDS, adfGeoTransform);
	//����GDALWarp�任ѡ��
	GDALWarpOptions* psWarpOptions = GDALCreateWarpOptions();
	psWarpOptions->papszWarpOptions = CSLDuplicate(NULL);
	psWarpOptions->eWorkingDataType = eDT; //��������
	psWarpOptions->eResampleAlg = GRA_NearestNeighbour; //��ֵ��ʽ
	psWarpOptions->hSrcDS = hSrcDS; //ԭʼͼ����Ϣ
	psWarpOptions->hDstDS = hDstDS; //Ŀ��ͼ����Ϣ

	psWarpOptions->pfnTransformer = GDALGenImgProjTransform;// GDALCreateGCPTransformer;
	psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer(hSrcDS, NULL, hDstDS, NULL, true, 0.0, 0);

	//psWarpOptions->pfnProgress = GDALTermProgress;
	//psWarpOptions->pProgressArg = pProcess;
	psWarpOptions->nBandCount = iBandCount;
	psWarpOptions->panSrcBands = (int*)CPLMalloc(psWarpOptions->nBandCount * sizeof(int));
	psWarpOptions->panDstBands = (int*)CPLMalloc(psWarpOptions->nBandCount * sizeof(int));
	for (int i = 0; i < iBandCount; i++) {
		psWarpOptions->panSrcBands[i] = i + 1;
		psWarpOptions->panDstBands[i] = i + 1;
	}
	//����GDALWarpִ�ж��󣬲�ʹ��GDALWarpOptions���г�ʼ��
	GDALWarpOperation outputWarpOperation;
	outputWarpOperation.Initialize(psWarpOptions);
	//ִ�д���
	outputWarpOperation.ChunkAndWarpImage(0, 0, nPixels, nLines);
	//�ͷź͹ر���Դ
	GDALDestroyRPCTransformer(psWarpOptions->pTransformerArg);
	GDALDestroyWarpOptions(psWarpOptions);
	GDALDestroyRPCTransformer(hTransformArg);
	GDALClose(hSrcDS);
	GDALClose(hDstDS);
	return SUCCESS;
}