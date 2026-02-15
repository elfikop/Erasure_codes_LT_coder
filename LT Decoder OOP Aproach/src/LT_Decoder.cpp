#include "LT_Decoder.h"
#include <iostream>


LT_Decoder::LT_Decoder()
    :inputs(nullptr), degres(nullptr),decodedpakiet(nullptr),tmpProcessedPacket(nullptr),sums(nullptr), startingDegres(nullptr), neighbours(nullptr)
     {
}

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
    decodedpacket = new unsigned char*[liczbaPakietow];

    for(int i=0; i<liczbaPakietow*300; i++) {
        inputs[i] = nullptr;
        tmpProcessedPacket[i] = NULL;
        degres[i] = 0;
        startingDegres[i] = 0;
        neighbours[i] = nullptr;
        sums[i] = nullptr;
        neighbours[i] = nullptr;
        if(i<liczbaPakietow){
            decodedpacket[i]=nullptr;
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
    checksum=0;// suma kontrolna zakodowanego symbolu
    for(int i=0; i<payloadLength;i++){
    checksum+=inputs[counter][i];
    }
    checksum+=int(degres[counter]);
    for(int i=0; i<int(degres[counter]);i++){
    checksum+=neighbours[counter][i];
    }
    if (checksum == 0) {
        checksumSize = 1;
    } else {
        checksumSize = 0;
        int tempSuma = checksum;
        while (tempSuma > 0) {
            tempSuma /= 10;
            checksumSize++;
        }
    }
    checkSum(checksum,counter);
}
bool LT_Decoder::checkSum(int suma, int counter){// funkcja obliczajaca sume kontrolna symbolu kodu jako int
    signsToChars(checksumSize, sums[counter]);
    if(suma != charToInt(sums[counter], checksumSize)) {
        deleteEncodedSymbol();
        return false;
    }
    else
        return true
}
void LT_Decoder::deleteMem(unsigned char* &tab, int length){
    if(tab!=nullptr){
        for(int i=0;i<length;i++)
            delete[] tab[i];
        delete[] tab;
        tab=nullptr;
    }
}

void LT_Decoder::copymem(int alrernateIndex){
    int sourceIndex=neighbours[alrernateIndex][0]-1;
	if(decodedpackets[sourceIndex]==nullptr){
		decodedpackets[sourceIndex]=new unsigned char[payloadLength];
		for(int i=0;i<payloadLength;i++){
			decodedpackets[sourceIndex][i]=inputs[alrernateIndex][i];
		}
	}
}

void LT_Decoder::deleteEncodedSymbol(int alrernateIndex){
    delete[] inputs[alrernateIndex];
    delete[] neighbours[alrernateIndex];
    delete[] sums[alrernateIndex];
    inputs[alrernateIndex] = nullptr;
    neighbours[alrernateIndex]=nullptr;
    degres[alrernateIndex] = 0;
}

void LT_Decoder::xorNeighbour(int neighbourToCheck, int neighbourPos){
    if(decodedpackets[neighbourToCheck-1]!=nullptr){
        for(int i=0; i<payloadLength;i++){
            inputs[counter][i]^=(unsigned char)decodedpackets[neighbourToCheck-1][i];
        }
        neighbours[counter][neighbourPos]=NULL;
        if(degres[counter]<2)
            cout<<"stopien mniejszy rowny 1!"<<endl;
        else
            degres[counter]--;
    }
}
bool LT_Decoder::procesCurrentSymbol(int alrernateIndex){
	if(degres[alrernateIndex]==1){
		int indexOdkodowany=neighbours[alrernateIndex][0];
		if(decodedpackets[indexOdkodowany-1]==nullptr){
			copymem(alrernateIndex);
			numOfDecodedPackets++;
		}
		deleteEncodedSymbol(alrernateIndex);
		for(int i=0;i<counter;i++){
			if(inputs[i]!=nullptr){
				for(int j=0;j<startingDegres[i];j++){
					if(neighbours[i][j]==indexOdkodowany){
						xorNeighbour(indexOdkodowany,j,i);
						if(degres[i]==1)procesCurrentSymbol(i);
					}
				}
			}
		}
	}
	return (numOfDecodedPackets==liczbaPakietow);
}
bool LT_Decoder::startDecoding(){
    counter=0;
    numOfDecodedPackets = 0;
    while(!file.eof()&&numOfDecodedPackets<liczbaPakietow){
        loadNextEncodedSymbol(counter);
        calcsum(counter);
        if(inputs[counter]!=nullptr){
            procesCurrentSymbol(counter);
            counter++;
        }
    }
    return (numOfDecodedPackets == liczbaPakietow);
}
bool LT_Decoder::restoreFile(){
    if(numOfDecodedPackets<liczbaPakietow)return false;
    std::ofstream out("reconstructed_file",std::ios::binary);
    for (int i=0;i<liczbaPakietow;i++){
        if(i==liczbaPakietow-1){
            out.write((char*)decodedpackets[i],kontrol);
        }else{
            out.write((char*)decodedpackets[i],payloadLength);
        }
    }
    out.close();
    return true;
}

LT_Decoder::~LT_Decoder(){
    deleteMem(inputs,liczbaPakietow*300);

    delete[] tmpProcessedPacket;
    delete[] degres;
    delete[] startingDegres;
    deleteMem(neighbours,liczbaPakietow*300);
    deleteMem(sums,liczbaPakietow*300)
    deleteMem(decodedpakiet,liczbaPakietow);
    if(file.is_open()) file.close();
}
