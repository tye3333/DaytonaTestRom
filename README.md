# DaytonaTestRom

Test program for Daytona arcade game on Model 2 hardware.

NOTE: Backup RAM is overwritten in this test but not the EEPROM on the IO board, so settings should be preserved.

USE THIS TEST PROGRAM ON YOUR HARDWARE AT YOUR OWN RISK! NO LIABILITY WILL BE ACCEPTED FOR ANY DAMAGE CAUSED BY THIS PROGRAM!

Take care when replacing the program eproms!

Tested for use on daytona.zip rom set for MAME, in emulation and on original hardware.

MAME ROM DESCRIPTION: Daytona USA (Japan, Revision A), Original Model 2 w/Model 1 sound board, Sega Game ID# 833-10651 DAYTONA TWIN, ROM board ID# 834-10798



# Tests

This program runs the following tests:

1) Quick (first 16 bytes) test of several important memory banks.
2) Full test of same memory banks.
3) Checksum test of data roms on rom board.
4) Colour test screen.
5) 3D Geometry test - only works in MAME at the moment.
6) IO board test.
7) Memory viewer - reading from program test rom.

Press START button to advance through tests - this assumes the IO board is present and correctly working in order for button presses to be detected.

This test rom will boot without the IO board connected, usually this results in a screen filled with corruption as the main program hangs while trying to read the configuration EEPROM on the IO board. It will not pass this stage without communication with the EEPROM, hence this test program can be useful as it bypasses this stage and gets on with diagnosing main & graphics board faults.


# How to install

For real hardware burn the following data to blank 27C1024 eproms:

roms/

epr-16722a.12

epr-16723a.13

Place epr-16722a.12 into the socket at IC12 and epr-16723a.13 into IC13 on the rom board. These are PROGRAM 0 & PROGRAM 1.


For demonstration purposes within MAME:

Copy the contents of daytona.zip romset into a folder named daytona/ within your usual mame roms/ folder. Copy the test rom files and OVERWRITE the same name files within the daytona/ folder. Rename the romset zip file to something like daytonaORIGINAL.zip to prevent mame from using that. Mame should then proceed to use the test roms within the daytona/ folder, it will however complain about the checksums being wrong. Skip that check, it's fine. Use -debug mode as well if it still complains. Tested with 64bit Mame v0.227 and it works fine.


# Future versions

Functionality to add:

1) IO board EEPROM read / write / testing.
2) Fix 3D geometry test to work on real hardware, unknown reason for why it doesn't currently work. Make it use geometry from other rom sets.
3) Find out if geometry processor can test further memory banks such as polygon ram, z-sort, stipple rams.
4) Sound board tests.
5) Force feedback board tests.
6) Add program upload facility to serial port.
7) Add further debug information to serial port.
8) Add memory test to CPU work ram before any stack use occurs, make the screen blink red for failure indication.
9) Add tests for texture memory to help indicate failed texture ram.
10) Make this test program work on other model 2 hardware (2A, 2B & 2C), currently only Daytona USA is supported.
11) BUG: Sometimes COMMS ram fails in MAME? Unknown why as the compare seems to hold the correct values.


# Compiling

Test program is coded in C, source code and i960 GCC compiler is included. Copy files to a directory anywhere on a Windows PC, run compile_TESTROM.bat and the i960 compiler should build a new set of eproms. Tested on Windows 11 okay.


# Debug port

Currently various debug prints are sent to the 8251 serial port used for MIDI messages to the sound board. A simple circuit can be made to connect to a USB->SERIAL for interaction with a PC.

Schematic PENDING.


# Additional info

The RAM tests were validated by socketting or removing chips on actual hardware.


