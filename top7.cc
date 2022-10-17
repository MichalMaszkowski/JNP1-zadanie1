#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <regex>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <cstdio>

namespace {
    enum rodzaj_linii { BLEDNA, NEW_MAX, TOP, GLOS, IGNORUJ};
    using numer_piosenki = int32_t;
    using liczba_glosow = size_t;
    using liczba_punktow = size_t;

    const numer_piosenki MINIMALNY_NUMER_PIOSENKI = 1;
    const numer_piosenki MAKSYMALNY_NUMER_PIOSENKI = 99999999;


    std::set<numer_piosenki> top7_notowanie;
    std::set<numer_piosenki> top7_podsumowanie;
    std::unordered_set<numer_piosenki> wypadniete;
    std::unordered_map<numer_piosenki> wyniki_notowania;

    rodzaj_linii rozpoznaj_wejscie (const std::string& linia_wejscia) {
        std::regex IGNORUJ_regex (R"(\s*)");
        std::regex NEW_MAX_regex (R"([ \r\t\f]*NEW[ \r\t\f]+[1-9]\d{0,7}\s*)");
        std::regex TOP_regex (R"([ \r\t\f]*TOP\s*)");
        std::regex GLOS_regex (R"(([ \r\t\f]*[1-9]\d{0,7})+\s*)");

        if (std::regex_match(linia_wejscia, IGNORUJ_regex)) return IGNORUJ;
        if (std::regex_match(linia_wejscia, NEW_MAX_regex)) return NEW_MAX;
        if (std::regex_match(linia_wejscia, TOP_regex)) return TOP;
        if (std::regex_match(linia_wejscia, GLOS_regex)) return GLOS;

        return BLEDNA;
    }

    void wypisz_linie_bledu(const std::string& linia_wejscia, const size_t numer_linii) {
        std::cerr << "Error in line " << numer_linii << ": " << linia_wejscia <<"/n";
    }

    bool potwierdz_poprawnosc_glosu(const std::string& linia_wejscia, const size_t numer_linii) {
        bool wynik = true;
        std::stringstream dane = stringstream(linia_wejscia);
        numer_piosenki aktualna;
        std::unordered_set<numer_piosenki> tymczasowy_zbior;
        while (wynik && (dane.peek() != EOF)) {
            dane >> aktualna;
            if (wypadniete.find(aktualna) != wypadniete.end()) {
                wynik = false;
            } else {
                if (tymczasowy_zbior.find(aktualna) == tymczasowy_zbior.end()) {
                    tymczasowy_zbior.insert(aktualna);
                } else {
                    wynik = false;
                }
            }
        }
        return wynik;
    }

    void zlicz_głos(const std::string& linia_wejscia) {
        std::stringstream dane = stringstream(linia_wejscia);
        numer_piosenki aktualna;
        while (dane.peek() != EOF) {
            dane >> aktualna;
            if (wyniki_notowania.find(aktualna) == wyniki_notowania.end()) {
                wyniki_notowania[aktualna] = 1;
            } else {
                ++(wyniki_notowania[aktualna]);
            }
        }
        return;
    }

    void otworz_nowe_notowanie(const std::string& linia_wejscia, const size_t numer_linii, numer_piosenki* MAX) {
        std::stringstream dane = stringstream(linia_wejscia);
        numer_piosenki nowy_MAX;
        std::string tymczas;
        dane >> tymczas; //zjada "NEW"
        dane >> tymczas; //zjada "MAX"
        dane >> nowy_MAX;
        if (nowy_MAX < MAX) {
            wypisz_linie_bledu(linia_wejscia, numer_linii);
        }
        else {
            MAX = nowy_MAX;
            /*
             *
             */
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();

    size_t numer_linii = 0;
    numer_piosenki MAX = 0; //początkowo nie ma notowania, co jest równoważne temu, że żaden numer piosenki nie jest dopuszczalny
    std::string linia_wejscia;

    while(getline(std::cin, linia_wejscia)) {
        ++numer_linii;
        switch (rozpoznaj_wejscie(linia_wejscia)) {
            case BLEDNA:
                wypisz_linie_bledu(linia_wejscia, numer_linii);
                break;
            case NEW_MAX:
                otworz_nowe_notowanie(linia_wejscia, numer_linii, &MAX);
                break;
            case TOP:
                wypisz_podsumowanie();
                break;
            case GLOS:
                if (potwierdz_poprawnosc_glosu(linia_wejscia)) {
                    zlicz_głos(linia_wejscia);
                } else {
                    wypisz_linie_bledu(linia_wejscia, numer_linii);
                }
                break;
            case IGNORUJ:
                break;
        }
    }
    return 0;
}
