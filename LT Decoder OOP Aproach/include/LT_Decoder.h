/*******************************************************************************
 * This file is part of Erasure_codes_LT_coder.                                *
 *                                                                             *
 * Copyright (C) 2024-2025 by Elfior                                           *
 *                                                                             *
 ******************************************************************************/
// -*- lsst-c++ -*-

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

        /** \brief Creates an object of the LT Decoder
        * sets the pointer values of the class members to nullptr.
        * sets the member variables of the class to 0
        */
        LT_Decoder();
        /** \brief Deletes an object of the LT Decoder
        * deletes the dynamically allocated memory used to store encoded symbols data if there are any
        */
        virtual ~LT_Decoder();

        friend void test_charToInt();
        friend void test_signsToChars();
        friend void test_wczytajPakiety();
        friend void test_loadNextEncodedSymbol();
        bool startDecoding();
        bool restoreFile();



    protected:

    private:
    /** \brief Length of the payload of an encoded symbol. */
    int pakietlength;
    /** \brief Number of input symbols used to encode the data, usually the first byte of an encoded file. */
    int liczbaPakietow;
    /** \brief represents the length of payload in the last input symbol, when the input file will be reconstructed from all input symbols recovered,
    * the kontrol amount of bytes of the last in symbol has to be wrote to the output file. */
    int kontrol;
    /** \brief amount of digits of the checksum of the currently processed encoded symbol. */
    int checksumSize;
    /** \brief calculated checksum of the currently processed encoded symbol */
    int checksum;
    /** \brief Index of currently processed encoded symbol (inputs[counter][x], degres[counter], neighbours[counter][x] */
    int counter;
    /** \brief Amount of currently decoded packets */
    int numOfDecodedPackets;
    const int payloadLength = 3;
    std::ifstream file;
    unsigned char* tmpProcessedPacket;
    unsigned char* degres;
    unsigned char* startingDegres;
    unsigned char** inputs;
    unsigned char** neighbours;
    unsigned char** sums;
    unsigned char** decodedpackets;
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
     * currently processed symbol.
     * \param suma is the checksum value of the currently processed encoded symbol calculated by the calcum() function
     * \param counter is the current encoded symbol that is about to be processed, used for example in indexing inputs[counter][x]
     * \return returns true if the encodedsymbol is valid, if not, false
     */
     bool checkSum(int suma, int counter);

     /**
     * @brief Copies the currently processed encoded symbol from inputs[counter]
     * to the decodedpackets[counter]
     */
     void copymem(int alrernateIndex);// if the degree one

     /**
     * @brief Checks the data under neighbourToCheck index in decodedpackets array
     * if its filled with values, xors inputs[counter] with the decodedpackets[neighbourToCheck]
     * sets the neighbours[neighbourPos] memory to NULL
     * decrements degree[counter]
     * \param neighbourToCheck is the index to check in decodedpackets array
     * \param neighbourPos is the index of currently processed neighbour form the neighbours[counter][neighbourPos] array
     */
     void xorNeighbour(int neighbourToCheck, int neighbourPos, int alrernateIndex);

     /**
     * @brief Deletes the currently processed encoded symbol
     */
     void deleteEncodedSymbol(int alrernateIndex);


     /**
     * @brief Processes the current encoded symbol
     * checks if the symbol degree is one, if so
     * signs its data to the decodedpackets array and does not increase counter value
     * deletes its data from the encoded symbols arrays (counter index)
     * if the current sybol has degree >1, checks the neighbours indexes of the encoded
     * symbol as in the decodedpackets array indexes if they are not nullptr
     * (already decoded)
     * check if the current symbol is not the degree of one if so, copymem, deletemem
     * \param tab is the pointer to the source array
     * \param length is the size of the source array
     * \return Returns true, if the amount of decoded symbols is equal to liczbapakietow i.e. the decoding proces is done
     */
     bool procesCurrentSymbol(int alrernateIndex);

     /**
     * @brief Deletes the given array of pointers memory
     * \param tab is the pointer to the source array
     * \param length is the size of the source array
     */
     void deleteMem(unsigned char* &tab, int length);


};

#endif // LT_DECODER_H
