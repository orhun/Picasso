/***************************************************************************
 *            calibration.c
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



//********************************************************************************************************************
// includes
//********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include "b8.h"



//** alles fuer kalibrierung ********************************************************************************



// berechnen der Pruefsumme fuer die Kalibrierdaten im EEPROM
int cal_pruefsumme(int anfang, int ende, unsigned char *dst)
{
	int result = 0;
	for (int k=anfang; k<=ende; k++) result += dst[k]+1;
	//fprintf(stdout, " Summe %4x  Zelle56 %4x \n", result, dst[56]);
	return(result & 0xFF);
}



// messung der PWM-Regelsteilheit
int cal_step3(void)
{
	float U10, Uhigh, U00;
	int pwm;
	char mode1ok; 	//Boolean;
	char calok = true;

	puts("\nVPP-Calibration: \n");
	//die höhere Spannung muss im ADC-Meßbereich liegen!!
	prog_set_signal(SIG_VPP_OFF);
	vpp_loop_off();
	prog_set_pwm(10, 10);
	sleepms(500);
	U10 = vpp_getVpp();
	pwm = 80;
	do
	{
		pwm = pwm-10;
		prog_set_pwm(pwm, pwm);
		sleepms(500);
		Uhigh  = vpp_getVpp();
	} while ((Uhigh >= 14) && (pwm >= 30));
	prog_set_pwm(10, 10);
	prog_set_signal(SIG_VPP_OFF);

	// falls der Regler nicht funktioniert, folgt nun eine division durch 0!!
	if (Uhigh <= U10)
	{
		fprintf(stderr, "## Vpp_off regulation not operational \n");
		calok = false;		//false
	} else {
		prog.d_gain_off  = (pwm-10) / (Uhigh-U10);			// pwm-Änderung für 1V
		prog.U00_off     = U10 - (Uhigh-U10)/((pwm-10)/10);		// Offset-Spannung
		prog.d_pwm0v_off = prog.U00_off * prog.d_gain_off ;		// Offset PWM
		fprintf(stdout, "-- U00_off = %f V   \n", prog.U00_off);  
		fprintf(stdout, "-- gain_off= %f %%/V \n", prog.d_gain_off  );  
		fprintf(stdout, "-- pwm0_off= %f %%   \n", prog.d_pwm0v_off);  
	};

	//die höhere Spannung muß im ADC-Meßbereich liegen!!
	prog_set_signal(SIG_VPP_ON);
	vpp_loop_off();
	prog_set_pwm(10, 10);
	sleepms(500);
	U10 = vpp_getVpp();
	// zwischendurch Test für Mode1 DC, reicht 48 (40%) aus, um unter Last 13V zu erreichen?
	pwm = 48;
	prog_set_pwm(pwm, pwm);
	sleepms(500);
	Uhigh = vpp_getVpp();
	if (Uhigh >= 13) {mode1ok = true} else {mode1ok = false};
	fprintf(stdout, "-- ULoad = %f V at PWM: %d % \n", Uhigh, pwm);  

	pwm = 80;
	do
	{
		pwm = pwm-10;
		prog_set_pwm(pwm, pwm);
		sleepms(500);
		Uhigh = vpp_getVpp();
	} while ((Uhigh >= 14) && (pwm >= 30));

	prog_set_pwm(10, 10);
	prog_set_signal(SIG_VPP_OFF);
	// falls der Regler nicht funktioniert, folgt nun eine division durch 0!!
	if (Uhigh <= U10)
	{
		fprintf(stderr, "## Vpp_off regulation not operational \n");
		calok = false;		//false
	} else {
		prog.d_gain_on  = (pwm-10) / (Uhigh-U10);			// pwm-Änderung für 1V
		prog.U00_on     = U10 - (Uhigh-U10)/((pwm-10)/10);		// Offset-Spannung
		prog.d_pwm0v_on = prog.U00_on * prog.d_gain_on  ;		// Offset PWM
		fprintf(stdout, "-- U00_on  = %f V   \n", prog.U00_on);  
		fprintf(stdout, "-- gain_on = %f %%/V \n", prog.d_gain_on  );  
		fprintf(stdout, "-- pwm0_on = %f %%   \n", prog.d_pwm0v_on);  
	};
	cal_write_data();
	if (calok)    fprintf(stdout, "-- Calibration O.K.; programmer operational \n");
	if (!mode1ok) fprintf(stderr, "## Vpp-Regulation-Mode1 can not be used!!   \n");
	return 0;  
};  //messung der PWM-Regelsteilheit



//0 - Befehl  WRITE_EDATA   READ_EDATA
//1 - Startadresse -low
//2 - Schreibschutzcode = Firmwareversion
//3 - Blocklänge
//4 - 1. Datenbyte
// Kalibrierdaten im EEPROM speichern
int cal_write_data(void)
{
	float v;

//	prog.d_Vusb_cal = cal_Kalibrierespannung();	// liefert aktuelle USB-Spannung
	// alles REAL-Werte -> 8 Byte  x 7 = 56 Byte (double)
	// Referenzspannung;
	// VppSpannungsteiler;
	// gain_off;
	// pwm0V_off;
	// gain_on;
	// pwm0V_on;
	// Vusb

	unsigned char buf[USB_BLOCKSIZE];
	int start = 0;
	int length = 57;
	for (int k=0; k<=60; k++) buf[k] = 0;

	memcpy(&buf,     &prog.d_Uz,        8);	//Referenzspannung
	memcpy(&buf[8],  &prog.d_DIV,       8);	//VppSpannungsteiler
	memcpy(&buf[16], &prog.d_gain_off,  8);
	memcpy(&buf[24], &prog.d_pwm0v_off, 8);
	memcpy(&buf[32], &prog.d_gain_on,   8);
	memcpy(&buf[40], &prog.d_pwm0v_on,  8);
	memcpy(&buf[48], &prog.d_Vusb_cal,  8);	//aktuelle Betriebsspannung
	buf[56] = cal_pruefsumme(0, 55, buf);
	
	if (prog_write_eedata(0, buf, 57))
	{
		fprintf(stderr, "## failes to save calibration-data");
		return -1;
	}
	return 0;
}; //savetoeeprom



// Kalibrierdaten aus Steuer-EEPROM auslesen
int cal_read_data(void)
{
	// Steuer-EEPROM auslesen
	//Sollwerte
	prog.d_Uz        = 3.3;
	prog.d_DIV       = 3.14;
	prog.d_gain_off  = 2.6;
	prog.d_pwm0v_off = 16;
	prog.d_gain_on   = 5.6;
	prog.d_pwm0v_on  = 35;
	prog.d_Vusb_cal  = 5;
	prog.d_korr	 = 1;

	unsigned char dst[USB_BLOCKSIZE];
	int start = 0;
	int length = 57;
	for (int k=0; k<=60; k++) dst[k] = 0;
	prog_read_eedata(start, length, dst);

	//for (int k=0; k<=60; k++) fprintf(stdout,"adr: %4x Wert %4x \n", k, dst[k]);
	//fprintf(stdout,"pruefsumme: %d \n", cal_pruefsumme(0, 55, dst));

	if (cal_pruefsumme(0, 55, dst) == dst[56])
	{
		memcpy(&prog.d_Uz, &dst, 8);
		memcpy(&prog.d_DIV, &dst[8], 8);
		memcpy(&prog.d_gain_off, &dst[16], 8);
		memcpy(&prog.d_pwm0v_off, &dst[24], 8);
		memcpy(&prog.d_gain_on, &dst[32], 8);
		memcpy(&prog.d_pwm0v_on, &dst[40], 8);
		memcpy(&prog.d_Vusb_cal, &dst[48], 8);
		if (f_i)
		{
			fprintf(stdout, "Uz        %f V \n", prog.d_Uz);		// Referenzspannung
			fprintf(stdout, "DIV       %f \n", prog.d_DIV);			// VppSpannungsteiler
			fprintf(stdout, "gain_off  %f \n", prog.d_gain_off);
			fprintf(stdout, "pwm0v_off %f \n", prog.d_pwm0v_off);
			fprintf(stdout, "gain_on   %f \n", prog.d_gain_on);
			fprintf(stdout, "pwm0v_on  %f \n", prog.d_pwm0v_on);
			fprintf(stdout, "Vusb_cal  %f V\n", prog.d_Vusb_cal);		// Betriebsspannung bei kalibrierung
		}
		fprintf(stdout,">> load calibration data\n");
	}
	else fprintf(stderr,"## no calibration data\n");

/*
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
*/
}



//** alles fuer VPP-Erzeugung ********************************************************************************



//Vpp-Stabilisierung aktivieren
// mode=0 : gar nicht
// mode=1 : kontinuierlich
// mode=2 : einmalig nach Vpp-Schaltung
// mode=3 : nur spannung herunterregeln
int vpp_loop_on(int mode)
{
	unsigned char L = prog.ADCL;
	unsigned char H = prog.ADCH & 0x03;	//hat nut 10 bit
	switch (mode)
	{
		case 2: H = H | 0x80; break;
		case 3: H = H | 0x40; break;
		case 0: H = 0; L = 0; break;
	}
	prog_set_vpp(L, H);
	return 0;
}



//Vpp-Stabilisierung abschalten
int vpp_loop_off(void)
{
	prog_set_vpp(0, 0);
	return 0;
}



//den Brenner an die aktuelle USB-Bus-Spannung anpassen
//dafuer wird die gespeicherte Uz und die gemessene Uz verrechnet
//Spannungskalibrierung
//liefert die USB-Betriebsspannung
float cal_Kalibrierespannung(void)
{
	vpp_loop_off();
	//Messen der Spannung an der Referenzdiode
	float Volt = vpp_getADC(1, 8);	// kanal 1, 8 Zyklen
	if (Volt != -1)
	{
		if (f_i) fprintf(stdout, "-- ADC-value   = %f \n", Volt);
		Volt = Volt * 5 / 1023;
		if (f_i) fprintf(stdout, "-- Raw-voltage = %f V\n", Volt);
		prog.d_korr = prog.d_Uz / Volt;
		float Vusb  = 5 * prog.d_korr;
		if (f_i) fprintf(stdout, "-- kor         = %f \n", prog.d_korr);
		if (f_i) fprintf(stdout, "-- Vdd         = %f V\n", Vusb);
		return Vusb;
	} else {
		fprintf(stderr,"## Calibration Error\n");
	}
	return 0;
}



//Messen des ADC-Ausgangswertes
//Kanal 0 : Vpp
//Kanal 1 : Uz
// aus mehreren Messungen wird der Durchschnitt ermittelt
// eine Messung dauert jeweils 15ms
// output: 10-Bit ADC in Real (noch nicht in volt umgerechnet)
float vpp_getADC(int kanal, int zyklen)
{
	float Volt = 0;
	int count  = 0;
	float Result = -1;

	//Messen der Spannung
	prog_set_an(kanal);
	sleepms(1);
	for (int k=1; k<= zyklen; k++)
	{
		Volt += prog_read_adc();
		count++;
	}
	
	if (count)
	{
		Volt = Volt / count;
		Result = Volt;
		prog.ADCL = (int)floor(Volt + 0.5) % 0x100;
		prog.ADCH = (int)floor(Volt + 0.5) / 0x100;
	} else {
		prog.ADCL = 0;
		prog.ADCH = 0;
	};
	return Result;
}; // get ADC



//Messen der Spannung
//Kanal 0 : Vpp
//Kanal 1 : Uz
//output: ADV-spannung in Volt
float vpp_getVoltage(int kanal, int Zyklen)
{
	float Volt;
	int k;
	int count;
	float Result = -1;
	//Messen der Spannung
	Volt = vpp_getADC(kanal, Zyklen);
	if (Volt != -1)
	{
		Volt = Volt * 5 / 1023;
		Volt = Volt * prog.d_korr;	//Kalibrierwert wegen abweichender USB-Spannung
		Result = Volt;
	};
	return Result;
}; //getVoltage



// messe die Perogrammierspannung in Volt
float vpp_getVpp(void)
{
	// kanal0 4 Messungen a 15 ms +60 ms
	return (vpp_getVoltage(0, 4) * prog.d_DIV); //3.1;
};



//messen der Programmierspannung
// wartet max 500 ms auf Stabilität
float vpp_getVpp_stable(void)
{
	int k;
	float U1;
	float U2;

	U1 = vpp_getVpp();
	k = 0;
	do
	{
		U2 = U1;
		sleepms(50);
		U1 = vpp_getVpp();
		k++;
	} while ((fabs(U2-U1) >= 0.05) && (k < 10));
	return U1;
}



//einstellen einer Programmierspannung
//dabei muß pwm begrenzt werden auf 10 .. 70
// pwm=120 entspricht 100%
int vpp_setVpp(float VppSoll)
{
	int	pwm;
	int	pwm_on;
	int	pwm_off;
	float	Vist;
	int	Versuche;
	int	step;
	int	step_s;
	int	old_step_s;
	float	Uerr;
	int	cross;
	float	pwm_cor;
	int	ADCsoll;
	if (prog.device != DEVICE_B8) return 0;		// Brenner9
	if (VppSoll < 5) return 0;			//3V-PIC

	if (fabs(vpp_getVpp()-VppSoll) < 0.2) return 0; // gut genug
	vpp_loop_off();
	if ((VppSoll > 14) || (VppSoll < 8.5))
	{
		fprintf(stderr, "## Vpp-soll: out of range   ");
		return 0;
  	};
	// Theorie:
	// Uout = Uin *  (Ton + Toff) / Toff
	//      = 4,7V * 120 /(120-pwm)
	//
	// 1V entspricht einer PWM-Änderung um 2,6
	// 1V entspricht einer PWM-Änderung um 5,8 unter Last

	// korrekturwert, da Betriebsspannung bei der Kalibrierung anders war
	pwm_cor = 1;
	// Vusb_cal:=4.75;
	// if Vusb_cal>4 then pwm_cor:=Vusb/Vusb_cal;
	pwm_cor = pwm_cor * pwm_cor;

	pwm_off = d2c(VppSoll * prog.d_gain_off /pwm_cor - prog.d_pwm0v_off);
	pwm_on  = d2c(VppSoll * prog.d_gain_on  /pwm_cor - prog.d_pwm0v_on - 0.5);

	pwm = pwm_off;
	//failsafe
	if (pwm < 0)      pwm    =   0;
	if (pwm > 100)    pwm    = 100;
	if (pwm_on < 0)   pwm_on =   0;
	if (pwm_on > 100) pwm_on = 100;

	if (f_i) fprintf(stdout, "PWM= %d  PWM_on= %d\n", pwm, pwm_on);
	prog_set_pwm( pwm,  pwm_on);

	// im Mode 1 wird mit einem festen PWM von 192/4=48 gearbeitet -> 40% -> 4us_on  6us_off
	// das reicht für 18,3V ohne Last und 14,2V mit Last

	if (prog.VppLoopMode > 1)
	{
		sleepms(200);

		Vist = vpp_getVpp_stable();
		if (f_i) fprintf(stdout, "Vpp-mess: %f V \n", Vist);
		Versuche = 0;
		step = 2;
		cross = 2;		//Sollwert darf maximal 2 mal durchgangen werden
		while ((fabs(Vist-VppSoll) > 0.2) && (fabs(step) >= 1) && (Versuche < 20) && (pwm > 5) && (pwm < 70) && (cross > 0))
		{
			Versuche++;
			//pwm kann schnell steigen, aber soll langsam fallen
			Uerr = VppSoll-Vist;
			step = (int)floor(Uerr * prog.d_gain_off +0.5);
			if (step >= 0) step_s = (int)floor(step * 0.8 + 0.5);		// >  0 : schneller Anstieg
			else if (step < -5)  step_s = (int)floor(step/5 + 0.5);		// < -5 : langsames Abfallen
			else if (step <= -1) step_s = -1;					//-1 .. -5
			else step_s = 0;								// 0.. -1
			pwm = pwm + step_s;

			if (Versuche > 0) if (step_s*old_step_s <= 0)cross--;		// Wechsel der Regelrichtung
			old_step_s = step_s;

			if (pwm > 70) pwm = 70;			// max. DC
			if (pwm <  5) pwm =  5;			// min. DC
			prog_set_pwm( pwm,  pwm_on);
			sleepms(50);
			Vist = vpp_getVpp_stable();
			if (f_i) fprintf(stdout, "Vpp-mess: %f V \n", Vist);
		};
		if (f_i) fprintf(stdout, "PWM: %d \n", pwm);
		vpp_loop_on(prog.VppLoopMode);
	}
	else		// VppLoopMode == 1
	{
		//SollADCwert für kontinuierliche Regelung festlegen
		ADCsoll = (int)floor((VppSoll/prog.d_DIV) / (5*prog.d_korr/1024) + 0.5);
		prog.ADCL = ADCsoll % 0x100;
		prog.ADCH = ADCsoll / 0x100;
		vpp_loop_on(1);

		if (f_i) fprintf(stdout, "PWM: %d \n", pwm);
		vpp_loop_on(prog.VppLoopMode);
		sleepms(100);
		if (f_i) fprintf(stdout, "Vpp-mess: %f V \n", vpp_getVpp() );
	};  
}//setVpp





