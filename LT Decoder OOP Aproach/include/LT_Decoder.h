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

    // Dane
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
    * based on the first 1 byte packet written in the file and LT_Encoder.kontrol based on the third packet.
    * initializes memory dynamically with liczbaPakietow size for all array members in the class.
    * \param filename is the name of the encoded file to be processed
    */
    void wczytajPakiety(const std::string& filename);

     /** \brief Reconstructs the int value encoded in the array rows with the following rules:
    * array values: "'2' '2' '4' '6'" equals int value: 2246
    * \param tab pointer to the array containing char digits, for instance
    * \param lenght number of char digits to be converted to int
    */
    int charToInt(unsigned char* tab, int lenght );

    /** \brief Reads a specified number of bytes from the file and allocates memory.
     * Reads 'ln' bytes from the file stream opened in wczytajPakiety(),
     * allocates a new array, and assigns its address to 'pointer'.
     * \param ln The number of bytes to read from the file.
     * \param pointer Reference to the pointer that will hold the allocated memory.
     */
    void signsToChars(int ln, unsigned char* &pointer);

    /**
     * @brief
     *
     *
     * * @param
     * @param
     */
    void loadNextEncodedSymbol(int counter);
    void calcsum(int counter);

      /**
     * @brief
     *
     *
     * * @param
     * @param
     */
    void deleteMem();

};

#endif // LT_DECODER_H
