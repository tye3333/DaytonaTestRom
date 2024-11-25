/*

serial.c 

8251 serial uart handler for comms port to sound board, potentially useful as debug monitor and program upload


*/

#include "main.h"
#include "serial.h"

int rxCount = 0;
int serialIntCount = 0;
int serialX = 0, serialY = 0;

void initSerialPort(void)
{
	/* baud 31250 (MIDI) */
	REG_SERIAL_CMD = 0x4e;
	waitCycles(5000);

	REG_SERIAL_CMD = 0x37;
	waitCycles(5000);
}

void sendSerialData(unsigned char txData)
{
	int i = 0;

	/* wait for TX EMPTY */
	while ((REG_SERIAL_CMD & FLAG_8251_TX_EMPTY) == 0)
	{
		if (i > 0x4000) break;

		i++;
	}

	REG_SERIAL_DATA = txData;
	REG_SERIAL_CMD = CMD_8251_RTS | CMD_8251_ERROR_RESET | CMD_8251_RX_ENABLE | CMD_8251_DTR | CMD_8251_TX_ENABLE;

	waitCycles(2000);
}

void sendSerialString(char *pszString)
{
	while (*pszString)
	{
		sendSerialData(*pszString);
		pszString++;
	}
}

void testSerialPort(void)
{
	int i = 0, status, rxData;

	printColourAt(1, 2, "TESTING SERIAL - PRESS START", 3);

	initSerialPort();

	waitCycles(1000);

	sendSerialString("MODEL 2 HARDWARE TEST ROM\r\n");

	waitCycles(1000);

	REG_IRQ_ENABLE = FLAG_IRQ_VBLANK;

	serialX = 0; serialY = 0;

	while (1)
	{
		sendSerialData(i);

		waitCycles(100);
		status = REG_SERIAL_CMD;
		waitCycles(100);
		rxData = REG_SERIAL_DATA;
		waitCycles(100);

		sprintf(gTemp, "STATUS: %08b, RX: %08b, RX COUNT: %d\0", status, rxData, rxCount);
		printAt(4, 5, gTemp);

		sprintf(gTemp, "COUNT:  %d\0", serialIntCount);
		printAt(4, 6, gTemp);


		if ((status & FLAG_8251_RX_READY) != 0)
		{
			rxData = REG_SERIAL_DATA;

			printTileAt(serialX, 12 + serialY, rxData);

			serialX++;
			if (serialX > 40)
			{
				serialX = 0;
				serialY++;

				if (serialY > 20) serialY = 0;
			}

			rxCount++;
			REG_SERIAL_CMD = CMD_8251_RTS | CMD_8251_ERROR_RESET | CMD_8251_RX_ENABLE | CMD_8251_DTR | CMD_8251_TX_ENABLE;
		}

		/*
		printBinaryCharAt(3, 7, status);
		printBinaryCharAt(3 + 10, 7, rxData);
		printTileAt(3 + 20, 7, rxData);
		*/

		i++;

		readIO();

		if ((gInputs & (1 << eIN_START)) == 0) break;

		sprintf(gTemp, "FRAME:  %08d\0", frameVBL);
		printAt(4, 4, gTemp);

		waitVBL();
	}

	for (i = 0; i < 60; i++)
	{
		sendSerialData(i & 0xff);
		waitVBL();
	}
}

void handleSerialIRQ(void)
{
	int status, rxData;

	REG_TILE0++;
	serialIntCount++;

	status = REG_SERIAL_CMD;

	if ((status & FLAG_8251_RX_READY) != 0)
	{
		rxData = REG_SERIAL_DATA;

		printTileAt(serialX, 12 + serialY, rxData);

		serialX++;
		if (serialX > 40)
		{
			serialX = 0;
			serialY++;

			if (serialY > 20) serialY = 0;
		}

		rxCount++;
	}

	REG_SERIAL_CMD = CMD_8251_RTS | CMD_8251_ERROR_RESET | CMD_8251_RX_ENABLE | CMD_8251_DTR | CMD_8251_TX_ENABLE;
}

unsigned char receiveBuffer[256];
int receiveBufferIndex;

void receiveSerialTest(void)
{
	unsigned char status, rxData;

	serialX = 0;
	serialY = 0;
	rxCount = 0;
	
	printColourAt(2, 0, "SERIAL RECEIVE TEST", 6);

	sendSerialString("SERIAL RECEIVE TEST\r\n");
	
	while(1)
	{
		receiveBufferIndex = 0;
		receiveBuffer[receiveBufferIndex] = 0;

		while(1)
		{
			while(1)
			{
				status = REG_SERIAL_CMD;
				if ((status & FLAG_8251_RX_READY) != 0) break;
			}

			rxData = REG_SERIAL_DATA;

			if (rxData == 0x0d)
			{
				receiveBuffer[receiveBufferIndex] = 0;
				receiveBuffer[receiveBufferIndex+1] = 0;
				receiveBuffer[receiveBufferIndex+2] = 0;
				break;
			}
			else if (rxData == 0x0a)
			{;}
			else
			{
				receiveBuffer[receiveBufferIndex] = rxData;
				receiveBufferIndex++;
				/*rxCount++;*/
			}
		}

		printAt(0, serialY + 2, receiveBuffer);
		serialY++;
		if (serialY >= 40) serialY = 0;

/*		sprintf(gTemp, "%02x %02x %02d\0", status, rxData, serialY);
		printAt(40, 0, gTemp);

		sendSerialString(receiveBuffer);
		sendSerialString("\r\n");
*/

	}
/*
		sprintf(gTemp, "%02x %02x\0", status, rxData);
		printAt(40, 0, gTemp);

		printTileAt(serialX, serialY + 2, rxData);

		serialX++;
		if (serialX > 40)
		{
			serialX = 0;
			serialY++;

			if (serialY >= 40) serialY = 0;
		}

		rxCount++;

		sprintf(gTemp, "RXCOUNT: %08d\0", rxCount);
		printAt(20, 0, gTemp);
*/
/*		REG_SERIAL_CMD = CMD_8251_RTS | CMD_8251_ERROR_RESET | CMD_8251_RX_ENABLE | CMD_8251_DTR | CMD_8251_TX_ENABLE; */


}






































