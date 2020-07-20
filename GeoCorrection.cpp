#include "GeoCorrection.h"

//int GeoCorrection::RPC_Correction(const char* pszSrcFile, const char* pszDstFile, GDALRPCInfo* psRPC, double dfPixErrThreshold, char** papszOptions, GDALRIOResampleAlg eResampleMethod, const char* pszFormat, GDALProgressFunc* pProcess) {
//	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
//	if (pProcess != NULL) {
//		process
//			pProcess->
//	}
//	GDALAllRegister();
//	//打开原始图像并获取图像相关信息
//	GDALDatasetH hSrcDS = GDALOpen(pszSrcFile, GA_ReadOnly);
//	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1));
//	int iBandCount = GDALGetRasterCount(hSrcDS);
//	//创建RPC坐标转换关系
//	void* hTransformArg = GDALCreateRPCTransformer(psRPC, FALSE, dfPixErrThreshold, papszOptions);
//	if (hTransformArg == NULL) {
//		GDALClose(hSrcDS);
//		return PARAM_ERROR;
//	}
//	//计算输出图像的四至范围，大小，六参数
//	double adfGeoTransform[6];
//	double adfExtent[4];
//	int nPixels, nLines;
//
//	if (GDALSuggestedWarpOutput2(hSrcDS, GDALRPCTransform, hTransformArg, adfGeoTransform, &nPixels, &nLines, adfExtent, 0) != CE_None) {
//		GDALClose(hSrcDS);
//		return PARAM_ERROR;
//	}
//	//创建输出图像
//	GDALDataType pszFormat =  GDALGetRasterDataType(hSrcDS);
//	GDALDriverH hDriver = GDALGetDriverByName(pszFormat);
//	GDALDatasetH hDstDS = GDALCreate(hDriver, pszDstFile, nPixels, nLines, iBandCount, eDT, NULL);//设置输出图像的信息
//	GDALSetProjection(hDstDS, SRS_WKT_WGS84_LAT_LONG);//设置参考系
//	GDALSetGeoTransform(hSrcDS, adfGeoTransform);
//	//构建GDALWarp变换选项
//	GDALWarpOptions* psWarpOptions = GDALCreateWarpOptions();
//	psWarpOptions->papszWarpOptions = CSLDuplicate(NULL);
//	psWarpOptions->eWorkingDataType = eDT; //数据类型
//	psWarpOptions->eResampleAlg = GRA_NearestNeighbour; //插值方式
//	psWarpOptions->hSrcDS = hSrcDS; //原始图像信息
//	psWarpOptions->hDstDS = hDstDS; //目标图像信息
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
//	//创建GDALWarp执行对象，并使用GDALWarpOptions进行初始化
//	GDALWarpOperation outputWarpOperation;
//	outputWarpOperation.Initialize(psWarpOptions);
//	//执行处理
//	outputWarpOperation.ChunkAndWarpImage(0, 0, nPixels, nLines);
//	//释放和关闭资源
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
	//打开原始图像并获取图像相关信息
	GDALDatasetH hSrcDS = GDALOpen(InputImageFilePath, GA_ReadOnly);
	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1));
	int iBandCount = GDALGetRasterCount(hSrcDS);
	//创建RPC坐标转换关系
	void* hTransformArg = GDALCreateRPCTransformer(&RPCInfo, FALSE, 0, RPCOptions);
	if (hTransformArg == NULL) {
		GDALClose(hSrcDS);
		return PARAM_ERROR;
	}
	//计算输出图像的四至范围，大小，六参数
	double adfGeoTransform[6];
	double adfExtent[4];
	int nPixels, nLines;

	if (GDALSuggestedWarpOutput2(hSrcDS, GDALRPCTransform, hTransformArg, adfGeoTransform, &nPixels, &nLines, adfExtent, 0) != CE_None) {
		GDALClose(hSrcDS);
		return PARAM_ERROR;
	}
	//创建输出图像
	GDALDataType pszFormat = GDALGetRasterDataType(hSrcDS);
	GDALDriverH hDriver = GDALGetDriver(pszFormat);
	GDALDatasetH hDstDS = GDALCreate(hDriver, OutputImageFilePath, nPixels, nLines, iBandCount, eDT, NULL);//设置输出图像的信息
	GDALSetProjection(hDstDS, SRS_WKT_WGS84_LAT_LONG);//设置参考系
	GDALSetGeoTransform(hSrcDS, adfGeoTransform);
	//构建GDALWarp变换选项
	GDALWarpOptions* psWarpOptions = GDALCreateWarpOptions();
	psWarpOptions->papszWarpOptions = CSLDuplicate(NULL);
	psWarpOptions->eWorkingDataType = eDT; //数据类型
	psWarpOptions->eResampleAlg = GRA_NearestNeighbour; //插值方式
	psWarpOptions->hSrcDS = hSrcDS; //原始图像信息
	psWarpOptions->hDstDS = hDstDS; //目标图像信息

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
	//创建GDALWarp执行对象，并使用GDALWarpOptions进行初始化
	GDALWarpOperation outputWarpOperation;
	outputWarpOperation.Initialize(psWarpOptions);
	//执行处理
	outputWarpOperation.ChunkAndWarpImage(0, 0, nPixels, nLines);
	//释放和关闭资源
	GDALDestroyRPCTransformer(psWarpOptions->pTransformerArg);
	GDALDestroyWarpOptions(psWarpOptions);
	GDALDestroyRPCTransformer(hTransformArg);
	GDALClose(hSrcDS);
	GDALClose(hDstDS);
	return SUCCESS;
}