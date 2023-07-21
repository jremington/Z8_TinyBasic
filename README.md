# Z8_TinyBasic
Game of Life and C loader for MicroMint Z8 Basic/Debug board

This is an implementation of John Conway's Game of Life that will run on the MicroMint Z8 Basic board (Zilog Z8 processor with 4K SRAM).
Code was modified from http://www.ittybittycomputers.com/ittybitty/tinybasic/

For some reason, upload fails after about 2,000 characters of Basic code are transmitted, so the code had to be stripped down to under that.
The C program reads a text file containing the Basic code, and transmits it, character by character (with appropriate delays) to the Z8 board via the serial port. The upload code strips :REM statements from the ends of Tiny Basic code lines, in order to save space. Serial Baud rate 2400 set by switches (assumed).

Tested with a 15x15 array, the Game of Life timestep on the Z8 takes about 4 minutes clock time.

Plenty of great info on the Z8 board and build in Basic/Debug at:

https://hc-ddr.hucki.net/wiki/doku.php/elektronik/z8671/basic_debug

http://cini.classiccmp.org/pdf/MicroMint/Z8_Microcomputer_Technical_Manual_1983.pdf

http://cini.classiccmp.org/pdf/MicroMint/Micromint_Z8_BasicComputer-Controller.pdf

https://www.mikrocontroller.net/attachment/448641/Z8_BASIC_Part_One.pdf

https://www.mikrocontroller.net/attachment/448642/Z8_BASIC_Part_Two.pdf
