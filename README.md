PicUsb
======

A super simple usb starter for for PIC18F microcontrollers. Designed to be a starting point for better understanding how to use USB with the 18F chip set.

This project acts as a HID keyboard. The code requires a switch on TRISB4. When this switch is pressed it will send key presses to the attached host. The project also uses LEDs on LATE0 and LATE1, but these are not required for proper functioning.

Based on the fine work of Jan Axelson found at http://pe.ece.olin.edu/ece/projects.html (which seems to now be a dead link, but can still be found at http://web.archive.org/web/20120630005824/http://pe.ece.olin.edu/ece/projects.html)

This project was written against the PIC 18F87J94 using the MA180033  - PIC18F87J94 Plug in Module evaluation board (http://www.microchipdirec...px?Keywords=MA180033), but should work reasonably well on any PIC 18F chip.

This project was written with a few objectives in mind:
- Highly readable/understandable code
- As small and simple as possible
- Uses the XC8 compiler
- Compiles and works on both Mac and PC

LINKS:
PIC18F97J94 Family Datasheet (usb section starts at page 525) - http://ww1.microchip.com/downloads/en/DeviceDoc/30575A.pdf
USB Made Simple (intro to USB spec) - http://www.usbmadesimple.co.uk/ums_1.htm