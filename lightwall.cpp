#include <delay_x.h>
#include <WProgram.h>
#include "lightwall.h"

/*
  Lightwall Library
  This code controls a series of GE Color-effects lights.
  It uses Digital pins 0-7 for output.

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
  
  
LightWall::LightWall()
{

}

LightWall::~LightWall()
{
  DDRD = B00000010;//Enable Serial
}


void LightWall::begin(uint8_t lightsPerString, uint8_t lightsPerRow) 
{
  intensity = DEFAULT_INTENSITY;
  for (int i=0;i<12;i++)
    { Buffer[i] = 0; }// Initial buffer is White
  LPS = lightsPerString;
  if (lightsPerRow > lightsPerString)
    LPR = lightsPerString; 
  else
    LPR = lightsPerRow;
  

  //Enable RX TX Pins for Serial com
  DDRD = B00000010;
  //delay to catch serial com
  //DO NOT REMOVE THIS DELAY OR YOU WILL NOT BE ABLE TO REFLASH THE ARDUINO
  pinMode(13, OUTPUT);   
  for (int i=0;i<20;i++) //Blink status led on board while we wait
  {
    digitalWrite(13, HIGH);   
    delay(500);               
    digitalWrite(13, LOW);    
    delay(500);
  }
  //Set full output
  DDRD = B11111111;
    
  //Set the initial addressing for the bulbs
  address_lights();
}


// fade to black over time (in ms)
// intensity will be left at 0
void LightWall::fadeout(uint16_t time)
{
  uint16_t ipg=time*1000/intensity;
  //Keep minimum gap
  if (ipg<50) ipg=50;
  for (int i=intensity-1; i>=0; i--)
  {
    intensity=i;
    send_frame(63);
    _delay_us(ipg);
  }
}

// fade in to default intensitiy over time (in ms)
void LightWall::fadein(uint16_t time)
{
  uint16_t ipg=time*1000/DEFAULT_INTENSITY;
  //Keep minimum gap
  if (ipg<50) ipg=50;
  
  for (int i=0; i<DEFAULT_INTENSITY; i++)
  {
    intensity=i;
    send_frame(63);
    _delay_us(ipg);
  }
}

// fade in to newIntensity over time (in ms)
void LightWall::fadein(uint16_t time, uint8_t newIntensity)
{
  uint16_t ipg=time*1000/newIntensity;
  //Keep minimum gap
  if (ipg<50) ipg=50;
  
  for (int i=0; i<newIntensity; i++)
  {
    intensity=i;
    send_frame(63);
    _delay_us(ipg);
  }
}

//Sets screen to black
void LightWall::blank_screen()
{
  for (int i=0;i<12;i++)
  {  Buffer[i]=0xFF;  }
  for(int i=0;i<LPS;i++)
  {
    send_frame(i); 
    _delay_us(50);
  }
}

//Set intensity
void LightWall::set_intensity(uint8_t newIntensity)
{
  if (newIntensity > DEFAULT_INTENSITY)
    intensity = DEFAULT_INTENSITY;
  else
    intensity = newIntensity;
}

uint8_t LightWall::lightsPerString()
  { return LPS; }
  
uint8_t LightWall::lightsPerRow()
  { return LPR; }





/* Sends the data loaded in Buffer[] out to the address "num" on all 8 channels simultaneously
 * You must wait at least 30 uSeconds between frames
 *
 * It is important to distinguish that this does not send an entire image "frame" of video, but 
 * rather the required data for 1 pixel on each of the 8 data channels.
 *
 * From: http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/
 *   The protocol on the data line is simple and self-clocked. Here are the low-level details:
 * 
 *   Idle bus state: Low
 *   Start Bit: High for 10 uSeconds
 *   0 Bit: Low for 10 uSeconds, High for 20 uSeconds
 *   1 Bit: Low for 20 uSeconds, High for 10 uSeconds
 *   Minimum quiet-time between frames: 30 uSeconds
 *   Each frame is 26 bits long and has the following format:

 *   Start bit
 *   6-Bit Bulb Address, MSB first
 *   8-Bit Brightness, MSB first
 *   4-Bit Blue, MSB first
 *   4-Bit Green, MSB first
 *   4-Bit Red, MSB first
 *
 * It appears as if the lights are clocked off the falling edge.
 * Data is tested half-way between clocks.
 *
 * 
 */
 void LightWall::send_frame(uint8_t addr) {

  // Save interupt status
  uint8_t oldSREG = SREG;
  // disable interupts
  cli();
  
  //start blip
  START_BLIP;
  
  //send the address (6 bits, MSB first)
  START_NUM;
  if (addr & B100000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_NUM;
  if (addr & B010000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_NUM;
  if (addr & B001000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_NUM;
  if (addr & B000100) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_NUM;
  if (addr & B000010) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_NUM;
  if (addr & B000001) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;

  //send DEFAULT_INTENSITY (8 bits, MSB first)
  START_BRIGHT;
  if (intensity & B10000000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B01000000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00100000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00010000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00001000) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00000100) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00000010) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;
  START_BRIGHT;
  if (intensity & B00000001) {OUT_ONE;} else {OUT_ZERO;}
  STOP_BIT;

  //send Blue (4 bits, MSB first)
  START_COLOR;
  OUT_VAR(Buffer[0]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[1]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[2]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[3]);
  STOP_BIT;
 
  //send Green (4 bits, MSB first)
  START_COLOR;
  OUT_VAR(Buffer[4]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[5]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[6]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[7]);
  STOP_BIT;

  //send Red (4 bits, MSB first)
  START_COLOR;
  OUT_VAR(Buffer[8]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[9]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[10]);
  STOP_BIT;
  START_COLOR;
  OUT_VAR(Buffer[11]);
  STOP_BIT;

  //Set line idle
  STOP_BLIP;
  
  // Re-enable interupts if they were enabled
  SREG = oldSREG;
}

/* 
 * On first power, all lights stop forwarding data and enter learning mode.
 * The first non-broadcast packet they see sets their address, after which
 * they begin forwarding data packets. 
 * We assume a serpentine pattern, starting with the 00 and reversing after 
 * lightsPerRow until lightsPerStrand is reached.
 *
 */

 void LightWall::address_lights()
{
  int row = 0;
  int rowMax = 0;
  for (int i=0;i<LPS;i++)
  {  
    if (row%2 == 0) //Forward
	  send_frame(i);
    else // Backwards
	  send_frame((((row+1)*LPR)-1)-(i%LPR));
    if ((i%LPR) == (LPR -1))
      row++;
    delay(250);
  }
  delay(3000);
}
