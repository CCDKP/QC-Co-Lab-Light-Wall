/*
  Lightwall Player v3.0
  This code controls a series of GE Color-effects lights.
  It uses Digital pins 0-7 for output.
  It also makes use of the SD Card shield v2.1 from SeeedStudios (http://www.seeedstudio.com/depot/sd-card-shield-for-arduino-v21-p-492.html)

  Written by Chris Cooper (CCooper@QCCoLab.com, @CC_DKP)
  Derived from the reference implementation by Mark Kruse
  Based on the research by Robert "Darco" Quattlebaum (http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/)
  
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

*/
#include <Fat16.h> //http://musicshield.googlecode.com/files/Fat16.zip
#include <lightwall.h>
#include <delay_x.h> // by Hans-Juergen Heinrichs

/*************************************************
 * Include message type modules here.            *
 * Don't forget to add them to the switch below. *
 *************************************************/
#include "messagetype3_0.h"

#define INTER_FRAME_GAP 500 // Default delay between frames, in mS


uint16_t ifg = INTER_FRAME_GAP;


SdCard card;
Fat16 file;
dir_t dirList;
uint16_t dirIndex = 0;
uint8_t header[2];
LightWall LW;

void setup() 
{

  LW.begin(30,15);
  if (!card.init()) error_die(8); //Error Initializing SD Card
  
  // initialize a FAT16 volume
  if (!Fat16::init(&card)) error_die(9); //Error Initializing Filesystem
}

void loop()
{
  
  //test directory for file
  if (file.readDir(&dirList, &dirIndex, DIR_ATT_VOLUME_ID))
  {
    if (!file.open(dirIndex, O_READ))
    { error_code(10); } //Error opening file
    else if (file.read(header, 2) <2) //read header
    {
      file.close();  
      error_code(4); //Unexpected EOF
    }
    else if (header[0] == 3)
    {
      switch (header[1]) 
      {
        /**************************
         * ADD MESSAGE TYPES HERE *
         **************************/
        case 0:
          PlayFile_3_0(file, LW, &error_code);
          break;      
        default:
          error_code(12); //Unsupported Message Type
          break;
      }
    }
    else 
    {
      error_code(11); //Unsupported Protocol
    }
  
    file.close();  
    dirIndex++;
  }
  else
  {
    dirIndex = 0;
    demo();
  }
}


//Test array, 1 bit color resolution, used for debug testing and demo() output.
//                        Don't forget to invert the data!
//               RGBPYTWB  GBPYTWBR  BPYTWBRG  PYTWBRGB  YTWBRGBP  TWBRGBPY  WBRGBPTY  BRGBPYTW
byte rbits[] = {B01100101,B11001010,B10010101,B00101011,B01010110,B10101100,B01011001,B10110010}; 
byte gbits[] = {B10110001,B01100011,B11000110,B10001101,B00011011,B00110110,B01101100,B11011000};   
byte bbits[] = {B11001001,B10010011,B00100111,B01001110,B10011100,B00111001,B01110010,B11100100};  
int demoBytes=8; //length of the demo array


// When this is called, the bits array is loaded with the next data set from the demo array 
int demopos=0;
void next_test_data()
{
  for (int i=0;i<4;i++)
  {
    LW.Buffer[i]=bbits[demopos];
    LW.Buffer[i+4]=gbits[demopos];
    LW.Buffer[i+8]=rbits[demopos];
  }
  demopos++;
  if (demopos==demoBytes) demopos=0;
}


// Demo Routine plays 
void demo()
{
  for (int i=0;i<10;i++)
  { 
    next_test_data();
    for (int j=0;j<30;j++)
    {
      LW.send_frame(j);
      _delay_us(48);

    }
    delay(1000);
  }
  LW.fadeout(3000);
  for (int i=0;i<30;i++)
  {
    LW.send_frame(i);
    _delay_us(48);
  }
  delay(1000);
  LW.fadein(3000);
  delay(1000);
}

// Soft Error
// Displays error in binary then continue
void error_code(int error)
{
  LW.blank_screen(); //clear the screen & buffer
  LW.Buffer[8]=LW.Buffer[9]=LW.Buffer[10]=LW.Buffer[11]=0x00; //Display in RED

  for (int i=0;i<16;i++)
  {
    if ((error >> i) & 1) 
      {
        LW.send_frame(i);
        _delay_us(50);
      }
  }
  // Blink error
  for(int i=0;i<10;i++)
  {
   delay(500); 
   LW.fadeout(3000);
   delay(500); 
   LW.fadein(3000);
  }
}

// Hard Error
// Alternate between error code and Demo() 
void error_die(int error)
{
  while (1)
  {
    error_code(error);
    for(int i=0;i<5;i++)
    {
      demo();
    }     
  }
}

