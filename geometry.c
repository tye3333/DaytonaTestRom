/* geometry.c  - geo processor routines */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "copro.h"
#include "matrix.h"
#include "geometry.h"

#define MEM_ROM_TEXTURE_PARAMS		0x02805120
#define MEM_ROM_POLY_PARAMS			0x02800000
#define MEM_ROM_TEXTURE_LIST1		0x028f0000
#define MEM_ROM_TEXTURE_LIST2		0x028f5000


int gGeoListIndex = 0;
int gGeoReadback = 0;

float g_fCurrentLOD = 0.0f;

const unsigned int* g_pObjectCurrent;

float firstMatrix[] = { 0.001300f, 0.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f, 0.000000f, 0.000800f, 0.000000f, 0.000000f, 0.000000f, 0.040000f };

short gGeoWindowFirst[12] = { -1, 128, 496, 512, 95, 396, 248, 316, 352, 152, 248, 316 };
short gGeoWindow[12] = { -1, 128, 496, 512, 248, 320, 248, 320, 248, 320, 248, 320 };
short gGeoWindowScene1[12] = { -1, 128, 496, 512, 248, 270, 248, 316, 352, 152, 248, 316 };

float gTranslate[3];

float fTest1 = 0.0f;

float fRotX = 1.0f;
float fRotY = 30.0f;
float fRotZ = 70.0f;

float testMatrix[12];
float tempMatrix[12];

float matrixX[12];
float matrixY[12];
float matrixZ[12];

vec3 axisX;
vec3 axisY;
vec3 axisZ;




const int gObjectList[] =
{
	/*	0x00587d0c, 0x00510000, 0x00b456d1, 0x00001388,	*/		/* 0 */
		0x00587d0c, 0x00510000, 0x00b456d1, 0x00001388,			/* 0	RED CAR BODY */
		0x00805000, 0x00800000, 0x00000004, 0x00001388,			/* 1 */
		0x00581040, 0x0050d050, 0x00b3cbc9, 0x00001388,			/* 2	WINDOW METAL STRIPS */
		0x00581100, 0x0050d088, 0x00b3ccba, 0x00001388,			/* 3 */
		0x0019dee2, 0x00124918, 0x008d7b13, 0x00001388,			/* 4 */
		0x0019e01a, 0x001249b0, 0x008d7c9a, 0x00001388,			/* 5 */
		0x0019dee2, 0x00124918, 0x008d7b13, 0x00001388,			/* 6 */
		0x0019e01a, 0x001249b0, 0x008d7c9a, 0x00001388,			/* 7 */
		0x00168a56, 0x001099c8, 0x008d7ab7, 0x00001388,			/* 8 */
		0x00583edc, 0x0050e4ac, 0x00b40771, 0x00001388,			/* 9	GREEN CAR BODY */
		0x00805070, 0x00800024, 0x00000091, 0x00001388,			/* 10 */
		0x001717c8, 0x0010f364, 0x008a814a, 0x00001388,			/* 11	FOUR STATIC WHEELS*/
		0x0016a6c0, 0x0010c824, 0x008d9e42, 0x00001388,			/* 12 */
		0x00584b4c, 0x0050ea10, 0x00b41751, 0x00001388,			/* 13 */
		0x00805070, 0x00800024, 0x00000091, 0x00001388,			/* 14 */
		0x001717c8, 0x0010f364, 0x008a814a, 0x00001388,			/* 15 */
		0x0016a6c0, 0x0010c824, 0x008d9e42, 0x00001388,			/* 16 */
		0x005825fc, 0x0050d9e4, 0x00b3e7b1, 0x00001388,			/* 17	BLUE CAR BODY */
		0x00805070, 0x00800024, 0x00000091, 0x00001388,			/* 18 */
		0x001717c8, 0x0010f364, 0x008a814a, 0x00001388,			/* 19 */
		0x0016a6c0, 0x0010c824, 0x008d9e42, 0x00001388,			/* 20 */
		0x0058326c, 0x0050df48, 0x00b3f791, 0x00001388,			/* 21	YELLOW CAR BODY*/
		0x00805070, 0x00800024, 0x00000091, 0x00001388,			/* 22 */
		0x001717c8, 0x0010f364, 0x008a814a, 0x00001388,			/* 23 */
		0x0016a6c0, 0x0010c824, 0x008d9e42, 0x00001388,			/* 24 */
		0x001bfbac, 0x00133ef8, 0x009219d9, 0x00001388,			/* 25 */
		0x001bf7ac, 0x00133df8, 0x009214c9, 0x00001388,			/* 26 */
		0x001befac, 0x00133bf8, 0x00920aa9, 0x00001388,			/* 27 */
		0x0005b758, 0x00004428, 0x0080f80d, 0x00001388,			/* 28 */
		0x0005db18, 0x000051d0, 0x008126a3, 0x00001388,			/* 29 */
		0x0005c606, 0x00004a3c, 0x00810b96, 0x00001388,			/* 30 */
		0x0005db20, 0x000051d4, 0x008126b8, 0x00001388,			/* 31 */
		0x00058800, 0x00003274, 0x0080bba4, 0x00001388,			/* 32 */
		0x0005ce5e, 0x00004d30, 0x0081160f, 0x00001388,			/* 33 */
		0x0005dba0, 0x00005214, 0x00812808, 0x00001388,			/* 34 */
		0x00058b98, 0x0000339c, 0x0080c02d, 0x00001388,			/* 35 */
		0x0005db28, 0x000051d8, 0x008126cd, 0x00001388,			/* 36 */
		0x0005dba8, 0x00005218, 0x0081281d, 0x00001388,			/* 37 */
		-1, -1, -1, -1
};



void initialiseGeoProcessor(void)
{
	initialiseGEOFIFO();

	copyGeoLogTableRAM();
	waitVBL();
	waitVBL();
	copyGeoTexParamRAM();
	waitVBL();
	waitVBL();
	copyGeoRenderModeLOD();
	waitVBL();
	waitVBL();
	copyGeoPolyParamRAM();
	waitVBL();
	waitVBL();
	copyGeoTexRAM1();
	waitVBL();
	waitVBL();
	copyGeoTexRAM2();
	waitVBL();
	waitVBL();
}

void clearGEOList(void)
{
	waitVBL();
	geoWriteEND();
	kickGEO();

	waitVBL();
	geoWriteEND();
	kickGEO();
}

void initialiseGEOFIFO(void)
{
	*(volatile unsigned int *)MEM_VIDEO_CTRL = 0;

	/* set geo write address to 0x900000 and write END instruction*/
	*(volatile unsigned int *)REG_GEO_SET_WRITE_ADDR = 0x00000000;
	*(volatile unsigned int *)REG_GEO_CMD_END = 0x00010000;

	/* set geo write address to 0x910000 and write END instruction */
	*(volatile unsigned int *)REG_GEO_SET_WRITE_ADDR = 0x00010000;
	*(volatile unsigned int *)REG_GEO_CMD_END = 0x00000000;

	*(volatile unsigned int *)REG_GEO_SET_READ_ADDR = 0x00900000;

	*(volatile unsigned int *)MEM_ZCLIP = 0xff;					/* ???? */
}

void geoWriteZSORT(unsigned int value)
{
	*(volatile unsigned int*)REG_GEO_CMD_ZSORT = 0;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = value;
}

void geoWriteLOD(float lod)
{
	*(volatile unsigned int*)REG_GEO_CMD_LOD = 0;
	*(volatile float*)REG_GEO_WRITE_WORD = lod;
}

void geoWriteMODE(unsigned int mode)
{
	*(volatile unsigned int*)REG_GEO_CMD_MODE = 0;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = mode;
}

void geoWriteWINDOW(short* pWindow)
{
	*(volatile unsigned int*)REG_GEO_CMD_WINDOW = 0;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[0] << 16) | pWindow[1];
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[2] << 16) | pWindow[3];
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[4] << 16) | pWindow[5];
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[6] << 16) | pWindow[7];
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[8] << 16) | pWindow[9];
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = (pWindow[10] << 16) | pWindow[11];
}

void geoWriteFOCUS(float fFocusX, float fFocusY)
{
	*(volatile unsigned int*)REG_GEO_CMD_FOCAL = 0;
	*(volatile float*)REG_GEO_WRITE_WORD = fFocusX;
	*(volatile float*)REG_GEO_WRITE_WORD = fFocusY;
}

void geoWriteLIGHT(float fLight1, float fLight2, float fLight3)
{
	*(volatile unsigned int*)REG_GEO_CMD_LIGHT = 0;
	*(volatile float*)REG_GEO_WRITE_WORD = fLight1;
	*(volatile float*)REG_GEO_WRITE_WORD = fLight2;
	*(volatile float*)REG_GEO_WRITE_WORD = fLight3;
}

void geoWriteOBJECT(unsigned int point, unsigned int header, unsigned int start, unsigned int count)
{
	*(volatile unsigned int*)REG_GEO_CMD_OBJECT = 0;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = point;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = header;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = start;
	*(volatile unsigned int*)REG_GEO_WRITE_WORD = count;
}

void geoWriteMATRIX(float* pMatrix)
{
	int i;

	*(volatile unsigned int*)REG_GEO_CMD_MATRIX = 0;

	for (i = 0; i < 12; i++)
	{
		*(volatile float*)REG_GEO_WRITE_WORD = *pMatrix;
		pMatrix++;
	}
}

void geoWriteTRANSLATE(float* pVector)
{
	int i;

	*(volatile unsigned int*)REG_GEO_CMD_TRANSLATE = 0;

	for (i = 0; i < 3; i++)
	{
		*(volatile float*)REG_GEO_WRITE_WORD = *pVector;
		pVector++;
	}
}

void geoWriteEND(void)
{
/*	*(volatile unsigned int*)REG_GEO_CMD_END = gGeoListIndex; */

	if (gGeoListIndex == 0x000000)
	{
		*(volatile unsigned int *)REG_GEO_CMD_END = 0x010000;
	}
	else
	{
		*(volatile unsigned int *)REG_GEO_CMD_END = 0x000000;
	}
}

void kickGEO(void)
{
	/*	*(volatile int*)0x00880250 = 0x2525;
		gGeoReadback = *(volatile int*)0x00884000;
	*/

	/* write END of GEO list command with pointer to next list */
/*	*(volatile unsigned int*)REG_GEO_CMD_END = gGeoListIndex; */

	if (gGeoListIndex == 0x000000)
	{
		*(volatile unsigned int *)REG_GEO_CMD_END = 0x010000;
	}
	else
	{
		*(volatile unsigned int *)REG_GEO_CMD_END = 0x000000;
	}


	/* set next read geo list addr */
/*	*(volatile unsigned int*)REG_GEO_SET_READ_ADDR = gGeoListIndex;*/
}


void swapGEOList(void)
{
	/* fetch the current geo write list address */
/*	int currentList = *(volatile int*)REG_GEO_SET_READ_ADDR;*/

	if (gGeoListIndex == 0x000000)
	{
		gGeoListIndex = 0x010000;
		*(volatile unsigned int *)REG_GEO_SET_WRITE_ADDR = 0x010000;
		*(volatile unsigned int *)REG_GEO_SET_READ_ADDR = 0x000000;
	}
	else
	{
		gGeoListIndex = 0x000000;
		*(volatile unsigned int *)REG_GEO_SET_WRITE_ADDR = 0x000000;
		*(volatile unsigned int *)REG_GEO_SET_READ_ADDR = 0x010000;
	}
}


void copyGeoTexParamRAM(void)
{
	int i;
	unsigned short* pSource;

	waitVBL();

	pSource = (unsigned short*)MEM_ROM_TEXTURE_PARAMS;

	*(volatile unsigned int*)REG_GEO_CMD_TEXPARAM = 0;

	for (i = 0; i < 0x80; i++)
	{
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = *pSource;
		pSource++;
	}

	kickGEO();
	waitVBL();
}

void copyGeoRenderModeLOD(void)
{
	geoWriteMODE(3);
	geoWriteMODE(1);
	geoWriteLOD(128.0f);

	g_fCurrentLOD = 128.0f;
}

void copyGeoLogTableRAM(void)
{
	waitVBL();

	geo_push_log_data();
	
	waitVBL();

#ifdef datanotpresent
	int i, j, addr;
	unsigned int* pSource;

	addr = 0x00000;
	pSource = (unsigned int*)geoLogTable_bin;

	for (j = 0; j < 4; j++)
	{
		/* make geom packets */
		*(volatile int*)REG_GEO_CMD_LOG = 0;			/* write command 05 to geom memory write pointer */
		*(volatile int*)REG_GEO_WRITE_WORD = addr;		/* write address to geo ram */
		*(volatile int*)REG_GEO_WRITE_WORD = 0x800;		/* write length 0x800 to geo ram */

		for (i = 0; i < 0x800; i++)
		{
			*(volatile int*)REG_GEO_WRITE_WORD = *pSource;
			pSource++;
		}

		addr += 0x2000;

		kickGEO();
		waitVBL();
	}
#endif
}

void copyGeoPolyParamRAM(void)
{
	int i, j, addr;
	unsigned int* pSource;

	waitVBL();

	addr = 0x00000;
	pSource = (unsigned int*)MEM_ROM_POLY_PARAMS;

	for (j = 0; j < 16; j++)
	{
		/* make geom packets */
		*(volatile unsigned int*)REG_GEO_CMD_POLYGON = 0;		/* write command 05 to geom memory write pointer */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = addr;		/* write address to geo ram */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = 0x800;	/* write length 0x800 to geo ram */

		for (i = 0; i < 0x800; i++)
		{
			*(volatile unsigned int*)REG_GEO_WRITE_WORD = *pSource;
			pSource++;
		}

		addr += 0x800;

		kickGEO();
		waitVBL();
	}
}

void copyGeoTexRAM1(void)
{
	int i, j, addr;
	unsigned short* pSource;

	waitVBL();

	addr = 0x00800000;
	pSource = (unsigned short*)MEM_ROM_TEXTURE_LIST1;

	for (j = 0; j < 16; j++)
	{
		/* make geom packets */
		*(volatile unsigned int*)REG_GEO_CMD_TEXTURE = 0;		/* write command 05 to geom memory write pointer */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = addr;		/* write address to geo ram */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = 0x800;	/* write length 0x800 to geo ram */

		for (i = 0; i < 0x800; i++)
		{
			*(volatile unsigned int*)REG_GEO_WRITE_WORD = *pSource;
			pSource++;
		}

		addr += 0x800;

		kickGEO();
		waitVBL();
	}
}

void copyGeoTexRAM2(void)
{
	int i, j, addr;
	unsigned short* pSource;

	waitVBL();

	addr = 0x00805000;
	pSource = (unsigned short*)MEM_ROM_TEXTURE_LIST2;

	for (j = 0; j < 22; j++)
	{
		/* make geom packets */
		*(volatile unsigned int*)REG_GEO_CMD_TEXTURE = 0;		/* write command 05 to geom memory write pointer */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = addr;		/* write address to geo ram */
		*(volatile unsigned int*)REG_GEO_WRITE_WORD = 0x800;	/* write length 0x800 to geo ram */

		for (i = 0; i < 0x800; i++)
		{
			*(volatile unsigned int*)REG_GEO_WRITE_WORD = *pSource;
			pSource++;
		}

		addr += 0x800;

		kickGEO();
		waitVBL();
	}
}

/* copy sample geo list of daytona car scene */
void copyGeoSceneRAM(int index)
{
#ifdef datanotpresent
	int i, size;
	unsigned int* pSource;
	unsigned int* pDestBuffer;

	waitVBL();

	if (index)
	{
/*		size = daytonascene1_bin_len;
		pSource = (unsigned int*)daytonascene1_bin;
*/	}
	else
	{
		size = daytonamapselectgeoram_bin_len;
		pSource = (unsigned int*)daytonamapselectgeoram_bin;
	}

	pDestBuffer = (unsigned int*)(MEM_GEORAM_0 + gGeoListIndex);

	for (i = 0; i < (size / 4); i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}
#endif
}

/* fill all geo ram with END command */
void clearGeoRAM(void)
{
	int i;
	unsigned int* pDestBuffer;

	pDestBuffer = (unsigned int*)MEM_GEORAM_0;

	for (i = 0; i < 0x8000; i++)
	{
		*pDestBuffer = 0x07800f0f;	/*0x00000000;*/
		pDestBuffer++;
	}
/*
	pDestBuffer = (unsigned int*)MEM_GEORAM_1;

	for (i = 0; i < (0x10000 / 4); i++)
	{
		*pDestBuffer = 0x07800f0f;
	}
*/
}





void createGEOScene(void)
{
	/* initialse scene params */
	geoWriteZSORT(0x40800000);
	geoWriteLOD(128.0f);
	geoWriteWINDOW(gGeoWindowScene1);
	geoWriteFOCUS(280.0f, 280.0f);
	geoWriteLIGHT(0.333f, 0.333f, 0.333f);

	/* set rotation matrix for all cars */
	geoWriteMATRIX(testMatrix);

	/* position RED car */
	gTranslate[0] = -5.0f;
	gTranslate[1] = 0.0f;
	gTranslate[2] = 9.0f;
	geoWriteTRANSLATE(gTranslate);

	/* render RED car */
	g_pObjectCurrent = &gObjectList[0 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[11 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[2 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[3 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);

	/* position GREEN car */
	gTranslate[0] = 0.0f;
	gTranslate[1] = 0.0f;
	gTranslate[2] = 9.0f;
	geoWriteTRANSLATE(gTranslate);

	/* render GREEN car */
	g_pObjectCurrent = &gObjectList[9 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[11 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[2 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);

	/* position BLUE car */
	gTranslate[0] = 5.0f;
	gTranslate[1] = 0.0f;
	gTranslate[2] = 9.0f;
	geoWriteTRANSLATE(gTranslate);

	/* render BLUE car */
	g_pObjectCurrent = &gObjectList[17 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[11 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);
	g_pObjectCurrent = &gObjectList[2 * 4];
	geoWriteOBJECT(g_pObjectCurrent[0], g_pObjectCurrent[1], g_pObjectCurrent[2], g_pObjectCurrent[3]);

	/* send to geometry processor */
	kickGEO();
}


/* render three cars */
void testGeo(void)
{
	int i;
	unsigned int* pDest;
	unsigned int* pSrc;
	unsigned int offset;

	float fResult = 0.0f;
	float fValue1 = 1.0f;
	float fValue2 = 1.0f;

	axisX.x = 1.0f;
	axisX.y = 0.0f;
	axisX.z = 0.0f;

	axisY.x = 0.0f;
	axisY.y = 1.0f;
	axisY.z = 0.0f;

	axisZ.x = 0.0f;
	axisZ.y = 0.0f;
	axisZ.z = 1.0f;

/*	clearTileMap(0);	*/
	printColourAt(1, 1, (char*)gHeaderText, 6);
	printColourAt(4, 3, "GEOMETRY TEST", 5);
	printColourAt(1, 40, "PRESS START TO CONTINUE", 6);

	while (1)
	{
		rotate(fRotX, axisX, matrixX);
		rotate(fRotY, axisY, matrixY);
		rotate(fRotZ, axisZ, matrixZ);

		matrixMultiply(matrixX, matrixY, tempMatrix);
		matrixMultiply(tempMatrix, matrixZ, testMatrix);

		testMatrix[9] = 0.0f;
		testMatrix[10] = 0.0f;
		testMatrix[11] = 9.0f;

		sprintf(gTemp, "XROT  %f   ", fRotX);
		printAt(1+(18*0), 5, gTemp);
		sprintf(gTemp, "YROT  %f   ", fRotY);
		printAt(1+(18*1), 5, gTemp);
		sprintf(gTemp, "ZROT  %f   ", fRotZ);
		printAt(1+(18*2), 5, gTemp);

		sprintf(gTemp, "m[0]  %f   ", testMatrix[0]);
		printAt(1 + (18 * 0), 7, gTemp);
		sprintf(gTemp, "m[1]  %f   ", testMatrix[1]);
		printAt(1 + (18 * 1), 7, gTemp);
		sprintf(gTemp, "m[2]  %f   ", testMatrix[2]);
		printAt(1 + (18 * 2), 7, gTemp);

		sprintf(gTemp, "m[3]  %f   ", testMatrix[3]);
		printAt(1 + (18 * 0), 8, gTemp);
		sprintf(gTemp, "m[4]  %f   ", testMatrix[5]);
		printAt(1 + (18 * 1), 8, gTemp);
		sprintf(gTemp, "m[5]  %f   ", testMatrix[6]);
		printAt(1 + (18 * 2), 8, gTemp);

		sprintf(gTemp, "m[6]  %f   ", testMatrix[6]);
		printAt(1 + (18 * 0), 9, gTemp);
		sprintf(gTemp, "m[7]  %f   ", testMatrix[7]);
		printAt(1 + (18 * 1), 9, gTemp);
		sprintf(gTemp, "m[8]  %f   ", testMatrix[8]);
		printAt(1 + (18 * 2), 9, gTemp);

		sprintf(gTemp, "m[9]  %f   ", testMatrix[9]);
		printAt(1 + (18 * 0), 10, gTemp);
		sprintf(gTemp, "m[10] %f   ", testMatrix[10]);
		printAt(1 + (18 * 1), 10, gTemp);
		sprintf(gTemp, "m[11] %f   ", testMatrix[11]);
		printAt(1 + (18 * 2), 10, gTemp);

		fRotX += 0.785f;
		fRotY += 0.965f;
		fRotZ += 1.145f;

#ifdef testcopro
		*(volatile unsigned int *)REG_COPRO_CMD_FCOSM_M1 = 0x1e1e;					/* copro command: fcosm fResult = fValue1 * cos( fValue2 )    */
		*(volatile unsigned int *)REG_COPRO_WRITE_WORD = fValue2;
		*(volatile unsigned int *)REG_COPRO_WRITE_WORD = fValue1;
		fResult = *(volatile unsigned int *)REG_COPRO_WRITE_WORD;
#endif

		if (fTest1 > 360.0f) fTest1 -= 360.0f;
		if (fRotX > 360.0f) fRotX -= 360.0f;
		if (fRotY > 360.0f) fRotY -= 360.0f;
		if (fRotZ > 360.0f) fRotZ -= 360.0f;

		readIO();

		/* exit if start button pressed */
		if ((gInputs & (1 << eIN_START)) == 0)
		{
			waitButtonStart();
			return;
		}

		waitVBL();

		createGEOScene();
	}
}




