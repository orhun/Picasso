usburn (fuer USB-Portbrenner Brenner8/9) V0.4 (16.05.2010)
=========================================================

Das ist die vierte Version von usburn fuer Linux.
Sie unterstuetzt den Brenner8 und den Brenner9.



Allgemeines
===========

Dies ist ein Linuxprogramms zur Ansteuerung meines Brenners8/9.

Mit einem Brenner 8 werden 14- und 16-Bit-Kern-PICs sowie einige 24-Bit Signalcontroller unterstuetzt.
Das sind die Serien PIC18Fxxxx, PIC16Fxxx, dsPIC30Fxxxx und viele PIC12Fxxx. Die PIC18FxxKxx werden mit dem Brenner8 und einem speziellen Adapter unterstuetzt.
Das Programm benoetigt dafuer einen Brenner8 mit Firmware 0.15 und die Database 24 oder neuer. All das befindet sich in dieser TAR-Datei

Mit einem Brenner 9 werden 3,3V-PICs der Serien PIC24F, PIC24H, dsPIC33 sowie PIC18FxxJxx unterstuetzt.
Das Programm benoetigt dafuer einen Brenner9 mit Firmware 3.12 und der Database 19 oder neuer. All das befindet sich in dieser TAR-Datei


Das Programm ist noch recht frisch, und wird noch viele Bugs enthalten. Ausserdem stellt es meinen ersten Gehversuch unter Linux und mit C++ dar.
Grosse Teile des Codes entstanden durch Umschreiben des Pascal-Codes meiner Windowssoftware, die ueber Jahre gewachsen ist.



Installation:
=============
Libusb installieren (z.B. mit dem Paketmanager der jeweiligen Linuxdistribution)
Das TAR-File in einen Ordner entpacken.
Eventuell das make-File anpassen.
make ausfuehren.




Nutzung:
========
Da der Zugriff auf den Brenner8&9 nicht über Kernelmodule, sondern direkt über libusb erfolgt,
sind eigentlich root-Rechte erforderlich. Damit alle User Zugriff bekommen, legt man eine
Datei "/etc/udev/rules.d/99-sprutbrenner" an und schreibt in diese Datei:

SUBSYSTEM=="usb", SYSFS{idProduct}=="ff0b", SYSFS{idVendor}=="04d8",
GROUP = "plugdev"

Damit sollte der Brenner ab dem nächsten Einstecken für alle User der plugdev-Gruppe (in der man
ja grundsätzlich sein muss, wenn man usb-Geräte benutzen möchte) verfügbar sein. Getested unter
Gentoo und Debian 4.0 & 5.0. (Danke Marcel)


Aenderungen zur Version 0.3
===========================
- Unterstuetzung der PIC18FxxKxx und PIX18LFxxKxx mit Brenner8+Adapter
- Unterstuetzung fuer PIC16LF7xx, PIC16LF1xxx (alles 3,3V-Typen) mit Brenner8+Adapter


Aenderungen zur Version 0.2
===========================
- automatische Umschaltung auf ICSP-Anschluss bei Nutzung der dsPIC30, dsPIC33, PIC24, PIC18FxxJxx, PIC10F und PIC1xF5xx
- fuer PIC24 gilt nun die Familie "24"
- BG (bandgap) -Einstellung des Herstellers bleiben erhalten
- OSCCAL bleibt erhalten
- OSCCAL kann auch manuell gesetzt werden (Option "-L" )


Aenderungen zur Version 0.1
===========================
- Problem mit der Falschen Erkennung einiger Typen behoben
- Optionen "-I" und "-O" ergänzt


Bekannte Probleme:
==================
- ich rate noch davon ab, PICs zu brennen, die einen OSCCAL-Wert im Flash oder in der Config enthalten. (ungetestet)
- ich rate dringend davon ab, PICs zu brennen, die einen BG (Bandgap)-Wert in der Config enthalten. (geht verloren!!)

- 16F639 wird als 16F636 behandelt (kein wirkliches Problem)
- 16F83 muss als 16F84 gebrannt werden
- 60- & 80-polige PICs muessen ueber die ICSP-Verbindung gebrannt werden.
- dsPIC30F-Typen muessen ueber die ICSP-Verbindung gebrannt werden.
- alle 3,3V-Typen muessen ueber die ICSP-Verbindung gebrannt werden.



Weitere Informationen und ein ausfuehrlichjes Handbuch findet man auf www.sprut.de

www.sprut.de/electronic/soft/usburn/linux/usburn_linux.htm


Ueber positive und negative Erfahrungen berichtet bitte an:
mailto: sprut@sprut.de


sprut 16.05.2010
http://www.sprut.de



//////////////////////////////////////////////////////////////////////////////////////////



Informationen fuer das Compilieren:
==================================



Folgende Biblitheken werden benoetigt:
-------------------------------------

LSB 3.2				(standardmaessig vorhanden)
libnsl.so.1
libusb-0.1.so.4			(libusb)




