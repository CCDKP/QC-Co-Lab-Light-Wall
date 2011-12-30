Lightwall Player v3.0
This code controls a series of GE Color-effects lights.
It uses Digital pins 0-7 for output.
It also makes use of the SD Card shield v2.1 from SeeedStudios 
    (http://www.seeedstudio.com/depot/sd-card-shield-for-arduino-v21-p-492.html)

Written by Chris Cooper (CCooper@QCCoLab.com, @CC_DKP)
Derived from the reference implementation by Mark Kruse
Based on the research by Robert "Darco" Quattlebaum (http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/)
  
= Data Files =

The player enumerates the root of the SD card, attempting to play specially crafted data files.
For more information on the data files, check the README.txt under the File Format folder.

= Error Codes = 

In the event of a failure, an error code will be displayed as a flashing red binary number.
A reference of existing error codes and their meanings can be found in ErrorCodes.txt

= Modules =

In order to support future growth of the data file format, Message Types are incuded as additional modules.
When adding support for additional message types, please add them as additional modules called by LightwallPlayer3.pde