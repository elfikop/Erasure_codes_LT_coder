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
    bool wynikNegatywny=decoder.checkSum(decoder.checksum,decoder.counter);
    if(wynikPozytywny==true&&wynikNegatywny==false&&decoder.inputs[1]==nullptr){
        cout<<"Test checkSum: +"<<endl;
    }else{
        cout<<"Test checkSum: !!! Blad weryfikacji sumy,obliczono: "<<decoder.checksum<<endl;
    }
}

void test_xorNeighbour() {
    //blank file with header
    string testFileName = "test_xor.txt";
    ofstream outFile(testFileName, ios::binary);
    outFile << "2;99;";
    outFile.close();


    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);


    int targetIndex = 0;
    int neighbourPos = 0;
    int neighbourIdx = 5;

    decoder.neighbours[targetIndex]=new unsigned char[1];
    decoder.neighbours[targetIndex][neighbourPos]=(unsigned char)neighbourIdx;
    //manual prep of encoded symbol
    decoder.inputs[targetIndex] = new unsigned char[decoder.payloadLength];
    decoder.inputs[targetIndex][0]=0b10101010; //170
    decoder.inputs[targetIndex][1]=0b11110000; //240
    decoder.inputs[targetIndex][2]=0b00001111; //15

    decoder.degres[targetIndex] = 3; //degree of the encoded symbol

    //manual prep of the decoded packet
    int decodedPos = neighbourIdx - 1;
    decoder.decodedpackets[decodedPos]=new unsigned char[decoder.payloadLength];
    decoder.decodedpackets[decodedPos][0]=0b01010101; //85
    decoder.decodedpackets[decodedPos][1]=0b11110000; //240
    decoder.decodedpackets[decodedPos][2]=0b11111111; //255


    decoder.xorNeighbour(neighbourIdx,0,targetIndex);

    // 4. Weryfikacja wynikow
    // Oczekiwane wyniki XOR:
    // 170 ^ 85  = 255 (0b11111111)
    // 240 ^ 240 = 0   (0b00000000)
    // 15  ^ 255 = 240 (0b11110000)
    bool check = true;
    if (decoder.inputs[targetIndex][0]!=255)
        check = false;
    if (decoder.inputs[targetIndex][1]!=0)
        check = false;
    if (decoder.inputs[targetIndex][2]!=240)
        check = false;


    if (decoder.degres[targetIndex] != 2) check = false;
    if (check) {
        cout << "Test xorNeighbour: +" << endl;
    } else {
        cout << "Test xorNeighbour: !!! Blad operacji XOR lub zmniejszania stopnia" << endl;
    }
}
void test_copymem(){
    string testFileName="test_copymem.txt";
    ofstream outFile(testFileName,ios::binary);
    outFile<<"2;99;";
    outFile.close();
    LT_Decoder decoder;
    decoder.wczytajPakiety(testFileName);
    int alternateIndex=0;
    int sourceVal=2;
    decoder.neighbours[alternateIndex]=new unsigned char[1];
    decoder.neighbours[alternateIndex][0]=(unsigned char)sourceVal;
    decoder.startingDegres[alternateIndex]=1; // DODAJ TÊ LINIÊ
    decoder.inputs[alternateIndex]=new unsigned char[3];
    decoder.inputs[alternateIndex][0]='X';
    decoder.inputs[alternateIndex][1]='Y';
    decoder.inputs[alternateIndex][2]='Z';
    decoder.copymem(alternateIndex);
    bool check=true;
    int expectedSourceIndex=sourceVal-1;
    if(decoder.decodedpackets[expectedSourceIndex]==nullptr){
        check=false;
    }else{
        if(decoder.decodedpackets[expectedSourceIndex][0]!='X'||
           decoder.decodedpackets[expectedSourceIndex][1]!='Y'||
           decoder.decodedpackets[expectedSourceIndex][2]!='Z'){
            check=false;
        }
    }
    if(decoder.numOfDecodedPackets!=1)check=false;
    if(check){
        cout<<"Test copymem: +"<<endl;
    }else{
        cout<<"Test copymem: !!! Blad kopiowania pamieci"<<endl;
    }
}
void test_deleteEncodedSymbol(){
	string testFileName="test_delete.txt";
	ofstream outFile(testFileName,ios::binary);
	outFile<<"1;99;";
	outFile.close();

	LT_Decoder decoder;
	decoder.wczytajPakiety(testFileName);

	decoder.inputs[0]=new unsigned char[3];
	decoder.neighbours[0]=new unsigned char[2];
	decoder.sums[0]=new unsigned char[2];
	decoder.degres[0]=5;

	decoder.deleteEncodedSymbol(0);

	bool check=true;
	if(decoder.inputs[0]!=nullptr||decoder.neighbours[0]!=nullptr||
	   decoder.sums[0]!=nullptr||decoder.degres[0]!=0){
		check=false;
	}

	if(check) {
		cout<<"Test deleteEncodedSymbol: +"<<endl;
	} else {
		cout<<"Test deleteEncodedSymbol: !!! Blad usuwania pamieci"<<endl;
	}
}

void test_procesCurrentSymbol() {
	string testFileName="test_process.txt";
	ofstream outFile(testFileName,ios::binary);
	outFile<<"2;99;"; // liczbaPakietow=2
	outFile.close();

	LT_Decoder decoder;
	decoder.wczytajPakiety(testFileName);


	decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
	decoder.degres[0]=1;
	decoder.startingDegres[0]=1;
	decoder.neighbours[0]=new unsigned char[1]{1};
	decoder.sums[0]=new unsigned char[1]{0};


	unsigned char xorVal='A'^'B';
	decoder.inputs[1]=new unsigned char[3]{xorVal,xorVal,xorVal};
	decoder.degres[1]=2;
	decoder.startingDegres[1]=2;
	decoder.neighbours[1]=new unsigned char[2]{1,2};
	decoder.sums[1]=new unsigned char[1]{0};


	decoder.counter=2;


	decoder.procesCurrentSymbol(0);

	bool check=true;

	// Czy Pakiet 0 trafi³ do odkodowanych?
	if(decoder.decodedpackets[0]==nullptr||decoder.decodedpackets[0][0]!='A') check=false;
        if(check==false)cout<<"Test procesCurrentSymbol: !!! Blad logiki dekodowania kaskadowego"<<1<<endl;
	// Czy Pakiet 1 zosta³ zredukowany (XOR) i odkodowany (efekt domina)?
	if(decoder.decodedpackets[1]==nullptr||decoder.decodedpackets[1][0]!='B') check=false;
        if(check==false)cout<<"Test procesCurrentSymbol: !!! Blad logiki dekodowania kaskadowego"<<2<<endl;

	// Czy licznik odkodowanych pakietów jest poprawny?
	if(decoder.numOfDecodedPackets!=2) check=false;
        if(check==false)cout<<"Test procesCurrentSymbol: !!! Blad logiki dekodowania kaskadowego"<<3<<endl;

	if(check) {
		cout<<"Test procesCurrentSymbol: +"<<endl;
	} else {
		cout<<"Test procesCurrentSymbol: !!! Blad logiki dekodowania kaskadowego"<<endl;
	}
}
void run_advanced_process_test(int scenario) {
	LT_Decoder decoder;
	string fn="test_adv.txt";
	ofstream f(fn);f<<"5;99;";f.close();
	decoder.wczytajPakiety(fn);
	//decoder.payloadLength=3;

	cout<<"Scenariusz "<<scenario<<": ";

	if(scenario==1){ // 1. Prosta kaskada (A -> A^B)
		decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.inputs[1]=new unsigned char[3]{(unsigned char)('A'^'B'),(unsigned char)('A'^'B'),(unsigned char)('A'^'B')};
		decoder.degres[1]=2;decoder.startingDegres[1]=2;
		decoder.neighbours[1]=new unsigned char[2]{1,2};decoder.sums[1]=new unsigned char[1]{0};
		decoder.counter=2;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==2&&decoder.decodedpackets[1][0]=='B')cout<<"+";
		else cout<<"!!! Blad prostej kaskady";
	}
	else if(scenario==2){ //
		for(int i=0;i<4;i++){
			decoder.sums[i]=new unsigned char[1]{0};
			if(i==0){
				decoder.inputs[i]=new unsigned char[3]{'A','A','A'};
				decoder.degres[i]=1;decoder.startingDegres[i]=1;
				decoder.neighbours[i]=new unsigned char[1]{1};
			}else{
				unsigned char val=(('A'+i-1)^('A'+i));
				decoder.inputs[i]=new unsigned char[3]{val,val,val};
				decoder.degres[i]=2;decoder.startingDegres[i]=2;
				decoder.neighbours[i]=new unsigned char[2]{(unsigned char)i,(unsigned char)(i+1)};
			}
		}
		decoder.counter=4;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==4&&decoder.decodedpackets[3][0]=='D')cout<<"+";
		else cout<<"!!! Blad dlugiej kaskady";
	}
	else if (scenario == 3) { //

    decoder.inputs[0] = new unsigned char[3]{'A','A','A'};
    decoder.degres[0] = 1; decoder.startingDegres[0] = 1;
    decoder.neighbours[0] = new unsigned char[1]{1}; decoder.sums[0] = new unsigned char[1]{0};


    for(int i = 1; i <= 3; i++) {
        decoder.inputs[i] = new unsigned char[3]{(unsigned char)('A'^('A'+i)), (unsigned char)('A'^('A'+i)), (unsigned char)('A'^('A'+i))};
        decoder.degres[i] = 2; decoder.startingDegres[i] = 2;
        decoder.neighbours[i] = new unsigned char[2]{1, (unsigned char)(i+1)};
        decoder.sums[i] = new unsigned char[1]{0};
    }

    decoder.counter = 4;
    decoder.procesCurrentSymbol(0);

    // Sprawdzamy czy kaskada przesz³a przez wszystkie 4 fragmenty
    bool ok = true;
    if(decoder.numOfDecodedPackets != 4) ok = false;
    if(decoder.decodedpackets[3] == nullptr || decoder.decodedpackets[3][0] != 'D') ok = false;

    if(ok) cout << "+";
    else cout << "!!! Blad rozgaliezienia (brak kaskady wielokrotnej)";
}
	else if(scenario==4){ // 4. Pakiet o stopniu 1, ktory jest juz w decodedpackets
		decoder.decodedpackets[0]=new unsigned char[3]{'A','A','A'};
		decoder.numOfDecodedPackets=1;
		decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.counter=1;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==1&&decoder.inputs[0]==nullptr)cout<<"+";
		else cout<<"!!! Blad ponownego procesowania";
	}
	else if(scenario==5){ //5. Wywolanie na stopniu > 1 (Ignorowanie)
		decoder.inputs[0]=new unsigned char[3]{'X','X','X'};
		decoder.degres[0]=2;decoder.startingDegres[0]=2;
		decoder.neighbours[0]=new unsigned char[2]{1,2};decoder.sums[0]=new unsigned char[1]{0};
		decoder.counter=1;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==0&&decoder.inputs[0]!=nullptr)cout<<"+";
		else cout<<"!!! Blad ignorowania stopnia > 1";
	}
	else if(scenario==6){ //6. Brak sasiadow w pamieci do redukcji
		decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.counter=1;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==1&&decoder.inputs[0]==nullptr)cout<<"+";
		else cout<<"!!! Blad izolowanego symbolu";
	}
	else if(scenario==7){ //7. Kaskada z "dziurami" (NULL w tablicy inputs)
		decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.inputs[1]=nullptr; // Dziura w pamieci
		decoder.inputs[2]=new unsigned char[3]{(unsigned char)('A'^'B'),(unsigned char)('A'^'B'),(unsigned char)('A'^'B')};
		decoder.degres[2]=2;decoder.startingDegres[2]=2;
		decoder.neighbours[2]=new unsigned char[2]{1,2};decoder.sums[2]=new unsigned char[1]{0};
		decoder.counter=3;
		decoder.procesCurrentSymbol(0);
		if(decoder.decodedpackets[1][0]=='B')cout<<"+";
		else cout<<"!!! Blad dziurawego countera";
	}
	else if(scenario==8){ //8. Odkodowanie ostatniego fragmentu (Licznik)
		decoder.liczbaPakietow=1;
		decoder.inputs[0]=new unsigned char[3]{'Z','Z','Z'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.counter=1;
		bool wynik=decoder.procesCurrentSymbol(0);
		if(wynik==true&&decoder.numOfDecodedPackets==1)cout<<"+";
		else cout<<"!!! Blad licznika koncowego";
	}
	else if(scenario==9){ //9. Nadmiarowosc (ten sam fragment drugi raz)
		decoder.decodedpackets[0]=new unsigned char[3]{'A','A','A'};
		decoder.numOfDecodedPackets=1;
		decoder.inputs[0]=new unsigned char[3]{'A','A','A'};
		decoder.degres[0]=1;decoder.startingDegres[0]=1;
		decoder.neighbours[0]=new unsigned char[1]{1};decoder.sums[0]=new unsigned char[1]{0};
		decoder.counter=1;
		decoder.procesCurrentSymbol(0);
		if(decoder.numOfDecodedPackets==1&&decoder.inputs[0]==nullptr)cout<<"+";
		else cout<<"!!! Blad nadmiarowosci";
	}
	else if (scenario == 10) {

    decoder.procesCurrentSymbol(0);

    if(decoder.inputs[1] == nullptr || (decoder.degres[1] == 1 && decoder.neighbours[1][1] == 0)) cout << "+";
    else cout << "!!! Blad XOR niepokolei";
}
	cout<<endl;
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
    test_xorNeighbour();
    test_copymem();
    test_deleteEncodedSymbol();
    test_procesCurrentSymbol();
    for(int i=1;i<=10;i++)
        run_advanced_process_test(i);
    cout << "koniec" << endl;


    LT_Decoder decoder;
    decoder.wczytajPakiety("example.txt");

    std::cout << "Rozpoczynam dekodowanie pliku example.txt..." << std::endl;

    if(decoder.startDecoding()) {
        std::cout << "Sukces: Wszystkie pakiety zostaly odkodowane!" << std::endl;

        if(decoder.restoreFile()) {
            std::cout << "Plik zostal zapisany jako 'reconstructed_file'" << std::endl;
        } else {
            std::cout << "Blad podczas zapisu pliku wynikowego." << std::endl;
        }
    } else {
        std::cout << "Nie udalo sie odkodowac pliku." << std::endl;
    }

    return 0;
}
