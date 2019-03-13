/***************************************************************************
 *            database.c
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
 * this file contains everything to use the pic-database-files
 * the following functions can be called from outside:
 *
 * int db_lookup_pic(void);
 * int db_findpicname(TPicDef& pic);
 * int db_listpics(__uint8_t software);
 * int db_findpicid(__uint16_t picid, TPicDef& pic);
 * int db_load_db(void);
 * word db_getdefConfMask(int adr);
 * void db_find_BG(void);
 */


//********************************************************************************************************************
// includes
//********************************************************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include "b8.h"



//********************************************************************************************************************
// Definitions
//********************************************************************************************************************

using namespace std;

//ACHTUNG hier liegt das Limit für die Databasegrösse !!
//atention: this limits the possible size of the database !!
TPicDef   CfPIC[P_filesize];		//  800 x 216 =  167 k
TCfgbits  CfCfgbits[C_filesize];	// 6000 x  22 =  132 k
TField    CfField[F_filesize];		//20000 x  25 =  488 k
TSetting  CfSetting[S_filesize];	//60000 x  20 = 1172 k
//TChecksum Cfchecksum[4000];		// 4000 x  12 =   47 k

int	EfPIC      = 0;
int	EfCfgbits  = 0;
int	EfField    = 0;
int	EfSetting  = 0;
int	Efchecksum = 0;
int	Etex       = 0;

TCfgbits	Cfgbits;
TField		Field;


//********************************************************************************************************************
// Hilsroutinen
//********************************************************************************************************************

// wandelt die Strins aus der Pascal-Darstellung ins C-Format
// converts Pascal-strings into C-strings
void stringconvert(TPicDef& pic)
{
	char k = pic.name[0];
	char l;
	if (k<21) for (l=0; l<k; l++) pic.name[l]=pic.name[l+1];
	pic.name[l]=0;

	k = pic.ExtraStr[0];
	if (k<17) for (l=0; l<k; l++) pic.ExtraStr[l]=pic.ExtraStr[l+1];
	pic.ExtraStr[l]=0;
}



//********************************************************************************************************************
// DB-Routinen
//********************************************************************************************************************

// finde den zum namen passenden PIC
// finds the PIC with the known name
// input: prog.picname
int db_findpicname(TPicDef& pic)
{
	FILE *fdatei;
	fdatei = fopen(P_filename, "r");
	if (fdatei == NULL)  
	{		
		fprintf(stderr, "## missing Database - end program\n");
		return(1);
	}

	TPicDef newpic;
	int picSize = sizeof(newpic);
	if (f_i) fprintf(stdout, " Datastructure-Size %d \n", picSize);
	fread (&newpic, picSize, 1, fdatei);
	stringconvert(newpic);
	fprintf(stdout, "Database detected V.%d (%s)\n",newpic.cpu, newpic.name);

	while (!feof(fdatei))
	{
		fread (&newpic, picSize, 1, fdatei);
		stringconvert(newpic);
		//fprintf(stdout, " Name %s \n", newpic.name);
		if (!strcmp(newpic.name, prog.picname)) break;
	}
	fclose(fdatei);

	if (!strcmp(newpic.name, prog.picname)) 
	{
		pic = newpic;
		return(0);
	}
	return(-1);
}



// finde den zur ID passenden PIC
// finds PIC-ID for the known PIC
int db_findpicid(__uint16_t picid, TPicDef& pic)
{
	FILE *fdatei;
	fdatei = fopen(P_filename, "r");
	if (fdatei == NULL)  
	{		
		fprintf(stderr, "## missing Database - end program\n");
		return(1);
	}

	TPicDef newpic;
	int picSize = sizeof(newpic);
	if (f_i) fprintf(stdout, " size of datastructure %d \n", picSize);
	fread (&newpic, picSize, 1, fdatei);
	stringconvert(newpic);
	fprintf(stdout, "Database detected V.%d (%s)\n",newpic.cpu, newpic.name);

	unsigned char aktcpu = 0;
	switch (prog.core)
	{
		case CORE_12: aktcpu = 12; break;	// 10 kommt nicht vor
		case CORE_14: aktcpu = 14; break;	// 16
		case CORE_16: aktcpu = 16; break;	// 18
		case CORE_17: aktcpu = 16; break;	// 18K
		case CORE_18: aktcpu = 16; break;	// 18J 
		case CORE_30: aktcpu = 24; break;
		case CORE_33: aktcpu = 24; break;
	}

	while (!feof(fdatei))
	{
		fread (&newpic, picSize, 1, fdatei);
		stringconvert(newpic);
		//fprintf(stdout, " Name %s \n", newpic.name);
		if ( ((newpic.devid.id & newpic.devid.idmask) == (picid & newpic.devid.idmask)) && (newpic.cpu == aktcpu) ) break;

	}
	fclose(fdatei);

	if ( ((pic.devid.id & pic.devid.idmask) == (picid & pic.devid.idmask)) && (newpic.cpu == aktcpu) )
	{
		pic = newpic;
		return(0);
	}
	return(-1);
}



// Auflistung aller PICs der Database, die sich mit der Firmware brennen lassen
// lists all PICs, that can be programmed with the used firmware
int db_listpics(__uint8_t software)
{
	FILE *fdatei;
	fdatei = fopen(P_filename, "r");
	if (fdatei != NULL) fprintf(stdout, "Database detected \n");
	else 
	{		
		fprintf(stderr, "## missing Database - end program\n");
		return(1);
	}
	TPicDef newpic;
	int picSize = sizeof(newpic);
	fread (&newpic, picSize, 1, fdatei);
	int lines = 0;
	while (!feof(fdatei))
	{
		if (lines % 20 == 0) { fprintf(stderr, "\nName              Pins\n"); lines++; }
		fread (&newpic, picSize, 1, fdatei);
		stringconvert(newpic);

		// Ausgeben nur, wenn Brennsoftwarekenner eingetragen ist
		// list only if supported
		if (
		((newpic.software ==  1) && ((prog.supp & 0x01) != 0)) ||   //10F
		((newpic.software ==  2) && ((prog.supp & 0x02) != 0)) ||   //16F
		((newpic.software ==  4) && ((prog.supp & 0x04) != 0)) ||   //18F
		((newpic.software ==  5) && ((prog.supp & 0x08) != 0)) ||   //30F
		((newpic.software == 16) && ((prog.supp & 0x10) != 0)) ||   //18FxxJ
		((newpic.software == 32) && ((prog.supp & 0x20) != 0)) ||   //24 & 33F
		((newpic.software == 64) && ((prog.supp & 0x40) != 0))      //18FxxK
		) {
			fprintf(stdout, "%-18s %3d \n", newpic.name, newpic.pins);
			lines++;
		}
	}
	fclose(fdatei);
	return(0);
}



// check if database is existing
int db_lookup_pic(void)
{

	FILE *fdatei;
	fdatei = fopen(P_filename, "r");
	if (fdatei != NULL) fprintf(stdout, "Database detected \n");
	else 
	{		
		fprintf(stderr, "## missing Database - end program\n");
		return(1);
	}

	TPicDef pic;
	int picSize = sizeof(pic);
	//fprintf(stdout, " Datensatzlänge %d \n", picSize);

	fread (&pic, picSize, 1, fdatei);
	fprintf(stdout, " Name %s \n", pic.name);
	stringconvert(pic);
	fprintf(stdout, " Name %s \n", pic.name);

	fread (&pic, picSize, 1, fdatei);
	stringconvert(pic);
	fprintf(stdout, " Name %s \n", pic.name);

	fread (&pic, picSize, 1, fdatei);
	stringconvert(pic);
	fprintf(stdout, " Name %s \n", pic.name);

	fclose(fdatei);
};



// load the database into memory
//öffnen aller nötigen files
//im Fehlerfall wird der name des fehlenden files ausgegeben
int db_load_db(void)
{
	FILE *fdatei;

	//pics
	fdatei = fopen(P_filename, "r");
	if (fdatei == NULL) {fprintf(stderr, "## missing Databasefile %s - end program \n", P_filename); return(1);}
	TPicDef newpic;
	int picSize = sizeof(newpic);
	fread (&newpic, picSize, 1, fdatei);	//datum und version
	EfPIC = 0;
	while (!feof(fdatei))
	{
		fread (&newpic, picSize, 1, fdatei);
		CfPIC[EfPIC++] = newpic;
		if (EfPIC >= P_filesize) {fprintf(stderr, "## Databasefile %s to big - end program \n", P_filename); return(1);}
	}
	CfPIC[EfPIC].cpu = -1;	//endekennzeichen
	if (f_i) fprintf(stdout, "%d PICs loaded from Database \n", EfPIC);
	fclose(fdatei);

	//configs
	fdatei = fopen(C_filename, "r");
	if (fdatei == NULL) {fprintf(stderr, "## missing Databasefile %s - end program \n", C_filename); return(1);}
	TCfgbits newconfig;
	int configSize = sizeof(newconfig);
	EfCfgbits = 0;
	while (!feof(fdatei))
	{
		fread (&newconfig, configSize, 1, fdatei);
		CfCfgbits[EfCfgbits++] = newconfig;
		if (EfCfgbits >= C_filesize) {fprintf(stderr, "## Databasefile %s to big - end program \n", C_filename); return(1);}
		//fprintf(stdout, "%d  \n", newconfig.Nr);
	}
	CfCfgbits[EfCfgbits].Nr = -1;
	if (f_i) fprintf(stdout, "%d Configs loaded from Database \n", EfCfgbits);
	fclose(fdatei);

	//fields
	fdatei = fopen(F_filename, "r");
	if (fdatei == NULL) {fprintf(stderr, "## missing Databasefile %s - end program \n", F_filename); return(1);}
	TField newfield;
	int fieldSize = sizeof(newfield);
	EfField = 0;
	while (!feof(fdatei))
	{
		fread (&newfield, fieldSize, 1, fdatei);
		CfField[EfField++] = newfield;
		if (EfField >= F_filesize) {fprintf(stderr, "## Databasefile %s to big - end program \n", F_filename); return(1);}
		//fprintf(stdout, "%d  \n", newfield.Nr);
	}
	CfField[EfField].Nr = -1;
	if (f_i) fprintf(stdout, "%d Fields loaded from Database \n", EfField);
	fclose(fdatei);

	//settings
	fdatei = fopen(S_filename, "r");
	if (fdatei == NULL) {fprintf(stderr, "## missing Databasefile %s - end program \n", S_filename); return(1);}
	TSetting newsetting;
	int settingSize = sizeof(newsetting);
	EfSetting = 0;
	while (!feof(fdatei))
	{
		fread (&newsetting, settingSize, 1, fdatei);
		CfSetting[EfSetting++] = newsetting;
		if (EfSetting >= S_filesize) {fprintf(stderr, "## Databasefile %s to big - end program \n", S_filename); return(1);}
		//fprintf(stdout, "%d  \n", newsetting.Nr);
	}
	CfSetting[EfSetting].Nr = -1;
	if (f_i) fprintf(stdout, "%d Settings loaded from Database \n", EfSetting);
	fclose(fdatei);
	return(0);
}


//übergibt die field mit der Nummer nr
//falls keine passende field gefunden wird, dann wird im result.Nr=0 übergeben
TField db_getFieldNr(int nr)
{
	TField result;
	int count;
	result.Nr = 0;
	count = 0;
	while ((count < EfField) & (CfField[count].Nr != nr)) count++;
	if (CfField[count].Nr == nr) result = CfField[count];
        else result.Nr = 0;
	return result;
}


//übergibt die cfgbits mit der Nummer nr
//falls keine passende cfgbits gefunden wird, dann wird im result.Nr=0 übergeben
TCfgbits db_getCfgbitsNr(int nr)
{
	TCfgbits result;
	result.Nr = 0;
	int count = 0;
	while ((count < EfCfgbits) & (CfCfgbits[count].Nr != nr)) count++;
	if (CfCfgbits[count].Nr == nr) result = CfCfgbits[count]; else result.Nr = 0;
	return result;
}


//übergibt die cfgbits zum PIC mit der Adresse adr
//falls keine passende cfgbits gefunden wird, dann wird im result.Nr=0 übergeben
TCfgbits db_getCfgbitsAdr(int adr)
{
	TCfgbits result;
	result = db_getCfgbitsNr(prog.pic.config);
	while ((result.addr != adr) & (result.cfgbitsnr > 0))  result = db_getCfgbitsNr(result.cfgbitsnr);
	if (result.addr != adr) result.Nr = 0;
	return result;
}


//ermittelt die zur Adresse passende Configmaske
// dh. alle Bits die auf 1 gesetzt werden können
word db_getdefConfMask(int adr)
{
	word result = 0;
	Cfgbits = db_getCfgbitsAdr(adr);

	if (Cfgbits.Nr > 0)
	{
		//eine passende config wurde gefunden
		if (Cfgbits.fieldNr > 0)
		{
			//sie hat auch fields
			Field  = db_getFieldNr(Cfgbits.fieldNr);
			result = Field.mask;
			while (Field.fieldNr > 0)
			{
				Field  = db_getFieldNr(Field.fieldNr);
				result = result | Field.mask;
			}
			//unused bits der Config stehen nicht immer auf 1 (z.B. 16F630)
			//man braucht sie aber nicht zu vergleichen
			//deshalb entfällt die nächste Zeile
			//result:=result or Cfgbits.unused;
		}
	}
	return result;
}


// findet die passende Config-Adresse und Bitmaske für BG
void db_find_BG(void)
{
	int adr;
	prog.BGmask  = 0x0000;
	prog.BGadr   = 0x0000;
	prog.BGvalue = 0x0000;
	if (prog.core != CORE_14) return;

	for (adr=prog.pic.cfgmem.min; adr<=prog.pic.cfgmem.max; adr++) 
	{
		Cfgbits = db_getCfgbitsAdr(adr);
		if ((Cfgbits.Nr > 0) && (Cfgbits.fieldNr > 0))
		{
			//eine passende config wurde gefunden
			//sie hat auch fields
			Field  = db_getFieldNr(Cfgbits.fieldNr);
			if ((Field.Nr !=0 ) && (Field.flags == 2))
			{
				prog.BGadr  = adr;
				prog.BGmask = Field.mask;
			}
			while (Field.fieldNr > 0)
			{
				Field  = db_getFieldNr(Field.fieldNr);
				if ((Field.Nr !=0 ) && (Field.flags == 2))
				{
					prog.BGadr  = adr;
					prog.BGmask = Field.mask;
				}
			}
		}
	}

	if ((prog.BGadr != 0x0000) || (prog.BGmask !=0))
	{
		prog_read_CONFIG();
		prog.BGvalue = prog.HexOut.Config[confNr(prog.BGadr)] & prog.BGmask;
		fprintf(stdout, "BG-Adr:%5.4x  BG-mask:%5.4x  BG-value:%5.4x \n",  prog.BGadr, prog.BGmask, prog.BGvalue);
	}

	return;
}



