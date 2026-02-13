#include "LT_Decoder.h"
#include <cassert>
#include <iostream>

using namespace std;
void test_charToInt() {
    LT_Decoder decoder;
    unsigned char dane[] = {1, 2, 3};
    int wynik = decoder.charToInt(dane, 3);
    if(wynik == 123) {
        std::cout << "Test charToInt: +" << std::endl;
    } else {
        std::cout << "Test charToInt: !!! wynik: " << wynik << std::endl;
    }
}
void test_wczytajPakiety(){
    string testFileName = "test_signs.txt";
    ofstream outFile(testFileName, ios::binary);
    outFile << "3;20;ABCDE";
    outFile.close();
    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);
    if(decoder.liczbaPakietow=3){
        if(decoder.kontrol=20){
            std::cout<<"Test wczytajPakiety: +"<<std::endl;
        }
        else
            std::cout<<"Test wczytajPakiety: !!! wynik: kontrol: "<<decoder.kontrol<<std::endl;
    }
    else
        std::cout<<"Test wczytajPakiety: !!! wynik: liczbaPakietow: "<<decoder.liczbaPakietow<<std::endl;
}
void test_signsToChars() {
    string testFileName = "test_signs.txt";
    ofstream outFile(testFileName, ios::binary);
    outFile << "1;123;ABCDE";
    outFile.close();

    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);

    unsigned char* ptr = nullptr;
    int dlugoscDoWczytania = 3;
    decoder.signsToChars(dlugoscDoWczytania, ptr);

    if (ptr != nullptr && ptr[0] == 'A' && ptr[1] == 'B' && ptr[2] == 'C') {
        cout << "Test signsToChars: +" << endl;
    } else {
        cout << "Test signsToChars: !!! blad zawartosci pamieci" << endl;
    }
    delete[] ptr;
}


int main()
{
    cout << "Hello world!" << endl;
    test_charToInt();
    test_wczytajPakiety();
    test_signsToChars();
    cout << "koniec" << endl;
    return 0;
}
