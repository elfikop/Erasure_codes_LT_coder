#include "LT_Encoder.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <string>


LT_Encoder::LT_Encoder()
:pakietlength(0),liczbaPakietow(0),kontrol(0),checksumSize(0),checksum(0),
	 counter(0),numOfDecodedPackets(0),tmpProcessedPacket(nullptr),degres(nullptr),
	 startingDegres(nullptr),inputs(nullptr),neighbours(nullptr),sums(nullptr),
	 decodedpackets(nullptr){
}

LT_Encoder::~LT_Encoder()
{
    //dtor
}
