#ifndef LT_DECODER_H
#define LT_DECODER_H


class LT_Decoder
{
    public:
        LT_Decoder(string);
        virtual ~LT_Decoder();

    protected:
    void LT_Decoder::wczytajPakiety(const std::string& filename);

    private:
    int pakietlenght;
    int liczbaPakietow;
    int licznika; //dlugosc pakietu
    int kontrol;

    // Dane
    unsigned char* tmpProcessedPacket;
    unsigned char** inputs;
    unsigned char* degres;
    int* sums;
    unsigned char** decodedpakiet;
    void deleteMem();

};

#endif // LT_DECODER_H
