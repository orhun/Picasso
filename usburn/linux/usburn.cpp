/***************************************************************************
 *            usburn.c
 *
 *  Sund Aug 31 18:57:43 2008
 *  Copyright  2008..2010  Joerg Bredendiek (sprut)
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
 * this is the main-function of usburn
 */


//********************************************************************************************************************
// includes
//********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "b8.h"



int main(int argc, char* argv[])
{
	int  areg_int;
	word areg_word;

	init_system();

	// flags resetten
	f_r = 0;	// read
	f_w = 0;	// write
	f_c = 0;	// compare
	f_e = 0;	// erase
	f_p = 0;	// remove cp
	f_i = 0;	// info
	f_d = 0;	// reanimation
	f_l = 0;	// list supportet PICs

	f_a = 0;	// autodetect 
	f_b = 0;	// blank check
	f_o = 0;	// activate bootloader
	f_u = 0;	// run mode afer programming
	f_s = 0;	// Vpp before Vdd
	f_h = 0;	// show help information
	f_S = 0;	// change SOCKET
	f_F = 0;	// change Family

	f_n = 0;	// normal-Mode, nicht Bootloader
	f_f = 0;	// load firmware
	f_k = 0;	// calibrate Brenner8
	f_t = 0;	// hardware-test


	/* Flag set by `--verbose'. */
	static int verbose_flag;
     
	int c;
     
	while (1)
	{
		static struct option long_options[] =
		{
 			/* These options set a flag. */
			//{"verbose", no_argument,       &verbose_flag, 1},
			//{"brief",   no_argument,       &verbose_flag, 0},
			/* These options don't set a flag.
			We distinguish them by their indices. */
			{"read",	no_argument,	0, 'r'},
			{"write",	no_argument,	0, 'w'},
			{"compare",	no_argument,	0, 'c'},
			{"erase",	no_argument,	0, 'e'},
			{"remove",	no_argument,	0, 'p'},
			{"info",	no_argument,	0, 'i'},
			{"reanimate",	no_argument,	0, 'd'},
			{"list",	no_argument,	0, 'l'},
			{"auto",	no_argument,	0, 'a'},
			{"blank",	no_argument,	0, 'b'},
			{"firmware",	no_argument,	0, 'f'},
			{"boot",	no_argument,	0, 'o'},
			{"normal",	no_argument,	0, 'n'},
			{"run",		no_argument,	0, 'u'},
			{"swap",	no_argument,	0, 's'},
			{"help",	no_argument,	0, 'h'},
			{"calibrate",	no_argument,	0, 'k'},
			{"test",	no_argument,	0, 't'},

			{"PIC",		required_argument, 0, 'P'},
			{"SOCKET",	required_argument, 0, 'S'},
			{"FAMILY",	required_argument, 0, 'F'},
			{"HEX",		required_argument, 0, 'H'},
			{"TIMING",	required_argument, 0, 'T'},
			{"VPP",		required_argument, 0, 'V'},
			{"OSCCAL",	required_argument, 0, 'L'},
			{"BG",		required_argument, 0, 'B'},
			{"IN",		required_argument, 0, 'I'},
			{"OUT",		required_argument, 0, 'O'},
			 {0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;
     
		c = getopt_long (argc, argv, "rwcepidlabfonushktP:S:F:H:T:V:L:B:I:O:",
			long_options, &option_index);
     
		/* Detect the end of the options. */
		if (c == -1)
			break;

		// optarg ist pointer auf char:  "extern char *optarg;"
     
		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0) break;
			if (f_i)
			{
				printf ("option %s", long_options[option_index].name);
				if (optarg) printf (" with arg %s", optarg);
				printf ("\n");
			}
		break;
     
		case 'r':
			f_r = 1;
			if (f_i) puts ("option -r");
		break;
     		case 'w':
			f_w = 1;
			if (f_i) puts ("option -w");
		break;
     		case 'c':
			f_c = 1;
			if (f_i) puts ("option -c");
		break;
		case 'e':
			f_e = 1;
			if (f_i) puts ("option -e");
		break;
		case 'p':
			f_p = 1;
			if (f_i) puts ("option -p");
		break;
		case 'i':
			f_i = 1;
			if (f_i) puts ("option -i");
		break;
		case 'd':
			f_d = 1;
			if (f_i) puts ("option -d");
		break;
		case 'l':
			f_l = 1;
			if (f_i) puts ("option -l");
		break;
		case 'a':
			f_a = 1;
			if (f_i) puts ("option -a");
		break;
		case 'b':
			f_b = 1;
			if (f_i) puts ("option -b");
		break;
		case 'f':
			f_f = 1;
			if (f_i) puts ("option -f");
		break;
		case 'o':
			f_o = 1;
			if (f_i) puts ("option -o");
		break;
		case 'n':
			f_n = 1;
			if (f_i) puts ("option -n");
		break;
		case 'u':
			f_u = 1;
			if (f_i) puts ("option -u");
		break;
		case 's':
			f_s = 1;
			if (f_i) puts ("option -s");
		break;
		case 'h':
			f_h = 1;
			if (f_i) puts ("option -h");
		break;
		case 'k':
			f_k = 1;
			if (f_i) puts ("option -k");
		break;
		case 't':
			f_t = 1;
			if (f_i) puts ("option -t");
		break;

     
		case 'P':	// PIC
			if (f_i) printf ("option -P with value `%s'\n", optarg);
			strcpy(prog.picname, optarg);
		break;
     		case 'S':	//SOCKET
			f_S = 1;
			if (f_i) printf ("option -S with value `%s'\n", optarg);
			if      (!strcmp(optarg,  "8"))  prog.socket = SOC_8_14;
			else if (!strcmp(optarg, "14"))  prog.socket = SOC_8_14;
			else if (!strcmp(optarg, "20"))  prog.socket = SOC_8_14;
			else if (!strcmp(optarg, "18"))  prog.socket = SOC_18_ICSP;
			else if (!strcmp(optarg, "28"))  prog.socket = SOC_28_40;
			else if (!strcmp(optarg, "40"))  prog.socket = SOC_28_40;
			else if (!strcmp(optarg, "ICSP"))  prog.socket = SOC_18_ICSP;
			else 
			{ fprintf(stderr, "wrong SOCKET-argument %s \n", optarg); f_S = 0;}
			//printf ("socket `%d'\n", prog.socket);
		break;
		case 'F':	//Family
			f_F = 1;
			if (f_i) printf ("option -F with value `%s'\n", optarg);
			if      (!strcmp(optarg, "10"))  prog.core = CORE_12;		// pic10F...
			else if (!strcmp(optarg, "16"))  prog.core = CORE_14;		// pic 16F
			else if (!strcmp(optarg, "18"))  prog.core = CORE_16;		// pic18F
			else if (!strcmp(optarg, "18j")) prog.core = CORE_18;		// pic18J
			else if (!strcmp(optarg, "18J")) prog.core = CORE_18;
			else if (!strcmp(optarg, "18k")) prog.core = CORE_17;		// pic18K
			else if (!strcmp(optarg, "18K")) prog.core = CORE_17;
			else if (!strcmp(optarg, "24"))  prog.core = CORE_33;		// pic24f
			else if (!strcmp(optarg, "30"))  prog.core = CORE_30;		// dspic30F
			else if (!strcmp(optarg, "33"))  prog.core = CORE_33;		// dspic33F
			else 
			{ fprintf(stderr, "wrong Family-argument %s \n", optarg); f_F = 0;}
		break;
     		case 'H':	//HEX-Filename
			if (f_i) printf ("option -H with value `%s'\n", optarg);
			strcpy(prog.InHexfilename, optarg);
			strcpy(prog.OutHexfilename, optarg);
		break;
     		case 'I':	//HexIn-Filename
			if (f_i) printf ("option -I with value `%s'\n", optarg);
			strcpy(prog.InHexfilename, optarg);
		break;
     		case 'O':	//HexOut-Filename
			if (f_i) printf ("option -O with value `%s'\n", optarg);
			strcpy(prog.OutHexfilename, optarg);
		break;
		case 'T':
			if (f_i) printf ("option -T with value `%s'\n", optarg);
		break;
     		case 'V':
			if (f_i) printf ("option -V with value `%s'\n", optarg);
		break;
     		case 'L':
			if (f_i) printf ("option -L with value `%s'\n", optarg);
			strcpy(prog.OsccalString, optarg);
			prog.OsccalPar = atoi(prog.OsccalString);
			// diffrent PIC-families have different OSCCAL-ranges
			// PIC10F: -64 .. 63
			// PIC12F:  00 .. 63
			if (f_i) fprintf(stdout, "OSCCAL %d in parameter \n", prog.OsccalPar);
			/*
			areg_word = ((prog.OsccalPar<<1) & 0x00FF) | MOVLW_12; 
			fprintf(stdout, "fuer 12bit: %5.4x \n", areg_word);
			areg_word = ((prog.OsccalPar<<2) &0x00FF) | RETLW_14; 
			fprintf(stdout, "fuer 14bit: %5.4x \n", areg_word);
			*/
		break;
     
     		case 'B':
			prog.BGnewvalue = 0x0000;
			if (f_i) printf ("option -B with value `%s'\n", optarg);
			strcpy(prog.BGString, optarg);
			prog.BGnewvalue = atoi(prog.BGString);
			if (f_i) fprintf(stdout, "BG %5.4x in parameter \n", prog.BGnewvalue);
		break;
     
		case '?':
			/*getopt_long already printed an error message. */
		break;
     
		default:
			abort ();
             }
         }
     
	/* Instead of reporting `--verbose'
	and `--brief' as they are encountered,
	we report the final status resulting from them. */
	if (verbose_flag) puts ("verbose flag is set");
     
	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc) printf ("%s ", argv[optind++]);
		putchar ('\n');
	}
     


	//help
	if (f_h)
	{
		puts(tietel);
		puts("\nBackend-tool for USB-PIC-programmer Brenner8 & Brenner9 (www.sprut.de)");
		puts("");
		puts("usburn <options>");
		puts("");
		puts("Options without parameters:");
		puts("-h --help        schow this help-screen");
		puts("-r --read        read PIC-content into HEX-file");
		puts("-w --write       write HEX-file into PIC");
		puts("-c --compare     compare PIC-content with HEX-file");
		puts("-e --erase       erase PIC");
		puts("-p --remove      remove codeprotection from PIC");
		puts("-i --info        show a lot of nonnecessary information");
		puts("-d --reanimate   reanimate a PIC, that dont reacts anymore");
		puts("-l --list        list all supported PIC-types");
		puts("-a --auto        autodetect PIC-type");
		puts("-b --blank       check if the PIC is blank");
		puts("-f --firmware    load new firmware");
		puts("-o --boot        switch programmer into bootloader-mode");
		puts("-n --normal      deactivate bootloader-mode");
		puts("-u --run         activate Vdd for target-PIC");		
		puts("-t --test        interactive test of the hardware");
		puts("");
		puts("");
		puts("Options with parameters:");
		puts("-S --SOCKET        PIC-Socket(Brenner8 only) or ICSP-connector");
		puts("           -S8    --SOCKET 8      8-pin-DIL-socket");
		puts("           -S14   --SOCKET 14    14-pin-DIL-socket");
		puts("           -S18   --SOCKET 18    18-pin-DIL-socket");
		puts("           -S20   --SOCKET 20    20-pin-DIL-socket");
		puts("           -S28   --SOCKET 28    28-pin-DIL-socket");
		puts("           -S40   --SOCKET 40    40-pin-DIL-socket");
		puts("           -SICSP --SOCKET ICSP  ICSP-connector");
		puts("");
		puts("-F --FAMILY      PIC-family (core architecture)");
		puts("           -F10   --FAMILY 10    PIC10F..., PIC1xF5...");
		puts("           -F16   --FAMILY 16    PIC16F...");
		puts("           -F18   --FAMILY 18    PIC18F...");
		puts("           -F18J  --FAMILY 18J   PIC18FxxJxx");
		puts("           -F18K  --FAMILY 18K   PIC18FxxKxx");
		puts("           -F24   --FAMILY 24    PIC24...");
		puts("           -F30   --FAMILY 30    dsPIC30F...");
		puts("           -F33   --FAMILY 33    dsPIC33F...");
		puts("");
		puts("-L --OSCCALL     Osccal-Value, only for some PICs");
		puts("           -Onn   --OSCCAL nn    nn is a number from -64 to 63 ");
		puts("");
		puts("-B --BG     BG-value, only for some PICs");
		puts("           -Bnn   --BG nn    nn is a number from 0 to 65535 ");
		puts("");
		puts("-H --HEX         name of HEX-file");
		puts("           -Hname.hex  --HEX name.hex  ");
		puts("-I --IN          name of output HEX-file (for read)");
		puts("           -Iname.hex  --IN name.hex  ");
		puts("-O --OUT         name of input HEX-file (for write & compare)");
		puts("           -Oname.hex  --OUT name.hex  ");

		puts("");
	}



	puts(tietel);	//"usburn V 0.4 (16.05.2010) by sprut (www.sprut.de)"

//***** B R E N N E R   F I N D E N ********************

	prog.fw = 0;
	fprintf(stdout, "search for Brenner8/9:  ");
	if (search_brenner8() == 0) fprintf(stdout, "detected, Fw: %d\n", prog.fw );
	else
	{
		fprintf(stderr,"## no Brenner8/9 detected - end program");
		cleanup_system();
		return(0);
	}

	if (prog.fw >= 14) prog.WRITE_EDATA_KEY = prog.fw; else prog.WRITE_EDATA_KEY = 0;



//***** F I R M W A R E   V A L I D I E R E N ************

	if (prog.fw > 14) prog_check_firmware();
	

	

//***** H A R D W A R E   T E S T  ***********************

	//test the hardware
	if (f_t)
	{
		if (prog.device == DEVICE_BOOT) 
		{
			fprintf(stderr, "## device in Bootloader-mode, test not possible  --  end program\n");
			cleanup_system();
			return(0);
		}
		
		test_hardware();
		cleanup_system();
		return(0);
	}


//***** B O O T L O A D E R ***********************

	// activate Bootloader	
	if (f_o || f_f)
	{
		if (prog.device != DEVICE_BOOT)
		{
			fprintf(stdout, ">> switch into  bootloader-mode \n");
			firm_boot_on();
			cleanup_system();
			sleep(3);
	
			fprintf(stdout, "search for Bootloader:  ");
			if (search_brenner8() == 0) fprintf(stdout, "detected, Fw: %d\n", prog.fw );
		}

		if (prog.device != DEVICE_BOOT)
		{
			fprintf(stderr, "## could not switch into bootloader-mode - end program\n");
 			cleanup_system();
			return -1;
		}
		
		if (f_f)
			//flash firmware into control-PIC
			if (!firm_upload()) firm_boot_off();

		cleanup_system();
		return 0;
	}


	//deactivate Bootloader (normally not needed)
	if ((f_n) && (prog.device == DEVICE_BOOT))
	{
		firm_boot_off();
		return 0;
	}
	// if still in Bootloader-Mode, then quit
	if (prog.device == DEVICE_BOOT) return 0;



//***** C A L I B R A T I O N ***********************

	// read control-PIC-EEPROM , i need this data only for Brenner8
	if (prog.device == DEVICE_B8)
	{
		cal_read_data();

		prog_set_an(1); // Uz
		float Uz = (float)prog_read_adc()/1024*5;
		prog_set_an(0); // vpp
		prog.d_korr =  	Uz / prog.d_Uz;
		if (f_i)
		{
			fprintf(stdout, "Uz %f V \n", Uz);
			fprintf(stdout, "Vpp %f V \n", (float)prog_read_adc()/1024*5*prog.d_DIV);
			fprintf(stdout, "korr %f V \n", prog.d_korr);
		}

		float VppSoll = 13;
		unsigned char pwm_off = d2c(VppSoll * prog.d_gain_off - prog.d_pwm0v_off);
		unsigned char pwm_on  = d2c(VppSoll * prog.d_gain_on  - prog.d_pwm0v_on);
		prog_set_pwm( pwm_off,  pwm_on);
	}



	// Korrekturwert für aktuelle Betriebsspannung ermitteln
	prog.d_Vusb_cal = cal_Kalibrierespannung();	// liefert aktuelle USB-Spannung
	//cal_Kalibrierespannung();


	
	//calibration of the Brenner8
	if ((f_k) && (prog.device == DEVICE_B8))
	{
		char taste;
		fprintf(stdout, "\n>> Programmer calibration \n\n  ");
		puts("+++ W A R N I G  / W A R N U N G +++");
		puts("Remove PIC from testsocket and ICSP-connector! ");
		puts("PIC aus Testfassung und ICSP-Verbinder entfernen! ");
		puts(" press ENTER to continue ");
		puts(" ");
		taste = getc (stdin);

		//Uz einstellen
		fprintf(stdout, "\n>> Programmer calibration\n  1. Step: adjust Uz, use + and - , press = if ready \n");
		do
		{
			fprintf(stdout, "Uz %f V \n", prog.d_Uz);
			taste = getc (stdin);
			if (taste == '+') prog.d_Uz += 0.01;
			if (taste == '-') prog.d_Uz -= 0.01;
		} while (taste != '=');	// ((taste == '+') || (taste == '-'));

		//Div einstellen
		fprintf(stdout, "\n>> Programmer calibration\n  2. Step: adjust Div, use + and - , press = if ready \n");
		do
		{
			fprintf(stdout, "Div %f ,  ", prog.d_DIV);
			float Vpp_div = vpp_getVpp();
			fprintf(stdout, "Vpp-mess %f V \n", Vpp_div );
			taste = getc (stdin);
			if (taste == '+') prog.d_DIV += 0.01;
			if (taste == '-') prog.d_DIV -= 0.01;
		} while (taste != '=');	// ((taste == '+') || (taste == '-'));

		//Regelsteilheit ermitteln
		fprintf(stdout, "\n>> Programmer calibration\n  3. Step: \n");
		cal_step3();

		// 10V
		fprintf(stdout, "press ENTER to generate Vpp = 10V \n");
		taste = getc (stdin);
		vpp_setVpp(10);
		fprintf(stdout, ">> now generating %f V \n\n", vpp_getVpp() );

		// 11V
		fprintf(stdout, "press ENTER to generate Vpp = 11V \n");
		taste = getc (stdin);
		vpp_setVpp(11);
		fprintf(stdout, ">> now generating %f V \n\n", vpp_getVpp() );

		// 12V
		fprintf(stdout, "press ENTER to generate Vpp = 12V \n");
		taste = getc (stdin);
		vpp_setVpp(12);
		fprintf(stdout, ">> now generating %f V \n\n", vpp_getVpp() );

		// 13V
		fprintf(stdout, "press ENTER to generate Vpp = 13V \n");
		taste = getc (stdin);
		vpp_setVpp(13);
		fprintf(stdout, ">> now generating %f V \n\n", vpp_getVpp() );

		fprintf(stdout, "press ENTER to quit program\n");
		taste = getc (stdin);

		cleanup_system();
		return 0;
	}



//***** N O R M A L   W O R K ***********************


	// von der Firmware unterstuetzte Familien herausbekommen
	prog_get_supported();

	// list of all supported pics
	if (f_l) db_listpics(0);

	// load Database into memory
	db_load_db();

	// Familie einstellen
	if (f_F) 
	{
		prog_set_core(prog.core);
		// zwangsweise ICSP bei PIC24, dsPIC30/33, PIC10F, PIC18FJ
		if (((prog.core >= CORE_30) || (prog.core == CORE_18) || (prog.core ==CORE_12)) && (prog.socket != SOC_18_ICSP))
		{
			prog.socket = SOC_18_ICSP;
			f_S = 1;
			fprintf(stderr,"## selected ICSP-Connector\n");
		}
	}


	// Sockel einstellen
	if (f_S) prog_set_socket(prog.socket);


	// Familie einstellen
	if (f_F) prog_set_core(prog.core);

	puts(" ");

	//Typ manuell einstellen für PIC10F... und reanimation
	// ansonsten Autodetect
	int chip_id = 0;
	if (f_d || (prog.core == CORE_12))
	{
		if ( !db_findpicname(prog.pic) )
		{
			fprintf(stdout, "selected Type: %s  \n", prog.pic.name);
			prog.chipid = prog.pic.devid.id & prog.pic.devid.idmask;
			prog.revision = 0;
			chip_id = prog.chipid;
		}
		else
		{
			fprintf(stdout, "## no match for: %s  -- end program \n", prog.picname);
			cleanup_system();
			return(-1);
		}
	}
	else
	{
		// Autodetect
		// many modern PICs can handle not more then Vpp=9V
		vpp_setVpp(8.5);
		chip_id = prog_read_chipid();
		if (f_i) fprintf(stdout, "Vpp=8,5V -> Chip-ID: %d Revision %d  ", chip_id, prog.revision);
		// other PICs need a higher Vpp, 12V is a good value
		// but i dont try this with PIC18FxxKxx, they all are limited to 9V
		if ( ((chip_id <= 0) || (chip_id == 0xFFFF)) && (prog.core != CORE_17) )
		{
			vpp_setVpp(12);
			chip_id = prog_read_chipid();
			if (f_i) fprintf(stdout, "Vpp=12V -> Chip-ID: %d Revision %d  ", chip_id, prog.revision);
		}
	}
	if (f_i) fprintf(stdout, "Chip-ID: %d Revision %d  ", chip_id, prog.revision);
	if (db_findpicid(chip_id, prog.pic) >= 0)  
	{
		//PIC exists in database
		fprintf(stdout, "detected Type: %s  Revision: %d \n", prog.pic.name, prog.revision);
		//Flash
		prog.max_flash = prog.pic.pgmmem.max;
		if (prog.core == 18) prog.max_flash = prog.pic.cfgmem.max;
		//EEPROM
		prog.max_ee = prog.pic.eedata.max;  // kein EEPROM  = -1
		if (prog.pic.eedata.max == -1) prog.max_ee = -1;
		// PIC-Typ einstellen
		prog_set_pictype(prog.pic);
		//Vpp einstellen
		fprintf(stdout, "Vpp = %f V \n", prog.pic.vpp.deflt);
		vpp_setVpp(prog.pic.vpp.deflt);

//		float VppSoll = prog.pic.vpp.deflt;
//		unsigned char pwm_off = d2c(VppSoll * prog.d_gain_off - prog.d_pwm0v_off);
//		unsigned char pwm_on  = d2c(VppSoll * prog.d_gain_on  - prog.d_pwm0v_on);
//		prog_set_pwm( pwm_off,  pwm_on);


		// BG-value has to be saved, if it is existing, only for CORE_14
		// BG ist a field with flags=2
		// only 1 BG field can exist 
		db_find_BG();


		// save old OSCCAL from Flash
		if (((prog.core == CORE_14) || (prog.core == CORE_12)) && (prog.pic.calmem.max !=-1)) 
		{
			// Calmem aus Flash lesen 
			// osccal u.ä. retten , nur Kern 12 & 14
			//V. 19 vom 14/04/2009
			//PIC10F200	0x00FF  ..  0x00FF
			//PIC10F202	0x01FF  ..  0x01FF
			//PIC10F204	0x00FF  ..  0x00FF
			//PIC10F206	0x01FF  ..  0x01FF
			//PIC10F220	0x00FF  ..  0x00FF
			//PIC10F222	0x01FF  ..  0x01FF
			//PIC12F508	0x01FF  ..  0x01FF
			//PIC12F509	0x03FF  ..  0x03FF
			//PIC12F510	0x03FF  ..  0x03FF
			//PIC12F519	0x03FF  ..  0x03FF
			//PIC12F629	0x03FF  ..  0x03FF
			//PIC12F675	0x03FF  ..  0x03FF
			//PIC16F505	0x03FF  ..  0x03FF
			//PIC16F506	0x03FF  ..  0x03FF
			//PIC16F526	0x03FF  ..  0x03FF
			//PIC16F630	0x03FF  ..  0x03FF
			//PIC16F676	0x03FF  ..  0x03FF
			prog_read_calmem(prog.pic.calmem.min, prog.pic.calmem.max);

    			// lese OSCCAL aus dem Flash-Ende
			// DB 19
			//PIC10F200 PIC10F202 PIC10F204 PIC10F206 PIC10F220 PIC10F222
			//PIC12F508 PIC12F509 PIC12F510 PIC12F519
			//PIC12F629 PIC12F675 PIC16F505 PIC16F506 PIC16F526 PIC16F630 PIC16F676
			// retten von 0x0FF , 0x1FF oder 0x3FF
			EraseTPIC(prog.HexOut);
			prog_read_flash(0, prog.max_flash);

			if (prog.core == CORE_14)
			{
				//PIC12F629 PIC12F675 PIC16F630 PIC16F676   
				areg_word = prog.HexOut.Flash[prog.max_flash] & 0x3FFF;  //max 14 Bit
				//stand im Flash ein RETLW ?
				if ((areg_word & 0x3C00) != 0x3400) fprintf(stdout, "## no OSCCAL in Flash \n");
				else 
				{
					prog.OsccalRom = (areg_word & 0x00FF) >> 2;
 					fprintf(stdout, "OSCCAL: %d in Flash \n",  prog.OsccalRom);
				}
				// OSCCAL in comandline?
				if (prog.OsccalPar != no_OSCCAL) {
					if ((prog.OsccalPar < 0) || (prog.OsccalPar > 63)) {
						fprintf(stderr, "OSCCAL %d in parameter out of range (0..63) - ignored \n", prog.OsccalPar);
						prog.OsccalPar = no_OSCCAL;
					}
				}
			}

			if (prog.core == CORE_12)
			{
				//PIC10F200 PIC10F202 PIC10F204 PIC10F206 PIC10F220 PIC10F222
				//PIC12F508 PIC12F509 PIC12F510 PIC12F519   
				areg_word = prog.HexOut.Flash[prog.max_flash] & 0x0FFF;  //max 12 Bit
				//stand im Flash ein MOVLW ?
				if ((areg_word & 0x0F00) != 0xC00) fprintf(stdout, "## no OSCCAL in Flash \n");
				else 
				{
					prog.OsccalRom = (areg_word & 0x00FF);
 					fprintf(stdout, "OSCCAL: %d in Flash \n",  prog.OsccalRom);
				}
				// OSCCAL in comandline?
				if (prog.OsccalPar != no_OSCCAL) {
					if ((prog.OsccalPar < -64) || (prog.OsccalPar > 63)) {
						fprintf(stderr, "OSCCAL %d in parameter out of range (-64..63) - ignored \n", prog.OsccalPar);
						prog.OsccalPar = no_OSCCAL;
					}
				}
			}

		}

	}
	else
	{
	 	fprintf(stderr, "## PIC-Type: unknown \n");
		cleanup_system();
		return(-1);
	}


	//CP entfernen , reanimieren
	if ((f_p) || (f_w) || (f_d)) prog_removecp();

	
	// check after reanimate
	if (f_d)
	{
		// Autodetect
		chip_id = prog_read_chipid();
		if (db_findpicid(chip_id, prog.pic) >= 0) fprintf(stdout, "detected Type: %s  Revision: %d \n", prog.pic.name, prog.revision);
		else
		{
			fprintf(stderr, "## reanimation failed  --  end program\n");
			cleanup_system();
			return (-1);
		}
	}



	//loeschen des PIC
	if ((f_e) || (f_w)) prog_erase();


	// hexin einlesen
	if ((f_w) || (f_c))
	{
		EraseTPIC(prog.HexIn);
		openhexfile();
	}


	// brennen des PIC , ausser config
	if (f_w)
	{
		prog_write_flash(0, prog.max_flash);
		if ((prog.max_ee > 0) && (prog.EndEE > 0)) prog_write_ee(0, prog.EndEE );
		prog_write_id();
	}


	// Auslesen des PIC
	if ((f_r) || (f_w) || (f_c)) 
	{
		EraseTPIC(prog.HexOut);
		prog_read_flash(0, prog.max_flash);
		if (prog.max_ee >= 0) prog_read_ee(0, prog.max_ee); //bzw. EndEE
		if (prog.pic.userid.min > 0) prog_read_ID();
		prog_read_CONFIG();
		if (f_r) savehexout();
	}

	
	// vergleichen ausser Config
	if ((f_c) || (f_w))
	{
		puts (">> compare PIC-data");
		// FLASH
		int fehler = 0;
		for (int k=prog.pic.pgmmem.min; k<=prog.pic.pgmmem.max; k++) if ((prog.HexOut.Flash[k] != prog.HexIn.Flash[k]) && (prog.HexIn.Flash[k] != 0xFFFF))
		{
			fehler++;
			if (f_i) fprintf(stdout, "  FLASH-Addrr : %5.4x : HexIn %5.4x <> HexOut %5.4x\n", k, prog.HexIn.Flash[k], prog.HexOut.Flash[k] ); 
		}
		if (fehler) fprintf(stdout, "  FLASH : %d error(s)\n", fehler); else fprintf(stdout, "  FLASH : OK\n"); 
		// EEPROM
		fehler = 0;
		if ((prog.max_ee > 0) && (prog.EndEE > 0))
		{
			for (int k=prog.pic.eedata.min; k<=prog.pic.eedata.max; k++) if ((prog.HexOut.ROM[k] != prog.HexIn.ROM[k]) && (prog.HexIn.ROM[k] != 0xFFFF))
			{
				fehler++;
				if (f_i) fprintf(stdout, "  EE   -Addrr : %5.4x : HexIn %5.4x <> HexOut %5.4x\n", k, prog.HexIn.ROM[k], prog.HexOut.ROM[k] ); 
			}
			if (fehler) fprintf(stdout, "  EEPROM: %d error(s)\n", fehler); else fprintf(stdout, "  EEPROM: OK\n"); 
		}
		// user-ID
		fehler = 0;
		if ((prog.pic.userid.min > 0) && (prog.core < CORE_30))
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++) if ((prog.HexOut.ID[idNr(k)] != prog.HexIn.ID[idNr(k)]) && (prog.HexIn.ID[idNr(k)] != 0xFFFF))
		{
			fehler++;
			if (f_i) fprintf(stdout, "  ID   -Addrr : %5.4x : HexIn %5.4x <> HexOut %5.4x\n", k, prog.HexIn.ID[idNr(k)], prog.HexOut.ID[idNr(k)] ); 
		}
		if (fehler) fprintf(stdout, "  ID    : %d error(s)\n", fehler); else fprintf(stdout, "  ID    : OK\n"); 
 	}



	// brennen der PIC-config
	if (f_w)
	{
		prog_write_config();
	}



	// vergleichen der Config

	if ((f_c) || (f_w))
	{
		prog_read_CONFIG();
		puts (">> compare PIC-Config");
		int fehler = 0;
		word confmask = 0;
		word sollwert = 0;
		for (int k=prog.pic.cfgmem.min; k<=prog.pic.cfgmem.max; k++) 
		{
			confmask = db_getdefConfMask(k);
			sollwert = prog.HexIn.Config[confNr(k)] & confmask;

			// include stored BG-value
			if ((prog.core == CORE_14) && (prog.BGmask != 0) && (prog.BGadr == k))  sollwert = (sollwert & ~prog.BGmask) | prog.BGvalue;

			if (((prog.HexOut.Config[confNr(k)] & confmask) != sollwert) && (prog.HexIn.Config[confNr(k)] != 0xFFFF))
			{
				fehler++;
				if (f_i)	fprintf(stdout, "  Conf -Addrr : %5.4x : HexIn %5.4x <> HexOut %5.4x  Conf-Mask : %5.4x \n",
						 k, prog.HexIn.Config[confNr(k)], prog.HexOut.Config[confNr(k)] , confmask); 
			}
		}
		if (fehler) fprintf(stdout, "  CONFIG: %d error(s)\n", fehler); else fprintf(stdout, "  CONFIG: OK\n"); 
	}


	// Target RUN
	if (f_u) prog_target_run();

	cleanup_system();

//	puts(">> press ENTER to stop continue");
//	char xx = getc (stdin);


}
