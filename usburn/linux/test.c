/***************************************************************************
 *            test.c
 *
 *  Sund Aug 31 18:57:43 2008
 *  Copyright  2008  Joerg Bredendiek (sprut)
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


/*
 * this file contains everything for the interactive test of the hardware
 * the following functions can be called from outside:
 *
 * int test_hardware(void)
 */

//********************************************************************************************************************
// includes
//********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include "b8.h"



//********************************************************************************************************************
// Routinen zum Testen der Brennerhardware
//********************************************************************************************************************

int test_hardware(void)
{
	char taste;
	char vdd = 4;
	char vpp = 10;
	
	if (prog.device == DEVICE_B9)
	{
		char vdd = 3;
		char vpp = 3;
	}


	prog_set_socket(SOC_ICSP);

	puts(" ");
	puts(" INTERACTIVE TEST OF THE HARDWARE ");
	puts(" ");
	puts("+++ W A R N I G  / W A R N U N G +++");
	puts("Remove PIC from testsocket and ICSP-connector! ");
	puts("PIC aus Testfassung und ICSP-Verbinder entfernen! ");
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	puts(" Part1: for all programmers");
	puts(" ");
	puts(" ");


	prog_set_led(LED1_OFF);		//green
	prog_set_led(LED2_OFF);		//yellow

	prog_set_signal(SIG_VDD_OFF);
	prog_set_signal(SIG_VPP_OFF);
	prog_set_signal(SIG_PGD_OFF);
	prog_set_signal(SIG_PGC_OFF);

	puts(" check: both LEDs off "); 
	puts(" ICSP: Pins 1...5 :  < 0.5 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	// 1 LEDs
	prog_set_led(LED1_ON);		//green
	puts("\nStep 11 :");
	puts(" check: green LED on "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_led(LED1_OFF);		//green
	prog_set_led(LED2_ON);		//yellow
	puts("\nStep 12 :");
	puts(" check: yellow LED on "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_led(LED2_OFF);		//yellow

	// 2 ICSP
	prog_set_signal(SIG_VPP_ON);
	puts("\nStep 21 :");
	fprintf(stdout," ICSP: Pin  1 :  > %d V \n", vpp); 
	if (prog.device == DEVICE_B8)
	fprintf(stdout," TS:   Pin  4 :  > %d V \n", vpp); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_VPP_OFF);
	prog_set_signal(SIG_VDD_ON);
	puts("\nStep 22 :");
	fprintf(stdout," ICSP: Pin  2 :  > %d V \n", vdd); 
	if (prog.device == DEVICE_B8)
	fprintf(stdout," TS:   Pin 36 :  > %d V \n", vdd); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_VDD_OFF);
	prog_set_signal(SIG_PGD_ON);
	puts("\nStep 23 :");
	fprintf(stdout," ICSP: Pin  4 :  > %d V \n", vdd); 
	if (prog.device == DEVICE_B8)
	fprintf(stdout," TS:   Pin 35 :  > %d V \n", vdd); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_PGD_OFF);
	prog_set_signal(SIG_PGC_ON);
	puts("\nStep 24 :");
	fprintf(stdout," ICSP: Pin  5 :  > %d V \n", vdd); 
	if (prog.device == DEVICE_B8)
	fprintf(stdout," TS:   Pin 34 :  > %d V \n", vdd); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_PGC_OFF);


	// 3 RUN
	prog_set_socket(SOC_OFF);
	prog_set_socket(SOC_RUN);
	puts("\nStep 31 :");
	fprintf(stdout," ICSP: Pin  1 :  > %d V \n", vdd); 
	fprintf(stdout," ICSP: Pin  1 :  < %d V \n", vdd+2); 
	fprintf(stdout," ICSP: Pin  2 :  > %d V \n", vdd); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_socket(SOC_OFF);
	puts("\nStep 32 :");
	puts(" ICSP: Pin  1 :  < 0.5 V "); 
	puts(" ICSP: Pin  2 :  > 0.5 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_socket(SOC_ICSP);


	// 4 VPP1
	if (prog.device == DEVICE_B8)
	{
		prog_set_socket(SOC_ICSP);
		prog_set_pwm( 1,  1);
		prog_set_signal(SIG_VPP_OFF);
		sleepms(500);
		puts("\nStep 41 :");
		puts(" ICSP: Pin  1 :  < 0.5 V "); 
		puts(" TS:   Pin  4 :  < 0.5 V "); 
		puts(" LSP1:        :   4..6 V "); 
		puts(" press ENTER to continue ");
		taste = getc (stdin);

		prog_set_signal(SIG_VPP_ON);
		puts("\nStep 42 :");
		puts(" ICSP: Pin  1 :   4..6 V "); 
		puts(" TS:   Pin  4 :   4..6 V "); 
		puts(" LSP1:        :   4..6 V "); 
		puts(" press ENTER to continue ");
		taste = getc (stdin);

		prog_set_pwm( 70,  70);
		prog_set_signal(SIG_VPP_OFF);
		sleepms(500);
		puts("\nStep 43 :");
		puts(" ICSP: Pin  1 :  < 0.5 V "); 
		puts(" TS:   Pin  4 :  < 0.5 V "); 
		puts(" LSP1:        :   > 14 V "); 
		puts(" press ENTER to continue ");
		taste = getc (stdin);

		prog_set_signal(SIG_VPP_ON);
		puts("\nStep 44 :");
		puts(" ICSP: Pin  1 :   > 14 V "); 
		puts(" TS:   Pin  4 :   > 14 V "); 
		puts(" LSP1:        :   > 14 V "); 
		puts(" press ENTER to continue ");
		taste = getc (stdin);

		prog_set_pwm( 10,  10);
	}

	if (prog.device == DEVICE_B9) return 0;

	// ab hier kein Brenner9 mehr 

	puts(" is this a Brenner8mini(P) <Y> ? ");
	taste = getc (stdin);
	if ((taste == 'y') || (taste == 'Y')) return 0;



	// 5 VPP2
	prog_set_socket(SOC_28_40);

	prog_set_pwm( 1,  1);
	prog_set_signal(SIG_VPP_OFF);
	sleepms(500);
	puts("\nStep 51 :");
	puts(" TS  : Pin  1 :  < 0.5 V "); 
	puts(" LSP1:        :   4..6 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_VPP_ON);
	puts("\nStep 52 :");
	puts(" TS:   Pin  1 :   4..6 V "); 
	puts(" LSP1:        :   4..6 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_pwm( 70,  70);
	prog_set_signal(SIG_VPP_OFF);
	sleepms(500);
	puts("\nStep 53 :");
	puts(" TS:   Pin  1 :  < 0.5 V "); 
	puts(" LSP1:        :   > 14 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_signal(SIG_VPP_ON);
	puts("\nStep 54 :");
	puts(" TS:   Pin  1 :   > 14 V "); 
	puts(" LSP1:        :   > 14 V "); 
	puts(" press ENTER to continue ");
	taste = getc (stdin);

	prog_set_pwm( 10,  10);


	// 6 Testsockel
/*
	prog_set_socket(SOC_28_40);



	prog_set_socket(SOC_8_14);
*/


	return 0;
}
