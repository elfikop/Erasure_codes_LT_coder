#include "LT_Decoder.h"
#include <cassert>
#include <iostream>

using namespace std;
void test_charToInt() {
    LT_Decoder decoder;
    unsigned char dane[] = {'1', '2', '3'};
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
    if(decoder.liczbaPakietow==3){
        if(decoder.kontrol==20){
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
    decoder.file.ignore(1);
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
void test_loadNextEncodedSymbol(){
    string testFileName = "test_symbol.txt";
    ofstream outFile(testFileName, ios::binary);

    outFile << "1;99;";
    outFile << "XYZ";
    outFile << (char)2;
    outFile << (char)10;
    outFile << (char)11;
    outFile.close();

    LT_Decoder decoder;

    decoder.wczytajPakiety(testFileName);

    decoder.loadNextEncodedSymbol(0);

    bool check = true;
    if (decoder.inputs[0][0] != 'X' || decoder.inputs[0][2] != 'Z') check = false;
    if (decoder.degres[0] != 2) check = false;
    if (decoder.neighbours[0][0] != 10 || decoder.neighbours[0][1] != 11) check = false;

    if (check) {
        cout << "Test loadNextEncodedSymbol: +" << endl;
    } else {
        cout << "Test loadNextEncodedSymbol: !!! Blad wczytywania struktury symbolu" << endl;
    }
}
void test_calcsum(){
    string testFileName="test_calcsum.txt";
    ofstream outFile(testFileName,ios::binary);
    outFile<<"1;99;";
    outFile<<(unsigned char)10<<(unsigned char)20<<(unsigned char)30;
    outFile<<(unsigned char)2;
    outFile<<(unsigned char)5<<(unsigned char)7;
    outFile.close();
    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);
    decoder.loadNextEncodedSymbol(0);
    decoder.calcsum(0);
    int oczekiwanaSuma=74;
    int oczekiwanyRozmiarChecksum=2;
    bool check=true;
    if(decoder.checksum!=oczekiwanaSuma)check=false;
    if(decoder.checksumSize!=oczekiwanyRozmiarChecksum)check=false;
    if(check){
        cout<<"Test calcsum: +"<<endl;
    }else{
        cout<<"Test calcsum: !!! Blad. Wynik: "<<decoder.checksum<<", Rozmiar: "<<decoder.checksumSize<<endl;
    }
}
void test_checkSum(){
    string testFileName="test_checksum.txt";
    ofstream outFile(testFileName,ios::binary);
    outFile<<"1;99;";
    outFile<<(unsigned char)10<<(unsigned char)20<<(unsigned char)30;
    outFile<<(unsigned char)1;
    outFile<<(unsigned char)5;
    outFile<<'6'<<'6'; // Suma kontrolna zapisana w pliku (10+20+30+1+5 = 66)

    outFile<<';';

    outFile<<(unsigned char)22<<(unsigned char)33<<(unsigned char)44;
    outFile<<(unsigned char)2;
    outFile<<(unsigned char)5;
    outFile<<(unsigned char)3;
    outFile<<'6'<<'6'<<';'<<2;
    outFile.close();
    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);
    decoder.loadNextEncodedSymbol(decoder.counter);
    //test no.1 correct sumcheck
    decoder.calcsum(decoder.counter);
    decoder.checksumSize=2;
    bool wynikPozytywny=decoder.checkSum(decoder.checksum,0);

    //test no.2 incorrect sumcheck
    decoder.counter++;
    decoder.loadNextEncodedSymbol(decoder.counter);
    decoder.calcsum(decoder.counter);
    //cout<<"wyliczonasumazplikufalse" <<decoder.charToInt(decoder.sums[decoder.counter],decoder.checksumSize)<<endl;
    bool wynikNegatywny=decoder.checkSum(decoder.checksum,decoder.counter);
    if(wynikPozytywny==true&&wynikNegatywny==false&&decoder.inputs[1]==nullptr){
        cout<<"Test checkSum: +"<<endl;
    }else{
        cout<<"Test checkSum: !!! Blad weryfikacji sumy,obliczono: "<<decoder.checksum<<endl;
    }
}

int main()
{
    cout << "Hello world!" << endl;
    test_charToInt();
    test_wczytajPakiety();
    test_signsToChars();
    test_loadNextEncodedSymbol();
    test_calcsum();
    test_checkSum();
    cout << "koniec" << endl;
    /*
	LT_Decoder decoder;
	decoder.wczytajPakiety("example.txt");
	std::cout<<"Rozpoczynam dekodowanie..."<<std::endl;
	if(decoder.startDecoding()){
		std::cout<<"Sukces: Wszystkie pakiety odkodowane!"<<std::endl;
		if(decoder.restoreFile()){
			std::cout<<"Plik zostal pomyslnie odtworzony jako 'reconstructed_file'"<<std::endl;
		}else{
			std::cout<<"Blad podczas zapisu pliku."<<std::endl;
		}
	}else{
		std::cout<<"Nie udalo sie odkodowac pliku. Za malo symboli lub uszkodzone dane."<<std::endl;
	}
    */

    return 0;
}
