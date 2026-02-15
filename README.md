Luby Transform codes file data encoding and decoding
Luby Transform codes provide a robust method for transmitting data over lossy channels by generating an endless stream of encoded packets from a source file.
This project implements the core logic of fountain codes through the LT_encoder and LT_decoder classes, allowing for the reconstruction of original information regardless of which specific packets are lost,
provided a sufficient threshold of data is received.

The LT_encoder class handles the initial fragmentation of the source file into blocks of a fixed size
For each encoded symbol, the encoder selects a degree based on a specific probability distribution and randomly chooses a corresponding number of source blocks.
These blocks are then combined using bitwise XOR operations to create the payload. The output format is structured to include metadata such as the degree and the indices of the source blocks involved,
followed by a calculated checksum to ensure data integrity during transmission.

The LT_decoder class manages the reconstruction process through a belief propagation algorithm.
It begins by initializing memory structures based on the header information of the received file, which includes the total number of source blocks. 
As symbols are read, the decoder verifies their integrity using the calcsum and checkSum methods. 
In this place the project is done by date 13.02.26, the future goals to complete the decoding process are:
If a symbol has a degree of one, it directly reveals a source block and signed to apropriate index of decoded packets dynamic array.
This newly recovered block is then used to reduce the degree of other buffered symbols through further XOR operations.
This cascading effect continues until all original source blocks are recovered.

Memory management in the decoder is handled through dynamic allocation of two-dimensional pointer arrays to store payloads, neighbor indices, and checksums.
Each symbol processed undergoes a verification step where the numerical sum of the payload, degree, and neighbors is compared against the stored checksum value to filter out corrupted packets before they enter the decoding graph.
If the checksum is not correct, the data decoder shall skip bytes read from file untill it reaches ';' sign which indicates the beggining of another encoding symbol.

Ideas to test the codes in practice:
- The final stage of the project after succsesfull decoding of an image is going to be to encode data, write it down on a compact disk, damage it by some sharp tools and then try to restore the original and decode them!
- Second idea is to modify the encoder and decoder clases to write and read to a serial port of the computer instead of the encoded file with a sufficient frequency, the idea would be to then transmit and catch the data via arduino with nrf24l01 module with
  the insuficient db settings for the distance between transmiter and the receiver ( some packets are lost some arent ). Transmit and decode the image and calculate the number of encoded symbols that were neeeded to decode the image and the num of encoded symbols sent.
  to compare LT codes with another transmition ways we can set the nrf24l01 settings to try to resend the packet untill the receiver acktknowledges the packet was received. Sending the packets with the same frequency and counting everything up(amount of packets sent) and comparing them to lt codes transmision would give us a nice comparison of reception overhead in both lt codes and standard transmision.
