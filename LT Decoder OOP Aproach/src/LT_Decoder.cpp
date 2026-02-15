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
    unsigned char* tmpProcessedPacket;
    unsigned char** inputs;
    unsigned char* degres;
    unsigned char* startingDegres;
    unsigned char** neighbours;
    unsigned char** sums;
    unsigned char** decodedpakiet;
    const int payloadLength = 3;
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

void LT_Decoder::loadNextEncodedSymbol(int counter){
    signsToChars(payloadLength,inputs[counter]);
    degres[counter]=file.get();
    startingDegres[counter]=degres[counter];
    signsToChars(int(degres[counter]), neighbours[counter]);
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
void LT_Decoder::calcsum(int counter){// funkcja obliczajaca sume kontrolna symbolu kodu jako int
    int suma=0;// suma kontrolna zakodowanego symbolu
    for(int i=0; i<payloadLength;i++){
    suma+=inputs[counter][i];
    }
    suma+=int(degres[counter]);
    for(int i=0; i<int(degres[counter]);i++){
    suma+=neighbours[counter][i];
    }
    if (suma == 0) {
        checksumSize = 1;
    } else {
        checksumSize = 0;
        int tempSuma = suma;
        while (tempSuma > 0) {
            tempSuma /= 10;
            checksumSize++;
        }
    }
    checkSum(suma,counter);
}
void LT_Decoder::checkSum(int suma, int counter){// funkcja obliczajaca sume kontrolna symbolu kodu jako int
    signsToChars(checksumSize, sums[counter]);
    if(suma != charToInt(sums[counter], checksumSize)) {
        delete[] inputs[counter];
        delete[] neighbours[counter];
        delete[] sums[counter];
        inputs[counter] = nullptr;
        neighbours[counter]=nullptr;
        degres[counter] = 0;
    } else {
    }
}


LT_Decoder::~LT_Decoder()
{
    ;
}
