cls
del temp\*.s

set G960BASE=.
set G960ARCH=KB
set G960LIB=lib
set G960LLIB=lib

.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\main.s main.c
.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\geometry.s geometry.c
.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\matrix.s matrix.c
.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\render.s render.c
.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\serial.s serial.c
.\bin\gcc960.exe -S -Ttestlinkrom -AKB -DGNU960 -lcca -lll -O2 -o temp\tests.s tests.c

.\bin\gas960.exe temp\main.s -o main.o
.\bin\gas960.exe temp\geometry.s -o geometry.o
.\bin\gas960.exe temp\matrix.s -o matrix.o
.\bin\gas960.exe temp\render.s -o render.o
.\bin\gas960.exe temp\serial.s -o serial.o
.\bin\gas960.exe temp\tests.s -o tests.o

.\bin\gas960.exe kx_init.s -o kx_init.o
.\bin\gas960.exe kx_ftbl.s -o kx_ftbl.o
.\bin\gas960.exe i_table.s -o i_table.o
.\bin\gas960.exe i_handle.s -o i_handle.o

.\bin\gld960 -o testrom.o -S -AKB -z -m -Ntemp\testrom.map -Ttestlinkrom -lckb -lmkb -lll -c kx_init.o kx_ftbl.o i_table.o i_handle.o geometry.o matrix.o render.o serial.o tests.o main.o

.\bin\grom960.exe -i -l 0x200000 -o roms\testrom.bin testrom.o

.\bin\romwakker.exe /p roms\testrom.bin roms\testromFull.bin 256 0
.\bin\romwakker.exe /f roms\testromFull.bin roms\testromFullFlipped.bin
.\bin\romwakker.exe /w roms\testromFullFlipped.bin roms\epr-16723a.13 roms\epr-16722a.12

del roms\testrom*.*
del *.o

