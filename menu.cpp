#include "menu.hpp"
#include "localization.hpp"

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
    {"Lingua: Italiano",           false}, // 17
    {"Chiudi",                     false}  // 18
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
    gestisci_sottomenu_generico(menuPunteggiatura, get_string("BANNER_PUNTEGGIATURA"), 1);
}

void gestisci_menu_parentesi() {
    gestisci_sottomenu_generico(menuParentesi, get_string("BANNER_PARENTESI"), 2);
}

void gestisci_menu_simboli() {
    gestisci_sottomenu_generico(menuSimboli, get_string("BANNER_SIMBOLI"), 6);
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
                    cout << get_string("LBL_TUTTI_FILE") << YELLOW << estensioneTarget << RESET << ")"; // Nota: estensioneTarget è definita in file_operations
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

            // Spiegazioni laterali del menu principale (quanti moduli sono attivi)
            int count = 0;
            if (&menu == &mainMenu) {
                if (i == 2) {
                    count = modAttivi(menuPunteggiatura);
                    cout << "  (" << count << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                } else if (i == 3) {
                    count = modAttivi(menuParentesi);
                    cout << "  (" << count << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                } else if (i == 7) {
                    count = modAttivi(menuSimboli);
                    cout << "  (" << count << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                } else if (i == 12) {
                    cout << get_string("LBL_CONVERTI_IN") << YELLOW << estensioneOutput << RESET << ")";
                }
            }

            // Spiegazioni laterali del subMenu, in particolare di "Conserva un Ripetuto" e "Sostituisci con spazio"
            if (&menu == &menuPunteggiatura || &menu == &menuSimboli || &menu == &menuParentesi) {
                if (i == (menu.size() - 2)) {
                    cout << EXT_TXT_RGB(255,165,0) << get_string("LBL_RIPETUTO_GUIDA") << RESET;
                } else if (i == (menu.size() - 3)) {
                    cout << EXT_TXT_RGB(255,165,0) << get_string("LBL_SPAZIO_GUIDA") << RESET;
                }
            }

            cout << endl;
        }

        tasto = getch();

        if (tasto == 'e' || tasto == 'E') {
            if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli || &menu == &subMenu) {
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

void aggiorna_testi_menu() {
    // Aggiornamento Main Menu
    mainMenu.at(0).testo = get_string("MENU_MOSTRA");
    mainMenu.at(1).testo = get_string("MENU_PUNTEGGIATURA");
    mainMenu.at(2).testo = get_string("MENU_PARENTESI");
    mainMenu.at(3).testo = get_string("MENU_SPAZI");
    mainMenu.at(4).testo = get_string("MENU_SPAZI_RIPETUTI");
    mainMenu.at(5).testo = get_string("MENU_NUMERI");
    mainMenu.at(6).testo = get_string("MENU_SIMBOLI");
    mainMenu.at(7).testo = get_string("MENU_MAIUSCOLE");
    mainMenu.at(8).testo = get_string("MENU_MINUSCOLE");
    mainMenu.at(9).testo = get_string("MENU_PAROLE");
    mainMenu.at(10).testo = get_string("MENU_DEF_EXT");
    mainMenu.at(11).testo = get_string("MENU_SOS_EXT");
    mainMenu.at(12).testo = get_string("MENU_ESPORTA");
    mainMenu.at(13).testo = get_string("MENU_ESGUI");
    // Indice 14 è la riga vuota " "
    mainMenu.at(15).testo = get_string("MENU_ABOUT");
    mainMenu.at(16).testo = get_string("MENU_LINGUA");
    mainMenu.at(17).testo = get_string("MENU_CHIUDI");

    // Aggiornamento SubMenu (Filtri)
    subMenu.at(0).testo = get_string("SUB_TUTTO");
    subMenu.at(1).testo = get_string("SUB_ESTENSIONI");
    subMenu.at(2).testo = get_string("SUB_SOTTOSTRINGHE");
    subMenu.at(3).testo = get_string("SUB_CIFRE");
    subMenu.at(4).testo = get_string("SUB_DATA");
    subMenu.at(5).testo = get_string("SUB_DIMENSIONE");
    subMenu.at(6).testo = get_string("SUB_ESEGUI");
    // Indice 7 è la riga vuota " "
    subMenu.at(8).testo = get_string("SUB_INDIETRO");

    // Aggiornamento Menu Punteggiatura
    menuPunteggiatura.at(0).testo = get_string("PUNT_VIRGOLA");
    menuPunteggiatura.at(1).testo = get_string("PUNT_PUNTO_VIRGOLA");
    menuPunteggiatura.at(2).testo = get_string("PUNT_PUNTO");
    menuPunteggiatura.at(3).testo = get_string("PUNT_DUE_PUNTI");
    menuPunteggiatura.at(4).testo = get_string("PUNT_TRATTINO");
    menuPunteggiatura.at(5).testo = get_string("PUNT_UNDERSCORE");
    menuPunteggiatura.at(6).testo = get_string("PUNT_ESCLAMATIVO");
    menuPunteggiatura.at(7).testo = get_string("PUNT_INTERROGATIVO");
    menuPunteggiatura.at(8).testo = get_string("PUNT_APOSTROFO");
    menuPunteggiatura.at(9).testo = get_string("PUNT_CIRCONFLESSO");
    menuPunteggiatura.at(10).testo = get_string("PUNT_VIRGOLETTE");
    menuPunteggiatura.at(11).testo = get_string("OP_CONSERVA_RIPETUTO");
    menuPunteggiatura.at(12).testo = get_string("OP_SOSTITUISCI_SPAZIO");
    // Indice 13 è " "
    menuPunteggiatura.at(14).testo = get_string("SUB_INDIETRO");

    // Aggiornamento Menu Parentesi
    menuParentesi.at(0).testo = get_string("PAR_ANG_SX");
    menuParentesi.at(1).testo = get_string("PAR_ANG_DX");
    menuParentesi.at(2).testo = get_string("PAR_ROT_SX");
    menuParentesi.at(3).testo = get_string("PAR_ROT_DX");
    menuParentesi.at(4).testo = get_string("PAR_QUAD_SX");
    menuParentesi.at(5).testo = get_string("PAR_QUAD_DX");
    menuParentesi.at(6).testo = get_string("PAR_GRAF_SX");
    menuParentesi.at(7).testo = get_string("PAR_GRAF_DX");
    menuParentesi.at(8).testo = get_string("OP_CONSERVA_RIPETUTO");
    menuParentesi.at(9).testo = get_string("OP_SOSTITUISCI_SPAZIO");
    // Indice 10 è " "
    menuParentesi.at(11).testo = get_string("SUB_INDIETRO");

    // Aggiornamento Menu Simboli
    menuSimboli.at(0).testo = get_string("SIMB_TILDE");
    menuSimboli.at(1).testo = get_string("SIMB_BACKSLASH");
    menuSimboli.at(2).testo = get_string("SIMB_STERLINA");
    menuSimboli.at(3).testo = get_string("SIMB_DOLLARO");
    menuSimboli.at(4).testo = get_string("SIMB_PERCENTUALE");
    menuSimboli.at(5).testo = get_string("SIMB_AMPERSAND");
    menuSimboli.at(6).testo = get_string("SIMB_DIVISO");
    menuSimboli.at(7).testo = get_string("SIMB_UGUALE");
    menuSimboli.at(8).testo = get_string("SIMB_SOMMA");
    menuSimboli.at(9).testo = get_string("SIMB_MOLTIPLICAZIONE");
    menuSimboli.at(10).testo = get_string("SIMB_E_ACC_GRAVE");
    menuSimboli.at(11).testo = get_string("SIMB_E_ACC_ACUTA");
    menuSimboli.at(12).testo = get_string("SIMB_CHIOCCIOLA");
    menuSimboli.at(13).testo = get_string("SIMB_C_CEDIGLIA");
    menuSimboli.at(14).testo = get_string("SIMB_O_ACC_GRAVE");
    menuSimboli.at(15).testo = get_string("SIMB_A_ACC_GRAVE");
    menuSimboli.at(16).testo = get_string("SIMB_GRADO");
    menuSimboli.at(17).testo = get_string("SIMB_CANCELLETTO");
    menuSimboli.at(18).testo = get_string("SIMB_U_ACC_GRAVE");
    menuSimboli.at(19).testo = get_string("SIMB_PARAGRAFO");
    menuSimboli.at(20).testo = get_string("SIMB_I_ACC_GRAVE");
    menuSimboli.at(21).testo = get_string("SIMB_BARRA_VERT");
    menuSimboli.at(22).testo = get_string("OP_CONSERVA_RIPETUTO");
    menuSimboli.at(23).testo = get_string("OP_SOSTITUISCI_SPAZIO");
    // Indice 24 è " "
    menuSimboli.at(25).testo = get_string("SUB_INDIETRO");
}

