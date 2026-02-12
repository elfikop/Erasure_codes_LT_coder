#ifndef LT_ENCODER_H
#define LT_ENCODER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

class LT_Encoder {
public:
    LT_Encoder(int pLen, double failureProb);
    virtual ~LT_Encoder();

    void wczytajPakiety(const std::string& filename);
    void generujSymbole(int ile, const std::string& outputFilename);

    int getLicznikStopnia1() const { return licznikstopnia1; }
    int getLicznikCalosciowy() const { return licznikcalosciowy; }

private:

    int pakietlenght;
    double D;
    float c = 1.0f;
    double R_val;
    int liczbaPakietow;
    int licznika;
    int kontrol;

    // Dane
    char** inputs;
    unsigned char* degre;
    unsigned char* encodedpakiet;
    unsigned char* temporary;
    unsigned char* sumach;
    double rozklad[256];


    int licznikstopnia1=0;
    int licznikcalosciowy=0;

    int obliczDlugosc(std::ifstream& file, int div);
    void tablicaSoliton();
    void tablicaRobustSoliton();
    double calculateR();
    int randRobustSoliton(unsigned int seed);
    void createDegree(int czas);
    void xorowanie(int a);
    void combine();
    void calcSum();
    void intToChar(int suma);
    void deleteMem();
};

#endif
