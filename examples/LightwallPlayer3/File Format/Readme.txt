Last revision: July 20th, 2011

Lightwall Protocol Spec.

The lightwall data file contains a two-byte header describing its version and message type.
Each message type may have additional header information.

The first Byte is always protocol version number. This document describes protocol version “3”.
The Second Byte defines what type of message is contained within the file.
The valid message types are listed below.


= Message Type 0: 8 channel interlaced Raw Stream (8cic Raw) = 


Byte 3 contains the number of pixels per channel (PPC) and must be less than 64

Byte 4 contains the number of columns per row (COL). It must be less than or equal to the number of pixels indicated in byte 3.

Bytes 5 & 6 is a 16-bit unsigned integer representing the pause time to use between frames in milliseconds.

Byte 7 starts the data stream.

The data consists of a stream of 12-byte long 8 channel interlaced color (8cic) packets described below. Each is sent out addressed sequentially, starting at 0 and continuing to one less than PPC before starting again at 0.


8cic data packets:
The 8-Channel interlaced color packets consist of 12 bytes of data.
It is sent out all 8 channels simultaneously to the same address on each channel. 
The data includes 4 bits of Blue, followed by 4 bits of Green, the 4 bits of Red. They are each stored most significant bit(MSB) first.

The 8 channel’s data is interlaced together. Channel 0’s data is always in the MSB, while channel 7 is in the least significant bit (LSB). This means the first byte in the frame contains the MSB of Blue color for each strand while the 12th byte contains the LSB of Red color for each strand. If less than 8 channels are needed, the data must be padded to 8 channels.

Finally, each byte must be inverted prior to writing (XOR 0xFF)


Proposed message types:

Message ID 1: 8 channel interlaced Per Frame Delay (8cic PFD)
	Similar to Type 0, but a 2 byte delay follows each 8cic packet.


Message ID 2: 8 channel interlaced addressed pixels (8cic addr)
	per-packet address & delay



A graphical version of this document will be updated here:
https://docs.google.com/spreadsheet/pub?hl=en_US&hl=en_US&key=0ArdMzSs8kb-YdHJ4ZWxndnE4S3A3X0E3ZEpKQmNzX2c&output=html
	