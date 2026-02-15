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

    int getLicznikStopnia1() const { return licznikstopnia1; }
    int getLicznikCalosciowy() const { return licznikcalosciowy; }


    /** \brief Opens the source file, calculates the number of packets and reads data into memory.
    * * Initializes 'liczbaPakietow' and dynamic memory for 'inputs'.
    * Also triggers the calculation of Soliton distributions and the R parameter.
    * \param filename name of the source file to be encoded.
    */
    void wczytajPakiety(const std::string& filename);

    /** \brief Generates a specified number of encoded symbols and saves them to a file.
    * * The output file starts with 'liczbaPakietow' and 'kontrol' values as metadata.
    * Each symbol is generated using createDegree, combine, and calcSum processes.
    * \param ile number of symbols to generate.
    * \param outputFilename name of the destination file.
    */
    void generujSymbole(int ile, const std::string& outputFilename);


private:

    int pakietlenght;
    double D;
    float c = 1.0f;
    double R_val;
    int liczbaPakietow;
    int licznika;
    int kontrol;
    char** inputs;
    unsigned char* degre;
    unsigned char* encodedpakiet;
    unsigned char* temporary;
    unsigned char* sumach;
    double rozklad[256];


    int licznikstopnia1=0;
    int licznikcalosciowy=0;

    /** \brief Calculates the number of packets based on file size and desired packet length.
    * * Sets the 'kontrol' attribute as the remainder of the file size divided by 'div'.
    * \param file reference to the opened input stream.
    * \param div the size of a single packet (pakietlenght).
    * \return the total number of packets (liczbaPakietow).
    */
    int obliczDlugosc(std::ifstream& file, int div);

    /** \brief Fills the 'rozklad' array with values of the Ideal Soliton Distribution.
    */
    void tablicaSoliton();

    /** \brief Adjusts the 'rozklad' array to follow the Robust Soliton Distribution.
    * * Adds the extra 'tau' components to the Ideal Soliton distribution and normalizes the result.
    */
    void tablicaRobustSoliton();

    /** \brief Calculates the R parameter used for the Robust Soliton Distribution.
    * \return the calculated R value.
    */
    double calculateR();

    /** \brief Draws a degree for an encoded symbol based on the Robust Soliton Distribution.
    * \param seed value used to initialize the random number generator.
    * \return the drawn degree (number of source packets to combine).
    */
    int randRobustSoliton(unsigned int seed);

    /** \brief Selects random source packets and XORs them to create an encoded symbol.
    * * Also populates the 'degre' array with the degree and indices of selected packets.
    * \param czas a value used for randomization (usually the current iteration index).
    */
    void createDegree(int czas);

    /** \brief Performs the XOR operation between 'encodedpakiet' and a chosen source packet.
    * \param a index of the source packet in the 'inputs' array (1-based).
    */
    void xorowanie(int a);

    /** \brief Merges 'encodedpakiet' and 'degre' information into a single 'temporary' array.
    * * Updates 'licznika' with the combined length of the payload and the degree data.
    */
    void combine();

    /** \brief Calculates the checksum of the combined symbol data.
    * * Sums all bytes in the 'temporary' array and calls intToChar to append the result.
    */
    void calcSum();

    /** \brief Converts the integer checksum into an array of digits and appends it to the final packet.
    * * Reallocates memory for 'encodedpakiet' to include the payload, degree data, and checksum digits.
    * \param suma the calculated checksum value.
    */
    void intToChar(int suma);

    /** \brief Frees dynamic memory allocated for temporary arrays during the generation of a single symbol.
    * * Clears 'degre', 'encodedpakiet', 'temporary', and 'sumach'.
    */
    void deleteMem();
};

#endif
