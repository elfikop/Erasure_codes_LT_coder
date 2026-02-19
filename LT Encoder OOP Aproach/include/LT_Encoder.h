#ifndef LT_ENCODER_H
#define LT_ENCODER_H


class LT_Encoder
{
    public:
        long long licznikstopnia1=0;
        int licznikcalosciowy=0;

        double R_val;
        float c_const;
        LT_Encoder();
        virtual ~LT_Encoder();

    protected:

    private:
        double D_prob;
        double rozklad[256];
        const int MAX_K=255;
        double suma_dist=0;
        int kontrol;
        char** inputs;
        int liczbaPakietow;
        unsigned char* degre;
        unsigned char* encodedpakiet;
        unsigned char* temporary;
        unsigned char* sumach;
        int licznika;

};

#endif // LT_ENCODER_H
