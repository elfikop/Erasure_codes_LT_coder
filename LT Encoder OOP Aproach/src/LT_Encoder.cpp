#include "LT_Encoder.h"
#include <cstring>
#include <cmath>
#include <algorithm>
#include <ctime>

LT_Encoder::LT_Encoder(int pLen,double failureProb)
    :pakietlenght(pLen),D(failureProb),inputs(nullptr),
     degre(nullptr),encodedpakiet(nullptr),temporary(nullptr),sumach(nullptr){
}

LT_Encoder::~LT_Encoder(){
    if(inputs){
        for(int i=0;i<liczbaPakietow;i++) delete[] inputs[i];
        delete[] inputs;
    }
    deleteMem();
}

int LT_Encoder::obliczDlugosc(std::ifstream& file,int div){
    file.seekg(0,std::ios::end);
    int liczbaznakow=file.tellg();
    file.seekg(0,std::ios::beg);
    kontrol=liczbaznakow%div;
    return (liczbaznakow+div-1)/div;
}

void LT_Encoder::wczytajPakiety(const std::string& filename){
    std::ifstream file(filename,std::ios::in|std::ios::binary);
    if(!file){std::cerr<<"Blad pliku!";return;}

    liczbaPakietow=obliczDlugosc(file,pakietlenght);
    inputs=new char*[liczbaPakietow];

    for(int i=0;i<liczbaPakietow;i++){
        inputs[i]=new char[pakietlenght];
        std::memset(inputs[i],0,pakietlenght);
        file.read(inputs[i],pakietlenght);
    }
    file.close();
    tablicaSoliton();
    R_val=calculateR();
    tablicaRobustSoliton();
}

double LT_Encoder::calculateR(){
    return c*std::log(liczbaPakietow/D)*std::sqrt(liczbaPakietow);
}

void LT_Encoder::tablicaSoliton(){
    rozklad[1]=1.0/liczbaPakietow;
    for(int i=2;i<=liczbaPakietow;++i){
        rozklad[i]=1.0/(i*(i-1));
    }
}

void LT_Encoder::tablicaRobustSoliton(){
    double rho[256]={0.0};
    double suma_lokalna=0;

    for(int i=1;i<=liczbaPakietow;++i){
        double tau=0;
        if(i<(liczbaPakietow/R_val)-1) tau=R_val/(i*liczbaPakietow);
        else if(i==(int)(liczbaPakietow/R_val)) tau=R_val*std::log(R_val/D)/liczbaPakietow;

        rozklad[i]+=tau;
        suma_lokalna+=rozklad[i];
    }

    for(int i=1;i<=liczbaPakietow;++i) rozklad[i]/=suma_lokalna;
    for(int i=2;i<=liczbaPakietow;++i) rozklad[i]+=rozklad[i-1];
}

int LT_Encoder::randRobustSoliton(unsigned int seed){
    static std::mt19937 gen(seed);
    std::uniform_real_distribution<> dist(0.0,1.0);
    double r=dist(gen);
    for(int i=1;i<=liczbaPakietow;++i){
        if(r<rozklad[i]) return i;
    }
    return 1;
}

void LT_Encoder::xorowanie(int a){
    for(int l=0;l<pakietlenght;l++){
        encodedpakiet[l]^=(unsigned char)inputs[a-1][l];
    }
}

void LT_Encoder::createDegree(int czas){
    std::srand(czas);
    int d=randRobustSoliton(std::time(0)+czas);

    if(d==1) licznikstopnia1++;
    licznikcalosciowy++;

    degre=new unsigned char[d+1];
    degre[0]=(unsigned char)d;
    encodedpakiet=new unsigned char[pakietlenght];
    std::memset(encodedpakiet,0,pakietlenght);

    for(int i=1;i<=d;i++){
        int target=(std::rand()%liczbaPakietow)+1;
        degre[i]=(unsigned char)target;
        xorowanie(target);
    }
}

void LT_Encoder::combine(){
    int len_deg=degre[0]+1;
    temporary=new unsigned char[pakietlenght+len_deg];
    std::memcpy(temporary,encodedpakiet,pakietlenght);
    std::memcpy(temporary+pakietlenght,degre,len_deg);
    licznika=pakietlenght+len_deg;
}

void LT_Encoder::intToChar(int suma_val){
    std::string s=std::to_string(suma_val);
    int len_sum=s.length();
    sumach=new unsigned char[len_sum];
    for(int i=0;i<len_sum;i++) sumach[i]=s[i]-'0';

    unsigned char* final_p=new unsigned char[licznika+len_sum];
    std::memcpy(final_p,temporary,licznika);
    std::memcpy(final_p+licznika,sumach,len_sum);

    delete[] encodedpakiet;
    encodedpakiet=final_p;
    licznika+=len_sum;
}

void LT_Encoder::calcSum(){
    int s=0;
    for(int i=0;i<licznika;i++) s+=temporary[i];
    intToChar(s);
}

void LT_Encoder::deleteMem(){
    delete[] degre;degre=nullptr;
    delete[] encodedpakiet;encodedpakiet=nullptr;
    delete[] temporary;temporary=nullptr;
    delete[] sumach;sumach=nullptr;
}

void LT_Encoder::generujSymbole(int ile,const std::string& outName){
    std::ofstream plik(outName,std::ios::out|std::ios::binary|std::ios::trunc);
    plik<<liczbaPakietow<<";";
    plik<<kontrol<<";";

    for(int i=0;i<ile;i++){
        createDegree(i);
        combine();
        calcSum();

        for(int j=0;j<licznika;j++) plik<<encodedpakiet[j];
        plik<<";";

        deleteMem();
    }
    plik.close();
}
