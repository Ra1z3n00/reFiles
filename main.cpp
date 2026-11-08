#include "utilities.hpp"
#include "menu.hpp"
#include "file_operations.hpp"
#include "localization.hpp"

// Comando di compilazione aggiornato:
// g++ main.cpp menu.cpp file_operations.cpp utilities.cpp localization.cpp -o reFiles -std=c++20


int nFiles = 0;

int main() {
    
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    system("cls");
    int pos = 1;
    cout << HIDE_CURSOR;
    cout << boolalpha;
    nFiles = conta_file_target();

    // Inizializza i testi della lingua predefinita all'avvio
    aggiorna_testi_menu();

    while (1) {
        cout << fs::current_path().string() << endl << endl;

        int scelta = menuScelta(pos, mainMenu);
        pos = scelta;

        switch (static_cast<MenuMain>(scelta)) {
            case MenuMain::MOSTRA_LISTA:
                mostra_lista();
                break;
            case MenuMain::CAMBIA_PATH:
                cambia_path();
                nFiles = conta_file_target();
                break;
            case MenuMain::REGOLE_TARGET:
                gestisci_menu_target();
                nFiles = conta_file_target();
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
            case MenuMain::CAMBIA_LINGUA: // <--- GESTIONE CAMBIO LINGUA NEL MAIN
                if (ottieni_lingua_corrente() == Lingua::Italiano) {
                    imposta_lingua(Lingua::Inglese);
                } else {
                    imposta_lingua(Lingua::Italiano);
                }
                aggiorna_testi_menu(); // Ricarica le etichette nel nuovo idioma
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