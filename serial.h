/* serial.h */

#ifndef SERIAL_H
#define SERIAL_H


/* flags for 8251 status read */
#define FLAG_8251_DSR				0x80
#define FLAG_8251_SYNDET			0x40
#define FLAG_8251_FRAME_ERROR		0x20
#define FLAG_8251_OVERRUN_ERROR		0x10
#define FLAG_8251_PARITY_ERROR		0x08
#define FLAG_8251_TX_EMPTY			0x04
#define FLAG_8251_RX_READY			0x02
#define FLAG_8251_TX_READY			0x01


/* command bits for 8251 */
#define CMD_8251_ENTER_HUNT			0x80
#define CMD_8251_RESET				0x40
#define CMD_8251_RTS				0x20
#define CMD_8251_ERROR_RESET		0x10
#define CMD_8251_SEND_BREAK			0x08
#define CMD_8251_RX_ENABLE			0x04
#define CMD_8251_DTR				0x02
#define CMD_8251_TX_ENABLE			0x01


extern int rxCount;
extern int serialIntCount;
extern int serialX;
extern int serialY;


extern void initSerialPort(void);
extern void testSerialPort(void);

extern void sendSerialData(unsigned char txData);
extern void sendSerialString(char *pszString);

extern void handleSerialIRQ(void);

extern void receiveSerialTest(void);













#endif


