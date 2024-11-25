/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "copro.h"
#include "data.h"
#include "matrix.h"
#include "geometry.h"
#include "render.h"
#include "serial.h"
#include "tests.h"

char gHeaderText[] = { "DAYTONA TEST ROM V0.3 by STEVE J\0" };

volatile unsigned int frameVBL = 0;
unsigned int frameVBLlast = 0;

int gErrorFrameBuffer = 0;
int gErrorMainRam = 0;
int gErrorIO = 0;

unsigned int gInputs;
unsigned int gInputsTrig;
unsigned int gInputsLast;

unsigned int gAnalog[8];

char gTemp[256];

int main(void)
{
	/* clear interrupts and disable for now */
	REG_IRQ_REQUEST = -1 ^ (FLAG_IRQ_VBLANK | FLAG_IRQ_SERIAL);
	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;
	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;

	/* enable i960 interrupts */
	switch_proc_pri_low();
	
	/* reset timers */
	REG_TIMER0 = 0;
	REG_TIMER1 = 0;
	REG_TIMER2 = 0;
	REG_TIMER3 = 0;

	/* disable COMMS board */
	*(volatile unsigned char *)MEM_COMM_CN = 0;
	*(volatile unsigned char *)MEM_COMM_FG = 0;

	/* init hardware registers */
	REG_HSYNC = -84;
	REG_VSYNC = -2;
	REG_RENDERMODE = 0x04;		/* no access to FB, 60hz rate */

	initIO();

	setBlackPalette();

/*	quickTestWorkRam(); this corrupts stack - only used assembly in init.s */

	/* reset layer positions */
	REG_LAYER1_HORIZ_POS = 0x0000;
	REG_LAYER1_VERT_POS = 0x0000;
	REG_LAYER2_HORIZ_POS = 0x0000;
	REG_LAYER2_VERT_POS = 0x8000;

	/* reset horiz scroll tables */
	memset((unsigned int*)MEM_LAYER1_HORIZ_TABLE, 0, 0x400);
	memset((unsigned int*)MEM_LAYER2_HORIZ_TABLE, 0, 0x400);

	/* allow writes to MAIN ram */
	REG_MAIN_RAM_PROTECT = 0;

	/* initialise serial port for debug messages */
	initSerialPort();

	/* print debug strings to serial port */
	waitCycles(100);
	sendSerialString("SERIAL OK\r\n\r\n");
	waitCycles(100);
	sendSerialString((char *)gHeaderText);
	waitCycles(100);
	sendSerialString("\r\n\r\n");
	waitCycles(100);
	sendSerialString("QUICK RAM TEST (16 BYTES) STARTING...\r\n");

	/* enable network COMMS board for ram tests */
	*(volatile unsigned char *)MEM_COMM_CN = 1;
	*(volatile unsigned char *)MEM_COMM_FG = 1;

	/* do QUICK (256 bytes) ram tests */
	doQuickRamTests(1);

	waitCycles(100);
	sendSerialString("QUICK RAM TEST COMPLETED\r\n\r\n");

	printColourAt(1, 1, (char *)gHeaderText, 6);
	printColourAt(43, 1, "BUILD:", 3);
	printColourAt(50, 1, __DATE__, 3);
	printColourAt(53, 2, __TIME__, 3);

	printColourAt(4, 4, "QUICK RAM TEST (16 BYTES)", 5);
	printColourAt(1, 40, "PRESS START TO CONTINUE", 6);

	/* return text layer back to normal after ram testing overwrite */
	setupTextLayer();

	waitVBL();
	waitButtonStart();

	setBlackPalette();
	clearTileMaps();
	waitVBL();

	waitCycles(100);
	sendSerialString("FULL RAM TEST STARTING...\r\n");

	/* now do LONGER ram tests */
	doQuickRamTests(0);

	printColourAt(1, 1, (char *)gHeaderText, 6);
	printColourAt(4, 4, "FULL RAM TEST", 5);
	printColourAt(1, 40, "PRESS START TO CONTINUE", 6);

	waitCycles(100);
	sendSerialString("FULL RAM TEST COMPLETED\r\n\r\n");
	
	setupTextLayer();

	waitVBL();

	waitButtonStart();

	setBlackPalette();
	clearTileMaps();
	waitVBL();

	/* do ROM checksum test */
	checkRomCRC();

	waitVBL();
	waitButtonStart();

	/* show colour test page */
	clearTileMaps();
	drawTestPageTilemap();
	waitVBL();
	waitButtonStart();

	setBlackPalette();
	clearTileMaps();
	waitVBL();

/*	clear_geo_buffer();*/
	send_data_copro_unknown();
	copro_upload_code();
	/* wait_for_copro(); */
	waitCopro();

	/* copy textures to texture ram banks */
	copyTextureRamMap(0);
	copyTextureRamGeneral();
	copyLumaRAM();
	waitVBL();

/*	receiveSerialTest(); */

/*	frameBufferTests(); */

	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;
	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;

	setPalette(0, 8, 8, 31);
	clearGeoRAM();

	setPalette(0, 31, 8, 31);	
	initialiseGeoProcessor();
	
	setPalette(0, 8, 31, 8);
	waitVBL();

	setPalette(0, 8, 31, 31);
	REG_RENDERMODE = 0x04;

	setPalette(0, 31, 31, 8);
	clearTileMap(0);

	waitVBL();

	setupTextLayer();

	/* test geometry processor - render three rotating cars to screen */
	testGeo();

	/* clear geometry processor list */
	clearGEOList();

	REG_LAYER1_VERT_POS = 0x0000;
	REG_LAYER2_VERT_POS = 0x8000;

	clearTileMaps();

	/* test IO board inputs */
	testDisplayIO();

	clearTileMaps();

	/* wait for user to release all VR buttons */
	waitButtonRelease(eIN_VR1);
	waitButtonRelease(eIN_VR2);
	waitButtonRelease(eIN_VR3);
	waitButtonRelease(eIN_VR4);

	/* show contents of PROGRAM ROM */
	ramViewer(MEM_PROGRAM_ROM);

	/* display character set */
	printCharSet();

	/* test rom palette */
	testRomPal();

	return 0;
}

/* NOTE: not working on model 2 original or 2A, framebuffer tests only work on 2B and 2C */
void frameBufferTests(void)
{
	clearTileMap(0);

	REG_LAYER1_VERT_POS = 0x0000;
	REG_LAYER2_VERT_POS = 0x8000;

	testFrameBuffer();
	REG_RENDERMODE = 0x04;

	waitButtonStart();
	clearTileMap(0);

	drawCrosshatchFramebuffer();
	
	waitButtonStart();

	drawMandelbrotFramebuffer();
	
	waitButtonStart();
}

void waitVBL(void)
{
	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;
	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;

	waitCycles(50000);

	REG_IRQ_REQUEST = -1 ^ FLAG_IRQ_VBLANK;

	/* wait for IRQ0 to increment VBL frame counter */
/*	while(frameVBL == frameVBLlast) {;}*/

	frameVBLlast = frameVBL;

	swapGEOList();
}

void waitButtonStart(void)
{
	while (1)
	{
		waitCycles(100000);

		readIO();

		if ((gInputs & (1 << eIN_START)) == 0) break;
	}

	while (1)
	{
		waitCycles(100000);

		readIO();

		if ((gInputs & (1 << eIN_START)) != 0) break;
	}
}

void waitButtonRelease(unsigned int buttonIndex)
{
	while (1)
	{
		waitCycles(100000);

		readIO();

		if ((gInputs & (1 << buttonIndex)) != 0) break;
	}
}

void readIO(void)
{
	unsigned int inpInv;
	if (*(volatile unsigned char*)0x1c00040 == 0x00) *(volatile unsigned char*)0x1c00040 = 0x01;

	gInputs = *(volatile unsigned char*)0x1c0001c;	gInputs <<= 8;
	gInputs |= *(volatile unsigned char*)0x1c00014;	gInputs <<= 8;
	gInputs |= *(volatile unsigned char*)0x1c00012;	gInputs <<= 8;
	gInputs |= *(volatile unsigned char*)0x1c00010;

	gAnalog[0] = *(volatile unsigned char *)0x1c00000;
	gAnalog[1] = *(volatile unsigned char *)0x1c00002;
	gAnalog[2] = *(volatile unsigned char *)0x1c00004;
	gAnalog[3] = *(volatile unsigned char *)0x1c00006;
	gAnalog[4] = *(volatile unsigned char *)0x1c00008;
	gAnalog[5] = *(volatile unsigned char *)0x1c0000a;
	gAnalog[6] = *(volatile unsigned char *)0x1c0000c;
	gAnalog[7] = *(volatile unsigned char *)0x1c0000e;

	inpInv = gInputs ^ -1;

	gInputsTrig = -1 ^ (inpInv & (inpInv ^ gInputsLast));
	gInputsLast = inpInv;
}

void initIO(void)
{
	*(volatile unsigned char*)0x1c00040 = 0x00;

	*(volatile unsigned char*)0x1c0001c = 0xff;
	*(volatile unsigned char*)0x1c00014 = 0xff;
	*(volatile unsigned char*)0x1c00012 = 0xff;
	*(volatile unsigned char*)0x1c00010 = 0xff;

	*(volatile unsigned char*)0x1c0001e &= 0xf0;

	*(volatile unsigned char*)0x1c00022 = 0xff;
	*(volatile unsigned char*)0x1c00024 = 0x01;

	*(volatile unsigned char*)0x1c00034 = 'S';
	*(volatile unsigned char*)0x1c00036 = 'E';
	*(volatile unsigned char*)0x1c00038 = 'G';
	*(volatile unsigned char*)0x1c0003a = 'A';

	*(volatile unsigned char*)0x1c00040 = 0x01;
}

void wait5000(void)
{
	asm("st			r3,(sp)		");
	asm("addo		4,sp,sp		");
	asm("lda		0x1388,r3	");
	asm("loopWait:				");
	asm("cmpdeco	1,r3,r3		");
	asm("bl			loopWait	");
	asm("subo		4,sp,sp		");
	asm("ld 		(sp),r3		");
}

void waitCopro(void)
{
	unsigned int check = 0;
	unsigned int coproFlag;

	while (1)
	{
		coproFlag = *(volatile unsigned int *)REG_COPRO;

		if ((coproFlag & 0x00000001) != 0) break;

		check = *(volatile unsigned int *)0x884000;
	}

	*(volatile unsigned int *)REG_COPRO_CMD_CLEAR_STACK = 0xffffffff;					/* copro clear_stack */
}

void clearVRam(unsigned short *pRam, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		*pRam = 0x20;
		pRam++;
	}
}

void copyMemory32(unsigned int* pSrc, unsigned int* pDst, int count)
{
	int i;

	for (i = 0; i < count; i++)
	{
		*pDst = *pSrc;
		pSrc++;
		pDst++;
	}
}

void copyMemory16(unsigned short* pSrc, unsigned short* pDst, int count)
{
	int i;

	for (i = 0; i < count; i++)
	{
		*pDst = *pSrc;
		pSrc++;
		pDst++;
	}
}

