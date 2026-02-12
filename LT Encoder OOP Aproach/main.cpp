#include "LT_Encoder.h"

int main() {
    int pLen;
    double failure;

    std::cout << "Dlugosc pakietu: "; std::cin >> pLen;
    std::cout << "Prawd. porazki: "; std::cin >> failure;

    LT_Encoder encoder(pLen, failure);
    encoder.wczytajPakiety("test.txt");
    encoder.generujSymbole(100, "example.txt");

    std::cout << "Generowanie zakonczone!" << std::endl;
    std::cout << "Stopien 1: " << encoder.getLicznikStopnia1() << std::endl;

    return 0;
}
