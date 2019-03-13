Readme.txt zur Database 24  vom 06.05.2010 für PBrennerNG, P18, dsProg und USBurn
(basierend auf MPLAB-IDE 8.50)


Inhalt
======

- Allgemeines
- Warum wird die Database überarbeitet?
- Legende
- wichtige Änderungen im Vergleich zur Database4
- alle Änderungen (auch die unwichtigen) im Vergleich zur Database4




Allgemeines
===========

Die Database enthält folgende Files:
- cekdef03.dat
- cfgdef03.dat
- fildef03.dat
- picdef03.dat
- setdef03.dat
- texdef03.dat
Alle Dateien sind gemeinsam in das Programmverzeichnis des Brennprogramms  PBrennerNG und/oder P18 und/oder dsProg und/oder USBurn zu kopieren. Dort ersetzen sie die gleichnamigen Dateien der vorherigen Database.

- picdef3.dll
Diese Database erfordert die picdef3.dll-Datei in der Version ab 3.3 (vom 03.04.2009) oder jünger. Die passende Version liegt der Database bei.

Diese Text-Datei beschreibt im Folgenden die Änderungen der Database19 im Vergleich zu Database18.



Warum wird die Database überarbeitet?
=====================================

1.
Der Hersteller Microchip passt die Programmierparameter kontinuierlich dem real existierenden Silizium an, diese Änderungen übernehme ich. Dabei handelt es sich meist um eine Verlangsamung des Timings, gelegentlich aber auch um größere Eingriffe in den Ablauf der PIC-Programmierung. Solche Änderungen betreffen meist nur PIC-Typen, die sich erst kurze Zeit auf dem Markt befinden.

2.
Neue PIC-Typen müssen in die Database aufgenommen werden. 

3.
Fehler die ich in der Behandlung einzelner PICs gemacht habe, muss ich korrigieren.




wichtige Änderungen im Vergleich zur Database 23
================================================
V. 23 vom 17/02/2010
V. 24 vom 06/05/2010
alt: 532 PIC-Typen  --> neu: 576 PIC-Typen

fuer diverse Typen wurden die Hardware-Informationen ueberarbeitet

PIC10F200 :  vpp.min vpp.max vpp.deflt
PIC10F202 :  vpp.min vpp.max vpp.deflt
PIC10F206 :  vpp.min vpp.max vpp.deflt
PIC12F519 :  eedata.min eedata.max
PIC12F617 neu in der Datenbank,  supported
PIC12F1822 neu in der Datenbank,  supported
PIC16F526 :  eedata.min eedata.max
PIC16F639 neu in der Datenbank,  supported
PIC16F716 :  latches.userid
PIC16F720 neu in der Datenbank,  supported
PIC16F721 neu in der Datenbank,  supported
PIC16F913 :  calmem.min calmem.max
PIC16F914 :  calmem.min calmem.max
PIC16F916 :  calmem.min calmem.max
PIC16F917 :  calmem.min calmem.max
PIC16F946 :  calmem.min calmem.max
PIC16F1823 neu in der Datenbank,  supported
PIC16F1824 neu in der Datenbank,  supported
PIC16F1825 neu in der Datenbank,  supported
PIC16F1826 neu in der Datenbank,  supported
PIC16F1827 neu in der Datenbank,  supported
PIC16F1828 neu in der Datenbank,  supported
PIC16F1829 neu in der Datenbank,  supported
PIC16F1933 :  blocksize devid.id
PIC16F1934 :  blocksize
PIC16F1936 :  blocksize
PIC16F1937 :  blocksize
PIC16F1938 neu in der Datenbank,  supported
PIC16F1939 neu in der Datenbank,  supported
PIC16F1946 neu in der Datenbank,  supported
PIC16F1947 neu in der Datenbank,  supported
PIC16LF720 neu in der Datenbank,  supported
PIC16LF721 neu in der Datenbank,  supported
PIC16LF1823 neu in der Datenbank,  supported
PIC16LF1824 neu in der Datenbank,  supported
PIC16LF1825 neu in der Datenbank,  supported
PIC16LF1826 neu in der Datenbank,  supported
PIC16LF1827 neu in der Datenbank,  supported
PIC16LF1828 neu in der Datenbank,  supported
PIC16LF1829 neu in der Datenbank,  supported
PIC16LF1933 :  blocksize devid.id
PIC16LF1934 :  blocksize
PIC16LF1936 :  blocksize
PIC16LF1937 :  blocksize
PIC16LF1938 neu in der Datenbank,  supported
PIC16LF1939 neu in der Datenbank,  supported
PIC16LF1946 neu in der Datenbank,  supported
PIC16LF1947 neu in der Datenbank,  supported
dsPIC33FJ128GP206A :  devid.id
dsPIC33FJ128GP306A :  devid.id
dsPIC33FJ128GP310A :  devid.id
dsPIC33FJ128GP706A :  devid.id
dsPIC33FJ128GP708A :  devid.id
dsPIC33FJ128GP710A :  devid.id
dsPIC33FJ128MC506A :  devid.id
dsPIC33FJ128MC510A :  devid.id
dsPIC33FJ128MC706A :  devid.id
dsPIC33FJ128MC708A :  devid.id
dsPIC33FJ128MC710A :  devid.id
dsPIC33FJ256GP506A :  devid.id
dsPIC33FJ256GP510A :  devid.id
dsPIC33FJ256GP710A :  devid.id
dsPIC33FJ256MC510A :  devid.id
dsPIC33FJ256MC710A :  devid.id
dsPIC33FJ32GS406 :  software userid.min userid.max devid.id
dsPIC33FJ32GS606 :  software userid.min userid.max devid.id
dsPIC33FJ32GS608 :  software userid.min userid.max devid.id
dsPIC33FJ32GS610 :  software userid.min userid.max devid.id
dsPIC33FJ64GP206A :  devid.id
dsPIC33FJ64GP306A :  devid.id
dsPIC33FJ64GP310A :  devid.id
dsPIC33FJ64GP706A :  devid.id
dsPIC33FJ64GP708A :  devid.id
dsPIC33FJ64GP710A :  devid.id
dsPIC33FJ64GS406 :  software userid.min userid.max devid.id
dsPIC33FJ64GS606 :  software userid.min userid.max devid.id
dsPIC33FJ64GS608 :  software userid.min userid.max devid.id
dsPIC33FJ64GS610 :  software userid.min userid.max devid.id
dsPIC33FJ64MC506A :  devid.id
dsPIC33FJ64MC508A :  devid.id
dsPIC33FJ64MC510A :  devid.id
dsPIC33FJ64MC706A :  devid.id
dsPIC33FJ64MC710A :  devid.id
PIC24HJ64GP206A :  devid.id
PIC24HJ64GP210A :  devid.id
PIC24HJ64GP506A :  devid.id
PIC24HJ64GP510A :  devid.id
PIC24HJ128GP206A :  devid.id
PIC24HJ128GP210A :  devid.id
PIC24HJ128GP306A :  devid.id
PIC24HJ128GP310A :  devid.id
PIC24HJ128GP506A :  devid.id
PIC24HJ128GP510A :  devid.id
PIC24HJ256GP206A :  devid.id
PIC24HJ256GP210A :  devid.id
PIC24HJ256GP610A :  devid.id
PIC18F66J90 :  software
PIC18F66J93 neu in der Datenbank,  supported
PIC18F67J90 :  software
PIC18F67J93 neu in der Datenbank,  supported
PIC18F86J60 :  wait.pgm
PIC18F86J65 :  wait.pgm
PIC18F86J72 neu in der Datenbank,  supported
PIC18F86J90 :  software
PIC18F86J93 neu in der Datenbank,  supported
PIC18F87J60 :  wait.pgm
PIC18F87J72 neu in der Datenbank,  supported
PIC18F87J90 :  software
PIC18F87J93 neu in der Datenbank,  supported
PIC18F96J60 :  wait.pgm
PIC18F96J65 :  wait.pgm
PIC18F97J60 :  wait.pgm
PIC18F13K22 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F13K50 :  eedata.max taktik.erase taktik.cp taktik.read_eeprom
PIC18F14K22 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F14K50 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F23K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F23K22 neu in der Datenbank,  supported
PIC18F24K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F24K22 neu in der Datenbank,  supported
PIC18F25K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F25K22 neu in der Datenbank,  supported
PIC18F26K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F26K22 neu in der Datenbank,  supported
PIC18F43K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F43K22 neu in der Datenbank,  supported
PIC18F44K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F44K22 neu in der Datenbank,  supported
PIC18F45K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F45K22 neu in der Datenbank,  supported
PIC18F46K20 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18F46K22 neu in der Datenbank,  supported
PIC18LF13K22 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18LF13K50 :  eedata.max taktik.erase taktik.cp taktik.read_eeprom
PIC18LF14K22 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18LF14K50 :  taktik.erase taktik.cp taktik.read_eeprom
PIC18LF23K22 neu in der Datenbank,  supported
PIC18LF24K22 neu in der Datenbank,  supported
PIC18LF25K22 neu in der Datenbank,  supported
PIC18LF26K22 neu in der Datenbank,  supported
PIC18LF43K22 neu in der Datenbank,  supported
PIC18LF44K22 neu in der Datenbank,  supported
PIC18LF45K22 neu in der Datenbank,  supported
PIC18LF46K22 neu in der Datenbank,  supported




wichtige Änderungen im Vergleich zur Database 22
================================================
V. 22 vom 18/12/2009
V. 23 vom 17/02/2010
alt: 532 PIC-Typen  --> neu: 532 PIC-Typen

Es wurden irrtümlich Config-Fehler bei den Typen 12F629 und 12F675 angezeigt. (Das Brennen war aber korrekt.)



wichtige Änderungen im Vergleich zur Database 21
================================================
V. 21 vom 24/07/2009
V. 22 vom 18/12/2009
alt: 532 PIC-Typen  --> neu: 532 PIC-Typen


alt: V. 21 vom 24/07/2009 (532 PIC-Typen)(davon werden 510 PICs durch meine Brennsoftware unterstützt)
neu: V. 22 vom 18/12/2009 (532 PIC-Typen)(davon werden 510 PICs durch meine Brennsoftware unterstützt)

Ein Bug verursachte gelegentlich Brennfehler am Ende des Programm- bzw. Datencodes bei dsPIC30-Typen.

dsPIC30F1010 :  power software taktik.erase taktik.cp
dsPIC30F2010 :  latches.pgm latches.eedata
dsPIC30F2011 :  latches.pgm latches.eedata
dsPIC30F2012 :  latches.pgm latches.eedata
dsPIC30F2020 :  power software taktik.erase taktik.cp
dsPIC30F2023 :  power software taktik.erase taktik.cp
dsPIC30F3010 :  latches.pgm latches.eedata
dsPIC30F3011 :  latches.pgm latches.eedata
dsPIC30F3012 :  latches.pgm latches.eedata
dsPIC30F3013 :  latches.pgm latches.eedata
dsPIC30F3014 :  latches.pgm latches.eedata
dsPIC30F4011 :  latches.pgm latches.eedata
dsPIC30F4012 :  latches.pgm latches.eedata
dsPIC30F4013 :  latches.pgm latches.eedata
dsPIC30F5011 :  latches.pgm latches.eedata
dsPIC30F5013 :  latches.pgm latches.eedata
dsPIC30F5015 :  latches.pgm latches.eedata
dsPIC30F5016 :  latches.pgm latches.eedata
dsPIC30F6010 :  latches.pgm latches.eedata
dsPIC30F6011 :  latches.pgm latches.eedata
dsPIC30F6012 :  latches.pgm latches.eedata
dsPIC30F6013 :  latches.pgm latches.eedata
dsPIC30F6014 :  latches.pgm latches.eedata
dsPIC30F6015 :  latches.pgm latches.eedata
dsPIC30F6010A :  latches.pgm latches.eedata
dsPIC30F6011A :  latches.pgm latches.eedata
dsPIC30F6012A :  latches.pgm latches.eedata
dsPIC30F6013A :  latches.pgm latches.eedata
dsPIC30F6014A :  latches.pgm latches.eedata
dsPIC30F2011es :  latches.pgm latches.eedata
dsPIC30F2012es :  latches.pgm latches.eedata
dsPIC30F6010es :  latches.pgm latches.eedata


wichtige Änderungen im Vergleich zur Database 19
================================================

alt: V. 19 vom 17/04/2009 (529 PIC-Typen) (davon werden 507 PICs durch meine Brennsoftware unterstützt)
neu: V. 20 vom 24/04/2009 (532 PIC-Typen) (davon werden 510 PICs durch meine Brennsoftware unterstützt)

Bei der Erstellung der DB19 waren folgende drei Typen leider "verloren" gegangen. Nun sind sie wieder dabei:

PIC16F54 
PIC16F57
PIC16F59 




wichtige Änderungen im Vergleich zur Database 18
================================================

alt: V. 18 vom 10/12/2008 (410 PIC-Typen) (davon werden 356 PICs durch meine Brennsoftware unterstützt)
neu: V. 19 vom 17/04/2009 (529 PIC-Typen) (davon werden 507 PICs durch meine Brennsoftware unterstützt)

Die Angabe der I/O-Funktionen der einzelnen PICs (I/O-Pins, ADC, USART, USB, CCP .....) ist bei vielen 
neuen Typen noch nicht in die Datenbank eingepflegt worden, da mir ein automatisiertes Einpflegen dieser 
Parameter z.Z. nicht mehr moeglich ist. Auf die Brennfunktion hat das keinerlei Einfluss.


Bei folgenden PICs wurden Brennparameter geaendert:

PIC10F204 :  vpp.min vpp.max vpp.deflt
PIC16F526 :  blocksize

PIC16F818 :  vpp.deflt
PIC16F819 :  vpp.deflt

PIC18F2410 :  calmem.min calmem.max
PIC18F2420 :  calmem.min calmem.max
PIC18F2423 :  calmem.min calmem.max
PIC18F4410 :  calmem.min calmem.max
PIC18F4420 :  calmem.min calmem.max
PIC18F4423 :  calmem.min calmem.max

dsPIC33FJ128GP802 :  devid.id
dsPIC33FJ128GP804 :  devid.id
dsPIC33FJ128MC202 :  devid.id
dsPIC33FJ128MC204 :  devid.id
dsPIC33FJ32GP302 :  devid.id
dsPIC33FJ32GP304 :  devid.id
dsPIC33FJ32MC302 :  devid.id
dsPIC33FJ32MC304 :  devid.id
dsPIC33FJ64GP802 :  pgmmem.max devid.id
dsPIC33FJ64GP804 :  devid.id
dsPIC33FJ64MC202 :  devid.id
dsPIC33FJ64MC204 :  devid.id

PIC24HJ32GP302 :  pgmmem.max devid.id
PIC24HJ32GP304 :  pgmmem.max devid.id
PIC24HJ64GP202 :  devid.id
PIC24HJ64GP204 :  devid.id
PIC24HJ64GP502 :  devid.id
PIC24HJ64GP504 :  devid.id

PIC18F63J11 :  wait.pgm
PIC18F63J90 :  wait.pgm
PIC18F64J11 :  wait.pgm
PIC18F64J90 :  wait.pgm
PIC18F65J11 :  wait.pgm
PIC18F65J50 :  wait.pgm
PIC18F65J90 :  wait.pgm
PIC18F66J11 :  wait.pgm
PIC18F66J16 :  wait.pgm
PIC18F66J50 :  wait.pgm
PIC18F66J55 :  wait.pgm
PIC18F67J11 :  wait.pgm
PIC18F67J50 :  wait.pgm
PIC18F83J11 :  wait.pgm
PIC18F83J90 :  wait.pgm
PIC18F84J11 :  wait.pgm
PIC18F84J90 :  wait.pgm
PIC18F85J10 :  wait.pgm
PIC18F85J11 :  wait.pgm
PIC18F85J15 :  wait.pgm
PIC18F85J50 :  wait.pgm
PIC18F85J90 :  wait.pgm
PIC18F86J10 :  wait.pgm
PIC18F86J11 :  wait.pgm
PIC18F86J15 :  wait.pgm
PIC18F86J16 :  wait.pgm
PIC18F86J50 :  wait.pgm
PIC18F86J55 :  wait.pgm
PIC18F87J10 :  wait.pgm
PIC18F87J11 :  wait.pgm
PIC18F87J50 :  wait.pgm


Folgende bereits in der DB enthaltenen PICs werden ab sofort von der Brennsoftware unterstuetzt:
(erfordert Brenner9 mit Firmware 3.12 oder neuer)

dsPIC33FJ128GP202
dsPIC33FJ128GP204
dsPIC33FJ128MC802
dsPIC33FJ128MC804
dsPIC33FJ64GP202
dsPIC33FJ64GP204
dsPIC33FJ64MC802
dsPIC33FJ64MC804

PIC24FJ64GA106
PIC24FJ64GA108
PIC24FJ64GA110
PIC24FJ64GB106
PIC24FJ64GB108
PIC24FJ64GB110
PIC24FJ128GA106
PIC24FJ128GA108
PIC24FJ128GA110
PIC24FJ192GA106
PIC24FJ192GA108
PIC24FJ192GA110
PIC24FJ256GA106
PIC24FJ256GA108
PIC24FJ256GA110
PIC24FJ128GB106
PIC24FJ128GB108
PIC24FJ128GB110
PIC24FJ192GB106
PIC24FJ192GB108
PIC24FJ192GB110
PIC24FJ256GB106
PIC24FJ256GB108
PIC24FJ256GB110

PIC24HJ128GP502
PIC24HJ128GP504


Folgende PICs wurden neu aufgenommen, und werden von der Brennsoftware auch unterstuetzt:

PIC16F722
PIC16F723
PIC16F724
PIC16F726
PIC16F727
PIC16F1933
PIC16F1934
PIC16F1936
PIC16F1937
PIC16LF722
PIC16LF723
PIC16LF724
PIC16LF726
PIC16LF727
PIC16LF1933
PIC16LF1934
PIC16LF1936
PIC16LF1937

dsPIC33FJ06GS101
dsPIC33FJ06GS102
dsPIC33FJ06GS202

dsPIC33FJ128GP206A
dsPIC33FJ128GP306A
dsPIC33FJ128GP310A
dsPIC33FJ128GP706A
dsPIC33FJ128GP708A
dsPIC33FJ128GP710A

dsPIC33FJ128MC506A
dsPIC33FJ128MC510A
dsPIC33FJ128MC706A
dsPIC33FJ128MC708A
dsPIC33FJ128MC710A

dsPIC33FJ16GS402
dsPIC33FJ16GS404
dsPIC33FJ16GS502
dsPIC33FJ16GS504
dsPIC33FJ256GP506A
dsPIC33FJ256GP510A
dsPIC33FJ256GP710A
dsPIC33FJ256MC510A
dsPIC33FJ256MC710A

dsPIC33FJ64GP206A
dsPIC33FJ64GP306A
dsPIC33FJ64GP310A
dsPIC33FJ64GP706A
dsPIC33FJ64GP708A
dsPIC33FJ64GP710A

dsPIC33FJ64MC506A
dsPIC33FJ64MC508A
dsPIC33FJ64MC510A
dsPIC33FJ64MC706A
dsPIC33FJ64MC710A

PIC24F04KA200
PIC24F04KA201
PIC24F08KA101
PIC24F08KA102
PIC24F16KA101
PIC24F16KA102
PIC24F32KA101
PIC24F32KA102
PIC24FJ32GA102
PIC24FJ32GA104
PIC24FJ64GA102
PIC24FJ64GA104





wichtige Änderungen im Vergleich zur Database 17
================================================

Bei einigen PICs gab es Fehlermeldungen ("ERROR in CONFIG") nach der automatischen Typerkennung oder 
nach dem Laden eines HEX-Files, die nicht wirklich auf ungültige Konfigurationen zurückgingen. 
Das Problem wurde behoben, indem für diese speziellen Fälle zusätzliche Konfigurationsoptionen 
definiert wurden.


neu aufgenommene Typen die experimentell unterstützt werden. Da ein konretes Dateblatt fehlt, sind einige Werte geraten.

PIC18F6628 :  software taktik.flash taktik.id taktik.config taktik.erase taktik.cp taktik.read_eeprom
PIC18F6723 :  software taktik.flash taktik.id taktik.config taktik.erase taktik.cp taktik.read_eeprom
PIC18F8628 :  software taktik.flash taktik.id taktik.config taktik.erase taktik.cp taktik.read_eeprom
PIC18F8723 :  software taktik.flash taktik.id taktik.config taktik.erase taktik.cp taktik.read_eeprom



wichtige Änderungen im Vergleich zur Database 16
================================================

Unmgenauigkeiten in der devid.id.mask blockierte die Erkennung von PIC18F2423/2523/4423/4523.

PIC18F2420 :  devid.idmask
PIC18F2520 :  devid.idmask
PIC18F4420 :  devid.idmask
PIC18F4520 :  devid.idmask



wichtige Änderungen im Vergleich zur Database 15
================================================

Wärend der Erstrellung der Database15 hatten sich leider Fehler eingeschlichen, die alle dsPIC30F-Typen betrafen.Diese Fehler wurden mit der Database16 korrigiert. Darüberhinaus gab es keine wesentlichen Änderungen.

alt: V. 15 vom 11.01.2008 (410 PIC-Typen) (davon werden 356 PICs durch meine Brennsoftware unterstützt)
neu: V. 16 vom 30.01.2008 (410 PIC-Typen) (davon werden 355 PICs durch meine Brennsoftware unterstützt)




wichtige Änderungen im Vergleich zur Database 14
================================================

Zu folgenden PIC-Typen gab es Änderungen, die sich direkt auf das Programmieren dieser Typen mit US-Burn, P18 oder PBrennerNG auswirken. Aufgelistet sind jeweils die Namen der geänderten Parameter, aber nicht deren Werte.

alt: V. 14 vom 10.09.2007 (390 PIC-Typen) (davon werden 336 PICs durch meine Brennsoftware unterstützt)
neu: V. 15 vom 11.01.2008 (410 PIC-Typen) (davon werden 356 PICs durch meine Brennsoftware unterstützt)



Veränderungen:

dsPIC30F2011es :  pgming.ovrpgm
dsPIC30F2012es :  pgming.ovrpgm
dsPIC33FJ128GP804 :  pgmmem.max 
dsPIC33FJ64GP802 :  pgmmem.max 
PIC24HJ256GP610 :  pgming.ovrpgm
PIC24HJ64GP504 :  devid.id
diverse PIC18FxxJxx: Blocksize



neu aufgenommene Typen die auch unterstützt werden sowie 
schon bekannte Typen, die nun ebenfalls unterstützt werden:

PIC10F200 
PIC10F202 
PIC10F204 
PIC10F206 
PIC10F220
PIC10F222 
PIC12F510 
PIC12F519 

PIC16F506 
PIC16F526 

PIC18F6393 
PIC18F6493 
PIC18F8393 
PIC18F8493 

dsPIC30F6010es

dsPIC33FJ16GP304
dsPIC33FJ16MC304
dsPIC33FJ32GP202 
dsPIC33FJ32GP204
dsPIC33FJ32MC202 
dsPIC33FJ32MC204 

PIC24FJ16GA002 
PIC24FJ16GA004 
PIC24FJ48GA002 
PIC24FJ48GA004 
PIC24HJ16GP304 

PIC24HJ32GP202
PIC24HJ32GP204 
PIC24HJ64GP502

PIC18F24K20
PIC18F25K20
PIC18F26K20 
PIC18F44K20 
PIC18F45K20 
PIC18F46K20 


Die PIC24/dsPIC33F-Typen werden ab US-Burn 1.7 mit Brenner9/Fw10 unterstützt.
Die PIC10F-Typen und die PIC1xF5x/5xx-Typen werden mit Brenner8/Fw11 unterstützt.



Ende

sprut
(www.sprut.de)






