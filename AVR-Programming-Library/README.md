AVR-Programming-Library
=======================

Files that are re-used throughout the book *Make: AVR Programming*

* **pinDefines.h**
	Includes all of the pin definitions for the projects in *Make: AVR Programming*.  When you
	need to know what to hook up where, have a look here.

* **+USART.c** and **USART.h**
	These are the main utility files used for serial input/output in the book.  They're not
	particularly clever, but they have a tiny memory and resource footprint and get you
	up and running very quickly.

* **binaryMacro.h**
	If you're using a compiler other than a recent AVR-GCC, it may not support
	native binary digits (e.g. +0b00111010+).  If so, this include file has a pre-processor
	macro that will let you enter them similarly: +B8(01010101)+.  

* **macros.h**
	This file includes pre-processor macro definitions for bit-twiddling: 
	+set_bit+, +clear_bit+, and +toggle_bit+.  Useful for those days when you're feeling
	lazy, or have forgotten your bit-wise logic operations.

* **portpins.h**
	This is a recent version of the +portpins.h+ file included with the Arduino environment.
	It includes the AVR pin definitions of the form +PB1+ and similar.  The version included
	with Arduino is very old -- you'll want to replace it with this one if you want to write
	in C using the Arduino IDE.

