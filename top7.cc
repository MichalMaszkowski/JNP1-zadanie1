#define MAKSYMALNY_DOZWOLONY_NUMER 99999999
#define MINIMALNY_DOZWOLONY_NUMER 1

#include <iostream>
#include <string>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <unordered_map>

enum rodzaj_linii { ERROR, NEW_MAX, TOP, VOTES };
using numer_piosenki = int32_t;
using liczba_glosow = int;
using liczba_punktow = int;

rodzaj_linii input_check_type (std::string input_line) {
    /**
     *
     */
}

void linia_bledu(const std::string& input_line, int line_number) {
    std::cerr << "Error in line" << line_number << ": " << input_line;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();

    std::set<song_number> top7_notowanie;
    std::set<song_number> top7_podsumowanie;
    std::unordered_set<song_number> wypadniete;
    std::unordered_map<song_number> wyniki_notowania




    int line_number;
    std::string input_line;
    getline(std::cin, input_line);

    switch (input_check_type(input_line)) {
        case ERROR: error_line(input_line, line_number); break;
        case NEW_MAX:
            case TOP:
                case VOTES:
    }
    return 0;
}
