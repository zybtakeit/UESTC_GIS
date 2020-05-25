// SatCoordTransU.h: interface for the CSatCoordTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SATCOORDTRANSU_H__B69CDE41_DA53_4E04_A912_A7A007119A0D__INCLUDED_)
#define AFX_SATCOORDTRANSU_H__B69CDE41_DA53_4E04_A912_A7A007119A0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef PROJECT_CREATE_LIB
#define PROJECT_LIB  __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"SatCordTrans.lib") 
#else
#pragma comment(lib,"SatCordTrans.lib") 
#endif
#else 
#define PROJECT_LIB  __declspec(dllexport)
#endif
/*
#ifdef _DEBUG_SPGEOCVT
#pragma comment(lib,"SpGeoCvtD.lib") 
#pragma message("Automatically linking with SpGeoCvtD.lib") 
#else
#pragma comment(lib,"SpGeoCvt.lib") 
#pragma message("Automatically linking with SpGeoCvt.lib") 
#endif
*/

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include <atlstr.h>
using namespace std;


#define SPGC_PI					3.1415926535897932384626433832795e0
#define SPGC_R2D				57.295779513082320876798154814105e0
#define SPGC_D2R				0.017453292519943295769236907684886e0

enum ELPS_TYPEU
{
    ET_WGS84        = 0,
    ET_BEIJING54    = 1,
    ET_XIAN80       = 2,
    ET_CN2000       = 3,
    ET_CUSTOM       = 4,    
};

#define SEMIMAJOR_WGS84	        6378137.0	
#define SEMIMINOR_WGS84		    6356752.31424517929

#define SEMIMAJOR_BEIJING54	    6378245.0	
#define SEMIMINOR_BEIJING54		6356863.018773047267

#define SEMIMAJOR_XIAN80	    6378140.0	
#define SEMIMINOR_XIAN80		6356755.2881575287

#define SEMIMAJOR_CN2000	    6378137.0	
#define SEMIMINOR_CN2000		6356752.3141

enum PROJECTION_TYPEU
{
    GEODETIC_LONLAT     = 0,   //经纬度  longlat
	TM_PROJECTION       = 1,   //横轴墨卡托投影	    tmerc
    GAUSS_PROJECTION    = 2,   //高斯－克吕格投影	tmerc 
	UTM_PROJECTION      = 3,   //通用横轴墨卡托投影 UTM
    LAMBERT_PROJECTION  = 4,   //兰波特等角投影 lcc
    GEODETIC_CENTRXYZ   = 5,   //地心坐标   
	MERC_PROJECTION     = 6    //墨卡托投影 merc 
};

enum ELEVATION_TYPEU
{
    GEODETIC_HEI    =  0,
    GEOID_DATA      =  1,
};


//#include "SpGeoCvt.h"


class PROJECT_LIB CSatCoordTrans  
{
public:
//	CSatCoordTrans();
//	virtual ~CSatCoordTrans();

	CSatCoordTrans();

	~CSatCoordTrans();

// Attributes

	// Set the translation and 7 parameters by GUI.
	bool SetInitPathName(const char* lpszInitPathName);

	// 
	bool SaveProjFile(const char* lpszInitPathName);

/**
			Set the coordinate system definition parameters, seven parameters between
			source and target ellipsoid, and elevation information(include the type 
			and geoid file) in a GUI dialog

			@parameter	bRemindSave			[in]	specify whether remind the user to save gcd file
			@parameter	strGcdFile			[out]	return the saved gcd file if not NULL

*/
	void Set_Parameters_ByGUI(BOOL bRemindSave = TRUE, char* strGcdFile = NULL);
//	BOOL Set_Geo_Par_ByGUI(){ return m_pdtDealer.Set_Cvt_Par_ByGUI(); };
//	BOOL Set_Par7_ByGUI()   { return m_parDealer.Set_Cvt_Par_ByGUI(); };
//	BOOL Set_Geoid_Par_ByGUI();

	void Set_Geo_Par( int elpsType,
					  int projType,
					  double a,double b,
					  double Origin_Latitude,  // deg
					  double Central_Meridian, // deg
					  double False_Easting ,
					  double False_Northing,
					  double Scale_Factor,
					  double Lamb_Lat1=0,
					  double Lamb_Lat12=0,       // deg
					  int eleType=0,
					  LPCSTR lpstrHeiAdj=NULL );

	void Get_Geo_Par( int *elpsType,
					  int *projType,
					  double *a,double *b,
					  double *Origin_Latitude , // deg
					  double *Central_Meridian, // deg
					  double *False_Easting ,
					  double *False_Northing,
					  double *Scale_Factor,
					  double *Lamb_Lat1,
					  double *Lamb_Lat12,       // deg
					  int *eleType,
					  char *lpstrHeiAdj=NULL );

	void Get7Parameters(double *dx,double *dy,double *dz,
						double *rx,double *ry,double *rz,
                        double *scale)
	{
		//m_parDealer.Get_Cvt_Par(dx, dy, dz, rx, ry, rz, scale);
		*dx=m_l2wDx;*dy=m_l2wDy;*dz=m_l2wDz;
		*rx=m_l2wRx;*ry=m_l2wRy;*rz=m_l2wRz;
		*scale=m_l2wScale;
	}

	void Set7Parameter(double dx,double dy,double dz,
					   double rx,double ry,double rz,
					   double scale)
	{
		//m_parDealer.Set_Cvt_Par(dx, dy, dz, rx, ry, rz, scale);
		m_l2wDx=dx,m_l2wDy=dy,m_l2wDz=dz;
		m_l2wRx=rx,m_l2wRy=ry,m_l2wRz=rz;
		m_l2wScale=scale;
	}

/**
			There are two method to translate panel geodetic coordinates(Longitude, Latitude)
			between different ellipsoids, seven parameters method and panel grid method. In
			this class, we just use one of them. So if you set the panel grid method, it will
			use it in spite of setting 7 parameters before

*/
//	BOOL SetPanelGridMethod(BOOL bPgMethod, const char* lpszGridFile);

	inline const char* GetPathName(){ return m_strPathName; };
	inline int GetEllipsoidType()   { return m_elpsType; };
	inline int GetProjType()        { return m_projType; };
	inline int GetElevationType()   { return m_eleType; };


// Operations

	BOOL Load4File(const char* lpszPathName);
	BOOL Save2File(const char* lpszPathName);
	
//	BOOL Import4RefFile(const char* lpszPathName);
//	BOOL InitIndirect(int nHoRefSystem, int nStripNo, int nVeRefSystem = 0, char cHemisphere = 'N');

	/*
	 * The function Convert_Geodetic_To_Object Convert the WGS84(L, B, H) in radians 
	 * to the project coordinate of the product coordinate system.
	 *
	 *    Longitude				: Geodetic latitude in radians			(input)
	 *    Latitude				: Geodetic longitude in radians			(input)
	 *    Altitude				: Geodetic height						(input)
	 *	  gx, gy, gz			: Project coordinate					(output)
	 *
	 */
	void Convert_Geodetic_To_Object( double Longitude, double Latitude, double Altitude, 
									 double *gx, double *gy, double *gz);
	void Convert_Geodetic_To_Object( double Longitude, double Latitude, 
									 double *gx, double *gy);

	/*
	 * The function Convert_Geodetic_Degree_To_Object Convert the WGS84(L, B, H) in degrees
	 * to the project coordinate of the product coordinate system.
	 *
	 *    Longitude				: Geodetic latitude in degrees			(input)
	 *    Latitude				: Geodetic longitude in degrees			(input)
	 *    Altitude				: Geodetic height						(input)
	 *	  gx, gy, gz			: Project coordinate					(output)
	 *
	 */
	void Convert_Geodetic_Degree_To_Object( double Longitude, double Latitude, double Altitude, 
									 double *gx, double *gy, double *gz);
	void Convert_Geodetic_Degree_To_Object( double Longitude, double Latitude, 
									 double *gx, double *gy);


	/*
	 * The function Convert_Object_To_Geodetic Convert the project coordinate of the 
	 * product coordinate system to the WGS84(L, B, H) in radians 
	 *
	 *	  gx, gy, gz			: Project coordinate					(input)
	 *    Longitude				: Geodetic latitude in radians			(output)
	 *    Latitude				: Geodetic longitude in radians			(output)
	 *    Altitude				: Geodetic height						(output)
	 *
	 */
	void Convert_Object_To_Geodetic( double gx, double gy, double gz,
									 double *Longitude, double *Latitude, double *Altitude );
	void Convert_Object_To_Geodetic( double gx, double gy,
									 double *Longitude, double *Latitude);
	/*
	 * The function Convert_Object_To_Geodetic_Degree Convert the project coordinate of the 
	 * product coordinate system to the WGS84(L, B, H) in degrees 
	 *
	 *	  gx, gy, gz			: Project coordinate					(input)
	 *    Longitude				: Geodetic latitude in degrees			(output)
	 *    Latitude				: Geodetic longitude in degrees			(output)
	 *    Altitude				: Geodetic height						(output)
	 *
	 */
	void Convert_Object_To_Geodetic_Degree( double gx, double gy, double gz,
									 double *Longitude, double *Latitude, double *Altitude );
	void Convert_Object_To_Geodetic_Degree( double gx, double gy,
									 double *Longitude, double *Latitude);

// Additional Operations
	char* GetProjStr();
	bool  SetProjStr(char* projs);

	bool StartProj();
	void EndProj();

	void operator=(CSatCoordTrans& CSObject);
	void Copy(CSatCoordTrans* pCSObject);

	//七参数计算
	//输入对应的A(XYZ)-B(X'Y'Z')坐对，实现A到B的7参数，至少需要三个坐标对
	//(scrX,scrY,scrZ),基准A的大地平面坐标
	//(desX,desY,desZ),基准B
	//cA[7],返回7个参数：cA[0-2]对应dx,dy,dz,cA[3-5]对应rx,ry,rz,cA[6]对应k
	//其中rx,ry,rz为秒，k为微米（scale=1+k/1000000.0）
	int ComputeParam7(vector<double>scrX,vector<double>scrY,vector<double>scrZ,\
		vector<double>desX,vector<double>desY,vector<double>desZ,double *cA) ;
private:
	string GetElpsString(); //m_ElpsType
	string GetToWgsString();//m_ProjType
	
	//获取关键值label的值 
	bool GetProjValue(CString projString,char* label,double& value);
	bool GetProjValue(CString projString,char* label,char* svalue);
	char* GetLonlatProjString();
protected:
	//CSpGeoCvtImp	m_oriDelaer;
	//CSpGeoCvtImp	m_pdtDealer;
	//CSpPar7Cvt	m_parDealer;
	//CSpAltCvt		m_altDealer;
	char			m_strPathName[512];
	char			m_strInitPathName[512];
	//投影参数
	//int    m_ElpsType ;
    //int    m_ProjType ;
    
    char   m_strHeiAdj[256];
 // char   m_strPathName[256];
 //   float* m_pGeoId;

	//投影具体参数
	int    m_elpsType;
	int    m_projType;
	double m_a;
	double m_b;
//	double m_e;  //第一偏心率
	double m_es; //第一偏心率平方
	double m_Origin_Latitude ; // rad
	double m_Central_Meridian; // rad
	double m_False_Easting ;
	double m_False_Northing;
	double m_Scale_Factor;
	char  m_lpstrHeiAdj[512];
	int    m_eleType  ;

	int    m_Project_Zone;
	double m_Lambert_B1;
	double m_Lambert_B2;
	double m_Lambert_FInvs;//=pExcep.ReadUserParamDoubleT("GeoCvtInf","LambertFInvs",lpszInitPathName);
	//兰柏特等角投影椭球偏率
	//七参数
	//CSpPar7Cvt //double m_l2wM[9],m_w2lM[9];
	double m_l2wDx,m_l2wDy,m_l2wDz;
	double m_l2wRx,m_l2wRy,m_l2wRz;
    double m_l2wScale;
//  char   m_strPathName[256];


	////
	char  m_longlatString[512];
	char  m_projString[1024];

	//void *projPJ;
	void *m_longlatProj;//日前没有使用
	void *m_Proj;//当前投影，用于正反解析

};

#endif // !defined(AFX_SATCOORDTRANSU_H__B69CDE41_DA53_4E04_A912_A7A007119A0D__INCLUDED_)
