#ifndef __LIGHTWALL_H
#define __LIGHTWALL_H
#include <WProgram.h>
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

// These are used to adjust the internal timings during output.
// Depending on the tolerances of individual bulbs you may need to adjust these.
#define START_BLIP   PORTD = 0xFF; _delay_us(10);
#define START_NUM    PORTD = 0x00; _delay_us(10); 
#define START_BRIGHT PORTD = 0x00; _delay_us(10); 
#define START_COLOR  PORTD = 0x00; _delay_us(10); 

#define OUT_ONE      PORTD = 0x00; _delay_us(10); 
#define OUT_ZERO     PORTD = 0xFF; _delay_us(10); 
#define OUT_VAR(var) PORTD =  var; _delay_us(10); 

#define STOP_BIT     PORTD = 0xFF; _delay_us(10); 
#define STOP_BLIP    PORTD = 0x00;

#define DEFAULT_INTENSITY (0xCC)


class LightWall {
  public:
  
  /*
  * This array is the data transmitted directly onto the bus using digital pins 0-7.
  * Once the lightwall is intialized, Serial Bus transmission will be blocked.
  *
  * To keep speed up, we transmit to the same addres on 8 strings simultaneously.
  *
  * All 8 strings are multiplexed into a single byte.
  *    Pin: 76543210
  *    Bit: 87654321      
  *
  * The lights use 12-bit color, so we need 12 bytes.
  * The bits are ordered 4 Blue, 4 Green, 4 Red, most significant bit first
  *
  * The data bus is inverted, so the data going in must be inverted as well,
  */
  uint8_t Buffer[12];
  
  // Prior to initializtion and addressing, a 30 second delay is introduced to provide time
  // to reprogram the arduino.
  LightWall(void);
  ~LightWall(void);
  
  
  // Each string can have a maximum of 63 bulbs. One string may span multiple rows.
  // lightsPerRow sets the logical breaking point. See address_lights() for more information.
  void begin(uint8_t lightsPerString, uint8_t lightsPerRow);
  
  void fadeout(uint16_t time);// fade to black over time (in ms) (leaves intesity at 0)
  void fadein(uint16_t time);// fade in to default intensitiy over time (in ms)
  void fadein(uint16_t time, uint8_t intensity);// fade in to newIntensity over time (in ms)
  void blank_screen();//Fills screen with Black
  void set_intensity(uint8_t intensity);
  uint8_t lightsPerString();
  uint8_t lightsPerRow();
  
  /*Sends the data loaded in Buffer[] out to the address "num" on all 8 channels simultaneously
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
  *
  *   Start bit
  *   6-Bit Bulb Address, MSB first
  *   8-Bit Brightness, MSB first
  *   4-Bit Blue, MSB first
  *   4-Bit Green, MSB first
  *   4-Bit Red, MSB first
  *
  * It appears as if the lights are clocked off the falling edge.
  * Data is tested half-way between clocks.
  */
  void send_frame(uint8_t address);
  
  private:
  uint8_t LPS;
  uint8_t LPR;
  uint8_t intensity;
  
  
  /* 
  * On first power, all lights stop forwarding data and enter learning mode.
  * The first non-broadcast packet they see sets their address, after which
  * they begin forwarding data packets. 
  *
  * To initialize the string, we walk through the lights, 
  * sending the desired address out in *physical* connection order.
  * We assume a serpentine pattern, starting with the 00 and reversing after 
  * lightsPerRow until lightsPerString is reached.
  *
  * QC Co-Lab's Strings are configured with 30 Lights each.
  * There are 15 lights per row, with each row wired in the opposite direction.
  * Physical Layout:
  * [00][01][02][03][04][05][06][07][08][09][10][11][12][13][14]
  *                                                             )
  * [29][28][27][26][25][24][23][22][21][20][19][18][17][16][15]
  *
  * Logical Layout after addressing:
  * [00][01][02][03][04][05][06][07][08][09][10][11][12][13][14]
  * [15][16][17][18][19][20][21][22][23][24][25][26][27][28][29]
  * 
  */
  void address_lights();
};

#endif

