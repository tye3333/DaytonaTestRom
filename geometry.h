/*

geometry.h

geometry renderer processor defines

*/

#ifndef GEOMETRY_H
#define GEOMETRY_H


#define MEM_GEORAM_0				0x00900000
#define MEM_GEORAM_1				0x00910000

#define REG_GEO_SET_READ_ADDR		0x803008
#define REG_GEO_SET_WRITE_ADDR		0x801008

#define REG_GEO_WRITE_WORD			0x804000

#define REG_GEO_CMD_NOP				0x800000
#define REG_GEO_CMD_OBJECT			0x800010
#define REG_GEO_CMD_DIRECT			0x800020
#define REG_GEO_CMD_WINDOW			0x800030
#define REG_GEO_CMD_TEXTURE			0x800040
#define REG_GEO_CMD_POLYGON			0x800050
#define REG_GEO_CMD_TEXPARAM		0x800060
#define REG_GEO_CMD_MODE			0x800070
#define REG_GEO_CMD_ZSORT			0x800080
#define REG_GEO_CMD_FOCAL			0x800090
#define REG_GEO_CMD_LIGHT			0x8000a0
#define REG_GEO_CMD_MATRIX			0x8000b0
#define REG_GEO_CMD_TRANSLATE		0x8000c0
#define REG_GEO_CMD_DATAPUSH		0x8000d0
#define REG_GEO_CMD_TEST			0x8000e0
#define REG_GEO_CMD_END				0x8000f0
#define REG_GEO_CMD_DUMMY			0x800100
/*#define REG_GEO_CMD_DUPE_OBJECT		0x800110*/
/*#define REG_GEO_CMD_DUPE_DIRECT		0x800120*/
/*#define REG_GEO_CMD_DUPE_WINDOW		0x800130*/
#define REG_GEO_CMD_LOG				0x800140
/*#define REG_GEO_CMD_DUPE_POLYGON	0x800150*/
#define REG_GEO_CMD_LOD				0x800160
/*#define REG_GEO_CMD_DUPE_MODE		0x800170*/
/*#define REG_GEO_CMD_DUPE_ZSORT		0x800180*/
/*#define REG_GEO_CMD_DUPE_FOCAL		0x800190*/
/*#define REG_GEO_CMD_DUPE_LIGHT		0x8001a0*/
/*#define REG_GEO_CMD_DUPE_MATRIX		0x8001b0*/
/*#define REG_GEO_CMD_DUPE_TRANSLATE	0x8001c0*/
#define REG_GEO_CMD_CODE_UPLOAD		0x8001d0
#define REG_GEO_CMD_CODE_JUMP		0x8001e0
/*#define REG_GEO_CMD_DUPE_END		0x8001f0*/

extern int gGeoListIndex;
extern int gGeoReadback;

extern void initialiseGeoProcessor(void);

extern void geoWriteZSORT(unsigned int value);
extern void geoWriteLOD(float lod);
extern void geoWriteMODE(unsigned int mode);
extern void geoWriteWINDOW(short* pWindow);
extern void geoWriteFOCUS(float fFocusX, float fFocusY);
extern void geoWriteLIGHT(float fLight1, float fLight2, float fLight3);
extern void geoWriteOBJECT(unsigned int point, unsigned int header, unsigned int start, unsigned int count);
extern void geoWriteMATRIX(float* pMatrix);
extern void geoWriteTRANSLATE(float* pVector);
extern void geoWriteEND(void);

extern void testGeo();
extern void kickGEO(void);
extern void swapGEOList(void);
extern void clearGEOList(void);

extern void initialiseGEOFIFO(void);

extern void copyGeoTexParamRAM(void);
extern void copyGeoRenderModeLOD(void);
extern void copyGeoLogTableRAM(void);
extern void copyGeoPolyParamRAM(void);
extern void copyGeoTexRAM1(void);
extern void copyGeoTexRAM2(void);

extern void copyGeoSceneRAM(int index);

extern void clearGeoRAM(void);



#endif




