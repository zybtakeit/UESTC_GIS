#pragma once
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "gdal_mdreader.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"

typedef enum
{
	SUCCESS = 0,
	PARAM_ERROR = 1,		// no initialize
} RETURN_CODE;

class GeoCorrection
{
public:
	//int RPC_Correction(const char* pszSrcFile, const char* pszDstFile, GDALRPCInfo* psRPC, double dfPixErrThreshold, char** papszOptions, GDALRIOResampleAlg eResampleMethod, const char* pszFormat, GDALProgressFunc* pProcess);
	int GCP_Correction();
	int RPCCorrect(const char* InputImageFilePath, const char* RPCInfoFilePath, const char* DEMInfoFilePath, const char* OutputImageFilePath,int RPC_Mode,int DEM_Mode);
	GDALRPCInfo CreateRPCInfo(const char* pszSrcFile);
	GDALRPCInfo LoadRPCInfo(const char* pszSrcFile);
	GDALRPCInfo LoadRPBInfo(const char* pszSrcFile);
};

