# Picasso
PIC16F877A based 5V/20MHz development board and PIC programmer

**[!] This is a discontinued project in 2016.**
_Documents are published in order to help people that want to create similar projects or think of new ideas._

# Idea

Programming Microchip's PIC (_peripheral interface controller_)'s and preparing 
electronic circuit board takes time and also inconvenient in circuit testing process. 
In other words, using the appropriate PIC programmer and creating the proper circuit board according to
desired PIC's datasheet is not practical for situations that needs quick actions like prototyping.
In addition to that there is platforms like [Arduino](https://www.arduino.cc), so PIC is not preferable
except embedded applications.
Therefore, idea behind this project is creating a development board/prototyping platform using PIC.

# How?

[Sprut](https://www.sprut.de) is a project that aims to produce PIC programmer hardware with 
serial, parallel and USB techniques. 
(see [Brenner](http://www.sprut.de/electronic/pic/brenner/index.htm))
_Sprut Brenner_ is a preferred and common programmer board for universal PIC applications.

_Brenner board that I was using:_
![sprut brenner](https://user-images.githubusercontent.com/24392180/54274226-33e89b00-4590-11e9-942c-0223aaa060f1.jpg)

There is a 40 pin programmer on board for supporting most of the PIC's. The board itself uses PIC18F2550
(see [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/39632e.pdf)) for communication
with software.

PIC16F877A (see [datasheet](http://ww1.microchip.com/downloads/en/devicedoc/39582b.pdf)) is a 40-pin microcontroller 
which is used frequently on PIC domain. (It has 256 bytes of EEPROM, 2 PWM 10-bit, ICD etc.)
This features of 877A was the main reason for my decision to use this microcontroller in this project.

![pic16f877a](https://user-images.githubusercontent.com/24392180/54274848-db1a0200-4591-11e9-8033-a9964c01a569.jpg)

In order to load your program to PIC (which is called _flashing hex file_) you have use a software called _US-Burn_.
US-Burn also supports various features such as flashing bootloaders, changing PIC configurations and disassembler.
It has Windows and Linux support.

![usburn](https://user-images.githubusercontent.com/24392180/54276031-e589cb00-4594-11e9-891a-142a71bb611a.jpg)

Basically if we replace the 40 pin programmer with 877A (or another PIC), put pins in order (like Arduino) and 
write a custom IDE with flashing features, we will have a 5V - 20MHz 877A-based development card. I'd like to call
it __PICasso.__

# v0.1

![v0 1](https://user-images.githubusercontent.com/24392180/54276419-fc7ced00-4595-11e9-8f1d-a17fbb2f0e0d.jpg)

Purpose of this version is seeing how easily can I use 877A's pins with this board.

* PIC16F877A
* 7805
* 1K & 220
* LED
* 4MHz crystal osc.
* 2 x 22pF