#define MAKSYMALNY_DOZWOLONY_NUMER 99999999
#define MINIMALNY_DOZWOLONY_NUMER 1

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>


enum rodzaj_linii { BLEDNA, NEW, TOP, GLOS };
using numer_piosenki = int32_t;
using liczba_glosow = int;
using liczba_punktow = int;

rodzaj_linii rozpoznanie_wejscia (std::string linia_wejscia) {
    /**
     *
     */
}

void linia_bledu(const std::string& linia_wejscia, const int numer_linii) {
    std::cerr << "Error in line" << line_number << ": " << input_line;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();

    std::set<numer_piosenki> top7_notowanie;
    std::set<numer_piosenki> top7_podsumowanie;
    std::unordered_set<numer_piosenki> wypadniete;
    std::unordered_map<numer_piosenki> wyniki_notowania




    int numer_linii = 1;
    std::string linia_wejscia;
    getline(std::cin, linia_wejscia);

    switch (rozpoznanie_wejscia(linia_wejscia)) {
        case BLEDNA:
            linia_bledu(linia_wejscia, numer_linii);
            break;
        case NEW:
            break;
        case TOP:
            break;
        case GLOS:
            break;
    }
    return 0;
}
