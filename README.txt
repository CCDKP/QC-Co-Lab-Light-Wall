 Lightwall Library
  This code controls a series of GE Color-effects lights.
  It uses Digital pins 0-7 for output.
  A player is included in the examples, which will read specially formatted data files
  from an SD card and display them on the lightwall. See the README in 
  examples/lightwallplayer3/ for more details.

  Written by Chris Cooper (CCooper@QCCoLab.com, @CC_DKP)
  Derived from the reference implementation by Mark Kruse
  Based on the research by Robert "Darco" Quattlebaum
  (http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/)
  
  Designed for the QC Co-Lab Hackerspace
  Http://www.qccolab.com
  
  Copyright 2011 Chris Cooper

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  
= Installation =
* Copy this entire directory to arduino/libraries/LightWall/ 
* Install the Fat16 library from http://musicshield.googlecode.com/files/Fat16.zip
    * (Required for example player, not for the core library)
* Done!

= Notes = 
This code has been tested with Arduino 0.22. As of the time of this writing, 
the Fat16 library from MusicShield is not compatible with Arduino 1.0

= Additional Resources = 

Here are some related projects which you may find useful:

8cic-encode - Mark Riedesel - https://github.com/Klowner/8cic-encode
Generates type 0 animation files for the lightwall player from animated GIF's or PNG globs.

LightWallEditor - Mark Kruse - https://github.com/krusem/LightWallEditor
A Visual Basic program to edit and create animation files for the lightwall player.

 
