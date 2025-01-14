/* tests.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "geometry.h"
#include "render.h"
#include "tests.h"

/* -------------------------------------------------------------------
   GLOBALS / DATA
   (Existing globals from your original code)
 ------------------------------------------------------------------- */

sMemoryResult gTestResultList[24];

unsigned int gBadRAM = 0;
unsigned int gBadRAMContentsWanted = 0;
unsigned int gBadRAMContentsActual = 0;

unsigned char gTGPID[] =
{
    0, 'T', 'A', 'H',
    0, 'A', 'K', 'O',
    0, 'Z', 'A', 'K',
    0, 'M', 'T', 'K'
};

sMemoryTest g_MemoryTestList2[] =
{
    { eMTT_WIDTH_8,                 (unsigned int)gTemp,    256,                    "WORK (DATA)    \0", },   /* 00 */
    { eMTT_WIDTH_16,                MEM_BACKUPRAM,          MEM_BACKUPRAM_SIZE / 2, "BACKUP         \0", },   /* 01 */
    { eMTT_WIDTH_32,                MEM_MAIN,               MEM_MAIN_SIZE / 4,      "MAIN (PROGRAM) \0", },   /* 02 */
    { eMTT_WIDTH_16_MASK_00FF,      0x01c00100,             256,                    "IO DUAL PORT   \0", },   /* 03 */
    { eMTT_WIDTH_32,                MEM_COMM_SHARE | 0x2000,MEM_COMM_SHARE_SIZE / 8,"COMMS          \0", },   /* 04 */

    { eMTT_WIDTH_16,                MEM_TILE0,              MEM_TILE_SIZE / 2,      "TILE 0         \0", },   /* 05 */
    { eMTT_WIDTH_16,                MEM_TILE1,              MEM_TILE_SIZE / 2,      "TILE 1         \0", },   /* 06 */
    { eMTT_WIDTH_16,                MEM_TILE2,              MEM_TILE_SIZE / 2,      "TILE 2         \0", },   /* 07 */
    { eMTT_WIDTH_16,                MEM_TILE3,              MEM_TILE_SIZE / 2,      "TILE 3         \0", },   /* 08 */
    { eMTT_WIDTH_16,                MEM_CHARS,              MEM_CHARS_SIZE / 4,     "CHARS          \0", },   /* 09 */
    { eMTT_WIDTH_16,                MEM_PALETTE,            MEM_PALETTE_SIZE / 2,   "PALETTE        \0", },   /* 10 */
    { eMTT_WIDTH_32_MASK_00FF00FF,  MEM_COLORXLAT,          MEM_COLORXLAT_SIZE / 4, "COLORXLAT      \0", },   /* 11 */
    { eMTT_WIDTH_32_MASK_000000FF,  MEM_LUMARAM,            MEM_LUMARAM_SIZE / 4,   "LUMA           \0", },   /* 12 */

    { -1,   -1, -1,                                             "---            \0", },
    { eMTT_WIDTH_32,                MEM_GEORAM_0,            256,                "GEO BUFFER 0   \0", },    /* 13 */
    { eMTT_WIDTH_32,                MEM_GEORAM_1,            256,                "GEO BUFFER 1   \0", },    /* 14 */
    { -1,   -1, -1,                                             "END            \0", },
};

sChipList g_ChipList[] =
{
    { 0,    "WORK RAM     \0", "CPU BOARD  \0", "IC12\0", "IC13\0", "IC14\0", "IC15\0", 8,  "CY7C199   \0", "25ns  \0" },
    { 1,    "BACKUP RAM   \0", "CPU BOARD  \0", "IC65\0", "IC66\0", "    \0",  "    \0", 8,  "6264      \0", "100ns \0" },
    { 2,    "MAIN(PROGRAM)\0", "CPU BOARD  \0", "IC16\0", "IC17\0", "    \0",  "    \0", 16, "M5M44170AJ\0", "?     \0" },
    { 3,    "IO DUAL PORT \0", "GFX BOARD  \0", "IC64\0", "    \0", "    \0",  "    \0", 8,  "?         \0", "      \0" },
    { 4,    "COMMS RAM    \0", "COMMS BOARD\0", "IC33\0", "IC34\0", "IC35\0", "IC36\0", 8,  "LH5268AD  \0", "100ns \0" },
    { 5,    "TILE RAM     \0", "GFX BOARD  \0", "IC45\0", "IC46\0", "    \0",  "    \0", 8,  "65256BLFP \0", "      \0" },
    { 9,    "CHAR RAM     \0", "GFX BOARD  \0", "IC47\0", "IC48\0", "IC49\0", "IC50\0", 8,  "KM658128  \0", "80ns  \0" },
    { 10,   "PALETTE RAM  \0", "GFX BOARD  \0", "IC54\0", "IC55\0", "    \0",  "    \0", 8,  "CY7C185   \0", "25ns  \0" },
    { 11,   "COLORXLAT    \0", "GFX BOARD  \0", "IC57\0", "IC58\0", "IC59\0", "    \0", 8,  "CY7C185   \0", "25ns  \0" },
    { 12,   "LUMA RAM     \0", "GFX BOARD  \0", "IC43\0", "    \0", "    \0",  "    \0", 8,  "CY7C199   \0", "15ns  \0" },
};

sROMTests g_ROMTestList[] =
{
    { 0x2800000, 0x080000, "IC6  " },
    { 0x2800002, 0x080000, "IC7  " },
    { 0x2400000, 0x200000, "IC8  " },
    { 0x2400002, 0x200000, "IC9  " },
    { 0x2000000, 0x200000, "IC10 " },
    { 0x2000002, 0x200000, "IC11 " },
    { -1, -1, "     " },
};

sROMTests g_ROMTestList93[] =
{
    { 0x2800000, 0x100000, "IC6  " },
    { 0x2800002, 0x100000, "IC7  " },
    { 0x2400000, 0x200000, "IC8  " },
    { 0x2400002, 0x200000, "IC9  " },
    { 0x2000000, 0x200000, "IC10 " },
    { 0x2000002, 0x200000, "IC11 " },
    { -1, -1, "     " },
};

sROMTestLookup g_ROMTestLookup[] =
{
    { 0,    0x9ce591f6, 0x200000, "mpr-16528.10 (daytona)\0   " },
    { 0,    0xf7095eaf, 0x200000, "mpr-16529.11 (daytona)\0   " },
    { 0,    0x44f1f5a0, 0x200000, "mpr-16808.8  (daytona)\0   " },
    { 0,    0x37a2dd12, 0x200000, "mpr-16809.9  (daytona)\0   " },
    { 0,    0x469f10fd, 0x080000, "epr-16724a.6 (daytona)\0   " },
    { 0,    0xba0df8db, 0x080000, "epr-16725a.7 (daytona)\0   " },

    { 0,    0x7478f0d2, 0x080000, "epr-17371.6  (daytonase)\0 " },
    { 0,    0x308a06a9, 0x080000, "epr-17372.7  (daytonase)\0 " },

    { 0,    0x1bb0d72d, 0x100000, "epr-16534a.6 (daytona93)\0 " },
    { 0,    0x459a8bfb, 0x100000, "epr-16535a.7 (daytona93)\0 " },

    { 0,    0xa94d8690, 0x080000, "epr-17967.6  (daytonas)\0  " },
    { 0,    0x9d5a92c6, 0x080000, "epr-17968.7  (daytonas)\0  " },

    { 0,    0xac5fc501, 0x200000, "mpr-17164.10 (vcop)\0      " },
    { 0,    0x82296d00, 0x200000, "mpr-17165.11 (vcop)\0      " },
    { 0,    0x60ddd41e, 0x200000, "mpr-17162.8  (vcop)\0      " },
    { 0,    0x8c1f9dc8, 0x200000, "mpr-17163.9  (vcop)\0      " },
    { 0,    0x59091a37, 0x080000, "epr-17168a.6 (vcop)\0      " },
    { 0,    0x0495808d, 0x080000, "epr-17169a.7 (vcop)\0      " },

    { 0,    0x2ab491c5, 0x200000, "mpr-16974.10 (desert)\0    " },
    { 0,    0xe24fe7d3, 0x200000, "mpr-16975.11 (desert)\0    " },
    { 0,    0x23e53748, 0x200000, "mpr-16972.8  (desert)\0    " },
    { 0,    0x77d6f509, 0x200000, "mpr-16973.9  (desert)\0    " },
    { 0,    0x38b3e574, 0x080000, "epr-16978.6  (desert)\0    " },
    { 0,    0xc314eb8b, 0x080000, "epr-16979.7  (desert)\0    " },

    { -1,   -1, -1, "                               \0" },
};

 /* show palette data */
void testRomPal(void)
{
    char temp[64];
    int i;
    unsigned int palAddr;

    palAddr = *(volatile unsigned int*)0x2600000;

    memcpy((unsigned char*)MEM_PALETTE, (unsigned char*)palAddr, 256 * 2);

    sprintf(temp, "PALADDR: %08x = %08x\0", 0x2600000, palAddr);
    printAt(4, 10, temp);
}

void testDisplayIO(void)
{
    printColourAt(1, 1, (char*)gHeaderText, 6);
    printColourAt(4, 3, "INPUT / OUTPUT TEST", 5);
    printColourAt(1, 40, "PRESS VR1 + VR2 + VR3 + VR4 TO CONTINUE", 6);

    while (1)
    {
        waitVBL();
        readIO();

        if (((gInputs & (1 << eIN_VR1)) == 0) &&
            ((gInputs & (1 << eIN_VR2)) == 0) &&
            ((gInputs & (1 << eIN_VR3)) == 0) &&
            ((gInputs & (1 << eIN_VR4)) == 0))
        {
            return;
        }
        displayIO();
    }
}

void displayIO(void)
{
    int i, ypos, inputs = gInputs;
    char temp[16];
    char* pText = NULL;

    ypos = 5;

    printAt(5, ypos, "IO DPRAM:");
    ypos++;
    displayIO_DPRAM(4, ypos);
    ypos += 6;

    printAt(4, ypos, "    fedcba9876543210fedcba9876543210");
    ypos++;
    sprintf(temp, "IN: %08b\0", gInputs);
    printAt(4, ypos, temp);
    ypos += 2;

    sprintf(temp, "STEER: %02x   \nACCEL: %02x   \nBRAKE: %02x   \nAN3:   %02x\0",
        gAnalog[0], gAnalog[1], gAnalog[2], gAnalog[3]);
    printAt(4, ypos, temp);

    sprintf(temp, "AN4:   %02x   \nAN5:   %02x   \nAN6:   %02x   \nAN7:   %02x\0",
        gAnalog[4], gAnalog[5], gAnalog[6], gAnalog[7]);
    printAt(20, ypos, temp);
    ypos += 6;

    /* invert some bits in inputs for display */
    inputs ^= 0x7000;

    for (i = 0; i < 32; i++)
    {
        switch (i)
        {
        case eIN_COIN1:     pText = "COIN1\0";        break;
        case eIN_COIN2:     pText = "COIN2\0";        break;
        case eIN_TEST:      pText = "TEST\0";         break;
        case eIN_SERVICE:   pText = "SERVICE\0";      break;
        case eIN_START:     pText = "START\0";        break;
        case eIN_VR1:       pText = "VR1\0";          break;
        case eIN_VR2:       pText = "VR2\0";          break;
        case eIN_VR3:       pText = "VR3\0";          break;
        case eIN_VR4:       pText = "VR4\0";          break;
        case eIN_GEAR24:    pText = "GEAR24\0";       break;
        case eIN_GEAR13:    pText = "GEAR13\0";       break;
        case eIN_GEAR34:    pText = "GEAR34\0";       break;
        case eIN_SW1:       pText = "IO BRD SW1\0";   break;
        case eIN_SW2:       pText = "IO BRD SW2\0";   break;
        case eIN_SW3:       pText = "IO BRD SW3\0";   break;
        case eIN_SW4:       pText = "IO BRD SW4\0";   break;
        default:            pText = NULL;
        }

        if (pText)
        {
            sprintf(temp, "%s\0", pText);
            printAt(4, ypos, temp);

            if ((inputs & 1) == 0)
                printColourAt(18, ypos, "ON ", 4);
            else
                printColourAt(18, ypos, "OFF", 0);

            ypos++;
        }
        inputs >>= 1;
    }

    ypos++;

    sprintf(temp, "DRIVE RX: %02x  DRIVE TX: %02x  LAMPS: %02x\0",
        REG_DRIVEBD_RX, REG_DRIVEBD_TX, REG_LAMP_OUT);
    printAt(4, ypos, temp);
}

void displayIO_DPRAM(int x, int y)
{
    char temp[64];
    char ioByte;
    int i;
    volatile unsigned char* pIOAddr = (volatile unsigned char*)MEM_IO_DPRAM;

    for (i = 0; i < 64; i++)
    {
        ioByte = *pIOAddr;
        pIOAddr++;
        sprintf(temp, "%02x\0", ioByte);
        printAt(x + ((i & 15) * 3), y + (i >> 4), temp);
    }
}

int checkBusFailure(unsigned int dataActual, unsigned int dataWanted)
{
    int failFlag = 0;

    if ((dataActual & 0x000000ff) != (dataWanted & 0x000000ff)) failFlag |= 1;
    if ((dataActual & 0x0000ff00) != (dataWanted & 0x0000ff00)) failFlag |= 2;
    if ((dataActual & 0x00ff0000) != (dataWanted & 0x00ff0000)) failFlag |= 4;
    if ((dataActual & 0xff000000) != (dataWanted & 0xff000000)) failFlag |= 8;

    return failFlag;
}

/* -------------------------------------------------------------------
   doQuickRamTests() - Original function that does memory tests
 ------------------------------------------------------------------- */
void doQuickRamTests(int quick)
{
    int ypos = 6;
    int i = 0;
    int n;
    int size;
    int testIndex = 0;
    int result = 0;
    int runtestcount = 4;

    if (quick)
        runtestcount = 1;

    /* do ram tests */
    while (1)
    {
        if (g_MemoryTestList2[i].testType == -1)
            break;

        size = (quick == 0) ? g_MemoryTestList2[i].memsize : 16;
        result = 1;

        for (n = 0; n < runtestcount; n++)
        {
            gBadRAM = 0;
            gBadRAMContentsActual = 0;
            gBadRAMContentsWanted = 0;

            switch (g_MemoryTestList2[i].testType)
            {
            case 0:
                result &= quickTestRam8(g_MemoryTestList2[i].memloc, size);
                break;
            case 1:
                result &= quickTestRam16(g_MemoryTestList2[i].memloc, size);
                break;
            case 2:
                result &= quickTestRam32(g_MemoryTestList2[i].memloc, size);
                break;
            case 3:
                result &= quickTestRamIODP(g_MemoryTestList2[i].memloc, size);
                break;
            case 4:
                result &= quickTestRamLuma(g_MemoryTestList2[i].memloc, size);
                break;
            case 5:
                result &= quickTestRamColourXLAT(g_MemoryTestList2[i].memloc, size);
                break;
            }
        }

        /* record test results */
        gTestResultList[testIndex].result = result;
        gTestResultList[testIndex].memoryLocation = gBadRAM;
        gTestResultList[testIndex].dataActual = gBadRAMContentsActual;
        gTestResultList[testIndex].dataWanted = gBadRAMContentsWanted;
        gTestResultList[testIndex].failFlag = checkBusFailure(gBadRAMContentsActual, gBadRAMContentsWanted);

        i++;
        testIndex++;
    }

    /* Now we've done the tests; let's display the results. */

    /* Re-initialize the text layer so fonts & palette are correct */
    setupTextLayer();
    clearTileMaps();

    i = 0;
    testIndex = 0;
    while (1)
    {
        if (g_MemoryTestList2[i].testType == -1)
            break;

        result = gTestResultList[testIndex].result;
        gBadRAM = gTestResultList[testIndex].memoryLocation;
        gBadRAMContentsActual = gTestResultList[testIndex].dataActual;
        gBadRAMContentsWanted = gTestResultList[testIndex].dataWanted;

        sprintf(gTemp, "%02d  %s ", testIndex, &g_MemoryTestList2[i].szName);
        printAt(1, ypos, gTemp);
        sendSerialString(gTemp);

        if (result)
        {
            /* passed */
            printColourAt(20, ypos, "PASS", 4);
            sendSerialString("PASS\r\n");
        }
        else
        {
            /* failed */
            switch (g_MemoryTestList2[i].testType)
            {
            case 0:
                sprintf(gTemp, "FAILED @ %08x (%02x != %02x)",
                    gBadRAM, gBadRAMContentsActual, gBadRAMContentsWanted);
                break;
            case 2:
                sprintf(gTemp, "FAILED @ %08x (%08x != %08x)",
                    gBadRAM, gBadRAMContentsActual, gBadRAMContentsWanted);
                break;
            default:
                sprintf(gTemp, "FAILED @ %08x (%04x != %04x)",
                    gBadRAM, gBadRAMContentsActual, gBadRAMContentsWanted);
                break;
            }

            printColourAt(20, ypos, gTemp, 2);
            sendSerialString(gTemp);
            sendSerialString("\r\n");
        }

        i++;
        ypos++;
        testIndex++;
    }

    /* Now do special tests for GEO RAM, if applicable */
    i++;
    while (1)
    {
        if (g_MemoryTestList2[i].testType == -1)
            break;

        sprintf(gTemp, "%02d  %s", testIndex, &g_MemoryTestList2[i].szName);
        printAt(1, ypos, gTemp);

        if (quickTestGeoRam(g_MemoryTestList2[i].memloc))
        {
            printColourAt(20, ypos, "PASS", 4);
        }
        else
        {
            sprintf(gTemp, "FAILED @ %08x", gBadRAM);
            printColourAt(20, ypos, gTemp, 2);
        }

        i++;
        ypos++;
        testIndex++;
    }

    ypos += 2;
    printICList();
}

void printICList(void)
{
    int i;
    unsigned int failFlags;

    printColourAt(1, 23, "MEMORY", 5);
    printColourAt(15, 23, "LOCATION", 5);
    printColourAt(49, 23, "TYPE", 5);

    for (i = 0; i < 10; i++)
    {
        printColourAt(1, 25 + i, g_ChipList[i].szName, 0);
        printColourAt(15, 25 + i, g_ChipList[i].szLocation, 0);
        printColourAt(49, 25 + i, g_ChipList[i].szChipType, 0);

        failFlags = gTestResultList[g_ChipList[i].testIndex].failFlag;

        printColourAt(27, 25 + i, g_ChipList[i].icA, (failFlags & 1) ? 2 : 4);
        printColourAt(32, 25 + i, g_ChipList[i].icB, (failFlags & 2) ? 2 : 4);
        printColourAt(37, 25 + i, g_ChipList[i].icC, (failFlags & 4) ? 2 : 4);
        printColourAt(42, 25 + i, g_ChipList[i].icD, (failFlags & 8) ? 2 : 4);
    }
}

void testTGPID(void)
{
    int ypos = 6;
    int i = 0;
    char* pTGPAddr;
    int iTGPIDError = 0;

    pTGPAddr = (char*)MEM_TGP_ID;

    printAt(1, ypos, "TGP ID:");

    for (i = 0; i < MEM_TGP_ID_SIZE; i++)
    {
        sprintf(gTemp, "%02x\0", *pTGPAddr);
        printAt(12 + (i * 3), ypos, gTemp);
        sprintf(gTemp, "%c\0", *pTGPAddr);
        printAt(12 + (i * 1), ypos + 1, gTemp);

        if (gTGPID[i] != *pTGPAddr) iTGPIDError = 1;
        pTGPAddr++;
    }

    if (!iTGPIDError)
    {
        printAt(1, ypos + 3, "TGP ID PASS");
    }
    else
    {
        printAt(1, ypos + 3, "TGP ID FAILED");
    }
}

/* Quick test functions (8/16/32/etc.) */
int quickTestRam32(unsigned int memloc, unsigned int size)
{
    int i;
    volatile unsigned int* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0xAAAAAAAA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0xAAAAAAAA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAAAAAAAA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x55555555;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0x55555555)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x55555555;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = i;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != i)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = i;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    return 1;
}

int quickTestRam16(unsigned int memloc, unsigned int size)
{
    int i;
    unsigned short data;
    volatile unsigned short* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0xAAAA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0xAAAA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAAAA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x5555;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0x5555)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x5555;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    data = 0;
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = data;
        pIOAddr++;
        data++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    data = 0;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != data)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = data;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
        data++;
    }

    return 1;
}

int quickTestRam8(unsigned int memloc, unsigned int size)
{
    int i;
    unsigned char data;
    volatile unsigned char* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0xAA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0xAA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x55;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != 0x55)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x55;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    data = 0;
    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = data;
        pIOAddr++;
        data++;
    }

    data = 0;
    pIOAddr = (volatile unsigned char*)memloc;
    for (i = 0; i < size; i++)
    {
        if (*pIOAddr != data)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = data;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
        data++;
    }

    return 1;
}

int quickTestRamIODP(unsigned int memloc, unsigned int size)
{
    int i;
    volatile unsigned short* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0xAA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != 0xAA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x55;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != 0x55)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x55;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = i;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned short*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != i)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = i;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    return 1;
}

int quickTestRamLuma(unsigned int memloc, unsigned int size)
{
    int i;
    unsigned char data;
    volatile unsigned int* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0xAA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != 0xAA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAAAA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x55;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != 0x55)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x5555;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    data = 0;
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = data;
        pIOAddr++;
        data++;
    }

    data = 0;
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0xff) != data)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = data;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
        data++;
    }

    return 1;
}

int quickTestRamColourXLAT(unsigned int memloc, unsigned int size)
{
    int i, comp;
    volatile unsigned int* pIOAddr;

    /* test ODD bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x00AA00AA;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0x00ff00ff) != 0xAA00AA)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0xAA00AA;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test EVEN bits */
    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = 0x00550055;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0x00ff00ff) != 0x00550055)
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = 0x550055;
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
    }

    /* test incremental */
    pIOAddr = (volatile unsigned int*)memloc;
    comp = 0x00667788;
    for (i = 0; i < size; i++)
    {
        *pIOAddr = comp;
        pIOAddr++;
        comp += 0x0030507;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    comp = 0x00667788;
    for (i = 0; i < size; i++)
    {
        if ((*pIOAddr & 0x00ff00ff) != (comp & 0x00ff00ff))
        {
            gBadRAM = (unsigned int)pIOAddr;
            gBadRAMContentsWanted = (comp & 0x00ff00ff);
            gBadRAMContentsActual = *pIOAddr;
            return 0;
        }
        pIOAddr++;
        comp += 0x0030507;
    }

    return 1;
}

int quickTestGeoRam(unsigned int memloc)
{
    int i;
    volatile unsigned int* pIOAddr = (volatile unsigned int*)memloc;

    for (i = 0; i < 16; i++)
    {
        *pIOAddr = 0x07800000;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)memloc;
    for (i = 0; i < 16; i++)
    {
        if (*pIOAddr != 0x07800000) return 0;
        pIOAddr++;
    }

    return 1;
}

int quickTestWorkRam(void)
{
    int i, result = 1;
    volatile unsigned int* pIOAddr = (volatile unsigned int*)MEM_WORKRAM;
    unsigned int data = 0x55667788;

    for (i = 0; i < MEM_WORKRAM_SIZE / 4; i++)
    {
        *pIOAddr = data;
        data += 0x01030507;
        pIOAddr++;
    }

    pIOAddr = (volatile unsigned int*)MEM_WORKRAM;
    data = 0x55667788;

    for (i = 0; i < MEM_WORKRAM_SIZE / 4; i++)
    {
        if (*pIOAddr != data) result = 0;
        data += 0x01030507;
        pIOAddr++;
    }

    return result;
}

/* calculate CRC for a given memory region */
unsigned int crc32(unsigned int crc, unsigned char* ptr, unsigned int buf_len, int hilo)
{
    static const unsigned int s_crc_table[256] =
    {
      0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535,
      0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
      0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
      0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
      0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
      0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
      0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC,
      0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
      0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB,
      0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
      0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB,
      0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
      0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA,
      0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE,
      0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
      0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
      0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409,
      0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
      0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739,
      0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
      0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268,
      0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0,
      0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8,
      0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
      0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
      0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
      0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7,
      0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
      0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE,
      0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
      0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6,
      0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
      0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
      0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
      0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
      0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
      0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };

    unsigned int crc32 = (unsigned int)crc ^ 0xFFFFFFFF;
    unsigned char* pByte_buf = (unsigned char*)ptr;

    buf_len <<= 1;
    while (buf_len)
    {
        if (((unsigned int)pByte_buf & 2) == hilo) crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[0]) & 0xFF];
        ++pByte_buf;
        --buf_len;
    }

    return ~crc32;
}


/* lookup rom name from CRC */
char* getRomName(unsigned int crc)
{
    int index = 0;
    while (g_ROMTestLookup[index].type != -1)
    {
        if (crc == g_ROMTestLookup[index].checksum)
            return g_ROMTestLookup[index].szName;
        index++;
    }
    return "unknown\0";
}

/* display named list of program and data roms found */
void checkRomCRC(void)
{
    unsigned int crc;
    char* szName;
    int i;
    int colour;

    clearTileMaps();

    printColourAt(1, 1, (char*)gHeaderText, 6);
    printColourAt(4, 3, "ROM BOARD TEST: CPU DATA", 5);

    sendSerialString("ROM BOARD TEST: CPU DATA\r\n");

    for (i = 0; i < 6; i++)
    {
        sprintf(gTemp, "%s CRC: ", g_ROMTestList[i].szName);
        printAt(2, 5 + i, gTemp);
    }

    for (i = 0; i < 6; i++)
    {
        sprintf(gTemp, "%s CRC: --------", g_ROMTestList[i].szName);
        printAt(2, 5 + i, gTemp);

        crc = crc32(
            0,
            (unsigned char*)(g_ROMTestList[i].memloc & ~1),  /* Align to even boundary */
            g_ROMTestList[i].size,                                    /* in 16-bit words? */
            (g_ROMTestList[i].memloc & 2)                              /* hilo = 0 or 2 */
        );

        szName = getRomName(crc);
        if (szName[0] == 'u')
            colour = 2;
        else
            colour = 0;

        sprintf(gTemp, "%s CRC: %08x = %s", g_ROMTestList[i].szName, crc, szName);
        printColourAt(2, 5 + i, gTemp, colour);
        sendSerialString(gTemp);
        sendSerialString("\r\n");
    }

    printColourAt(1, 40, "PRESS START TO CONTINUE", 6);
    sendSerialString("ROM BOARD TEST: CPU DATA COMPLETED\r\n\r\n");
}

/* write and readback data to frame buffer ram */
void testFrameBuffer(void)
{
    int i;
    unsigned short* pBufferA;
    unsigned short* pBufferB;

    gErrorFrameBuffer = 0;

    REG_RENDERMODE = 0x04;  /* access FB, 60hz rate */

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        *pBufferA = 0;
        pBufferA++;

        *pBufferB = 0;
        pBufferB++;
    }

    REG_RENDERMODE = 0x05;  /* access FB, 60hz rate */

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        *pBufferA = i * 2;
        pBufferA++;

        *pBufferB = i * 2;
        pBufferB++;
        waitCycles(50);
    }

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        *pBufferA ^= 0xffff;
        pBufferA++;

        *pBufferB ^= 0xffff;
        pBufferB++;
        waitCycles(50);
    }

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        *pBufferA ^= 0xffff;
        pBufferA++;

        *pBufferB ^= 0xffff;
        pBufferB++;
    }

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        if (*pBufferA != ((i * 2) & 0xffff)) gErrorFrameBuffer |= 1;
        if (*pBufferB != ((i * 2) & 0xffff)) gErrorFrameBuffer |= 2;
        pBufferA++;
        pBufferB++;
        waitCycles(50);
    }

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        *pBufferA = 0;
        pBufferA++;

        *pBufferB = 0;
        pBufferB++;
    }

    pBufferA = (unsigned short*)MEM_FRAMEBUFFER_A;
    pBufferB = (unsigned short*)MEM_FRAMEBUFFER_B;

    for (i = 0; i < MEM_FRAMEBUFFER_SIZE / 2; i++)
    {
        if (*pBufferA != 0) gErrorFrameBuffer |= 1;
        if (*pBufferB != 0) gErrorFrameBuffer |= 2;
        pBufferA++;
        pBufferB++;
    }

    clearTileMap(0);

    printColourAt(1, 1, (char*)gHeaderText, 6);

    if ((gErrorFrameBuffer & 1) != 0)
    {
        printColourAt(2, 5, "FRAMEBUFFER 0 ERROR", 2);
    }
    else
    {
        printColourAt(2, 5, "FRAMEBUFFER 0 PASS", 4);
    }

    if ((gErrorFrameBuffer & 2) != 0)
    {
        printColourAt(2, 6, "FRAMEBUFFER 1 ERROR", 2);
    }
    else
    {
        printColourAt(2, 6, "FRAMEBUFFER 1 PASS", 4);
    }
}


/* -------------------------------------------------------------------
   NEW: BURN-IN TEST LOOP
   Repeatedly runs the full (not quick) memory tests until error or START
 ------------------------------------------------------------------- */
void doBurnInRamTests(void)
{
    int iteration = 0;
    int keepGoing = 1;
    int i;

    /* Re-initialize the text layer for correct palette, etc. */
    setupTextLayer();
    clearTileMaps();

    printColourAt(1, 1, "BURN-IN TEST (FULL MEMORY)", 6);
    printAt(1, 3, "PRESS AND HOLD VR4 TO EXIT BURN-IN TEST");

    for (i = 0; i < 500; i++)
    {
        waitVBL();
        readIO();
    }

    while (keepGoing)
    {
        iteration++;

        /* Perform a full memory test each loop */
        doQuickRamTests(0);

        /* Wait a frame, read inputs */
        waitVBL();
        readIO();

        /* If VR4 is pressed, exit the burn-in test */
        if ((gInputs & (1 << eIN_VR4)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 10, "VR4 PRESSED -> EXITING BURN-IN TEST...", 5);
            keepGoing = 0;
            for (i = 0; i < 500; i++)
            {
                waitVBL();
                readIO();
            }
        }
    }

    printColourAt(1, 12, "BURN-IN TEST ENDED.", 6);
    for (i = 0; i < 500; i++)
    {
        waitVBL();
        readIO();
    }
}

/* -------------------------------------------------------------------
   EXAMPLE MAIN TEST MENU
   If VR1 is pressed -> doBurnInRamTests()
 ------------------------------------------------------------------- */

void mainTestMenu(void)
{
    /* Re-initialize text layer to ensure correct palette, fonts, etc. */
    setupTextLayer();
    clearTileMaps();

    /* Print the menu instructions. */
    printColourAt(1, 1, "MAIN TEST MENU", 6);
    printAt(1, 3, "Press VR1 for BURN-IN memory test.");
    printAt(1, 4, "Press VR2 for QUICK memory test.");
    printAt(1, 5, "Press VR3 for FULL memory test.");
    printAt(1, 7, "Press START to return to main code.");  /* Press START to EXIT the program */

    /* Main loop for the menu */
    while (1)
    {
        waitVBL();
        readIO();

        /*
         * If START is pressed, exit back to the main code
         * (i.e., exit this menu altogether).
         */
        if ((gInputs & (1 << eIN_START)) == 0)
        {
            clearTileMaps();
            return;  /* Return from this function to main() */
        }

        /*--------------------------------------------------
          If VR1 is pressed -> do burn-in memory test
         --------------------------------------------------*/
        if ((gInputs & (1 << eIN_VR1)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "BURN-IN MEMORY TEST", 6);

            /*
             * Repeatedly runs full memory tests until error
             * OR you decide to exit (we'll fix that to VR4).
             */
            doBurnInRamTests();

            /*
             * After returning from burn-in, re-display main menu
             */
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "MAIN TEST MENU", 6);
            printAt(1, 3, "Press VR1 for BURN-IN memory test.");
            printAt(1, 4, "Press VR2 for QUICK memory test.");
            printAt(1, 5, "Press VR3 for FULL memory test.");
            printAt(1, 7, "Press START to return to main code.");
        }

        /*--------------------------------------------------
          If VR2 is pressed -> Quick memory test (one pass)
         --------------------------------------------------*/
        if ((gInputs & (1 << eIN_VR2)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "QUICK MEMORY TEST", 6);

            /* Run the quick test */
            doQuickRamTests(1);

            /*
             * Wait for VR4 to return to MENU,
             * rather than START. That’s the key change!
             */
            printAt(1, 23, "Press VR4 to return to MENU");
            while (1)
            {
                waitVBL();
                readIO();
                if ((gInputs & (1 << eIN_VR4)) == 0)
                {
                    break;  /* break out of this wait loop */
                }
            }

            /* Re-display main menu */
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "MAIN TEST MENU", 6);
            printAt(1, 3, "Press VR1 for BURN-IN memory test.");
            printAt(1, 4, "Press VR2 for QUICK memory test.");
            printAt(1, 5, "Press VR3 for FULL memory test.");
            printAt(1, 7, "Press START to return to main code.");
        }

        /*--------------------------------------------------
          If VR3 is pressed -> Full memory test (one pass)
         --------------------------------------------------*/
        if ((gInputs & (1 << eIN_VR3)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "FULL MEMORY TEST", 6);

            /* Run the full RAM test */
            doQuickRamTests(0);

            /*
             * Again, wait for VR4 to return to MENU,
             * so we don’t get confused with START.
             */
            printAt(1, 23, "Press VR4 to return to MENU");
            while (1)
            {
                waitVBL();
                readIO();
                if ((gInputs & (1 << eIN_VR4)) == 0)
                {
                    break;
                }
            }

            /* Optionally clear or set black palette */
            setBlackPalette();
            clearTileMaps();
            waitVBL();

            /* Re-display main menu */
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "MAIN TEST MENU", 6);
            printAt(1, 3, "Press VR1 for BURN-IN memory test.");
            printAt(1, 4, "Press VR2 for QUICK memory test.");
            printAt(1, 5, "Press VR3 for FULL memory test.");
            printAt(1, 7, "Press START to return to main code.");
        }
    }
}

void mainGraphicsTestMenu(void)
{
    /* small warm-up */
    int i;
    for (i = 0; i < 250; i++)
    {
        waitVBL();
        readIO();
    }

    /* Re-init text palette & font layer */
    setupTextLayer();
    clearTileMaps();

    /* NOTE: changed text to say "Press START to return to MAIN CODE" */
    printColourAt(1, 1, "GRAPHICS TEST MENU", 6);
    printAt(1, 3, "Press VR1 to TEST FRAMEBUFFER RAM");
    printAt(1, 4, "Press VR2 to TEST TEXTURE RAM");
    printAt(1, 6, "Press START to return to MAIN CODE");

    while (1)
    {
        waitVBL();
        readIO();

        /* 1) If START is pressed -> EXIT BACK TO main.c */
        /*    (we no longer check VR4 here) */
        if ((gInputs & (1 << eIN_START)) == 0)
        {
            clearTileMaps();
            return; /* Exits this function => back to main.c */
        }

        /* If VR1 is pressed -> run the Framebuffer test */
        if ((gInputs & (1 << eIN_VR1)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "TESTING FRAMEBUFFER RAM", 6);

            testFrameBufferN(); /* does RAM checks, sets rendermode, etc. */

            /* Wait for VR4 to return from sub-test */
            printAt(1, 23, "Press VR4 to return");
            while (1)
            {
                waitVBL();
                readIO();
                /* 2) Press VR4 => break out => show main menu again */
                if ((gInputs & (1 << eIN_VR4)) == 0)
                    break;

                /* 3) If START is pressed here, also return to main.c immediately */
                if ((gInputs & (1 << eIN_START)) == 0)
                {
                    clearTileMaps();
                    return; /* jump straight back to main.c */
                }
            }

            /* Re-display this menu after sub-test is done */
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "GRAPHICS TEST MENU", 6);
            printAt(1, 3, "Press VR1 to TEST FRAMEBUFFER RAM");
            printAt(1, 4, "Press VR2 to TEST TEXTURE RAM");
            printAt(1, 6, "Press START to return to MAIN CODE");
        }

        /* If VR2 is pressed -> run the Texture RAM test */
        if ((gInputs & (1 << eIN_VR2)) == 0)
        {
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "TESTING TEXTURE RAM", 6);

            testTextureRam(); /* read/write patterns in texture memory */

            /* Wait for VR4 to return from sub-test */
            printAt(1, 23, "Press VR4 to return");
            while (1)
            {
                waitVBL();
                readIO();
                /* Press VR4 => break out => main menu again */
                if ((gInputs & (1 << eIN_VR4)) == 0)
                    break;

                /* Or press START => exit to main.c immediately */
                if ((gInputs & (1 << eIN_START)) == 0)
                {
                    clearTileMaps();
                    return;
                }
            }

            /* Re-display the main menu text */
            setupTextLayer();
            clearTileMaps();
            printColourAt(1, 1, "GRAPHICS TEST MENU", 6);
            printAt(1, 3, "Press VR1 to TEST FRAMEBUFFER RAM");
            printAt(1, 4, "Press VR2 to TEST TEXTURE RAM");
            printAt(1, 6, "Press START to return to MAIN CODE");
        }
    }
}


/*
 * testTextureRam()
 *
 * This function attempts to do a "quick test" (fill and verify)
 * on the two main texture RAM banks, as well as optional MIP banks,
 * using a 16-bit write/read pattern (similar to doQuickRamTests).
 *
 * You may need to adapt the addresses, sizes, or set rendermode
 * for the original Model 2 hardware.
 */

 /* tests.c or wherever you place your new code */

void testTextureRam(void)
{
    int result0 = 1;
    int result1 = 1;
    int result0Mip = 1;
    int result1Mip = 1;
    char tempMsg[64];

    /*
     * According to MAME's model2.c:
     *   map(0x04000000, 0x041fffff) -> model2_3d_textureram
     * But only 0x20000 bytes (128 KB) is the "unique" portion,
     * repeated across the 2 MB region.
     *
     * We'll define "Bank 0" and "Bank 1" simply as two different
     * offsets inside that range. Similarly for MIP.
     * Feel free to adjust if you only want to test one contiguous block.
     */

#define TEXRAM_0_ADDR   (0x04000000)            /* Start of texture RAM */
#define TEXRAM_1_ADDR   (0x04010000)            /* Example: halfway in that 128 KB */
#define TEXRAM_0_MIP    (0x04008000)            /* Another offset for "MIP" portion */
#define TEXRAM_1_MIP    (0x04018000)            /* Another offset */
#define TEXRAM_SIZE     (0x20000)   /* 128 KB unique region in MAME */
#define TEXRAM_WORDSIZE (TEXRAM_SIZE / 2) /* For a 16-bit style test => 0x10000 words */

     /*
      * If your MIP region is actually smaller, you can reduce
      * the quickTestRam16(...) size.
      * For demonstration, we’re doing partial tests.
      */

      /* 1) Prepare screen for output */
    setupTextLayer();
    clearTileMaps();
    printColourAt(1, 1, "TEXTURE RAM TEST (MAME MODEL2)", 6);
    printAt(1, 3, "Testing main texture banks & MIP...");

    /*
     * 2) Possibly enable CPU read/write for texture RAM.
     *    On actual hardware, you might set:
     *      REG_RENDERMODE = 0x05
     *    So let's do that here just in case:
     */
    REG_RENDERMODE = 0x05;

    /*
     * 3) Test TEXTURE RAM BANK 0
     *    (the first half of MAME’s 128 KB unique region)
     */
    result0 = quickTestRam16(TEXRAM_0_ADDR, TEXRAM_WORDSIZE / 2);
    /* We pass half-size here:
       If you want to test all 128 KB at once, do
         quickTestRam16(TEXRAM_0_ADDR, TEXRAM_WORDSIZE);
       But let's say we do half.
    */
    if (!result0) {
        sprintf(tempMsg, "Texture RAM 0 FAIL @ %08x", gBadRAM);
        printColourAt(1, 5, tempMsg, 2);
    }
    else {
        printColourAt(1, 5, "Texture RAM 0 PASS", 4);
    }

    /*
     * 4) Test TEXTURE RAM BANK 1
     *    (the second half or a separate offset for demonstration)
     */
    result1 = quickTestRam16(TEXRAM_1_ADDR, TEXRAM_WORDSIZE / 2);
    if (!result1) {
        sprintf(tempMsg, "Texture RAM 1 FAIL @ %08x", gBadRAM);
        printColourAt(1, 6, tempMsg, 2);
    }
    else {
        printColourAt(1, 6, "Texture RAM 1 PASS", 4);
    }

    /*
     * 5) Test MIP TEXTURE RAM BANK 0
     *    Often smaller. If your code references
     *    MEM_TEXTURE_SRC_0_MIP, for example,
     *    you can do quickTestRam16(...) with a smaller size
     *    (like 0x8000 words if it’s 0x10000 bytes).
     */
    result0Mip = quickTestRam16(TEXRAM_0_MIP, (0x10000 / 2));
    /* Example: if MIP is 0x10000 bytes -> 0x8000 words */

    if (!result0Mip) {
        sprintf(tempMsg, "Texture MIP 0 FAIL @ %08x", gBadRAM);
        printColourAt(1, 7, tempMsg, 2);
    }
    else {
        printColourAt(1, 7, "Texture MIP 0 PASS", 4);
    }

    /*
     * 6) Test MIP TEXTURE RAM BANK 1
     */
    result1Mip = quickTestRam16(TEXRAM_1_MIP, (0x10000 / 2));
    if (!result1Mip) {
        sprintf(tempMsg, "Texture MIP 1 FAIL @ %08x", gBadRAM);
        printColourAt(1, 8, tempMsg, 2);
    }
    else {
        printColourAt(1, 8, "Texture MIP 1 PASS", 4);
    }

    /*
     * 7) Optionally restore normal render mode
     *    so CPU can't overwrite texture RAM unexpectedly
     */
    REG_RENDERMODE = 0x04;

    /*
     * 8) Done - let user see results, maybe wait for input
     */
    printAt(1, 10, "TEXTURE RAM TEST COMPLETE");
    /* Instead of "Press VR4 to exit," we want START here: */
    printAt(1, 12, "Press VR4 to return");

    /*
     * 9) Wait for user to press START
     *    (Replace eIN_VR4 with eIN_START, or whichever bit is your START button)
     */
    while (1)
    {
        waitVBL();
        readIO();
        if ((gInputs & (1 << eIN_VR4)) == 0) /* 0 means pressed */
            break;
    }

    /*
     * 10) Clear screen and jump back to mainGraphicsTestMenu
     *     so user sees the original menu again.
     */
    setupTextLayer();
    clearTileMaps();
    mainGraphicsTestMenu();  /* call the menu function */
    return;                  /* done */
}

void testFrameBufferN(void)
{
    /*
     * We'll assume each frame buffer is ~0x60000 bytes in size,
     * corresponding to 512×384 (196,608 pixels) at 2 bytes per pixel.
     * In 16-bit words, that’s 0x30000 words.
     *
     * If your code already has #defines like MEM_FRAMEBUFFER_A,
     * use them. Otherwise, define them:
     */
#define FB_A_ADDR       0x01200000    /* Example address, adapt as needed */
#define FB_B_ADDR       0x01260000    /* Example address, adapt as needed */
#define FB_WORDSIZE     (0x60000 / 2) /* 0x30000 16-bit words */

    int resultA = 1;
    int resultB = 1;
    char tempMsg[64];

    /* Prepare text layer so you can see messages */
    setupTextLayer();
    clearTileMaps();

    printColourAt(1, 1, "FRAMEBUFFER TEST", 6);
    printAt(1, 3, "Testing Frame Buffer A & B in 16-bit writes...");

    /*
     * 1) Enable CPU read/write to frame buffer
     * Typically set REG_RENDERMODE = 0x05 (or 0x05 + additional bits)
     * on Model 2 to allow direct CPU access
     */
    REG_RENDERMODE = 0x05;

    /*
     * 2) Test Frame Buffer A
     * Using your "quickTestRam16"-style pattern:
     *   - fill with 0xAAAA, check
     *   - fill with 0x5555, check
     *   - fill incremental, check
     */
    resultA = quickTestRam16(FB_A_ADDR, FB_WORDSIZE);
    if (!resultA)
    {
        /* Using global gBadRAM/gBadRAMContentsWanted if your code sets them */
        sprintf(tempMsg, "FB A FAIL @ %08x (got %04x, wanted %04x)",
            gBadRAM, gBadRAMContentsActual, gBadRAMContentsWanted);
        printColourAt(1, 5, tempMsg, 2); /* Red text for fail */
    }
    else
    {
        printColourAt(1, 5, "Frame Buffer A PASS", 4); /* Green text for pass */
    }

    /*
     * 3) Test Frame Buffer B
     */
    resultB = quickTestRam16(FB_B_ADDR, FB_WORDSIZE);
    if (!resultB)
    {
        sprintf(tempMsg, "FB B FAIL @ %08x (got %04x, wanted %04x)",
            gBadRAM, gBadRAMContentsActual, gBadRAMContentsWanted);
        printColourAt(1, 6, tempMsg, 2);
    }
    else
    {
        printColourAt(1, 6, "Frame Buffer B PASS", 4);
    }

    /*
     * 4) If needed, revert back to normal rendering
     * (Set REG_RENDERMODE = 0x04 typically means no CPU access to FB.)
     */
    REG_RENDERMODE = 0x04;

    /*
     * 5) Print final message or prompt user to exit
     */
    printAt(1, 8, "FRAMEBUFFER TEST COMPLETE");
    /* Instead of "Press START to exit," we want START here: */
    printAt(1, 12, "Press VR4 to return");

    /*
     * 9) Wait for user to press START
     *    (Replace eIN_VR4 with eIN_START, or whichever bit is your START button)
     */
    while (1)
    {
        waitVBL();
        readIO();
        if ((gInputs & (1 << eIN_VR4)) == 0) /* 0 means pressed */
            break;
    }

    /*
     * 10) Clear screen and jump back to mainGraphicsTestMenu
     *     so user sees the original menu again.
     */
    setupTextLayer();
    clearTileMaps();
    mainGraphicsTestMenu();  /* call the menu function */
    return;                  /* done */
}