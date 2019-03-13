/***************************************************************************
 *            programmer_usb.c
 *
 *  Mon Aug 21 18:54:33 2006
 *  Copyright  2006  Florian Demski
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
 * this file contains all functions for programmer-eccess for normal opperation
 */


//********************************************************************************************************************
// includes
//********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <iostream>
#include <time.h>
#include "b8.h"

using namespace std;

//#define DEBUG

struct programmer prog;

 

// warteschleife fuer Millisekunden
void sleepms(unsigned int mseconds)
{
    clock_t goal = mseconds*CLOCKS_PER_SEC/1000 + clock();
    while (goal > clock());
}


// round double to unsigned char
unsigned char d2c(double d)
{
  return d<0?(unsigned char)(d-.5):(unsigned char)(d+.5);
}


// look for programmer
int search_brenner8(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	struct usb_interface_descriptor *iface;
	usb_dev_handle *udev;
	int mode, usbmode, ep_in, ep_out;

	usb_find_busses();
	usb_find_devices();

	for (bus = usb_busses; bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			udev = usb_open(dev);

			if (udev) {
				if (dev->descriptor.idVendor == SPRUT_VID &&
				    dev->descriptor.idProduct == SPRUT_PID &&
				    dev->descriptor.bNumConfigurations == 1 &&
				    dev->config[0].bNumInterfaces == 1 &&
				    dev->config[0].interface[0].num_altsetting == 1 &&
				    dev->config[0].interface[0].altsetting[0].bNumEndpoints==2
				    ){
					iface = &dev->config[0].interface[0].altsetting[0];
					mode = MODE_NORMAL;
					usbmode = 0;
					ep_in = 0xFF;
					ep_out = 0xFF;

					// feststellen ob bul oder interrupt benutzt vwerden muss
					if ((iface->endpoint[0].bmAttributes&USB_ENDPOINT_TYPE_MASK) 
						== USB_ENDPOINT_TYPE_INTERRUPT &&
					    (iface->endpoint[1].bmAttributes&USB_ENDPOINT_TYPE_MASK) 
						== USB_ENDPOINT_TYPE_INTERRUPT) {
						
						// normal mode 
						usbmode = MODE_INT;
					}
					if ((iface->endpoint[0].bmAttributes&USB_ENDPOINT_TYPE_MASK) 
						== USB_ENDPOINT_TYPE_BULK &&
					    (iface->endpoint[1].bmAttributes&USB_ENDPOINT_TYPE_MASK) 
						== USB_ENDPOINT_TYPE_BULK) {
						
						// boot loader mode 
						usbmode = MODE_BULK;
						if (f_i) fprintf(stdout, "seems to be in BULK mode\n");
					}

					
					if (usbmode == MODE_INT || usbmode == MODE_BULK) {
						if (iface->endpoint[0].bEndpointAddress & 
							USB_ENDPOINT_DIR_MASK) {
							ep_in = iface->endpoint[0].bEndpointAddress;
						} else {
							ep_out = iface->endpoint[0].bEndpointAddress;
						}
					
						if (iface->endpoint[1].bEndpointAddress & 
							USB_ENDPOINT_DIR_MASK) {
							ep_in = iface->endpoint[1].bEndpointAddress;
						} else {
							ep_out = iface->endpoint[1].bEndpointAddress;
						}
					
						if (ep_in != 0xFF && ep_out != 0xFF) {
							prog.dev = udev;
							prog.ep_in = ep_in & USB_ENDPOINT_ADDRESS_MASK ;
							prog.ep_out = ep_out & USB_ENDPOINT_ADDRESS_MASK ;
							prog.interface = iface->bInterfaceNumber;
							prog.usbmode = usbmode;
							
							/* try to get fw version and programmer type */
							if (!usb_claim_interface(udev, prog.interface)) {
								int rc;
								
								if (f_i) fprintf(stdout, "interface claimed\n");							
								rc = prog_get_version();
								
								if (rc >= 0) {
									if (f_i) fprintf(stdout, "READ_VERSION %d \n", rc);
									prog.fw = (unsigned char) rc;
									
									usb_get_string_simple(prog.dev, 
										dev->descriptor.iProduct, 
										prog.usb_name, sizeof(prog.usb_name));
									
									return 0;
								}
								
								fprintf(stderr, "unexpected answer to READ_VERSION %d \n", rc);
								
								usb_release_interface(udev, prog.interface);
							} else {
								perror("problem claiming interface");
                                                                if (errno = EPERM) fprintf(stderr, "You don't have sufficient rights (try sudo)\n");
 								return -1;
							}
						}
					}
					
				}
				usb_close(udev);
			}
		}
    }
	return -ENODEV;
}


// universal routine to communicate with programmer
// data_in  - pointer to input-Daten
// dataz    - zahl der Input-Bytes
// data_out - pointer zu output daten
// rc       - zahl der output-byte
int programmer_command(unsigned char *data_in, int datasz, unsigned char *data_out)
{
	int rc;
	
#ifdef DEBUG
	int i;
	
	fprintf(stderr, "=>");
	for (i=0; i<datasz; i++) fprintf(stderr, " %02X", data_in[i]);
	fprintf(stderr, "\n");
#endif

	if (prog.usbmode == MODE_INT) {
		rc = usb_interrupt_write(prog.dev, prog.ep_out, (char*)data_in, datasz, TIMEOUT_WRITE);
	} else {
		rc = usb_bulk_write(prog.dev, prog.ep_out, (char*)data_in, datasz, TIMEOUT_WRITE);
	}

	if (rc < 0)		// Fehler
		return rc;
	
	if ((data_in[0] == CMD_RESET) || (data_in[0] == BOOT_RESET)) 
		return 0;
	
	if (rc != datasz) 
		return -EFAULT;
		
	if (prog.usbmode == MODE_INT) {
		rc = usb_interrupt_read(prog.dev, prog.ep_in, (char*)data_out, 
				USB_BLOCKSIZE, TIMEOUT_READ);
	} else {
		rc = usb_bulk_read(prog.dev, prog.ep_in, (char*)data_out, 
				USB_BLOCKSIZE, TIMEOUT_READ);
	}

#ifdef DEBUG
	fprintf(stderr, "<=");
	for (i=0; i<rc; i++) fprintf(stderr, " %02X", data_out[i]);
	fprintf(stderr, "\n");
#endif

	return rc;		
}



// identify sprut-device and
// read version of firmware
// after call in buf:
//	buf[0]	0x00
//	buf[1]	nonsense
//	buf[2]	version (e.g. firmwareversion)
//	buf[3]	device: 0-Brenner8; 1=Bootloader 2=USB4A 3=Brenner9
int prog_get_version(void)
{
	int rc;
	unsigned char cmd[] = { CMD_READ_VERSION };
	unsigned char buf[USB_BLOCKSIZE];
	rc = programmer_command(cmd, sizeof(cmd), buf);
	//fprintf(stdout, "rc %d bu0 %d bu1 %d bu2 %d bu3 %d \n", rc, buf[0], buf[1], buf[2], buf[3]);
	if (rc < 0)	return rc;				//Fehler
	if (rc != 4) return -EFAULT;			//falsche Bytezahl
	if (memcmp(buf, cmd, 1)) return -EFAULT;	// erstes Byte komt nicht zurueck

	prog.fw     = buf[2]; 
	prog.device = buf[3];
	switch (buf[3])
	{
		case 0x00: prog.mode = MODE_NORMAL; fprintf(stdout, " Brenner8 ");  break;	// Brenner8
		case 0x01: prog.mode = MODE_BOOT;   fprintf(stdout, " Bootloader ");break; 	// bootloader
		case 0x03: prog.mode = MODE_NORMAL; fprintf(stdout, " Brenner9 ")  ;break;	// Brenner9
		defautlt:  prog.mode = MODE_NORMAL; return(-1); break; // falsches device
	}
	return (int)buf[2];
}



// switch LEDs on and off
int prog_set_led(unsigned char led)
{
	unsigned char cmd[] = { CMD_LED_ONOFF, led };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}



// get supported PIC-families
int prog_get_supported(void)
{
	prog.supp = 0xFF;	// yeaaa, I can do everything

	unsigned char cmd[] = { CMD_SUPPORTED };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 2) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	prog.supp = buf[1];

	fprintf(stdout,"supported: ");
	if ((prog.supp & 0x01) != 0) fprintf(stdout," PIC10F ,"); 
	if ((prog.supp & 0x02) != 0) fprintf(stdout," PIC16F ,"); 
	if ((prog.supp & 0x04) != 0) fprintf(stdout," PIC18F ,");  
	if ((prog.supp & 0x08) != 0) fprintf(stdout," dsPIC30F ,");   
	if ((prog.supp & 0x10) != 0) fprintf(stdout," PIC18FxxJ ,");   
	if ((prog.supp & 0x20) != 0) fprintf(stdout," PIC24 , dsPIC33F ,");  
	if ((prog.supp & 0x40) != 0) fprintf(stdout," PIC18FxxK ,");  
	fprintf(stdout,"\n");
}


// read ADC
int prog_read_adc(void)
{
	unsigned char cmd[] = { CMD_RD_ADC };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 3) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return (buf[1] | (buf[2] << 8));
}


// select input for ADC
int prog_set_an(unsigned char an)
{
	unsigned char cmd[] = { CMD_SET_AN, an };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// set signal for test
int prog_set_signal(unsigned char signal)
{
	unsigned char cmd[] = { CMD_SET_SIGNAL, signal };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


//legt den pwm-Wert fuer Vpp mit und ohne Last fest
int prog_set_pwm(unsigned char pwm_off, unsigned char pwm_on)
{
	unsigned char cmd[] = { CMD_SET_PWM, pwm_off, pwm_on };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


int prog_set_vpp(unsigned char vppl, unsigned char vpph)
{
	unsigned char cmd[] = { CMD_SET_VPP, vppl, vpph };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// read EEPROM of controller PIC - im Bootloader-Mode
int _prog_boot_reed_eedata(int start, int length, unsigned char *dst)
{
	unsigned char cmd[] = { BOOT_READ_EEDATA, 
							length, 
							(unsigned char)(start & 0xFF), 
							(unsigned char)((start >> 8) & 0xFF), 
							(unsigned char)((start >> 16) & 0xFF) };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	if ((start + length) > CTRL_EEPROM_SIZE || !dst || !length || length > 59) 
		return -EFAULT;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < sizeof(cmd) + length) return -EFAULT;
	if (memcmp(buf, cmd, sizeof(cmd))) return -EFAULT;

	memcpy(dst, &buf[sizeof(cmd)], length);
	return 0;
}


// read EEPROM of controller PIC - im Programmer-Mode
int _prog_read_eedata(int start, int length, unsigned char *dst)
{
	unsigned char cmd[] = { CMD_READ_EEDATA, 
							(unsigned char)(start & 0xFF), 
							(unsigned char)((start >> 8) & 0xFF),
							length };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	if ((start + length) > CTRL_EEPROM_SIZE || !dst || !length || length > 59) 
		return -EFAULT;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < sizeof(cmd) + length) return -EFAULT;
	if (memcmp(buf, cmd, sizeof(cmd))) return -EFAULT;
	
	memcpy(dst, &buf[sizeof(cmd)], length);
	return 0;
}


// read EEPROM of controller-PIC 
int prog_read_eedata(int start, int length, unsigned char *dst)
{
	int rc;
	while (length) {
		/* although 59 bytes would be possible, take an even number of bytes */
		int this_shot = (length > 58) ? 58 : length; 
		
		if (prog.mode == MODE_BOOT) {
			rc = _prog_boot_reed_eedata(start, this_shot, dst);
		} else {
			rc = _prog_read_eedata(start, this_shot, dst);
		}
		
		if (rc < 0) return rc;
		
		start += this_shot;
		dst += this_shot;
		length -= this_shot;
	}
	return 0;
}


// write EEPROM of the controller-PIC in bootloader-mode
int _prog_boot_write_eedata(int start, unsigned char *src, int length)
{
	unsigned char cmd[USB_BLOCKSIZE] = { BOOT_WRITE_EEDATA, 
											length, 
											(unsigned char)(start & 0xFF), 
											(unsigned char)((start >> 8) & 0xFF), 
											(unsigned char)((start >> 16) & 0xFF) };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	if ((start + length) > CTRL_EEPROM_SIZE || !src || !length || length > 59) 
		return -EFAULT;
	
	memcpy(&cmd[5], src, length);
	
	rc = programmer_command(cmd, length + 5, buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// write EEPROM of the controller-PIC in programmer-mode
int _prog_write_eedata(int start, unsigned char *src, int length)
{
	unsigned char cmd[USB_BLOCKSIZE] = { CMD_WRITE_EEDATA, 	(unsigned char)(start                & 0xFF), 
								(unsigned char)(prog.WRITE_EDATA_KEY & 0xFF),
								length, };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	if ((start + length) > CTRL_EEPROM_SIZE || !src || !length || length > 59) 
		return -EFAULT;
	
	memcpy(&cmd[4], src, length);	
	
	rc = programmer_command(cmd, length + 4, buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;	
	return 0;
}


// write EEPROM of the controller-PIC
int prog_write_eedata(int start, unsigned char *src, int length)
{
	int rc;
	
	while (length) {
		/* although 59 bytes would be possible, take an even number of bytes */
		int this_shot = (length > 58) ? 58 : length; 
		
		if (prog.mode == MODE_BOOT) {
			rc = _prog_boot_write_eedata(start, src, this_shot);
		} else {
			rc = _prog_write_eedata(start, src, this_shot);
		}
		
		if (rc < 0) return rc;

		start += this_shot;
		src += this_shot;
		length -= this_shot;
	}
	return 0;
}


// reset programmer
int prog_reset(void)
{
	unsigned char cmd[] = { prog.mode == MODE_BOOT ? BOOT_RESET : CMD_RESET };
	return programmer_command(cmd, sizeof(cmd), NULL);
}


//read Chip-ID to identify PIC-Type
int prog_read_chipid(void)
{
	unsigned char cmd[] = { CMD_READ_CHIPID };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	word Rev = 0;
	word ID  = 0;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 3) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;

	ID = buf[1] | (buf[2] << 8);	
	// 14 und 16 bit
	Rev = ID & 0x001F;
	// 24 bit
	if (prog.core >= CORE_30) Rev = buf[3] | (buf[4] << 8);

	prog.chipid   = ID;
	prog.revision = Rev;

	return buf[1] | (buf[2] << 8);	//ID
}



// set 24-bit address for programmer operation
int prog_set_address(int start, int stop)
{
	unsigned char cmd[] = { CMD_SET_ADDRESS,
						(unsigned char)(start & 0xFF), 
						(unsigned char)((start >>  8) & 0xFF),
						(unsigned char)((start >> 16) & 0xFF),
						(unsigned char)(stop & 0xFF), 
						(unsigned char)((stop >>  8) & 0xFF),
						(unsigned char)((stop >> 16) & 0xFF) };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0)	return rc;
	if (rc < 1)	return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// loescht HexOut bzw HexIn
int EraseTPIC(TPIC & PIC)
{
  for (int k=0; k<0x40000; k++) PIC.Flash[k] = 0xFFFF;  //longword  32 Bit ohne Vorzeichen
  for (int k=0; k< 0x1000; k++) PIC.ROM[k]   = 0xFFFF;
  for (int k=0; k<      8; k++) PIC.ID[k]    = 0xFFFF;
  for (int k=0; k<     24; k++) PIC.Config[k]= 0xFFFF;
}


// Auslesen des Flash-Programmspeichers von start bis stop
int prog_read_flash(int start, int stop)
{
	fprintf(stdout,">> read PIC-FLASH ");
	if (f_i) fprintf(stdout,"from %5.4x fo %5.4x ", start, stop);
	prog_set_address(start, stop);
	byte Flash[0x80000];		  //512k
	int flashpointer = 0;
	unsigned char cmd[] = { CMD_READ_FLASH};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	for (int k=0; k<buf[1]; k++) Flash[flashpointer++] = buf[k+3];
	while (buf[2]) {
		rc = programmer_command(cmd, sizeof(cmd), buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;
		for (int k=0; k<buf[1]; k++)   Flash[flashpointer++] = buf[k+3];
		fprintf(stdout,"."); fflush(stdout);
	} 
	fprintf(stdout,"\n");

	//combine single bytes to program-code-words
	int max_Flash = flashpointer;
	switch (prog.core)
	{
	case CORE_12:
	case CORE_14:
		max_Flash = ((flashpointer+1) / 2)-1;
		for (int k=0; k<= max_Flash; k++)   prog.HexOut.Flash[k] = Flash[2*k] + Flash[2*k+1]*256;
	break;

	case CORE_16:
	case CORE_17:
	case CORE_18:
		max_Flash = flashpointer-1;
 		for (int k=0; k< max_Flash; k++)
		{
			prog.HexOut.Flash[k] = Flash[k];
			//fÃ¼r PIC18FxxJxx die Config einblenden
			if ( (k >= prog.pic.cfgmem.min) & (k <= prog.pic.cfgmem.max))  prog.HexOut.Config[k-prog.pic.cfgmem.min] = prog.HexIn.Flash[k];
		}
		max_Flash--; // for the following test-data-output
	break;

	case CORE_30:
	case CORE_33:
		max_Flash = ((flashpointer+1) / 3)-1;
		for (int k=0; k < max_Flash; k++) 
		{
			prog.HexOut.Flash[2*k]   = Flash[3*k]+Flash[3*k+1]*256; // LSB, MSB
			prog.HexOut.Flash[2*k+1] = Flash[3*k+2];                // USB
		}
		max_Flash = 2*(max_Flash-1)  +1; // for the following test-data-output
	break;
	}

	//anzeigen zum test
	if (f_i)
	for (int k=0; k<=max_Flash; k++)
	{
		if ((k % 0x10) == 0x0)  fprintf(stdout, "%5.4x :",k);
		fprintf(stdout, "%5.4x", prog.HexOut.Flash[k]);
		if ((k % 0x10) == 0x0F) fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	
	return 0;
}//prog_read_flash


// Auslesen des CalMem aus dem Flash von start bis stop
// weiter unten steht noch so eine routine, die nutze ich aber wohl gar nicht 
// nur Core_12 und Core_14
int prog_read_calmem(int start, int stop)
{
	fprintf(stdout,">> read CALMEM ");
	if (f_i) fprintf(stdout,"from %5.4x fo %5.4x ", start, stop);
	prog_set_address(start, stop);
	byte Flash[0x80000];		  //512k
	int flashpointer = 0;
	unsigned char cmd[] = { CMD_READ_FLASH};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	for (int k=0; k<buf[1]; k++) Flash[flashpointer++] = buf[k+3];
	while (buf[2]) {
		rc = programmer_command(cmd, sizeof(cmd), buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;
		for (int k=0; k<buf[1]; k++) Flash[flashpointer++] = buf[k+3];
		fprintf(stdout,"."); fflush(stdout);
	} 
	fprintf(stdout,"\n");

	//combine single bytes to program-code-words
	for (int k=0; k<= ((flashpointer+1) / 2)-1; k++)  prog.Calmem[start+k] = Flash[2*k] + (Flash[2*k+1] << 8);

	//anzeigen zum test
	if (f_i)
	for (int k=0; k<= ((flashpointer+1) / 2)-1; k++)
	{
		if ((k % 0x10) == 0x0)  fprintf(stdout, "%5.4x :",start+k);
		fprintf(stdout, "%5.4x", prog.Calmem[start+k]);
		if ((k % 0x10) == 0x0F) fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	
	return 0;
}// prog_read_calmem


// Auslesen des EEPROMs von start bis stop
int prog_read_ee(int start, int stop)
{
	fprintf(stdout, ">> read PIC-EEPROM");
	prog_set_address(start, stop);
	word EE[0x1000];		  
	int eepointer = 0;
	unsigned char cmd[] = { CMD_READ_EEPROM};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;

	do
	{
		rc = programmer_command(cmd, sizeof(cmd), buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;
		for (int k=1; k <= buf[1]; k++) EE[eepointer++] = buf[k+2];
		fprintf(stdout,"."); fflush(stdout);
	} while(buf[2]);
	fprintf(stdout,"\n");

	//anzeigen zum test
	if (f_i)
	{
		for (int k=0; k<eepointer; k++)
		{
			fprintf(stdout, "%3.2x", EE[k]);
			if ((k % 0x10) == 0x0F) fprintf(stdout, "\n");
		}
		fprintf(stdout, "\n");
	}

	if (prog.core == CORE_30)
	{
		for (int k=0; k<=(eepointer-1); k++)
		{
			if ((k % 2) == 0)  
			{
				prog.HexOut.ROM[(prog.pic.eedata.min-0x7FF000) + k] = EE[k] + EE[k+1]*256;
			}
			else prog.HexOut.ROM[(prog.pic.eedata.min-0x7FF000) + k] = 0;
		}
	}
	else

	for (int m=0; m<= (eepointer-1); m++) prog.HexOut.ROM[m] = EE[m];
	return 0;
}



// Auslesen der ID-daten
int prog_read_ID(void)
{
	puts (">> read PIC-ID-data");
	if (prog.core >= CORE_30) return(0);		//ACHTUNG für Kern=33 gibt es eine ID
	if (prog.pic.userid.min == -1) return (0);	//18FxxJxx
	prog_set_address(prog.pic.userid.min, prog.pic.userid.max);

	unsigned char cmd[] = { CMD_READ_IDDATA};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;

	switch (prog.core)
	{	
		case CORE_14:
			for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
				prog.HexOut.ID[idNr(k)] = buf[2*(k-prog.pic.userid.min)+3];
		break;
		case CORE_16:
		case CORE_17:
			for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
				prog.HexOut.ID[idNr(k)] = buf[(k-prog.pic.userid.min)+3];
		break;
	}

	//anzeigen zum test
	if (f_i)
	{
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
		{
			fprintf(stdout, "%5.4x", prog.HexOut.ID[idNr(k)]);
		}
		fprintf(stdout, "\n");
	}

	return (0);
}


// Auslesen der Config-daten
int prog_read_CONFIG(void)
{
	puts (">> read PIC-Config");
	prog_set_address(prog.pic.cfgmem.min, prog.pic.cfgmem.max);
	unsigned char cmd[] = { CMD_READ_CONFIG};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;

	for (int k=prog.pic.cfgmem.min; k<=prog.pic.cfgmem.max; k++)
	{
		switch (prog.core)
		{
		case CORE_12: prog.HexOut.Config[confNr(k)] = buf[2 * (k-prog.pic.cfgmem.min)+3] + buf[2* (k-prog.pic.cfgmem.min)+4] * 256; break;
		case CORE_14: prog.HexOut.Config[confNr(k)] = buf[2 * (k-prog.pic.cfgmem.min)+3] + buf[2* (k-prog.pic.cfgmem.min)+4] * 256; break;
		case CORE_16: prog.HexOut.Config[confNr(k)] = buf[k-prog.pic.cfgmem.min+3]; break;
		case CORE_17: prog.HexOut.Config[confNr(k)] = buf[k-prog.pic.cfgmem.min+3]; break;
		case CORE_18: prog.HexOut.Config[confNr(k)] = buf[k-prog.pic.cfgmem.min+3]; break;
		case CORE_30:
		case CORE_33: prog.HexOut.Config[confNr(k)] = buf[2 * (k-prog.pic.cfgmem.min)+3] + buf[2* (k-prog.pic.cfgmem.min)+4] * 256; break;
		}
	}

	//anzeigen zum test
	if (f_i)
	{
		for (int k=prog.pic.cfgmem.min; k<=prog.pic.cfgmem.max; k++)
		{
			fprintf(stdout, "%5.4x", prog.HexOut.Config[confNr(k)]);
		}
		fprintf(stdout, "\n");
	}

	return(0);
}


// Verifizieren bzw. signieren der Firmware
int prog_check_firmware(void)
{
	puts (">> check firmware");
	unsigned char cmd[] = { CMD_PSUMM, 0x00, 0x08, 0x5F, 0xFF};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	word psummF, psummE;
	byte psummFl, psummFh;

	// pruefsumme von 0800 bis 5FFF berechnen	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 3) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	psummFl = buf[1];
	psummFh = buf[2];
	psummF  = buf[1] + (buf[2]<<8);

	// pruefsumme aus EEPROM auslesen (FC-FD)
	unsigned char cmd1[] = { CMD_READ_EEDATA, 0xFC, 0x00, 0x02};
	rc = programmer_command(cmd1, sizeof(cmd1), buf);
	if (rc < 0) return rc;
	if (rc < 6) return -EFAULT;
	if (memcmp(buf, cmd1, 1)) return -EFAULT;
	psummE = buf[4] + (buf[5]<<8);

	if (psummF != psummE)
	{
		if (psummE = 0xFFFF)
		{
			// frische Firmware signieren
			fprintf(stdout, "sign firmware \n");
			unsigned char cmd2[] = { CMD_WRITE_EEDATA, 0xFC, prog.WRITE_EDATA_KEY, 0x02, psummFl, psummFh};
			rc = programmer_command(cmd2, sizeof(cmd2), buf);
			if (rc < 0) return rc;
			if (rc < 1) return -EFAULT;
			if (memcmp(buf, cmd2, 1)) return -EFAULT;
		} else
		{
		         fprintf(stdout, "firmware defectiv \n");
		}
	}

	//anzeigen zum test
	if (f_i) fprintf(stdout, "Fw-checksum:  %5.4x ;  Checksum in EEPROM:  %5.4x\n", psummF, psummE);
	return(0);
}


// rettet den calmem bei CORE_14-PICs mit calmem
int savecalmem(void)
{
	//fprintf(stdout,"calmemsaved: %d calmem.min: %d \n", prog.calmemsaved, prog.pic.calmem.min);
	if (prog.calmemsaved != 0) return 0;
	if (prog.pic.calmem.min < 0) return 0 ;		//failsave
	int k;
	int Flashpointer = 0;
	byte Flash[0x4000];
	puts (">> save CALMEM (eg. OSCCAL)");
	prog_set_address(prog.pic.calmem.min, prog.pic.calmem.max);
	unsigned char buf[USB_BLOCKSIZE];
	do
	{
		unsigned char cmd[] = { CMD_READ_FLASH};
		int rc;
		rc = programmer_command(cmd, sizeof(cmd), buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;
		for (int k=1; k<=buf[1]; k++)	Flash[Flashpointer++] = buf[k+2];
	} while (buf[2]);
	
	for (int k=0; k<=((Flashpointer+1) / 2)-1; k++) prog.Calmem[prog.pic.calmem.min+k] = Flash[2*k] + Flash[2*k+1]*256;
	prog.calmemsaved++;
	return(0);
}  // savecalmem


// hexin und calmem zusammenführen
// beim PIC18FxxJxx die Config eintragen
word HexinMux(int Adr)
{
	word wert = 0;
	//normalfall: nichts zu tun
	if ((Adr >= prog.pic.pgmmem.min) && (Adr <= prog.pic.pgmmem.max)) wert = prog.HexIn.Flash[Adr];

	//calmem eintragen  
	if ((Adr >= prog.pic.calmem.min) && (Adr <= prog.pic.calmem.max))
	{
		wert = prog.Calmem[Adr];

		//an dieser Stelle kann ein manuell eingestellter OSCCAL eingefügt werden
		// prog.OsccalPar enthält 0..63 (PIC12) oder -64..63 (PIC10)
		if ((Adr == prog.pic.pgmmem.max) && (prog.OsccalPar != no_OSCCAL))
		switch (prog.core)
		{
			case CORE_12: 			
				// 7 bit mit vorzeichen  63 .. -64
				// 0111 111x	max freq   = 63<<1
				// 0000 000x	centerfreq = 0
				// 1000 000x	min freq   = -64<<1
				wert  = ((prog.OsccalPar<<1) & 0x00FF) | MOVLW_12; 
			break;
			case CORE_14: 			
				// 6 bit ohne Vorzeichen 63 .. 00
				// c5-c4-c3-c2-c1-c0-x-x
				// 1111 11xx	max freq   = 63<<2
				// 1000 00xx	centerfreq = 32<<2
				// 0000 00xx	min freq   = 0
				wert = ((prog.OsccalPar<<2) &0x00FF) | RETLW_14; 
			break;	// 6 bit ohne Vorzeichen 63 .. 00
		}

	}
	//für PIC18FxxJxx die Config einblenden
	if ((Adr >= prog.pic.cfgmem.min) && (Adr <= prog.pic.cfgmem.max)) wert = prog.HexIn.Config[Adr-prog.pic.cfgmem.min];
	return(wert);
} // HexinMux



// Brennen des Flash-Programmspeichers
int prog_write_flash(int start, int stop)
{
	int Adrr = 0;
	word FMaske = 0xFFFF;
	switch (prog.core)
	{
		case CORE_12: FMaske = 0x0FFF; break;
		case CORE_14: FMaske = 0x3FFF; break;  // PIC16
		case CORE_16: FMaske = 0x00FF; break;  // PIC18
		case CORE_17: FMaske = 0x00FF; break;  // PIC18xxKxx
		case CORE_18: FMaske = 0x00FF; break;  // PIC18xxjxx
		case CORE_30:                          // dsPIC30
		case CORE_33: FMaske = 0xFFFF; break;  // PIC34, dsPIC33
	}

	fprintf(stdout,">> program FLASH");

	//letzte benutzte Adresse feststellen
	int lastusedadr = prog.pic.pgmmem.max;				//EndFlash  vergisst den letzten Block
	if (prog.core < CORE_30)
	{
		while (((HexinMux(lastusedadr) & 0xF00000) != 0) && (lastusedadr > 0)) lastusedadr--;
		//bis pufferende auffüllen
		while ((lastusedadr % prog.pic.latches.pgm) != (prog.pic.latches.pgm-1)) lastusedadr++;
		if (lastusedadr > prog.pic.pgmmem.max) lastusedadr = prog.pic.pgmmem.max;
	};

	if (prog.core == CORE_18) lastusedadr = prog.pic.cfgmem.max;	//config liegt am Flash-Ende beim 18FxxJxx

	if (prog.core >= CORE_30)
	{
		lastusedadr = prog.EndFlash;		//+PICdata.latches.pgm;//EndFlash  vergisst den letzten Block !!!
		//bis pufferende auffüllen
		while ((lastusedadr % prog.pic.latches.pgm) != (prog.pic.latches.pgm-1)) lastusedadr++;
	}

	// die END_Adresse ist die letzte Adresse, die gebrannt wird
	// alternativ kann auch das Endekennzeichen des Blocks benutzt werden	
	prog_set_address(Adrr, lastusedadr);

	while (Adrr <= lastusedadr)
	{
		unsigned char anzahl ;
		unsigned char endekennzeichen;
		if ((lastusedadr-Adrr) >= 29) anzahl = 30; else anzahl = lastusedadr-Adrr+1;
		if ((anzahl+Adrr) > lastusedadr)  endekennzeichen = 0x00; else endekennzeichen = 0x01;	// Ende-Kennzeichen: 0=letztes Paket,
		unsigned char cmd[USB_BLOCKSIZE] = { CMD_WRITE_FLASH, (unsigned char)(anzahl << 1), endekennzeichen };
		unsigned char buf[USB_BLOCKSIZE];
		int rc;

		for (int k=0; k<=anzahl-1; k++)
		{
			word Puffer = HexinMux(k+Adrr) & FMaske;
			cmd[2*k+3] = Puffer % 0x100;    //low
			cmd[2*k+4] = Puffer / 0x100;    //high
		}
		rc = programmer_command(cmd, 63, buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;	

		Adrr += anzahl;
		printf("."); fflush(stdout);
	}
	fprintf(stdout,"\n");
	return 0;
}  // prog_write_flash



// Brennen des EEPROM-Datenspeichers
// EndEE ist letzte benutzte EEPROM-Adresse
// ### diese routine löscht 12F675  ###
int prog_write_ee(int start, int stop)
{
	int Adrr = prog.pic.eedata.min;
	int anzahl;
	unsigned char endekennzeichen;

	if (prog.EndEE < 0) return(0);
	fprintf(stdout,">> program EEPROM");
	prog_set_address(Adrr, prog.EndEE);
	prog_set_address(Adrr, prog.EndEE);
	
	while (Adrr <= prog.EndEE)
	{
		if ((prog.EndEE-Adrr) >= 29) anzahl=30; else anzahl=prog.EndEE-Adrr+1;
		if ((anzahl+Adrr) > prog.EndEE) endekennzeichen = 0x00;   // Ende-Kennzeichen: 0=letztes Paket,
			else endekennzeichen = 0x01;
		unsigned char cmd[USB_BLOCKSIZE] = { CMD_WRITE_EEPROM, (unsigned char)(anzahl << 1), endekennzeichen };
		unsigned char buf[USB_BLOCKSIZE];

		for (int k=0; k<=anzahl-1; k++)
		{
			if (prog.core == CORE_30)
			{
				cmd[2*k+3] = prog.HexIn.ROM[Adrr-0x7FF000+k] % 256;
				cmd[2*k+4] = prog.HexIn.ROM[Adrr-0x7FF000+k] / 256;
			} else {
				cmd[2*k+3] = prog.HexIn.ROM[Adrr+k] % 256;
				cmd[2*k+4] = prog.HexIn.ROM[Adrr+k] / 256;
			};
		};
		int rc;
		rc = programmer_command(cmd, 63, buf);
		if (rc < 0) return rc;
		if (rc < 1) return -EFAULT;
		if (memcmp(buf, cmd, 1)) return -EFAULT;	

		Adrr += anzahl;
		// (EndEE+1) ansonsten absturz bei nur 1 Byte EEPROM-Daten
		printf("."); fflush(stdout);
	}
	fprintf(stdout,"\n");
}  // prog_write_ee



//Config brennen
// Puffergröße ist fest auf 2 eingestellt
int prog_write_config(void)
{
	int bytezahl;
	word puffer;
	if (prog.core == CORE_18) return(0);
	puts (">> program CONFIG");
	prog_set_address(prog.pic.cfgmem.min, prog.pic.cfgmem.max);
	unsigned char endekennzeichen = 0;
	unsigned char anzahl ;
	anzahl = 2 * (prog.pic.cfgmem.max - prog.pic.cfgmem.min + 1);   // Anzahl der im Paket enthaltenen Bytes
	unsigned char cmd[USB_BLOCKSIZE] = { CMD_WRITE_CONFIG, (unsigned char)anzahl, endekennzeichen };
	unsigned char buf[USB_BLOCKSIZE];

	for (int k=prog.pic.cfgmem.min; k<=prog.pic.cfgmem.max; k++)
	{
		puffer = prog.HexIn.Config[confNr(k)];
		// include stored BG-value
		if (prog.core == CORE_14)
		{
			if (f_i) fprintf(stdout, "BG-Adr:%5.4x  BG-mask:%5.4x  BG-value:%5.4x puffer: %5.4x ",  prog.BGadr, prog.BGmask, prog.BGvalue, puffer);
			if ((prog.BGmask != 0) && (prog.BGadr == k))  puffer = (puffer & ~prog.BGmask) | prog.BGvalue;
			if (f_i) fprintf(stdout, "---> %5.4x \n", puffer);
		}
		cmd[3+ (k-prog.pic.cfgmem.min)*2] = puffer % 0x100;   //low
		cmd[4+ (k-prog.pic.cfgmem.min)*2] = puffer / 0x100;   //high
	}
	bytezahl = 2 * (prog.pic.cfgmem.max - prog.pic.cfgmem.min+1) +3;
	int rc;
	rc = programmer_command(cmd, bytezahl, buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;	

	return(0);
}  // prog_write_config




//User-ID brennen
int prog_write_id(void)
{
	int lastusedadr;
	int bytezahl;
	int Puffer;
	word IDMaske = 0x000F;

	if (prog.core >= CORE_30) return(0);	// ACHTUNG für Kern=33 gibt es eine ID
	if (prog.pic.userid.min == -1) return (0);
	puts (">> program user-ID");
	prog_set_address(prog.pic.userid.min, prog.pic.userid.max);
	unsigned char anzahl = 2* (prog.pic.userid.max-prog.pic.userid.min+1); 
	unsigned char endekennzeichen = 0;
	unsigned char cmd[USB_BLOCKSIZE] = { CMD_WRITE_IDDATA, (unsigned char)anzahl, endekennzeichen };
	unsigned char buf[USB_BLOCKSIZE];

	switch (prog.core)
	{
		case CORE_12: IDMaske = 0x000F; break;
		case CORE_14: IDMaske = 0x000F; break;
		case CORE_16: IDMaske = 0x00FF; break;
		case CORE_17: IDMaske = 0x00FF; break;
		case CORE_33: IDMaske = 0xFFFF; break;
	}

	switch (prog.core)
	{
	case CORE_12: 
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
		{
			cmd[3+ (k-prog.pic.userid.min)*2] = (prog.HexIn.ID[idNr(k)] | 0xF0) & 0x00FF;
			cmd[4+ (k-prog.pic.userid.min)*2] = 0xFF;
		}
		bytezahl = 2 * (prog.pic.userid.max - prog.pic.userid.min+1) +3;
	break;
	case CORE_14: 
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
		{
			cmd[3+(k-prog.pic.userid.min)*2] = (prog.HexIn.ID[idNr(k)] | 0xF0) & 0x00FF;
			cmd[4+(k-prog.pic.userid.min)*2] = 0xFF;
		}
		bytezahl =2* (prog.pic.userid.max-prog.pic.userid.min+1)+3;
	break;
	case CORE_16: 
	case CORE_17: 
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
			cmd[3+(k-prog.pic.userid.min)] = prog.HexIn.ID[idNr(k)] & 0x00FF;
		bytezahl = (prog.pic.userid.max-prog.pic.userid.min+1)+3;
	break;
	case CORE_33: 
		for (int k=prog.pic.userid.min; k<=prog.pic.userid.max; k++)
		{
			Puffer = prog.HexIn.ID[idNr(k)] & IDMaske;
			cmd[2*k+3] = Puffer % 0x100;    //low
			cmd[2*k+4] = Puffer / 0x100;    //high
		}
		bytezahl = (prog.pic.userid.max-prog.pic.userid.min+1)*2+3;
	break;
	}
	int rc;
	rc = programmer_command(cmd, bytezahl, buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;	

	return(0);
}  // prog_write_id



// erase PIC
int prog_erase(void)
{
	if ((prog.core == CORE_14) && (prog.pic.calmem.min >= 0)) savecalmem();
	puts (">> erase PIC");
	unsigned char cmd[] = { CMD_ERASE };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}



// remove codeprotection
int prog_removecp(void)
{
	if ((prog.core == CORE_14) && (prog.pic.calmem.min >= 0)) savecalmem();
	puts (">> remove codeprotection");
	unsigned char cmd[] = { CMD_REMOVECP };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// Selct the PIC-Core (Family)
int prog_set_core(unsigned char core)
{
	unsigned char cmd[] = { CMD_SET_CORE, core };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}


// Uebertragen der PIC-Parameter aus der Database an den Brenner
int prog_set_pictype(TPicDef& pic)
{
	T_PICtype PICdata;

	PICdata.cpu			= pic.cpu;
	PICdata.power			= pic.power;
	PICdata.blocksize		= pic.blocksize;
	PICdata.pins 			= pic.pins;
	PICdata.vpp			= (byte)floor(pic.vpp.deflt*10); 			// 130  = 13,0V
	PICdata.panelsize		= 0;							// singelpanel ?
	if (pic.pgming.panelsize < pic.pgmmem.max) PICdata.panelsize = pic.pgming.panelsize;	// multipanel  !

	PICdata.taktik.flash		= pic.taktik.flash;
	PICdata.taktik.eeprom		= pic.taktik.eeprom;
	PICdata.taktik.id		= pic.taktik.id;
	PICdata.taktik.config		= pic.taktik.config;
	PICdata.taktik.erase		= pic.taktik.erase;
	PICdata.taktik.cp		= pic.taktik.cp;
	PICdata.taktik.read_eeprom	= pic.taktik.read_eeprom;

	PICdata.latches.pgm		= pic.latches.pgm;
	PICdata.latches.eedata		= pic.latches.eedata;
	PICdata.latches.userid		= pic.latches.userid;
	PICdata.latches.cfg		= pic.latches.cfg;
	PICdata.latches.rowerase	= pic.latches.rowerase;

	PICdata.wait.pgm		= pic.wait.pgm;
	PICdata.wait.lvpgm		= pic.wait.lvpgm;
	PICdata.wait.eedata		= pic.wait.eedata;
	PICdata.wait.cfg		= pic.wait.cfg;
	PICdata.wait.userid		= pic.wait.userid;
	PICdata.wait.erase 		= pic.wait.erase;
	PICdata.wait.lverase		= pic.wait.lverase;

	if (pic.name == "PIC18F1320") PICdata.wait.eedata = 6000;
	if (pic.name == "PIC18F1220") PICdata.wait.eedata = 6000;

	unsigned char cmd[USB_BLOCKSIZE] = { CMD_SET_PICTYPE};
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	unsigned char length = sizeof(PICdata);
	memcpy(&cmd[1], &PICdata, length);	
	
	rc = programmer_command(cmd, length + 1, buf);

	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}  // prog_set_pictype


// selects socket or ICSP
int prog_set_socket(unsigned char socket)
{
	unsigned char cmd[] = { CMD_SET_SOC, socket };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;
	return 0;
}



//aktivieren der Betriebsspannung
int prog_target_run(void)
{
	if (prog.socket != SOC_18_ICSP)
	{
		fprintf(stderr, "## Target-Run-Function only aviable for ICSP-Connector\n");
		return -EFAULT;
	}

	// alles hochohmig
	unsigned char cmd[] = { CMD_SET_SOC, SOC_OFF };
	unsigned char buf[USB_BLOCKSIZE];
	int rc;
	rc = programmer_command(cmd, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd, 1)) return -EFAULT;

	//Run aktivieren
	unsigned char cmd1[] = { CMD_SET_SOC, SOC_RUN };
	buf[USB_BLOCKSIZE];
	rc = programmer_command(cmd1, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd1, 1)) return -EFAULT;
	prog_set_led(1);	// gelbe LED an
	puts(">> press ENTER to stop Target-Run");

	char c = getc (stdin);

	// alles hochohmig
	unsigned char cmd2[] = { CMD_SET_SOC, SOC_OFF };
	buf[USB_BLOCKSIZE];
	rc = programmer_command(cmd2, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd2, 1)) return -EFAULT;
	//wieder sockel 18/ICSP einstellen
	unsigned char cmd3[] = { CMD_SET_SOC, prog.socket };
	buf[USB_BLOCKSIZE];
	rc = programmer_command(cmd3, sizeof(cmd), buf);
	if (rc < 0) return rc;
	if (rc < 1) return -EFAULT;
	if (memcmp(buf, cmd3, 1)) return -EFAULT;
	prog_set_led(5);	// gelbe LED aus

	return 0;
}  // prog_target_run



int prog_regulate_vpp(double lower, double mid, double upper)
{
	int rc, i;
	unsigned char pwm_on, pwm_off;
	double v, v_mean = 0;
	
	pwm_off = (unsigned char)(mid * prog.gainOff - prog.pwm0VOff);
	pwm_on = (unsigned char)(mid * prog.gainOn - prog.pwm0VOn);
	
	fprintf(stderr, "prog_regulate_vpp(%.02f, %.02f, %.02f) --> %d, %d\n", 
		lower, mid, upper, pwm_off, pwm_on);
	
	
/*	rc = prog_set_vpp(0x00, 0x40);
	if (rc < 0)
		return rc;
	
	sleep(1); */
	
	rc = prog_set_pwm(pwm_off, pwm_on);
	if (rc < 0)
		return rc;
	
	usleep(500000);
	
	for (i=0; i<5; i++) {
		//rc = measure_vpp(&v);		DEBUG_SPRUT
		if (rc < 0)
			return rc;
		
		v_mean += v;
	}
	
	v_mean /= 5;
	fprintf(stderr, "prog_regulate_vpp(%.02f, %.02f, %.02f) --> %.02f\n", 
		lower, mid, upper, v_mean);
	
	if ((v_mean < lower) || (v_mean > upper)) {
		prog_set_vpp(10, 10);
		
		return -EFAULT;
	}
	
	return 0;
}  // prog_regulate_vpp


int prog_identify(unsigned char core, unsigned char socket)
{
	int rc;
	
	rc = prog_set_socket(socket);
	if (rc < 0) return rc;

	rc = prog_set_core(core);
	if (rc < 0) return rc;
	
	rc = prog_regulate_vpp(12.0, 12.5, 13.0);
	if (rc < 0) return rc;
	
	return prog_read_chipid();
}  // prog_identify


void init_system(void)
{
//	init_endian();		DEBUG_SPRUT
	usb_init();
	memset(&prog, 0, sizeof(prog));
	
	prog.socket        = SOC_18_ICSP;
	prog.core          = CORE_14;
	prog.flags1._byte  = 0;
	prog.flags2._byte  = 0;
	prog.flags3._byte  = 0;
	prog.max_flash	   = 0;
	prog.max_ee        = 0;
	prog.calmemsaved   = 0;
	prog.pic.vpp.deflt = 130;
	prog.supp          = 0xFF;
	prog.device 	   = -1;
	prog.VppLoopMode   = 3;			//0-nichts / 1-immer / 2-einmalig / 3- nur herunterregeln
	prog.U00_off       = 6.16;		// Offsetspannung
	prog.U00_on        = 6.33;		// Offsetspannung
	strcpy(prog.InHexfilename, "HexIn.hex");
	strcpy(prog.OutHexfilename, "HexOut.hex");
	strcpy(prog.picname, "");
	prog.OsccalPar     = no_OSCCAL;
	prog.OsccalRom	   = no_OSCCAL;
	prog.BGmask  	   = 0x0000;
	prog.BGadr   	   = 0x0000;
	prog.BGvalue 	   = 0x0000;
	prog.BGnewvalue	   = 0x0000;
	prog.WRITE_EDATA_KEY = 0;

}

void cleanup_system(void)
{
	if (prog.dev) {
//		prog_reset();
		usb_release_interface(prog.dev, prog.interface);
		usb_reset(prog.dev);
		usb_close(prog.dev);
	}
}

