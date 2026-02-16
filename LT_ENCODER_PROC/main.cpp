#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>

using namespace std;

// Liczniki do testowania dzia³ania kodowania
int licznikstopnia1 = 0;
int licznikcalosciowy = 0;

double R;      // uzywane przy rozk³adzie robust-soliton
float c;       // sta³a do obliczania R
double D;      // niezawodnoœæ
double rozklad[256];
const int MAX_K = 255;
double suma = 0;

// zmienna kontrolna: reszta z dzielenia liczby znaków przez d³ugoœæ pakietu
int kontrol;
char** inputs; // tabela 2D przechowuj¹ca symbole wejœciowe
int liczbaPakietow;
unsigned char* degre; // stopieñ oraz indeksy "sasiadów"
unsigned char* encodedpakiet;
unsigned char* temporary; // pakiet pomocniczy
unsigned char* sumach;    // suma kontrolna jako tablica znaków

int licznika; // lokalna przewidywana d³ugoœæ pakietu

// Funkcja sprawdzaj¹ca ile pakietów zostanie wygenerowanych
int dlugosc(ifstream& c, int div) {
    char znak;
    int liczbaznakow = 0;
    while (c.get(znak)) {
        liczbaznakow++;
    }
    c.clear();   // usuwanie flagi strumienia
    c.seekg(0);  // Resetowanie wskaŸnika na pocz¹tek
    kontrol = liczbaznakow % div;
    return (liczbaznakow + div - 1) / div; // Zaokr¹glenie w górê
}

void wczytajpakiety(int pakietln) {
    ifstream file("test.txt", ios::in | ios::binary);
    if (!file) {
        cout << "Nie udalo sie otworzyc pliku!";
    }

    liczbaPakietow = dlugosc(file, pakietln);
    inputs = new char*[liczbaPakietow]; // Alokacja pamiêci na tablicê 2D

    for (int i = 0; i < liczbaPakietow; i++) {
        inputs[i] = new char[pakietln];
        memset(inputs[i], 0, pakietln);
    }

    for (int j = 0; j < liczbaPakietow; j++) {
        file.read(inputs[j], pakietln);
        cout << "pakiet nr " << (j + 1) << ". ";
        for (int d = 0; d < pakietln; d++)
            cout << inputs[j][d];
        cout << endl;
    }
    file.close();
}

void xorowanie(int a, int pakietlenght) {
    for (int l = 0; l < pakietlenght; l++) {
        encodedpakiet[l] ^= inputs[a - 1][l];
    }
    cout << endl;
}

double calculateR(int inp) {
    cout << "R: " << (c * log((inp / D)) * sqrt(inp)) << endl;
    return (c * log((inp / D)) * sqrt(inp));
}

void tablicasoliton(int inp) {
    double rho[MAX_K + 1] = {0.0};
    rho[1] = 1.0 / inp;
    cout << "pr kom 1: " << rho[1] << endl;
    for (int i = 2; i <= inp; ++i) {
        rho[i] = 1.0 / (i * (i - 1));
    }
    rozklad[1] = rho[1];
    for (int i = 2; i <= inp; ++i) {
        rozklad[i] = rho[i];
    }
}

void tablica_robust_soliton(int inp, double R) {
    double rho[MAX_K + 1] = {0.0};
    rho[1] = R / (inp);
    if (1 <= ((inp / R) - 1)) {
        for (int i = 2; i < ((inp / R) - 1); i++) {
            if ((i == (inp / R)) && (i != 1))
                rho[i] = (R * log(R / D));
            else
                rho[i] = R / (i * inp);
        }
    }
    for (int i = 1; i <= inp; ++i) {
        rozklad[i] = rozklad[i] + rho[i];
        cout << "komorka robust-soliton " << rozklad[i] << endl;
        suma = suma + rozklad[i];
    }
    cout << "suma robust soliton: " << suma << endl;
    for (int i = 1; i <= inp; ++i) {
        rozklad[i] = rozklad[i] / suma;
        cout << "pr komorki robust-soliton:" << i << ". " << rozklad[i] << endl;
    }
    for (int i = 2; i <= inp; i++) {
        rozklad[i] = rozklad[i] + rozklad[i - 1];
        cout << "koncowa tablica od indeksu 2: " << rozklad[i] << endl;
    }
}

int randrobustsoliton(int inp, unsigned int a) {
    static mt19937 gen(a);
    uniform_real_distribution<> dist(0.0, 1.0);
    double r = dist(gen); // Losowa liczba z przedzia³u [0, 1)
    for (int i = 1; i <= inp; ++i) {
        if (r < rozklad[i]) {
            return i;
        }
    }
    return 0;
}

void degree(int pakietlenght, int czas) {
    bool aktywacja = true;
    srand(czas);
    int table = randrobustsoliton(liczbaPakietow, time(0)); // Liczba s¹siadów

    if (table == 1)
        licznikstopnia1 += 1;

    licznikcalosciowy += 1;
    degre = new unsigned char[table + 1];
    degre[0] = table; // Stopieñ zapisany na pocz¹tku tablicy
    cout << "Stopieñ symbolu kodu: " << (int)degre[0] << endl;

    encodedpakiet = new unsigned char[pakietlenght];
    int i = 1;
    while (i <= table) {
        int temp = (rand() % liczbaPakietow) + 1;
        bool powtorka = false;
        for (int j = 1; j <= i; j++) {
            if (degre[j] == temp) {
                powtorka = true;
                break;
            }
        }
        if (!powtorka) {
            degre[i] = temp;
            cout << "Sasiad symbolu kodu: " << (int)degre[i] << endl;
            if (aktywacja) {
                memcpy(encodedpakiet, inputs[temp - 1], pakietlenght);
                aktywacja = false;
            } else {
                xorowanie(temp, pakietlenght);
            }
            i++;
        }
    }
}

void combine(int len1) {
    int len2 = ((int)degre[0] + 1);
    temporary = new unsigned char[len1 + len2];
    licznika = len1 + len2;
    for (int l = 0; l < len1; l++) {
        temporary[l] = encodedpakiet[l];
    }
    for (int l = len1; l < len1 + len2; l++) {
        temporary[l] = degre[l - len1];
    }
}

void combine2(int len2, int len1) {
    delete[] encodedpakiet;
    len1 += ((int)degre[0] + 1);
    cout << "dlugosc 3( zksorowany pakiet + indeksy ) " << len1 << endl;
    cout << "dlugosc 4 ( suma kontrolna w charach ) " << len2 << endl;
    encodedpakiet = new unsigned char[len1 + len2];
    licznika = len1 + len2;
    for (int l = 0; l < len1; l++) {
        encodedpakiet[l] = temporary[l];
    }
    for (int l = len1; l < len1 + len2; l++) {
        encodedpakiet[l] = sumach[l - len1];
    }
}

void inttochar(int suma_val, int pakietln) {
    int i = 1;
    int counterofch = 1;
    do {
        i = i * 10;
        if (i < suma_val)
            counterofch += 1;
    } while (i < suma_val);

    sumach = new unsigned char[counterofch];
    cout << "wartosci tablicy zawierajacej sume kontrolna: ";
    for (int j = 0; j < counterofch; j++) {
        sumach[j] = suma_val % 10;
        cout << (int)sumach[j] << " ; ";
        suma_val = suma_val / 10;
    }
    cout << endl;
    combine2(counterofch, pakietln);
}

void calcsum(int pakietln) {
    int suma1 = 0;
    for (int i = 0; i < pakietln + degre[0] + 1; i++) {
        suma1 += temporary[i];
    }
    inttochar(suma1, pakietln);
}

void deletemem() {
    delete[] degre;
    delete[] encodedpakiet;
    delete[] temporary;
    delete[] sumach;
}

void deleteinputs() {
    for (int k = 0; k < liczbaPakietow; k++) {
        delete[] inputs[k];
    }
    delete[] inputs;
}

int main() {
    int pakietlenght;
    cout << "podaj dlugosc pakietu" << endl;
    cin >> pakietlenght;

    wczytajpakiety(pakietlenght);

    cout << "podaj prawdopodobienstwo nie powodzenia sie dekodowania w procesie LT" << endl;
    cout << "Gdy wypisze tablice prawdopodobienstw, nacisnij znak z klawiatury a pozniej enter aby kontynuowac" << endl;
    cin >> D;

    cout << "podaj stala C wieksza od zera, najlepiej rowna 1: ";
    cin >> c;
    cout << endl;

    tablicasoliton(liczbaPakietow);
    tablica_robust_soliton(liczbaPakietow, calculateR(liczbaPakietow));

    float znakkk;
    cin >> znakkk;

    ofstream plik("example.txt", ios::out | ios::binary | ios::trunc);
    plik<<liczbaPakietow<<";";
    plik<<kontrol<<";";
    for (int i = 0; i < (liczbaPakietow * 5); i++) {
        cout << "=======================================================" << endl;
        degree(pakietlenght, (time(0) + i));
        combine(pakietlenght);
        calcsum(pakietlenght);

        cout << "wypisuje wynikowy pakiet ";
        for (int j = 0; j < licznika; j++) {
            plik << encodedpakiet[j];
            cout << (j + 1) << ". " << encodedpakiet[j] << " " << (int)encodedpakiet[j] << " ";
        }
        cout << endl;
        plik << ";";
        deletemem();
    }

    deleteinputs();
    plik.close();

    cout << "liczba jedynek wynosi: " << licznikstopnia1 << endl;
    cout << "liczba operacji wynosi: " << licznikcalosciowy << endl;

    return 0;
}
