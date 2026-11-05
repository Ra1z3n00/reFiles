#include "menu.hpp"
#include "localization.hpp"
#include "file_operations.hpp"

namespace fs = std::filesystem;

// --- DEFINIZIONE DEI VETTORI DI MENU ---
vector<valoriMenu> mainMenu {
    {"Mostra Lista Files",         false}, // 1
    {"Cambia Posizione",           false}, // 2
    {"Regole Target",              false}, // 3
    {"Rimuovi Punteggiatura",      false}, // 4
    {"Rimuovi Parentesi",          false}, // 5
    {"Rimuovi Spazi",              false}, // 6
    {"Rimuovi Spazi Ripetuti",     false}, // 7
    {"Rimuovi Numeri",             false}, // 8
    {"Rimuovi Simboli",            false}, // 9
    {"Rimuovi Parole",             false}, // 10
    {"Sostituisci con Maiuscole",  false}, // 11
    {"Sostituisci con Minuscole",  false}, // 12
    {"Sostituisci estensione",     false}, // 13
    {"Esporta lista file",         false}, // 14
    {"[ ESEGUI RINOMINA ]",        false}, // 15
    {" ",                          false}, // 16
    {"About Me",                   false}, // 17
    {"Lingua: Italiano",           false}, // 18
    {"Chiudi",                     false}  // 19
};

vector<valoriMenu> subMenu_regoleCerca {
    {"Tutto",                        true},
    {"Definisci Estensioni",        false},
    {"Cerca Sottostringhe",         false},
    {"Cerca Cifre",                 false},
    {"Data files (GG/MM/AAAA)",     false},
    {"Dimensione",                  false},
    {"Esegui",                      false},
    {" ",                           false},
    {"Torna al menu principale",    false}
};

vector<valoriMenu> subMenu_regoleTarget {
    {"Tutto",                        true},
    {"Definisci Estensioni",        false},
    {"Definisci Sottostringhe",     false},
    {"Con Cifre",                   false},
    {"Definisci Cifre",             false},
    {"Data files (GG/MM/AAAA)",     false},
    {"Dimensione",                  false},
    {"Inizia con",                  false},
    {"Finisce con",                 false},
    {"Menu Selezione",               true},
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

vector<valoriMenu> menu_infoEsporta {
    {"Nome",                    false},
    {"Estensione",              false},
    {"Dimensione",              false},
    {"Anno (Se Disponibile)",   false},
    {"Esegui",                  false},
    {"",                        false},
    {"Torna Indietro",          false},
};

vector<valoriMenu> menu_formatoFile {
    {"TXT (Testo semplice)",                false},
    {"CSV (Valori separati da virgola)",    false},
    {"JSON (Formato strutturato)",          false},
    {"[Esporta]",                           false},
    {"",                                    false},
    {"Torna Indietro",                      false},
};

int modAttivi(const vector<valoriMenu>& menu) {
    int attivi = 0;
    for (const auto& i : menu) {
        if (i.decisione) attivi++;
    }
    return attivi;
}

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

void gestisci_menu_punteggiatura() {
    gestisci_sottomenu_generico(menuPunteggiatura, get_string("BANNER_PUNTEGGIATURA"), static_cast<int>(MenuMain::RIMUOVI_PUNTEGGIATURA) - 1);
}

void gestisci_menu_parentesi() {
    gestisci_sottomenu_generico(menuParentesi, get_string("BANNER_PARENTESI"), static_cast<int>(MenuMain::RIMUOVI_PARENTESI) - 1);
}

void gestisci_menu_simboli() {
    gestisci_sottomenu_generico(menuSimboli, get_string("BANNER_SIMBOLI"), static_cast<int>(MenuMain::RIMUOVI_SIMBOLI) - 1);
}

int menuScelta(int pos, const vector<valoriMenu>& menu) {
    char tasto;
    cout << SAVE_CURSOR;
    int max = maxStringUTF8(menu);

    do {
        cout << RESTORE_CURSOR;

        for (size_t i = 1; i <= menu.size(); i++) {
            cout << CLEAR_LINE << "\r";

            cout << ((pos == (int)i) ? "> " GREEN LAMP : "  ");

            size_t byteLen = menu.at(i - 1).testo.length();
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
                MenuMain voce = static_cast<MenuMain>(i);
                if (voce == MenuMain::RIMUOVI_PUNTEGGIATURA || 
                    voce == MenuMain::RIMUOVI_PARENTESI || voce == MenuMain::RIMUOVI_SPAZI || 
                    voce == MenuMain::RIMUOVI_SPAZI_RIPETUTI || voce == MenuMain::RIMUOVI_NUMERI || 
                    voce == MenuMain::RIMUOVI_SIMBOLI || voce == MenuMain::RIMUOVI_PAROLE || 
                    voce == MenuMain::SOSTITUISCI_MAIUSCOLE || voce == MenuMain::SOSTITUISCI_MINUSCOLE || 
                    voce == MenuMain::SOSTITUISCI_ESTENSIONE) {
                    
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                } else if (voce == MenuMain::SOSTITUISCI_ESTENSIONE) {
                    cout << " =" << get_string("LBL_CONVERTI_IN") << YELLOW << estensioneOutput << RESET << ")";
                }
            }
            else if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli) {
                if (menu.at(i - 1).testo != " " && i != menu.size() && i != menu.size() - 1) {
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                }
            } 
            else if (&menu == &subMenu_regoleCerca || &menu == &subMenu_regoleTarget) {
                // escludiamo gli ultimi 3 elementi (Menu Selezione/Esegui, riga vuota e Torna al menu)
                size_t limiteStato =  menu.size() - 3;

                if (i >= 1 && i <= limiteStato) {
                    cout << " = [" << (menu.at(i - 1).decisione ? GREEN : RED) << menu.at(i - 1).decisione << RESET << "]";
                    cout << ((menu.at(i - 1).decisione) ? " " : "");
                }
                
                // Mostra l'elenco delle estensioni target a destra se definite
                if (&menu == &subMenu_regoleTarget && i == 2 && !estensioniTargetMultiple.empty()) {
                    cout << "  (Estensioni: ";
                    for (size_t idx = 0; idx < estensioniTargetMultiple.size(); ++idx) {
                        cout << YELLOW << estensioniTargetMultiple[idx] << RESET;
                        if (idx < estensioniTargetMultiple.size() - 1) cout << " - ";
                    }
                    cout << ")";
                }
            }

            // Spiegazioni laterali del menu principale
            if (&menu == &mainMenu) {
                MenuMain voce = static_cast<MenuMain>(i);
                int count = 0;
                
                switch (voce) {
                    case MenuMain::REGOLE_TARGET: {
                        int nFiles = conta_file_target();
                        cout << "           = (" << YELLOW << nFiles << RESET << ((nFiles == 1) ? get_string("LBL_FILE_SELEZIONATO") : get_string("LBL_FILE_SELEZIONATI")) << ")";
                        break;
                    }
                    case MenuMain::RIMUOVI_PUNTEGGIATURA:
                        count = modAttivi(menuPunteggiatura);
                        cout << " = (" << YELLOW << count << RESET << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                        break;
                    case MenuMain::RIMUOVI_PARENTESI:
                        count = modAttivi(menuParentesi);
                        cout << " = (" << YELLOW << count << RESET << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                        break;
                    case MenuMain::RIMUOVI_SIMBOLI:
                        count = modAttivi(menuSimboli);
                        cout << " = (" << YELLOW << count << RESET << ((count == 1) ? get_string("LBL_ELEM_ATTIVO") : get_string("LBL_ELEM_ATTIVI"));
                        break;
                    case MenuMain::RIMUOVI_PAROLE:
                        count = paroleDaRimuovere.size();
                        cout << " = (" << YELLOW << count << RESET << ((count == 1) ? get_string("LBL_PAROLA_INSERITA") : get_string("LBL_PAROLE_INSERITE"));
                        break;
                    case MenuMain::SOSTITUISCI_ESTENSIONE:
                        cout << " =" << get_string("LBL_CONVERTI_IN") << YELLOW << estensioneOutput << RESET << ")";
                        break;
                    default:
                        break;
                }
            }

            // Spiegazioni laterali dei sottomenu
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
            if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli || 
                &menu == &subMenu_regoleCerca || &menu == &subMenu_regoleTarget) {
                return menu.size();
            }
        }

        // Il tasto Q è attivo ESCLUSIVAMENTE nei sottomenu Punteggiatura, Parentesi e Simboli
        if (tasto == 'q' || tasto == 'Q') {
            if (&menu == &mainMenu) {
                MenuMain voceSelezionata = static_cast<MenuMain>(pos);
                
                // La Q nel mainMenu funziona ESCLUSIVAMENTE per Punteggiatura, Parentesi e Simboli
                if (voceSelezionata == MenuMain::RIMUOVI_PUNTEGGIATURA || 
                    voceSelezionata == MenuMain::RIMUOVI_PARENTESI || 
                    voceSelezionata == MenuMain::RIMUOVI_SIMBOLI) {
                    
                    int idx = pos - 1;
                    vector<valoriMenu>* targetCorrente = nullptr;
                    if (voceSelezionata == MenuMain::RIMUOVI_PUNTEGGIATURA) targetCorrente = &menuPunteggiatura;
                    else if (voceSelezionata == MenuMain::RIMUOVI_PARENTESI) targetCorrente = &menuParentesi;
                    else if (voceSelezionata == MenuMain::RIMUOVI_SIMBOLI) targetCorrente = &menuSimboli;

                    if (targetCorrente != nullptr) {
                        size_t limiteVoci = targetCorrente->size() - 2; // Esclude riga vuota e "Torna al menu"
                        bool giaTuttiAttivi = true;
                        
                        for (size_t i = 0; i < limiteVoci; i++) {
                            if (targetCorrente->at(i).testo != " " && !targetCorrente->at(i).decisione) {
                                giaTuttiAttivi = false;
                                break;
                            }
                        }
                        
                        bool nuovoStato = !giaTuttiAttivi;
                        for (size_t i = 0; i < limiteVoci; i++) {
                            if (targetCorrente->at(i).testo != " ") {
                                targetCorrente->at(i).decisione = nuovoStato;
                            }
                        }
                        mainMenu.at(idx).decisione = nuovoStato;
                    }
                } 
                else if (voceSelezionata == MenuMain::SOSTITUISCI_ESTENSIONE) {
                    // Attiva o disattiva direttamente con Q
                    mainMenu.at(pos - 1).decisione = !mainMenu.at(pos - 1).decisione;
                    continue;
                }
                // Per tutte le altre voci del mainMenu, la Q viene ignorata completamente
                continue;
            }
            else if (&menu == &menuPunteggiatura || &menu == &menuParentesi || &menu == &menuSimboli) {
                size_t limiteVoci = menu.size() - 2;
                bool giaTuttiAttivi = true;
                
                for (size_t i = 0; i < limiteVoci; i++) {
                    if (menu.at(i).testo != " " && !menu.at(i).decisione) {
                        giaTuttiAttivi = false;
                        break;
                    }
                }
                
                bool nuovoStato = !giaTuttiAttivi;
                
                vector<valoriMenu>* targetCorrente = nullptr;
                int indiceMain = -1;
                if (&menu == &menuPunteggiatura) { 
                    targetCorrente = &menuPunteggiatura; 
                    indiceMain = static_cast<int>(MenuMain::RIMUOVI_PUNTEGGIATURA) - 1; 
                }
                else if (&menu == &menuParentesi) { 
                    targetCorrente = &menuParentesi; 
                    indiceMain = static_cast<int>(MenuMain::RIMUOVI_PARENTESI) - 1; 
                }
                else if (&menu == &menuSimboli) { 
                    targetCorrente = &menuSimboli; 
                    indiceMain = static_cast<int>(MenuMain::RIMUOVI_SIMBOLI) - 1; 
                }

                if (targetCorrente != nullptr) {
                    for (size_t i = 0; i < limiteVoci; i++) {
                        if (targetCorrente->at(i).testo != " ") targetCorrente->at(i).decisione = nuovoStato;
                    }
                    mainMenu.at(indiceMain).decisione = nuovoStato;
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

        if (tasto == 'e' || tasto == 'E') {
            return elements; 
        }

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
    MenuMain opzione = static_cast<MenuMain>(scelta);

    if (opzione == MenuMain::SOSTITUISCI_MAIUSCOLE && mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MAIUSCOLE) - 1).decisione) {
        mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MINUSCOLE) - 1).decisione = false;
    } 
    else if (opzione == MenuMain::SOSTITUISCI_MINUSCOLE && mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MINUSCOLE) - 1).decisione) {
        mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MAIUSCOLE) - 1).decisione = false;
    } 
    else if (opzione == MenuMain::RIMUOVI_SPAZI && mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI) - 1).decisione) {
        mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI_RIPETUTI) - 1).decisione = false;
    } 
    else if (opzione == MenuMain::RIMUOVI_SPAZI_RIPETUTI && mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI_RIPETUTI) - 1).decisione) {
        mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI) - 1).decisione = false;
    }
}

void aggiorna_testi_menu() {
    // Aggiornamento Main Menu
    mainMenu.at(0).testo = get_string("MENU_MOSTRA");
    mainMenu.at(1).testo = get_string("MENU_POSIZIONE");
    mainMenu.at(2).testo = get_string("MENU_TARGET");
    mainMenu.at(3).testo = get_string("MENU_PUNTEGGIATURA");
    mainMenu.at(4).testo = get_string("MENU_PARENTESI");
    mainMenu.at(5).testo = get_string("MENU_SPAZI");
    mainMenu.at(6).testo = get_string("MENU_SPAZI_RIPETUTI");
    mainMenu.at(7).testo = get_string("MENU_NUMERI");
    mainMenu.at(8).testo = get_string("MENU_SIMBOLI");
    mainMenu.at(9).testo = get_string("MENU_PAROLE");
    mainMenu.at(10).testo = get_string("MENU_MAIUSCOLE");
    mainMenu.at(11).testo = get_string("MENU_MINUSCOLE");
    mainMenu.at(12).testo = get_string("MENU_SOS_EXT");
    mainMenu.at(13).testo = get_string("MENU_ESPORTA");
    mainMenu.at(14).testo = get_string("MENU_ESGUI");
    // Indice 15 è la riga vuota " "
    mainMenu.at(16).testo = get_string("MENU_ABOUT");
    mainMenu.at(17).testo = get_string("MENU_LINGUA");
    mainMenu.at(18).testo = get_string("MENU_CHIUDI");

    // Aggiornamento subMenu_regoleCerca (Filtri)
    subMenu_regoleCerca.at(0).testo = get_string("SUB_TUTTO");
    subMenu_regoleCerca.at(1).testo = get_string("SUB_ESTENSIONI");
    subMenu_regoleCerca.at(2).testo = get_string("SUB_SOTTOSTRINGHE");
    subMenu_regoleCerca.at(3).testo = get_string("SUB_CIFRE");
    subMenu_regoleCerca.at(4).testo = get_string("SUB_DATA");
    subMenu_regoleCerca.at(5).testo = get_string("SUB_DIMENSIONE");
    subMenu_regoleCerca.at(6).testo = get_string("SUB_ESEGUI");
    // Indice 7 è la riga vuota " "
    subMenu_regoleCerca.at(8).testo = get_string("SUB_INDIETRO");

    // Aggiornamento subMenu_regoleTarget
    subMenu_regoleTarget.at(0).testo = get_string("SUB_TUTTO");
    subMenu_regoleTarget.at(1).testo = get_string("SUB_ESTENSIONI");
    subMenu_regoleTarget.at(2).testo = get_string("SUB_STRINGHE");
    subMenu_regoleTarget.at(3).testo = get_string("SUB_CON_CIFRE");
    subMenu_regoleTarget.at(4).testo = get_string("SUB_DEF_CIFRE");
    subMenu_regoleTarget.at(5).testo = get_string("SUB_DATA_FILES");
    subMenu_regoleTarget.at(6).testo = get_string("SUB_DIMENSIONE");
    subMenu_regoleTarget.at(7).testo = get_string("SUB_INIZIA_CON");
    subMenu_regoleTarget.at(8).testo = get_string("SUB_FINISCE_CON");
    subMenu_regoleTarget.at(9).testo = get_string("SUB_M_SELEZIONE");
    // Indice 10 è la riga vuota " "
    subMenu_regoleTarget.at(11).testo = get_string("SUB_INDIETRO");

    // Aggiornamento menu_infoEsporta
    menu_infoEsporta.at(0).testo = get_string("SUB_MENU_INFO_EXP_NOME"); // Nome
    menu_infoEsporta.at(1).testo = get_string("SUB_MENU_INFO_EXP_EXT");  // Estensione
    menu_infoEsporta.at(2).testo = get_string("SUB_MENU_INFO_EXP_DIM");  // Dimensione
    menu_infoEsporta.at(3).testo = get_string("SUB_MENU_INFO_EXP_YEAR"); // Anno (Se Disponibile)
    menu_infoEsporta.at(4).testo = get_string("SUB_MENU_INFO_EXP_EXE");  // Esegui
    // Indice 5 è la riga vuota " "
    menu_infoEsporta.at(6).testo = get_string("SUB_MENU_INFO_EXP_BACK"); // Torna Indietro

    // Aggiornamento menu_formatoFile 
    menu_formatoFile.at(0).testo = get_string("SUB_MENU_FORM_FILE_TXT");  // TXT (Testo semplice)
    menu_formatoFile.at(1).testo = get_string("SUB_MENU_FORM_FILE_CSV");  // CSV (Valori separati da virgola)
    menu_formatoFile.at(2).testo = get_string("SUB_MENU_FORM_FILE_JSON"); // JSON (Formato strutturato)
    menu_formatoFile.at(3).testo = get_string("SUB_MENU_FORM_FILE_EXE");  // [Esporta]
    // Indice 4 è la riga vuota " "
    menu_formatoFile.at(5).testo = get_string("SUB_MENU_FORM_FILE_BACK"); // Torna Indietro

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
    // Indice 13 è la riga vuota " "
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



    // Se l'estensione di output è attualmente sullo stato "non impostata", 
    // aggiorniamo la stringa con la traduzione della lingua corrente appena selezionata:
    if (estensioneOutput == "Non Impostata" || estensioneOutput == "Not Set" || estensioneOutput == get_string("EXT_SET_NAME")) {
        estensioneOutput = get_string("EXT_SET_NAME");
    }
}

