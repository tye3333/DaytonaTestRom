/* tests.h */

#ifndef TESTS_H
#define TESTS_H

typedef struct
{
    int testType;
    unsigned int memloc;
    unsigned int memsize;
    char szName[16];

} sMemoryTest;

typedef struct
{
    unsigned int result;
    unsigned int memoryLocation;
    unsigned int dataActual;
    unsigned int dataWanted;
    unsigned int failFlag;

} sMemoryResult;

typedef struct
{
    unsigned int testIndex;
    char szName[14];
    char szLocation[12];
    char icA[5];
    char icB[5];
    char icC[5];
    char icD[5];
    unsigned char busWidth;
    char szChipType[11];
    char szSpeed[7];
} sChipList;

typedef struct
{
    unsigned int memloc;
    unsigned int size;
    char szName[6];

} sROMTests;

typedef struct
{
    int type;
    unsigned int checksum;
    unsigned int size;
    char szName[32];

} sROMTestLookup;


/*
 * TEST TYPES:
 *
 * 0  byte     8
 * 1  short   16
 * 2  int     32
 * 3  short with mask   0x00ff
 * 4  int with mask     0x000000ff
 * 5  int with mask     0x00ff00ff
 */

enum
{
    eMTT_WIDTH_8,
    eMTT_WIDTH_16,
    eMTT_WIDTH_32,
    eMTT_WIDTH_16_MASK_00FF,
    eMTT_WIDTH_32_MASK_000000FF,
    eMTT_WIDTH_32_MASK_00FF00FF,
};

/* Function prototypes */
void testFrameBuffer(void);
void testRomPal(void);
void testDisplayIO(void);

void displayIO(void);
void displayIO_DPRAM(int x, int y);

int quickTestRam(int type, unsigned int memloc);
int quickTestGeoRam(unsigned int memloc);

void doQuickRamTests(int quick);

int quickTestRam8(unsigned int memloc, unsigned int size);
int quickTestRam16(unsigned int memloc, unsigned int size);
int quickTestRam32(unsigned int memloc, unsigned int size);
int quickTestRamIODP(unsigned int memloc, unsigned int size);
int quickTestRamLuma(unsigned int memloc, unsigned int size);

int quickTestWorkRam(void);

char* getRomName(unsigned int crc);

void checkRomCRC(void);

unsigned int crc32(unsigned int crc, unsigned char* ptr, unsigned int buf_len, int hilo);

void printICList(void);

/* NEW: burn-in test */
void doBurnInRamTests(void);

#endif
