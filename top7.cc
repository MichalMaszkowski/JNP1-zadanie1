#include <iostream>
#include <cstdint>
#include <algorithm>
#include <regex>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <sstream>
#include <cstdio>
#include <utility>

namespace {
    // Typy linii wejścia.
    enum rodzaj_linii {BLEDNA, NEW_MAX, TOP, GLOS, IGNORUJ};

    using numer_piosenki = int32_t;
    using liczba_glosow = size_t;
    using liczba_punktow = size_t;

    const numer_piosenki MINIMALNY_NUMER_PIOSENKI = 1;

    std::unordered_map<numer_piosenki, int> top7_notowanie; //(piosenka, jej numer w ostatnim notowaniu)
    std::unordered_map<numer_piosenki, std::pair<liczba_punktow, int>> top7_podsumowanie; //(piosenka, para (liczba punktów, numer w ostatnim podsumowaniu)
    std::unordered_set<numer_piosenki> wypadniete; // Zbiór piosenek, które odpadły i nie można na nie głosować.
    std::unordered_map<numer_piosenki, liczba_glosow> wyniki_notowania;

    rodzaj_linii rozpoznaj_wejscie (const std::string& linia_wejscia) {
        // Regexy rozpoznają jeden z 4 typów linii.
        // Jeżeli żaden wzorzec nie pasuje, to znaczy, że linia jest błędna.
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
        std::cerr << "Error in line " << numer_linii << ": " << linia_wejscia <<"\n";
    }

    bool potwierdz_poprawnosc_glosu(const std::string& linia_wejscia, numer_piosenki MAX) {
        std::stringstream dane = std::stringstream(linia_wejscia);
        numer_piosenki aktualna;
        std::unordered_set<numer_piosenki> tymczasowy_zbior;
        while (dane.peek() != EOF) {
            // Ten if zapobiega białym znakom znajdującym się na końcu linii.
            if (isspace(dane.get())) continue;
            else dane.unget();

            dane >> aktualna;
            // Sprawdza czy głos nie jest za duży ani za mały.
            if ((aktualna > MAX) || (aktualna < MINIMALNY_NUMER_PIOSENKI)) return false;
            // Sprawdza czy dana piosenka już nie odpadła.
            if (wypadniete.find(aktualna) != wypadniete.end()) return false;
            // Sprawdza czy w w tej linii już oddano głos na daną piosenkę.
            if (tymczasowy_zbior.find(aktualna) != tymczasowy_zbior.end()) return false;

            tymczasowy_zbior.insert(aktualna);
        }

        return true;
    }

    void zlicz_glos(const std::string& linia_wejscia) {
        std::stringstream dane = std::stringstream(linia_wejscia);
        numer_piosenki aktualna;
        while (dane.peek() != EOF) {
            // Ten if zapobiega białym znakom znajdującym się na końcu linii.
            if (isspace(dane.get())) continue;
            else dane.unget();

            dane >> aktualna;

            if (wyniki_notowania.find(aktualna) == wyniki_notowania.end()) {
                wyniki_notowania[aktualna] = 1;
            } else {
                ++(wyniki_notowania[aktualna]);
            }
        }
    }

    void wypisz_notowanie () {
        // Sortujemy wyniki używając seta.
        std::set<std::pair<liczba_glosow, numer_piosenki>> zbior_sortujacy;
        for (auto & iter : wyniki_notowania) {
            zbior_sortujacy.insert(std::make_pair (-(iter.second), iter.first)); //Sortuje leksykograficznie pary (-l. glosow, nr. piosenki)
        }

        int miejsce = 1;
        std::unordered_map<numer_piosenki, int> aktualne_notowanie;

        for (auto iter = zbior_sortujacy.begin(); iter != zbior_sortujacy.end() && miejsce < 8; ++iter) {
            // Sprawdzamy miejsce piosenki w poprzednim notowaniu.
            if (top7_notowanie.find(iter -> second) != top7_notowanie.end()) {
                std::cout << iter -> second << " " << top7_notowanie[iter -> second] - miejsce << "\n";
            }
            else {
                std::cout << iter -> second << " -\n";
            }

            aktualne_notowanie.insert({iter -> second, miejsce});
            miejsce++;
        }

        // Dodajemy odrzucone piosenki do zbioru wypadniętych.
        for (auto & iter : top7_notowanie) {
            if (aktualne_notowanie.find(iter.first) == aktualne_notowanie.end()) {
                wypadniete.insert(iter.first);
            }
        }

        // Czyścimy strukturę pomocniczą i zastępujemy stare notowanie.
        wyniki_notowania.clear();
        top7_notowanie.clear();
        top7_notowanie = aktualne_notowanie;
    }

    void zaktualizuj_punkty_podsumowania(){
        for (auto & iter : top7_notowanie) {
            // Zwiększamy liczbę punktów lub dodajemy nowy utwór to topu.
            if (top7_podsumowanie.find(iter.first) != top7_podsumowanie.end()) {
                top7_podsumowanie[iter.first].first += 8 - iter.second;
            }
            else {
                top7_podsumowanie.insert({iter.first, std::make_pair(8 - iter.second, 0)});
            }
        }
    }

    void otworz_nowe_notowanie(const std::string& linia_wejscia, const size_t numer_linii, numer_piosenki* MAX) {
        std::stringstream dane = std::stringstream(linia_wejscia);
        numer_piosenki nowy_MAX;
        std::string tymczasowy;
        dane >> tymczasowy; // Omija napis "NEW".
        dane >> nowy_MAX;
        if (nowy_MAX < *MAX) {
            wypisz_linie_bledu(linia_wejscia, numer_linii);
        }
        else {
            *MAX = nowy_MAX;
            wypisz_notowanie();
            zaktualizuj_punkty_podsumowania();
        }
    }

    void wypisz_podsumowanie() {
        // Sortujemy używając seta.
        std::set<std::pair<liczba_glosow, numer_piosenki>> zbior_sortujacy;
        for (auto & iter : top7_podsumowanie) {
            zbior_sortujacy.insert(std::make_pair (iter.second.first, -iter.first));
        }

        int pozycja = 1;
        for (auto iter = zbior_sortujacy.rbegin(); iter != zbior_sortujacy.rend(); ++iter) {
            if (pozycja < 8) {
                // Wypisujemy tylko 7 pierwszych pozycji.
                auto iter_top7 = top7_podsumowanie.find(-iter -> second);
                int poprzednia_pozycja = iter_top7 -> second.second;
                if (poprzednia_pozycja != 0)
                    std::cout << -iter -> second << " " << poprzednia_pozycja - pozycja << "\n";
                else
                    std::cout << -iter -> second << " -\n";
                iter_top7->second.second = pozycja;
                pozycja++;
            }
            else {
                // Pozostałym utworom ustawiamy miejsce w poprzednim podsumowaniu na 0, czyli brak.
                if (wypadniete.find(-iter -> second) != wypadniete.end()) {
                    top7_podsumowanie.erase(top7_podsumowanie.find(-iter -> second));
                }
                else {
                    top7_podsumowanie.find(-iter -> second)->second.second = 0;
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();

    size_t numer_linii = 0;
    numer_piosenki MAX = 0; //początkowo nie ma notowania, co jest równoważne temu, że żaden numer piosenki nie jest dopuszczalny
    std::string linia_wejscia;

    // Wczytujemy po kolei linie i w zależności od tego, co zwróci funkcja rozpoznająca tym linii uruchamiamy odpowiednie funkcje.
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
                if (potwierdz_poprawnosc_glosu(linia_wejscia, MAX)) {
                    zlicz_glos(linia_wejscia);
                }
                else {
                    wypisz_linie_bledu(linia_wejscia, numer_linii);
                }
                break;
            case IGNORUJ:
                break;
        }
    }
    return 0;
}
