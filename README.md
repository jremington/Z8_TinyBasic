# Z8_TinyBasic
**C loader and Game of Life for MicroMint Z8 Basic/Debug board**

I dug my trusty old Z8 Micro out of storage, and it still works perfectly, after 42 years!

This post includes an implementation of John Conway's Game of Life that will run on the MicroMint Z8 Basic board (Zilog Z8 processor with 4K SRAM).
Code was modified from http://www.ittybittycomputers.com/ittybitty/tinybasic/

The C program to upload Basic code was developed using the Code::Blocks IDE as a Windows console program. For some reason, upload fails after about 2,000 characters of Basic code are transmitted, and I could not track down the cause, so the Game of Life code had to be stripped down to under that.
The C program reads a text file containing the Basic code, and transmits it, character by character (with appropriate delays) to the Z8 board via the serial port. The upload code strips :REM statements from the ends of Tiny Basic code lines, in order to save space. The loader assumes serial Baud rate 2400, set by switches on the Z8 board.

Note: to reserve the bottom portion of RAM for user data and/or machine language code, addressed using operators "@" and "^", add the size of the desired block to pointer R8-R9 (big endian!) and type NEW. On power up reset, R8-9 contains HEX 0800. For example, to reserve RAM locations 0-2047 for user data and 2048-4095 for the BASIC program and data, press the Z8 RESET button and type the following at the ":" prompt. Newly loaded code will be stored at RAM locations 2048 and above.
```
:@8=%10
:NEW
```
Tested with a 15x15 array, one Game of Life timestep on the Z8 takes about 4 minutes clock time.

Plenty of great info on the Z8 board and build in Basic/Debug at:

https://hc-ddr.hucki.net/wiki/doku.php/elektronik/z8671/basic_debug

http://cini.classiccmp.org/pdf/MicroMint/Z8_Microcomputer_Technical_Manual_1983.pdf

http://cini.classiccmp.org/pdf/MicroMint/Micromint_Z8_BasicComputer-Controller.pdf

https://www.mikrocontroller.net/attachment/448641/Z8_BASIC_Part_One.pdf

https://www.mikrocontroller.net/attachment/448642/Z8_BASIC_Part_Two.pdf

#NOTE: digital outputs are TTL levels, and may need pullup resistors to interface with CMOS inputs.
