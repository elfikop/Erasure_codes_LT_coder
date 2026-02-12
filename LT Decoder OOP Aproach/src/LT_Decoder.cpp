#include "LT_Decoder.h"

LT_Decoder::LT_Decoder()
    :inputs(nullptr), degres(nullptr),decodedpakiet(nullptr),tmpProcessedPacket(nullptr),sums(nullptr)
     {
}
void LT_Decoder::wczytajPakiety(const std::string& filename){
    std::ifstream file(filename,std::ios::in|std::ios::binary);
    if(!file){std::cerr<<"Blad pliku!";return;}
    std::string temp;
    int i=0;
    std::getline(file, temp, ';');
    liczbaPakietow = std::stoi(temp);
    inputs=new char*[liczbaPakietow];
    std::getline(file, temp, ';');
    kontrol = std::stoi(temp);

    inputs = new unsigned char*[liczbaPakietow];
    for(int i=0; i<liczbaPakietow; i++) {
        originalData[i] = nullptr;
    }

    file.close();
}


LT_Decoder::~LT_Decoder()
{
    //dtor
}
