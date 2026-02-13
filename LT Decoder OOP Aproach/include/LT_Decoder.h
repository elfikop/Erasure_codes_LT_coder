#ifndef LT_DECODER_H
#define LT_DECODER_H
#include <cmath>
#include <string>
#include <fstream>

void test_charToInt();
void test_signsToChars();
void test_wczytajPakiety();

class LT_Decoder
{
    public:
        LT_Decoder();
        virtual ~LT_Decoder();

        friend void test_charToInt();
        friend void test_signsToChars();
        friend void test_wczytajPakiety();

    protected:

    private:
    int pakietlenght;
    int liczbaPakietow;
    int licznika; //dlugosc pakietu
    int kontrol;

    // Dane
    std::ifstream file;
    unsigned char* tmpProcessedPacket;
    unsigned char** inputs;
    unsigned char* degres;
    unsigned char* startingDegres;
    unsigned char** neighbours;
    unsigned char** sums;
    unsigned char** decodedpakiet;

    void wczytajPakiety(const std::string& filename);
    void deleteMem();
    int charToInt(unsigned char* tab, int lenght );
    void signsToChars(int ln, unsigned char* &pointer);
    void wczytajKolejnyPakiet(const std::string& filename);

};

#endif // LT_DECODER_H
