#include "menu.hpp"

// --- DEFINIZIONE DEI VETTORI DI MENU ---
vector<valoriMenu> mainMenu {
    {"Mostra Lista Files",         false}, // 1
    {"Rimuovi Punteggiatura",      false}, // 2
    {"Rimuovi Parentesi",          false}, // 3
    {"Rimuovi Spazi",              false}, // 4
    {"Rimuovi Spazi Ripetuti",     false}, // 5
    {"Rimuovi Numeri",             false}, // 6
    {"Rimuovi Simboli",            false}, // 7
    {"Sostituisci con Maiuscole",  false}, // 8
    {"Sostituisci con Minuscole",  false}, // 9
    {"Rimuovi Parole:",            false}, // 10
    {"Definisci estensioni",       false}, // 11
    {"Sostituisci estensione",     false}, // 12
    {"Esporta lista file",         false}, // 13
    {"[ ESEGUI RINOMINA ]",        false}, // 14
    {" ",                          false}, // 15
    {"About Me",                   false}, // 16
    {"Chiudi",                     false}  // 17
};

vector<valoriMenu> subMenu {
    {"Tutto",                       false},
    {"Estensioni specificate",      false},
    {"Cerca Sottostringhe",         false},
    {"Cerca Cifre",                 false},
    {"Data specifica (GG/MM/AAAA)", false},
    {"Dimensione",                  false},
    {"Esegui",                      false},
    {" ",                           false},
    {"Torna al menu principale",    false}
};

vector<valoriMenu> menuPunteggiatura {
    {",  (Virgola)",                false},
    {";  (Punto e virgola)",        false},
    {".  (Punto)",                  false},
    {":  (Due punti)",              false},
    {"-  (Trattino)",               false},
    {"_  (Underscore)",             false},
    {"!  (Punto esclamativo)",      false},
    {"?  (Punto interrogativo)",    false},
    {"'  (Apostrofo)",              false},
    {"^  (Accento circonflesso)",   false},
    {"\"  (Virgolette)",            false},
    {"Conserva un Ripetuto",        false},
    {"Sostituisci con Spazio",      false},
    {" ",                           false},
    {"Torna al menu principale",    false}
};

vector<valoriMenu> menuParentesi {
    {"< (Angolare sinistra)",       false},
    {"> (Angolare destra)",         false},
    {"( (Rotonda sinistra)",        false},
    {") (Rotonda destra)",          false},
    {"[ (Quadrata sinistra)",       false},
    {"] (Quadrata destra)",         false},
    {"{ (Graffa sinistra)",         false},
    {"} (Graffa destra)",           false},
    {"Conserva un Ripetuto",        false},
    {"Sostituisci con Spazio",      false},
    {" ",                           false},
    {"Torna al menu principale",    false}
};

vector<valoriMenu> menuSimboli {
    {"[~] (Tilde)",                   false},
    {"[\\] (Barra rovesciata)",       false},
    {"[£] (Sterlina)",                false},
    {"[$] (Dollaro)",                 false},
    {"[%] (Percentuale)",             false},
    {"[&] (E commerciale)",           false},
    {"[/] (Diviso)",                  false},
    {"[=] (Uguale)",                  false},
    {"[+] (Somma)",                   false},
    {"[*] (Moltiplicazione)",         false},
    {"[è] (E accentata) ",            false},
    {"[é] (E acuta) ",                false},
    {"[@] (Chiocciola)",              false},
    {"[ç] (C cediglia) ",             false},
    {"[ò] (O accentata) ",            false},
    {"[à] (A accentata) ",            false},
    {"[°] (Grado)",                   false},
    {"[#] (Cancelletto)",             false},
    {"[ù] (U accentata) ",            false},
    {"[§] (Paragrafo)",               false},
    {"[ì] (I accentata) ",            false},
    {"[|] (Barra verticale)",         false},
    {"Conserva un Ripetuto",          false}, 
    {"Sostituisci con Spazio",        false},
    {" ",                             false},
    {"Torna al menu principale",      false}
};

// --- FUNZIONI DI UTILITÀ PER I MENU ---
int modAttivi(const vector<valoriMenu>& menu) {
    int attivi = 0;
    for (const auto& i : menu) {
        if (i.decisione) attivi++;
    }
    return attivi;
}

// Funzione generica unificata per gestire i sottomenu (Punteggiatura, Parentesi, Simboli)
void gestisci_sottomenu_generico(vector<valoriMenu>& menuTarget, const string& titoloBanner, int indiceMainMenu) {
    int pos = 1;
    while (1) {
        system("cls");
        cout << BOLD << titoloBanner << RESET << endl << endl;
        
        int scelta = menuScelta(pos, menuTarget);
        pos = scelta;

        if (scelta == (int)menuTarget.size()) {
            break; // Torna al menu principale
        }

        menuTarget.at(scelta - 1).decisione = !menuTarget.at(scelta - 1).decisione;
    }

    int cont = modAttivi(menuTarget);
    mainMenu.at(indiceMainMenu).decisione = (cont > 0);
}

// --- SOTTOMENU SPECIFICI SEMPLIFICATI ---
void gestisci_menu_punteggiatura() {
    gestisci_sottomenu_generico(menuPunteggiatura, "=== CONFIGURAZIONE PUNTEGGIATURA ===", 1);
}

void gestisci_menu_parentesi() {
    gestisci_sottomenu_generico(menuParentesi, "=== CONFIGURAZIONE PARENTESI ===", 2);
}

void gestisci_menu_simboli() {
    gestisci_sottomenu_generico(menuSimboli, "=== CONFIGURAZIONE SIMBOLI ===", 6);
}

// --- FUNZIONE MENU INTERATTIVO PER VECTOR ---
int menuScelta(int pos, const vector<valoriMenu>& menu) {
    char tasto;
    cout << SAVE_CURSOR;
    int max = maxStringUTF8(menu);

    do {
        cout << RESTORE_CURSOR;

        for (size_t i = 1; i <= menu.size(); i++) {
            cout << CLEAR_LINE << "\r";

            cout << ((pos == (int)i) ? "> " GREEN LAMP : "  ");

            int byteLen = menu.at(i - 1).testo.length();
            int realChars = 0;
            size_t idx = 0;
            while (idx < byteLen) {
                unsigned char c = menu.at(i - 1).testo[idx];
                int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;
                realChars++;
                idx += len;
            }
            int utf8ExtraBytes = byteLen - realChars;
            int larghezzaAdattata = max + utf8ExtraBytes;

            cout << left << setw(larghezzaAdattata) << menu.at(i - 1).testo << RESET;

            if (&menu == &mainMenu) {
                if (i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 10 || i == 12) {
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                } else if (i == 11) {
                    cout << " =          (Tutti i file: " << YELLOW << estensioneTarget << RESET << ")"; // Nota: estensioneTarget è definita in file_operations
                }
            } 
            else if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli) {
                if (menu.at(i - 1).testo != " " && i != menu.size() && i != menu.size() - 1) {
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                }
            } 
            else if (&menu == &subMenu) {
                if (i >= 1 && i <= 6) {
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                }
            }

            int count = 0;
            if (&menu == &mainMenu) {
                if (i == 2) {
                    count = modAttivi(menuPunteggiatura);
                    cout << "  (" << count << ((count == 1) ? " elemento attivo)" : " elementi attivi)");
                } else if (i == 3) {
                    count = modAttivi(menuParentesi);
                    cout << "  (" << count << ((count == 1) ? " elemento attivo)" : " elementi attivi)");
                } else if (i == 7) {
                    count = modAttivi(menuSimboli);
                    cout << "  (" << count << ((count == 1) ? " elemento attivo)" : " elementi attivi)");
                } else if (i == 12) {
                    cout << "  (Converti in: " << YELLOW << estensioneOutput << RESET << ")";
                }
            }

            cout << endl;
        }

        tasto = getch();

        if (tasto == 'e' || tasto == 'E') {
            if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli) {
                return menu.size();
            }
        }

        if (tasto == 'q' || tasto == 'Q') {
            if (&menu == &mainMenu) {
                int indiceSelezionato = pos - 1;
                vector<valoriMenu>* sottomenuTarget = nullptr;
                if (indiceSelezionato == 1) sottomenuTarget = &menuPunteggiatura;
                else if (indiceSelezionato == 2) sottomenuTarget = &menuParentesi;
                else if (indiceSelezionato == 6) sottomenuTarget = &menuSimboli;

                if (sottomenuTarget != nullptr) {
                    size_t limiteVoci = sottomenuTarget->size() - 2;
                    bool giaTuttiAttivi = mainMenu.at(indiceSelezionato).decisione;
                    for (size_t i = 0; i < limiteVoci; i++) {
                        if (sottomenuTarget->at(i).testo != " " && !sottomenuTarget->at(i).decisione) {
                            giaTuttiAttivi = false;
                            break;
                        }
                    }
                    bool nuovoStato = !giaTuttiAttivi;
                    mainMenu.at(indiceSelezionato).decisione = nuovoStato;
                    for (size_t i = 0; i < limiteVoci; i++) {
                        if (sottomenuTarget->at(i).testo != " ") sottomenuTarget->at(i).decisione = nuovoStato;
                    }
                    continue;
                }
            }
        }

        if (tasto == 72 && pos > 1) {
            pos--;
            if (pos > 1 && menu.at(pos - 1).testo == " ") pos--;
        } 
        else if (tasto == 80 && pos < (int)menu.size()) {
            pos++;
            if (pos < (int)menu.size() && menu.at(pos - 1).testo == " ") pos++;
        }

    } while (tasto != '\r' && tasto != '\n');

    return pos;
}

// Gestisce un menu per stringhe
int menuScelta(unsigned pos, unsigned elements, ...) {

    char tasto;
    va_list args;
    const char **options = (const char**)malloc(elements * sizeof(char*));

    if(options == NULL) return -1;

    va_start(args, elements);   // Inizializzazione della lista di argomenti

    for (unsigned i = 0; i < elements; i++) {
        options[i] = va_arg(args, char*);
    }

    va_end(args);
    std::cout << SAVE_CURSOR;   // Salva la posizione corrente del cursore

    do {
        std::cout << RESTORE_CURSOR;     // Ripristina la posizione del cursore salvata

        for (unsigned i = 1; i <= elements; i++) {
            std::cout << CLEAR_LINE << "\r";    // \33[2K cancella la riga corrente, \r ritorna all'inizio della riga
            printf("%s%s%s\n", (pos == i) ? "> " UNDERLINE GREEN LAMP: "  ", options[i - 1], (pos == i) ? RESET " <" : "  ");
        }

        tasto = getch();

        if (tasto == 72 && pos != 1) {               // Tasto freccia su
            pos--;
        } else if (tasto == 80 && pos != elements) { // Tasto freccia giù
            pos++;
        }

    } while (tasto != '\r' && tasto != '\n'); // \r o \n è il codice ASCII per Invio

    free(options);
    return pos;
}

void switcher(int scelta) {
    mainMenu.at(scelta - 1).decisione = !mainMenu.at(scelta - 1).decisione;
}

void mutuaEsclusione(int scelta) {
    if (scelta == 8 && mainMenu.at(7).decisione) {
        mainMenu.at(8).decisione = false;
    } else if (scelta == 9 && mainMenu.at(8).decisione) {
        mainMenu.at(7).decisione = false;
    } else if (scelta == 4 && mainMenu.at(3).decisione) {
        mainMenu.at(4).decisione = false;
    } else if (scelta == 5 && mainMenu.at(4).decisione) {
        mainMenu.at(3).decisione = false;
    }
}