#include <WProgram.h>
#include <Fat16.h>
#include <lightwall.h>
#include <delay_x.h>
#include "messagetype3_0.h"

static uint8_t header_3_0[2];
static uint16_t ifg = 500;
uint16_t readsize;

void PlayFile_3_0(Fat16 &file, LightWall &LW, void (*error_code)(int)) {
 /*Message Type 0 header: 4 bytes
  * 1 Byte:  Lights per string
  * 1 Byte:  Lights per Row
  * 2 Bytes: Inter-frame pause
  */ 
 if (file.read(header_3_0, 2) <2)
  {
    error_code(4); //Unexpected EOF
    return;
  }

  if (header_3_0[0] != LW.lightsPerString())
  {
    error_code(13); //Mismatched light count
    return;
  }
 
   if (header_3_0[1] != LW.lightsPerRow())
  {
    error_code(14); //Mismatched Row Size
    return;
  }
  if (file.read(&ifg, 2) <2) // Bytes 4 & 5 are Inter-frame pause
  {
    error_code(4); //Unexpected EOF
    return;
  }
  if (ifg == 0)
  { 
    ifg=1; 
  }

  readsize = file.read(LW.Buffer, 12);
  if (readsize <12)//File should never be empty here
    error_code(4); // Unexpected EOF

  int i = 0;
  while (readsize==12)
  {
    for (i = 0; i < LW.lightsPerString(); i++)
    {
      LW.send_frame(i);
      _delay_us(30);
      readsize = file.read(LW.Buffer, 12);
    }
    delay(ifg);
  }
  
  LW.fadeout(5000);

  LW.blank_screen();
  //reset intensity from fadeout
  LW.set_intensity(0xFF); //intensity=DEFAULT_INTENSITY;
  LW.send_frame(63);
  _delay_us(50);

  delay(1000);
}

