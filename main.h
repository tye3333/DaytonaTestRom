/*

main.h

include hardware register defines

*/

#ifndef MAIN_H
#define MAIN_H

#define FLAG_IRQ_VBLANK			0x0001
#define FLAG_IRQ_SERIAL			0x0400

#define SET_RGB(r,g,b)   ((b <<  10) | (g << 5) | (r))

#define MEM_PROGRAM_ROM			0x00000000

#define MEM_DATA_ROM_IC6		0x02800000
#define MEM_DATA_ROM_IC7		0x02800002
#define MEM_DATA_ROM_IC8		0x02400000
#define MEM_DATA_ROM_IC9		0x02400002
#define MEM_DATA_ROM_IC10		0x02000000
#define MEM_DATA_ROM_IC11		0x02000002

#define MEM_DATA_ROM_IC6_SIZE	0x80000
#define MEM_DATA_ROM_IC7_SIZE	0x80000
#define MEM_DATA_ROM_IC8_SIZE	0x200000
#define MEM_DATA_ROM_IC9_SIZE	0x200000
#define MEM_DATA_ROM_IC10_SIZE	0x200000
#define MEM_DATA_ROM_IC11_SIZE	0x200000

#define MEM_MAIN				0x00200000
#define MEM_MAIN_SIZE			0x00020000

#define MEM_WORKRAM				0x00500000
#define MEM_WORKRAM_SIZE		0x00100000

#define MEM_TILE0				0x01000000
#define MEM_TILE1				0x01002000
#define MEM_TILE2				0x01004000
#define MEM_TILE3				0x01006000
#define MEM_TILE_SIZE			0x00002000
#define MEM_CHARS				0x01080000
#define MEM_CHARS_SIZE			0x00100000

#define MEM_LAYER1_HORIZ_POS	0x0100a000
#define MEM_LAYER2_HORIZ_POS	0x0100a004
#define MEM_LAYER1_VERT_POS		0x0100a008
#define MEM_LAYER2_VERT_POS		0x0100a00c

#define MEM_LAYER1_HORIZ_TABLE	0x01008000
#define MEM_LAYER2_HORIZ_TABLE	0x01008400

#define MEM_COPRO_CTRL			0x00980000			/* 0x80000000 = halt COPRO? */
#define MEM_COPRO_FIFO_CTRL		0x00980004			/* always 0xffffffff */
#define MEM_GEO_CTRL_W			0x00980008
#define MEM_VIDEO_CTRL			0x0098000c

#define MEM_HSYNC				0x01040000
#define MEM_VSYNC				0x01060000
#define MEM_SYNC_SWITCH			0x01070000

#define MEM_PALETTE				0x01800000
#define MEM_PALETTE_SIZE		0x00004000

#define MEM_COLORXLAT			0x01810000
#define MEM_COLORXLAT_SIZE		0x0000c000

#define MEM_ZCLIP				0x0181c000

#define MEM_COMM_SHARE			0x01a00000
#define MEM_COMM_SHARE_SIZE		0x00004000
#define MEM_COMM_CN				0x01a04000			/* bit 0 = enable (1) / disable (0) ? */
#define MEM_COMM_FG				0x01a04002			/* bit 0 = RW: master (1) / slave (0), bit 7 = RW? frame toggle flag  */

#define MEM_BACKUPRAM			0x01d00000
#define MEM_BACKUPRAM_SIZE		0x00004000

#define MEM_MAIN_DATA			0x02000000			/* to 0x02ffffff */
#define MEM_MAIN_DATA_SIZE		0x01000000

#define MEM_RENDERMODE			0x10000000			/* bit 0 = frame buffer access, bit 2 = refresh freq, 0 = 30hz, 1 = 60hz */

#define MEM_IO_DPRAM			0x01c00000
#define MEM_IO_DPRAM_SIZE		0x1000

#define MEM_LAMP_OUT			0x01c0001e
#define MEM_DRIVEBD_TX			0x01c00022
#define MEM_DRIVEBD_RX			0x01c00014

#define MEM_SERIAL_DATA			0x01c80000			/* model 2 orig daytona ONLY */
#define MEM_SERIAL_CMD			0x01c80002			/* model 2 orig daytona ONLY */
#define MEM_SERIAL_SIZE			4

#define MEM_FRAMEBUFFER_A		0x11600000
#define MEM_FRAMEBUFFER_B		0x11680000
#define MEM_FRAMEBUFFER_SIZE	0x80000

#define MEM_TEXTURERAM_0		0x12200000
#define MEM_TEXTURERAM_1		0x12600000			/* DOAA model 2C ??? */
#define MEM_TEXTURERAM_0_MIP	0x12380000
#define MEM_TEXTURERAM_1_MIP	0x12780000

#define MEM_TEXTURE_SRC_0		0x02200000			/* map */
#define MEM_TEXTURE_SRC_1		0x02300000			/* map */
#define MEM_TEXTURE_SRC_2		0x02400000			/* map */
#define MEM_TEXTURE_SRC_3		0x02500000			/* general textures such as cars? */

#define MEM_TEXTURE_SRC_0_MIP	0x022c0000			/* assuming these are mip maps for now */
#define MEM_TEXTURE_SRC_1_MIP	0x023c0000
#define MEM_TEXTURE_SRC_2_MIP	0x024c0000
#define MEM_TEXTURE_SRC_3_MIP	0x025c0000

#define MEM_LUMARAM				0x12800000
#define MEM_LUMARAM_SIZE		0x00020000

#define MEM_TGP_ID				0x00980030
#define MEM_TGP_ID_SIZE			0x00000010

#define MEM_SECURITY			0x01d80000
#define MEM_SECURITY_SIZE		0x00008000

#define MEM_CPU_WAIT_STATES		0x00e00000

#define MEM_MAIN_RAM_PROTECT	0x00f80000

#define REG_LAMP_OUT	MMIO_REG(MEM_LAMP_OUT, unsigned char)
#define REG_DRIVEBD_TX	MMIO_REG(MEM_DRIVEBD_TX, unsigned char)
#define REG_DRIVEBD_RX	MMIO_REG(MEM_DRIVEBD_RX, unsigned char)

#define MMIO_REG(addr, type)   (*(volatile type *)(addr))

#define GPIONEW					MMIO_REG(0x20000000, unsigned int)
#define GPIONEW1				MMIO_REG(0x20000008, unsigned int)
#define GPIONEW2				MMIO_REG(0x20000020, unsigned int)
#define TILERAM					MMIO_REG(0x01000000, unsigned short)

#define REG_HSYNC				MMIO_REG(MEM_HSYNC, unsigned short)
#define REG_VSYNC				MMIO_REG(MEM_VSYNC, unsigned short)
#define REG_RENDERMODE			MMIO_REG(MEM_RENDERMODE, unsigned short)

#define REG_IRQ_REQUEST			MMIO_REG(0x00e80000, unsigned int)
#define REG_IRQ_ENABLE			MMIO_REG(0x00e80004, unsigned int)

#define REG_TIMER0				MMIO_REG(0x00f00000, unsigned int)
#define REG_TIMER1				MMIO_REG(0x00f00004, unsigned int)
#define REG_TIMER2				MMIO_REG(0x00f00008, unsigned int)
#define REG_TIMER3				MMIO_REG(0x00f0000C, unsigned int)

#define REG_MAIN_RAM_PROTECT	MMIO_REG(MEM_MAIN_RAM_PROTECT, unsigned int)

#define REG_SERIAL_DATA			MMIO_REG(MEM_SERIAL_DATA, unsigned short)
#define REG_SERIAL_CMD			MMIO_REG(MEM_SERIAL_CMD, unsigned short)

#define REG_LAYER1_HORIZ_POS	MMIO_REG(0x0100a000, unsigned short)
#define REG_LAYER2_HORIZ_POS	MMIO_REG(0x0100a004, unsigned short)
#define REG_LAYER1_VERT_POS		MMIO_REG(0x0100a008, unsigned short)
#define REG_LAYER2_VERT_POS		MMIO_REG(0x0100a00c, unsigned short)

#define REG_LAYER1_HORIZ_TABLE	MMIO_REG(0x01008000, unsigned short)
#define REG_LAYER2_HORIZ_TABLE	MMIO_REG(0x01008400, unsigned short)

#define REG_TILE0				MMIO_REG(MEM_TILE0, unsigned short)


enum
{
	eIN_COIN1,		/* 00 */
	eIN_COIN2,		/* 01 */
	eIN_TEST,		/* 02 */
	eIN_SERVICE,	/* 03 */
	eIN_START,		/* 04 */
	eIN_VR1,		/* 05 */
	eIN_VR2,		/* 06 */
	eIN_VR3,		/* 07 */
	eIN_VR4,		/* 08 */
	eIN_u9,			/* 09 */
	eIN_ua,			/* 0A */
	eIN_ub,			/* 0B */
	eIN_GEAR24,		/* 0C */
	eIN_GEAR13,		/* 0D */
	eIN_GEAR34,		/* 0E */
	eIN_uf,			/* 0F */

	eIN_u10,		/* 10 */
	eIN_u11,		/* 11 */
	eIN_u12,		/* 12 */
	eIN_u13,		/* 13 */
	eIN_u14,		/* 14 */
	eIN_u15,		/* 15 */
	eIN_u16,		/* 16 */
	eIN_u17,		/* 17 */
	eIN_SW4,		/* 18 */
	eIN_SW3,		/* 19 */
	eIN_SW2,		/* 1A */
	eIN_SW1,		/* 1B */
	eIN_u1C,		/* 1C */
	eIN_u1D,		/* 1D */
	eIN_u1E,		/* 1E */
	eIN_u1F,		/* 1F */

};

extern char gHeaderText[];

extern int gErrorFrameBuffer;

extern unsigned int gInputs;
extern unsigned int gInputsTrig;
extern unsigned int gInputsLast;

unsigned int gAnalog[8];

extern char gTemp[256];

extern volatile unsigned int frameVBL;



extern void clearVRam(unsigned short* pRam, int size);
extern void waitCycles(int count);
extern void wait5000(void);

extern void testString(void);
extern void testRomPal(void);
extern void testFrameBuffer(void);

extern void copyMemory32(unsigned int* pSrc, unsigned int* pDst, int count);
extern void copyMemory16(unsigned short* pSrc, unsigned short* pDst, int count);

extern void initIO(void);
extern void readIO(void);

extern void procOSK(void);

extern void ramViewer(unsigned int ramLocation);

extern void waitButtonStart(void);

extern void frameBufferTests(void);

extern void waitVBL(void);

extern void waitButtonRelease(unsigned int buttonIndex);

extern void send_data_copro_unknown(void);
extern void copro_upload_code(void);
extern void wait_for_copro(void);
extern void	init_luma_ram(void);

extern void initSerialPort(void);
extern void sendSerialData(unsigned char txData);

extern void testSerialPort(void);

extern void geo_push_log_data(void);
extern void _build_colour_translation(void);

extern void copyColourFont(void);

extern void waitCopro(void);

extern void irq_vblank(void);
extern void switch_proc_pri_low(void);

extern void setupTextLayer(void);



#endif

