#include "LT_Decoder.h"
#include <iostream>
#include <string>
#include <queue>

LT_Decoder::LT_Decoder()
	:pakietlength(0),liczbaPakietow(0),kontrol(0),checksumSize(0),checksum(0),
	 counter(0),numOfDecodedPackets(0),tmpProcessedPacket(nullptr),degres(nullptr),
	 startingDegres(nullptr),inputs(nullptr),neighbours(nullptr),sums(nullptr),
	 decodedpackets(nullptr){
}

void LT_Decoder::wczytajPakiety(const std::string& filename){
	file.open(filename,std::ios::in|std::ios::binary);
	if(!file){std::cout<<"Blad pliku!";return;}
	std::string temp;

	std::getline(file,temp,';');
	liczbaPakietow=std::stoi(temp);

	std::getline(file,temp,';');
	kontrol=std::stoi(temp);
//	payloadLength = 4;
	file.seekg(-1, std::ios::cur);
    counter=0;
	int total=liczbaPakietow*300;
	inputs=new unsigned char*[total];
	tmpProcessedPacket=new unsigned char[10000];
	degres=new unsigned char[total];
	startingDegres=new unsigned char[total];
	neighbours=new unsigned char*[total];
	sums=new unsigned char*[total];
	decodedpackets=new unsigned char*[liczbaPakietow];

	for(int i=0;i<total;i++){
		inputs[i]=nullptr;
		degres[i]=0;
		startingDegres[i]=0;
		neighbours[i]=nullptr;
		sums[i]=nullptr;
		if(i<liczbaPakietow)decodedpackets[i]=nullptr;
	}
}

void LT_Decoder::loadNextEncodedSymbol(int counter){
    file.ignore(1);
	signsToChars(payloadLength,inputs[counter]);
	degres[counter]=file.get();
	startingDegres[counter]=degres[counter];
	signsToChars(int(degres[counter]),neighbours[counter]);
}

void LT_Decoder::signsToChars(int ln,unsigned char* &pointer){
	pointer=new unsigned char[ln];
	for(int i=0;i<ln;i++)pointer[i]=file.get();
}

int LT_Decoder::charToInt(unsigned char* tab,int lenght){
	int checkSum=0,r=1;
	for(int i = 0; i < lenght; i++){
		checkSum+=((tab[i])*r);
		r*=10;
	}
	return checkSum;
}

void LT_Decoder::calcsum(int counter){
	checksum=0;
	for(int i=0;i<payloadLength;i++)checksum+=inputs[counter][i];
	checksum+=int(degres[counter]);
	for(int i=0;i<int(degres[counter]);i++)checksum+=neighbours[counter][i];

	if(checksum==0)checksumSize=1;
	else{
		checksumSize=0;
		int tempSuma=checksum;
		while(tempSuma>0){tempSuma/=10;checksumSize++;}
	}
}

bool LT_Decoder::checkSum(int suma,int counter){
	signsToChars(checksumSize,sums[counter]);
	if(suma!=charToInt(sums[counter],checksumSize)){
		deleteEncodedSymbol(counter);
		return false;
	}
	return true;
}

void LT_Decoder::copymem(int alrernateIndex){
	int indexOdkodowany=0;
	for(int k=0;k<startingDegres[alrernateIndex];k++){
		if(neighbours[alrernateIndex][k]!=0){
			indexOdkodowany=neighbours[alrernateIndex][k];
			break;
		}
	}
	if(indexOdkodowany==0) return;

	int sourceIndex=indexOdkodowany-1;
	if(decodedpackets[sourceIndex]==nullptr){
		decodedpackets[sourceIndex]=new unsigned char[payloadLength];
		for(int i=0;i<payloadLength;i++)decodedpackets[sourceIndex][i]=inputs[alrernateIndex][i];
		numOfDecodedPackets++;
	}
}

void LT_Decoder::deleteEncodedSymbol(int alrernateIndex){
	delete[] inputs[alrernateIndex];inputs[alrernateIndex]=nullptr;
	delete[] neighbours[alrernateIndex];neighbours[alrernateIndex]=nullptr;
	delete[] sums[alrernateIndex];sums[alrernateIndex]=nullptr;
	degres[alrernateIndex]=0;
	checksum=0;
	checksumSize=0;
}

void LT_Decoder::xorNeighbour(int neighbourToCheck, int neighbourPos, int targetIndex) {
    if (neighbours[targetIndex] == nullptr) return;
    if (neighbours[targetIndex][neighbourPos] == 0) return;

    if (decodedpackets[neighbourToCheck - 1] != nullptr) {
        for (int i = 0; i < payloadLength; i++)
            inputs[targetIndex][i] ^= decodedpackets[neighbourToCheck - 1][i];

        if (degres[targetIndex] > 0) {
            degres[targetIndex]--;
            neighbours[targetIndex][neighbourPos] = 0;
        }
    }
}

bool LT_Decoder::procesCurrentSymbol(int alternateIndex) {
    std::queue<int> pakietyDoOdkodowania;

    if(degres[alternateIndex]==1) {
        pakietyDoOdkodowania.push(alternateIndex);
    }

    while(!pakietyDoOdkodowania.empty()) {
        int currentIdx = pakietyDoOdkodowania.front();
        pakietyDoOdkodowania.pop();

        if(inputs[currentIdx]==nullptr) continue;
        if(degres[currentIdx]!=1) continue;

        int indexOdkodowany = 0;
        for(int k=0; k<startingDegres[currentIdx]; k++) {
            if(neighbours[currentIdx][k]!=0) {
                indexOdkodowany = neighbours[currentIdx][k];
                break;
            }
        }

        if(indexOdkodowany==0) continue;

        if(decodedpackets[indexOdkodowany-1]==nullptr) {
            copymem(currentIdx);
        }
        deleteEncodedSymbol(currentIdx);
        for(int i=0; i<counter; i++) {
            if(inputs[i]!=nullptr && neighbours[i]!=nullptr) {
                for(int j=0; j<startingDegres[i]; j++) {
                    if(neighbours[i][j]==indexOdkodowany) {
                        xorNeighbour(indexOdkodowany, j, i);
                        if(degres[i]==1) {
                            pakietyDoOdkodowania.push(i);
                        }
                    }
                }
            }
        }
    }
    return (numOfDecodedPackets==liczbaPakietow);
}

bool LT_Decoder::startDecoding(){
    totalCounter=0;
    int badsums=0;
	counter=0;
	//std::cout<<"aktualnie przetworzony: "<<std::endl;
	numOfDecodedPackets=0;
	while(!file.eof()&&numOfDecodedPackets<liczbaPakietow){
		loadNextEncodedSymbol(counter);
        //std::cout<<"aktualnie zaladowany stopien: "<<int(degres[counter]);
		calcsum(counter);
		if(checkSum(checksum,counter)){
			procesCurrentSymbol(counter);
			counter++;
			totalCounter++;
			//std::cout<<" liczba otrzymanych s. k: "<<totalCounter<<" Zaladowany w komorce "<<counter-1<<" l. zdekodowanych: "<<numOfDecodedPackets<<std::endl;
		}
        else {
        badsums++;
        totalCounter++;
        //std::cout<<" liczba otrzymanych s. k: "<<totalCounter<<" bledna suma"<<std::endl;
        std::string junk;
        std::getline(file, junk, ';');
        //std::cout<<"==="<<std::endl;
        //std::cout<<junk<<" wielkosc sumy "<<checksumSize<<" suma "<<checksum<<std::endl;
        //std::cout<<"==="<<std::endl;
        file.seekg(-1, std::ios::cur);
		}
	}
	//std::cout<<"l. blednych sum"<<badsums<<std::endl;
	return (numOfDecodedPackets==liczbaPakietow);
}


bool LT_Decoder::restoreFile(){
	if(numOfDecodedPackets<liczbaPakietow)return false;
	std::ofstream out("reconstructed_file.txt",std::ios::binary);
	for(int i=0;i<liczbaPakietow;i++){
		if(i==liczbaPakietow-1)out.write((char*)decodedpackets[i],kontrol);
		else out.write((char*)decodedpackets[i],payloadLength);
	}
	out.close();
	return true;
}

void LT_Decoder::deleteMem(unsigned char** &tab,int length){
	if(tab!=nullptr){
		for(int i=0;i<length;i++)delete[] tab[i];
		delete[] tab;
		tab=nullptr;
	}
}

LT_Decoder::~LT_Decoder(){
	int total=liczbaPakietow*300;
	deleteMem(inputs,total);
	deleteMem(neighbours,total);
	deleteMem(sums,total);
	if(decodedpackets!=nullptr){
		for(int i=0;i<liczbaPakietow;i++)delete[] decodedpackets[i];
		delete[] decodedpackets;
	}
	delete[] tmpProcessedPacket;
	delete[] degres;
	delete[] startingDegres;
	if(file.is_open())file.close();
}
