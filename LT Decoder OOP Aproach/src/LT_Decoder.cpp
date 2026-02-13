#include "LT_Decoder.h"
#include <iostream>

LT_Decoder::LT_Decoder()
    :inputs(nullptr), degres(nullptr),decodedpakiet(nullptr),tmpProcessedPacket(nullptr),sums(nullptr), startingDegres(nullptr), neighbours(nullptr)
     {
}
/*private:
    int pakietlenght;
    int liczbaPakietow;
    int licznika; //dlugosc pakietu
    int kontrol;

    // Dane
    unsigned char* tmpProcessedPacket; //tablica 1d
    unsigned char** inputs;
    unsigned char* degres;
    unsigned char* startingDegres;
    unsigned char** neighbours;
    unsigned char* sums;
    unsigned char** decodedpakiet;
    void deleteMem();
    */
void LT_Decoder::wczytajPakiety(const std::string& filename){
    file.open(filename,std::ios::in|std::ios::binary);
    if(!file){std::cout<<"Blad pliku!";return;}
    std::string temp;
    int i=0;
    std::getline(file, temp, ';');
    liczbaPakietow = std::stoi(temp);
    std::getline(file, temp, ';');
    kontrol = std::stoi(temp);

    inputs = new unsigned char*[liczbaPakietow*300];
    tmpProcessedPacket = new unsigned char[10000];
    degres = new unsigned char[liczbaPakietow*300];
    startingDegres = new unsigned char[liczbaPakietow*300];
    neighbours = new unsigned char*[liczbaPakietow*300];
    sums = new unsigned char*[liczbaPakietow*300];
    decodedpakiet = new unsigned char*[liczbaPakietow];

    for(int i=0; i<liczbaPakietow*300; i++) {
        inputs[i] = nullptr;
        tmpProcessedPacket[i] = NULL;
        degres[i] = 0;
        startingDegres[i] = 0;
        neighbours[i] = nullptr;
        sums[i] = nullptr;
        neighbours[i] = nullptr;
        if(i<liczbaPakietow){
            decodedpakiet[i]=nullptr;
        }
    }
}

void LT_Decoder::wczytajKolejnyPakiet(const std::string& filename){

    }
void LT_Decoder::signsToChars(int ln, unsigned char* &pointer){
        pointer = new unsigned char[ln];
        for(int i=0; i<ln; i++){
            pointer[i]=file.get();
        }
    }


int LT_Decoder::charToInt(unsigned char* tab, int lenght ){
        int checkSum=0;
        int r=1;
        for(int i=lenght-1; i>=0;i--){
            checkSum=(checkSum+(tab[i]*r));
            r*=10;
        }
        return checkSum;
    }


LT_Decoder::~LT_Decoder()
{
    ;
}
