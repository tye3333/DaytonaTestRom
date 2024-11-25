/* render.c - misc render functions */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "font.h"
#include "geometry.h"
#include "render.h"
#include "tests.h"


unsigned int initPalette[] =
{
	0x90900000,
	0x82108200,
	0xC010C000,
	0xC210C200,
	0x801FE318,
	0x831F83E0,
	0xFC1FFC00,
	0xFFFFF1E7,
};

unsigned short gTestPalette[] =
{
	SET_RGB(3, 3, 3), SET_RGB(0, 0, 8), SET_RGB(0, 0, 16), SET_RGB(0, 0, 31),
	SET_RGB(8, 0, 0), SET_RGB(8, 0, 8), SET_RGB(8, 0, 16), SET_RGB(8, 0, 31),
	SET_RGB(16, 0, 0), SET_RGB(16, 0, 8), SET_RGB(16, 0, 16), SET_RGB(16, 0, 31),
	SET_RGB(31, 0, 0), SET_RGB(31, 0, 8), SET_RGB(31, 0, 16), SET_RGB(31, 0, 31),
	SET_RGB(16, 0, 0), SET_RGB(16, 8, 0), SET_RGB(16, 16, 0), SET_RGB(16, 31, 0),
	SET_RGB(31, 0, 0), SET_RGB(31, 8, 0), SET_RGB(31, 16, 0), SET_RGB(31, 31, 0),



/*	SET_RGB(0, 0, 0), SET_RGB(0, 0, 31), SET_RGB(31, 0, 0), SET_RGB(31, 0, 31), SET_RGB(0, 31, 0), SET_RGB(0, 31, 31), SET_RGB(31, 31, 0), SET_RGB(31, 31, 31),
	SET_RGB(0, 0, 0), SET_RGB(0, 0, 24), SET_RGB(24, 0, 0), SET_RGB(24, 0, 31), SET_RGB(0, 24, 0), SET_RGB(0, 24, 24), SET_RGB(24, 24, 0), SET_RGB(24, 24, 24),
	SET_RGB(0, 0, 0), SET_RGB(0, 0, 16), SET_RGB(16, 0, 0), SET_RGB(16, 0, 16), SET_RGB(0, 16, 0), SET_RGB(0, 16, 16), SET_RGB(16, 16, 0), SET_RGB(16, 16, 16),
	SET_RGB(0, 0, 0), SET_RGB(0, 0, 8), SET_RGB(8, 0, 0), SET_RGB(8, 0, 8), SET_RGB(0, 8, 0), SET_RGB(0, 8, 8), SET_RGB(8, 8, 0), SET_RGB(8, 8, 8),*/
};

/* set up text layer for printing */
void setupTextLayer(void)
{
	setTextLayerPalette();
	setCarPalette();

	build_colour_translation();

	copyColourFont();
	
	clearGeoRAM();

	/* what does this do? maybe clears odd / even flag bit */
	*(volatile int *)MEM_VIDEO_CTRL = 0;

	REG_LAYER1_VERT_POS = 0x0000;
	REG_LAYER2_VERT_POS = 0x8000;
}

static void drawTileGradient(int x, int y, int palette)
{
	int i;
	unsigned short *pTileRam = (unsigned short *)MEM_TILE0;

	pTileRam = (unsigned short *)MEM_TILE0 + (y * 64) + x;

	for (i = 0; i < 32; i++)
	{
		*pTileRam = 0x8000 | (((palette * 32) + i) * 128);
		pTileRam++;
	}
}

void drawTestPageTilemap(void)
{
	int i, j, k;
	unsigned short *pTileRam = (unsigned short *)MEM_TILE0;
	unsigned char *pCharRam = (unsigned char *)MEM_CHARS;
	unsigned short *pPal = (unsigned short *)MEM_PALETTE + (8 * 16);

	/* write tile */
	for (j = 0; j < 128; j++)
	{
		pCharRam = (unsigned char *)MEM_CHARS + (j * 32 * 128);

		for (i = 0; i < 32; i++)
		{
			*pCharRam = 1 | (1 << 4);
			pCharRam++;
		}
	}

	/* create palettes */
	for (j = 0; j < 24; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = gTestPalette[j];
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(j, j, j);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(j, 0, 0);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(0, j, 0);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(0, 0, j);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(j, 0, j);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(0, j, j);
			pPal++;
		}
	}

	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 16; i++)
		{
			*pPal = SET_RGB(j, j, 0);
			pPal++;
		}
	}


	/* draw gradient */
	for (j = 0; j < 8; j++)
	{
		drawTileGradient(4, 4 + (j * 4), j);
		drawTileGradient(4, 5 + (j * 4), j);
		drawTileGradient(4, 6 + (j * 4), j);
	}

	/*	setTextLayerPalette();
		build_colour_translation();
	*/
	printColourAt(1, 1, "COLOUR TEST SCREEN", 0);
	printColourAt(4, 3, "0", 0);
	printColourAt(34, 3, "31", 0);

	printColourAt(37, 5, "PALETTE", 0);
	printColourAt(37, 9, "WHITE", 0);
	printColourAt(37, 13, "RED", 0);
	printColourAt(37, 17, "GREEN", 0);
	printColourAt(37, 21, "BLUE", 0);
	printColourAt(37, 25, "MAGENTA", 0);
	printColourAt(37, 29, "CYAN", 0);
	printColourAt(37, 33, "YELLOW", 0);

	printColourAt(1, 40, "PRESS START TO CONTINUE", 6);
}











/* all black palette to blank screen */
void setBlackPalette(void)
{
	memset((unsigned char *)0x1800000, 0x4000, 0x00);
}

void setCarPalette(void)
{
	memcpy((unsigned char *)0x1802000, (unsigned char *)0x0284f4ec, 0x7de);
}

void setTextLayerPalette(void)
{
	memcpy((unsigned char *)0x1800000, (unsigned char *)initPalette, 0x20);
	memcpy((unsigned char *)0x1802000, (unsigned char *)initPalette, 0x20);
}

/* copy palette from rom data */
void getGamePalette(unsigned int *pData)
{
	int i, count;
	unsigned int *pDst;

	(unsigned int)pDst = *(unsigned int *)pData;
	pData++;
	count = *pData;
	pData++;

	memcpy(pDst, pData, count * 4);
}

/* place multiple copies of font bitmap in char ram with modified palette index colours */
void copyColourFont(void)
{
	int i, j;

	unsigned char *pChar = (unsigned char *)MEM_CHARS;
	unsigned short *pPal = (unsigned short *)MEM_PALETTE;

	/* copy same font 4x */
	for (i = 0; i < 8; i++)
	{
		memcpy(pChar, gFont, 128 * 32);
		pChar[(89 * 32)+29] = 0;			/* resolve pixel glitch in 'Y' character font data */
		pChar += 128 * 32;
	}

	/* create eight palettes */
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 16; j++)
		{
			switch (j)
			{
			case 0:
				*pPal = SET_RGB(5, 5, 5);
				break;

			case 1:
				switch (i)
				{
				case 0:		*pPal = SET_RGB(31, 31, 31);	break;			/* WHITE    */
				case 1:		*pPal = SET_RGB(0, 0, 31);		break;			/* BLUE     */
				case 2:		*pPal = SET_RGB(31, 0, 0);		break;			/* RED      */
				case 3:		*pPal = SET_RGB(31, 0, 31);		break;			/* MAGENTA  */
				case 4:		*pPal = SET_RGB(0, 31, 0);		break;			/* GREEN    */
				case 5:		*pPal = SET_RGB(0, 31, 31);		break;			/* CYAN     */
				case 6:		*pPal = SET_RGB(31, 31, 0);		break;			/* YELLOW   */
				case 7:		*pPal = SET_RGB(0, 0, 0);		break;			/* BLACK    */
				}
				break;

			case 2:
				if (i != 7) { *pPal = SET_RGB(0, 0, 0); }
				else { *pPal = SET_RGB(11, 11, 11); }
				break;
			}

			pPal++;
		}
	}
}

/* clear tile layer page with 'space' characters */
void clearTileMapNew(unsigned short* pTile)
{
	int i;
	for (i = 0; i < 64 * 64; i++)
	{
		*pTile = 0x8020;
		pTile++;
	}
}

/* clear all tile map data */
void clearTileMaps(void)
{
	/* clear all tile maps */
	clearTileMapNew((unsigned short*)MEM_TILE0);
	clearTileMapNew((unsigned short*)MEM_TILE1);
	clearTileMapNew((unsigned short*)MEM_TILE2);
	clearTileMapNew((unsigned short*)MEM_TILE3);

	memset((unsigned short*)0x01008000, 0, MEM_TILE_SIZE);
	memset((unsigned short*)0x0100a000, 0, MEM_TILE_SIZE);
	memset((unsigned short*)0x0100c000, 0, MEM_TILE_SIZE);
	memset((unsigned short*)0x0100e000, 0, MEM_TILE_SIZE);
}

/* clear specific tile map */
void clearTileMap(int index)
{
	switch (index)
	{
	case 0: clearTileMapNew((unsigned short*)MEM_TILE0); break;
	case 1: clearTileMapNew((unsigned short*)MEM_TILE1); break;
	case 2: clearTileMapNew((unsigned short*)MEM_TILE2); break;
	case 3: clearTileMapNew((unsigned short*)MEM_TILE3); break;
	}
}

/* user positionable ram viewer */
void ramViewer(unsigned int ramLocation)
{
	int i;
	unsigned int offset;

	offset = 0;

	while (1)
	{
		waitVBL();

		readIO();

		if ((gInputs & (1 << eIN_VR1)) == 0) offset -= 16;
		if ((gInputs & (1 << eIN_VR2)) == 0) offset += 16;
		if ((gInputs & (1 << eIN_VR3)) == 0) offset -= 256;
		if ((gInputs & (1 << eIN_VR4)) == 0) offset += 256;

		printColourAt(1, 1, (char *)gHeaderText, 6);
		printColourAt(4, 3, "RAM VIEWER: VR1 =-10, VR2 =+10, VR3 =-100, VR4 =+100 ", 5);

		displayMemory(2, 5, ramLocation + offset);
	}
}

/* view contents of ram at specified location */
void displayMemory(int x, int y, unsigned int offset)
{
	char temp[64];
	char ioByte;
	int i;
	volatile unsigned char* pAddr;
	int yoffset = y + 15;

	for (i = 0; i < 16; i++)
	{
		sprintf(temp, "%08x\0", offset + (i * 16));
		printAt(x, y + i, temp);
		printAt(x, yoffset + y + i, temp);
	}

	pAddr = (volatile unsigned char*)offset;
	for (i = 0; i < 256; i++)
	{
		ioByte = *pAddr; pAddr++;
		sprintf(temp, "%02x\0", ioByte);
		printAt(10 + x + ((i & 15) * 3), y + (i >> 4), temp);
	}

	pAddr = (volatile unsigned char*)offset;
	for (i = 0; i < 256; i++)
	{
		ioByte = *pAddr; pAddr++;
		printTileAt(10 + x + (i & 15), yoffset + y + (i >> 4), ioByte);
	}
}

void setPalette(int index, unsigned char r, unsigned char g, unsigned char b)
{
	*(unsigned short*)(0x1800000 + index) = (b << 10) | (g << 5) | r;
}

void printTileAt(int x, int y, int tile)
{
	unsigned short* pTileRam = (unsigned short*)0x01000000 + (0x40 * y) + x;
	*pTileRam = tile;
}

void printAt(int x, int y, char *pText)
{
	int i;
	int xorig = x;
	unsigned short *pTileRam;

	pTileRam = (unsigned short *)0x01000000 + (0x40 * y) + x;

	for (i = 0; i < strlen(pText); i++)
	{
		if (pText[i] == 0) break;
		if (pText[i] == '\n')
		{
			x = xorig;
			y++;

			pTileRam = (unsigned short *)0x01000000 + (0x40 * y) + x;
			i++;
		}

		*pTileRam = 0x8000 | pText[i];
		pTileRam++;
	}

}

void printColourAt(int x, int y, char *pText, int colour)
{
	int i;
	int xorig = x;
	unsigned short *pTileRam;

	pTileRam = (unsigned short *)MEM_TILE0 + (0x40 * y) + x;

	for (i = 0; i < strlen(pText); i++)
	{
		if (pText[i] == 0) break;
		if (pText[i] == '\n')
		{
			x = xorig;
			y++;

			pTileRam = (unsigned short *)MEM_TILE0 + (0x40 * y) + x;
			i++;
		}

		*pTileRam = 0x8000 | (pText[i] & 0x7f) | (colour << 7);
		pTileRam++;
	}

}

void setCharLinePalette(int x, int y, int length, int index)
{
	int i;
	unsigned short *pTileRam;
	unsigned short data;

	pTileRam = (unsigned short *)MEM_TILE0 + (0x40 * y) + x;

	for (i = 0; i < length; i++)
	{
		data = *pTileRam;

		data &= 0xffff ^ 0x0780;
		data |= index << 7;

		*pTileRam = data;
		pTileRam++;
	}
}

void printCharSet(void)
{
	int i, j;

	unsigned short* pTile = (unsigned short*)(MEM_TILE0 + (64 * 0));

	for (i = 0; i < 0x10000; i++)
	{
		*pTile = i;
		pTile++;
	}
}

void showGauge32(int x, int y, int count)
{
	int i;

	count >>= 3;

	for (i = 0; i < 32; i++)
	{
		if (count == i)
		{
			printTileAt(x + i + 1, y, 0x16);
		}
		else
		{
			printTileAt(x + i + 1, y, 0x15);
		}
	}

	printTileAt(x, y, 0x13);
	printTileAt((x + 33), y, 0x12);
}

void drawBox(int x, int y, int w, int h)
{
	int i;

	printTileAt(x, y, 0x18);
	printTileAt(x + w, y, 0x19);
	printTileAt(x, y + h, 0x1a);
	printTileAt(x + w, y + h, 0x1b);

	for (i = 1; i < w; i++)
	{
		printTileAt(x + i, y, 0x15);
		printTileAt(x + i, y + h, 0x15);
	}

	for (i = 1; i < h; i++)
	{
		printTileAt(x, y + i, 0x16);
		printTileAt(x + w, y + i, 0x16);
	}
}

/* set pixel in framebuffer */
void setPixelFramebuffer(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	*(volatile unsigned short*)(MEM_FRAMEBUFFER_A + ((512 * (y + 64)) + x) * 2) = (b << 10) | (g << 5) | r;
	*(volatile unsigned short*)(MEM_FRAMEBUFFER_B + ((512 * (y + 64)) + x) * 2) = (b << 10) | (g << 5) | r;

}

/* draw line in framebuffer */
void drawLineFramebuffer(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b)
{
	int sx, sy, e2, dx, dy, err;

	if ((x0 < 0) || (y0 < 0) || (x0 >= 512) || (y0 >= 384)) return;
	if ((x1 < 0) || (y1 < 0) || (x1 >= 512) || (y1 >= 384)) return;

	dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	err = dx + dy; /* error value e_xy */

	for (;;)
	{
		setPixelFramebuffer(x0, y0, r, g, b);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}

/* draw rectangle in framebuffer */
void drawRectFramebuffer(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b)
{
	int y;
	for (y = y0; y < y1; y++)
	{
		drawLineFramebuffer(x0, y, x1, y, r, g, b);
	}
}

/* render crosshatch and colour bars, model 2b & 2c hardware ONLY */
void drawCrosshatchFramebuffer(void)
{
	int x, y, i;

	REG_RENDERMODE = 0x05;		/* access FB, 60hz rate */

	/* draw vertical lines */
	for (x = -1; x < 511; x += 16)
	{
		drawLineFramebuffer(x, 0, x, 383, 31, 31, 31);
		drawLineFramebuffer(x + 1, 0, x + 1, 383, 31, 31, 31);
	}

	/* draw horizontal lines */
	for (y = -1; y < 383; y += 16)
	{
		drawLineFramebuffer(0, y, 511, y, 31, 31, 31);
		drawLineFramebuffer(0, y + 1, 511, y + 1, 31, 31, 31);
	}

	/* RGB colour bars */
	for (i = 0; i < 32; i++)
	{
		drawRectFramebuffer(64 + (i * 8), 80, 71 + (i * 8), 80 + 64, i, 0, 0);
		drawRectFramebuffer(64 + (i * 8), 80 + 64, 71 + (i * 8), 80 + 128, 0, i, 0);
		drawRectFramebuffer(64 + (i * 8), 80 + 128, 71 + (i * 8), 80 + 192, 0, 0, i);
	}

	/* CMYW colour bars */
	for (i = 0; i < 32; i++)
	{
		drawRectFramebuffer(64 + (i * 8), 288, 71 + (i * 8), 288 + 16, i, 0, i);
		drawRectFramebuffer(64 + (i * 8), 288 + 16, 71 + (i * 8), 288 + 32, i, i, 0);
		drawRectFramebuffer(64 + (i * 8), 288 + 32, 71 + (i * 8), 288 + 48, 0, i, i);
		drawRectFramebuffer(64 + (i * 8), 288 + 48, 71 + (i * 8), 288 + 64, i, i, i);
	}
}

#define numMandelRows	384
#define numMandelCols	512

int mandelbrotCalc(float x, float y)
{
	int maxIteration = 64;
	int iteration = 0;
	float re = 0;
	float im = 0;
	float temp;

	while ((re * re + im * im <= 4) && (iteration < maxIteration))
	{
		temp = re * re - im * im + x;
		im = 2 * re * im + y;
		re = temp;

		iteration++;
	}

	return iteration;
}

/* render a mandelbrot */
void drawMandelbrotFramebuffer(void)
{
	int i, j, color;
	float x, y;

	REG_RENDERMODE = 0x05;		/* access FB, 60hz rate */

	for (j = 0; j < numMandelRows; j++)
	{
		for (i = 0; i < numMandelCols; i++)
		{
			x = ((float)i / numMandelCols - 0.6f) * 4.0f;
			y = ((float)j / numMandelRows - 0.5f) * 4.0f;

			color = mandelbrotCalc(x, y);

			setPixelFramebuffer(i, j, color, color, color);
		}
	}
}

void copyLumaRAM(void)
{
	int i, count;
	unsigned char *pSource;
	unsigned int *pDestBuffer;

	count = *(unsigned int *)0x02802fc4;
	pSource = (unsigned char *)0x02802fc8;
	pDestBuffer = (unsigned int *)MEM_LUMARAM;

	count *= 0x80;

	for (i = 0; i < count; i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}
}


void copyTextureRamMap(int mapIndex)
{
	int i;
	unsigned short *pSource = (unsigned short *)MEM_TEXTURE_SRC_0;
	unsigned int *pDestBuffer;

	switch (mapIndex)
	{
	case 0:	pSource = (unsigned short *)MEM_TEXTURE_SRC_0; break;
	case 1:	pSource = (unsigned short *)MEM_TEXTURE_SRC_1; break;
	case 2:	pSource = (unsigned short *)MEM_TEXTURE_SRC_2; break;
	}

	pDestBuffer = (unsigned int *)MEM_TEXTURERAM_0;
	for (i = 0; i < 0x60000; i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}

	switch (mapIndex)
	{
	case 0:	pSource = (unsigned short *)MEM_TEXTURE_SRC_0_MIP; break;
	case 1:	pSource = (unsigned short *)MEM_TEXTURE_SRC_1_MIP; break;
	case 2:	pSource = (unsigned short *)MEM_TEXTURE_SRC_2_MIP; break;
	}

	pDestBuffer = (unsigned int *)MEM_TEXTURERAM_0_MIP;
	for (i = 0; i < 0x200 * 0x80; i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}
}

void copyTextureRamGeneral(void)
{
	int i;
	unsigned short *pSource;
	unsigned int *pDestBuffer;

	pSource = (unsigned short *)MEM_TEXTURE_SRC_3;
	pDestBuffer = (unsigned int *)MEM_TEXTURERAM_1;
	for (i = 0; i < 0x60000; i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}

	pSource = (unsigned short *)MEM_TEXTURE_SRC_3_MIP;
	pDestBuffer = (unsigned int *)MEM_TEXTURERAM_1_MIP;
	for (i = 0; i < 0x200 * 0x80; i++)
	{
		*pDestBuffer = *pSource;
		pSource++; pDestBuffer++;
	}
}









