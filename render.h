/* render.h */

#ifndef RENDER_H
#define RENDER_H


extern void clearTileMaps(void);
extern void clearTileMap(int index);

extern void displayMemory(int x, int y, unsigned int offset);

extern void ramViewer(unsigned int ramLocation);

extern void printAt(int x, int y, char* pText);
extern void printColourAt(int x, int y, char *pText, int colour);
extern void printTileAt(int x, int y, int tile);
extern void printCharSet(void);

extern void setPalette(int index, unsigned char r, unsigned char g, unsigned char b);
extern void setsetBlackPalette(void);
extern void setCarPalette(void);
extern void setTextLayerPalette(void);

extern void showGauge32(int x, int y, int count);
extern void drawBox(int x, int y, int w, int h);

extern void drawRectFramebuffer(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);
extern void drawLineFramebuffer(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);
extern void setPixelFramebuffer(int x, int y, unsigned char r, unsigned char g, unsigned char b);
extern void drawCrosshatchFramebuffer(void);
extern void drawMandelbrotFramebuffer(void);

extern void setCharLinePalette(int x, int y, int length, int index);

extern void copyTextureRamMap(int mapIndex);
extern void copyTextureRamGeneral(void);
extern void copyLumaRAM(void);
extern void getGamePalette(unsigned int *pData);

extern void drawTestPageTilemap(void);


extern void printBinaryCharAt(int x, int y, unsigned char binary);


#endif

