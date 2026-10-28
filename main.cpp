#include "utilities.hpp"
#include "menu.hpp"
#include "file_operations.hpp"

// Comando di compilazione consigliato:
// g++ main.cpp menu.cpp file_operations.cpp utilities.cpp -o reFiles -std=c++20

int main() {
    system("cls");
    int pos = 1;
    cout << HIDE_CURSOR;
    cout << boolalpha;

    while (1) {
        cout << fs::current_path().string() << endl << endl;

        int scelta = menuScelta(pos, mainMenu);
        pos = scelta;

        switch (static_cast<MenuMain>(scelta)) {
            case MenuMain::MOSTRA_LISTA:
                mostra_lista();
                break;
            case MenuMain::RIMUOVI_PUNTEGGIATURA:
                gestisci_menu_punteggiatura();
                break;
            case MenuMain::RIMUOVI_PARENTESI:
                gestisci_menu_parentesi();
                break;
            case MenuMain::RIMUOVI_SPAZI:
            case MenuMain::RIMUOVI_SPAZI_RIPETUTI:
                switcher(scelta);
                mutuaEsclusione(scelta);
                break;
            case MenuMain::RIMUOVI_NUMERI:
                switcher(scelta);
                break;
            case MenuMain::RIMUOVI_SIMBOLI:
                gestisci_menu_simboli();
                break;
            case MenuMain::SOSTITUISCI_MAIUSCOLE:
            case MenuMain::SOSTITUISCI_MINUSCOLE:
                switcher(scelta);
                mutuaEsclusione(scelta);
                break;
            case MenuMain::RIMUOVI_PAROLE:
                gestisci_menu_parole();
                break;
            case MenuMain::DEFINISCI_ESTENSIONI:
                imposta_estensioni();
                break;
            case MenuMain::SOSTITUISCI_ESTENSIONE:
                sostituisci_estensioni();
                break;
            case MenuMain::ESPORTA_LISTA:
                esporta_lista();
                break;
            case MenuMain::ESGUI_RINOMINA:
                esegui_rinomina();
                break;
            case MenuMain::RIGA_VUOTA:
                break;
            case MenuMain::ABOUT_ME:
                aboutMe();
                break;
            case MenuMain::CHIUDI:
                system("cls");
                cout << SHOW_CURSOR;
                return 0;
        }

        system("cls");
    }

    return 0;
}