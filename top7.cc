#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <regex>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

namespace {
    enum rodzaj_linii { BLEDNA, NEW_MAX, TOP, GLOS };
    using numer_piosenki = int32_t;
    using liczba_glosow = size_t;
    using liczba_punktow = size_t;

    const numer_piosenki MINIMALNY_NUMER_PIOSENKI = 1;
    const numer_piosenki MAKSYMALNY_NUMER_PIOSENKI = 99999999;


    std::set<numer_piosenki> top7_notowanie;
    std::set<numer_piosenki> top7_podsumowanie;
    std::unordered_set<numer_piosenki> wypadniete;
    std::unordered_map<numer_piosenki> wyniki_notowania

    rodzaj_linii rozpoznaj_wejscie (std::string linia_wejscia) {
        /**
         *
         */
    }

    void wypisz_linie_bledu(const std::string& linia_wejscia, const int numer_linii) {
        std::cerr << "Error in line" << line_number << ": " << input_line;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();

    size_t numer_linii = 0;
    std::string linia_wejscia;

    while(getline(std::cin, linia_wejscia)) {
        ++numer_linii;
        switch (rozpoznaj_wejscie(linia_wejscia)) {
            case BLEDNA:
                wypisz_linie_bledu(linia_wejscia, numer_linii);
                break;
            case NEW_MAX:
                otworz_nowe_notowanie(linia_wejscia);
                break;
            case TOP:
                wypisz_podsumowanie();
                break;
            case GLOS:
                if (sprawdz_poprawnosc_glosu(linia_wejscia)) {
                    zlicz_głos(linia_wejscia)
                } else {
                    wypisz_linie_bledu(linia_wejscia, numer_linii);
                }
                break;
        }
    }
    return 0;
}
