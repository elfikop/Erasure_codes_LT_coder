/*******************************************************************************
 * This file is part of Erasure_codes_LT_coder.                                *
 *                                                                             *
 * Copyright (C) 2024-2025 by Elfior                                           *
 *                                                                             *
 ******************************************************************************/


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
        friend void test_loadNextEncodedSymbol();

    protected:

    private:
    int pakietlenght;
    int liczbaPakietow;
    int licznika; //dlugosc pakietu
    int kontrol;
    int checksumSize;
    std::ifstream file;
    unsigned char* tmpProcessedPacket;
    unsigned char** inputs;
    unsigned char* degres;
    unsigned char* startingDegres;
    unsigned char** neighbours;
    unsigned char** sums;
    unsigned char** decodedpakiet;
    const int payloadLength = 3;


    /** \brief Opens the encoded file, sets the LT_Encoder::liczbaPakietow atribute
    *
    * based on the first 1 byte packet written in the file and LT_Encoder.kontrol based on the third packet.
    * initializes memory dynamically with liczbaPakietow size for all array members in the class.
    * \param filename is the name of the encoded file to be processed
    *
    */
    void wczytajPakiety(const std::string& filename);

    /** \brief Reconstructs the int value encoded in the array rows with the following rules:
    * array values: "'2' '2' '4' '6'" equals int value: 2246
    * \param tab pointer to the array containing char digits, for instance
    * \param lenght number of char digits to be converted to int
    */
    int charToInt(unsigned char* tab, int lenght );

     /** \brief Reads a specified number of bytes from the file and allocates memory.
     *
     * allocates a new array, and assigns its address to 'pointer'.
     * Reads 'ln' bytes from the file stream opened in wczytajPakiety() and
     * signs them to the apropriate elements of 'pointer'
     * \param ln The number of bytes to read from the file.
     * \param pointer Reference to the pointer that will hold the allocated memory.
     */
    void signsToChars(int ln, unsigned char* &pointer);

     /**
     * \brief Uses signsToChars to finish the task, loads 'payloadlenght' amount of signs to the 'inputs' array',
     * sets the apropriate element of 'degres' to the right payloadlenght sign read from the input file
     * Loads the list of neighbours to the 'neighbours' array.
     * \param counter is the current encoded symbol that is about to be processed, used for example in indexing inputs[counter][x]
     */
     void loadNextEncodedSymbol(int counter);

     /**
     * \brief Calculates the checksum of the [counter] encoded symbol based on the 'inputs', 'neighbours' and 'degres' arrays with the given index
     *
     * Function calls calcsum()
     * \param counter is the current encoded symbol that is about to be processed, used for example in indexing inputs[counter][x]
     */
     void calcsum(int counter);

     /**
     * \brief checks if the calculated checksum and the one read from the encoded symbol are equal. if not, it deletes the memory of the
     * currently processed symbol. sets the inputs[counter], neighbours[counter] to nullptr and degres[counter] to 0
     * \param suma is the checksum value of the currently processed encoded symbol calculated by the calcum() function
     * \param counter is the current encoded symbol that is about to be processed, used for example in indexing inputs[counter][x]
     */
     void checkSum(int suma, int counter);

     /**
     * @brief deletes the given array memory
     *
     *
     *
     *
     */
     void deleteMem();

};

#endif // LT_DECODER_H
