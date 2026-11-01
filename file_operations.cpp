#include "file_operations.hpp"
#include "localization.hpp"


// --- DEFINIZIONE DELLE VARIABILI DI CONFIGURAZIONE ---
string estensioneOutput = "Non Impostata";
vector<string> paroleDaRimuovere;
vector<string> sottostringheEsportate;
vector<string> estensioniEsportate;
string dataEsportate = "";

Tempi dataModalitaEsporta = Tempi::Oggi;
DirezioneDim dimTipoEsporta = DirezioneDim::Maggiori;
double dimValoreEsporta = 0.0;

vector<string> estensioniTargetMultiple;
vector<string> sottostringheTarget;
vector<string> cifreTarget;
string dataTarget = "";
Tempi dataModalitaTarget = Tempi::Oggi;
DirezioneDim dimTipoTarget = DirezioneDim::Maggiori;
double dimValoreTarget = 0.0;
vector<string> iniziaConTarget;
vector<string> finisceConTarget;

vector<SelezioneFileItem> fileSelezionatiManualmente;

string to_lowercase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return tolower(c); });
    return str;
}

string to_uppercase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return toupper(c); });
    return str;
}

void cambia_path() {
    int pos = 1;
    while(1) {
        system("cls");
        cout << get_string("POS_CURRENT");
        cout << fs::current_path().string() << endl << endl;

        enum decisioni { AVANTI = 1, INDIETRO, DECISIONE, TORNA_AL_MAIN };
        int scelta = menuScelta(pos, 4, get_string("POPUP_AVANTI").c_str(), 
                                    get_string("POPUP_INDIETRO").c_str(), 
                                    get_string("POPUP_DEFINISCI").c_str(), 
                                    get_string("SUB_INDIETRO").c_str()
                                );
        pos = scelta;
        cout << endl;

        switch (scelta) {
            case AVANTI: {
                vector<string> cartelle;
                try {
                    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                        if (entry.is_directory()) {
                            cartelle.push_back(entry.path().filename().string());
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    cout << RED << get_string("ERRORE_ACCESSO") << RESET << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                }

                if (cartelle.empty()) {
                    cout << YELLOW << get_string("AVVISO_NO_CARTELLA") << RESET << endl << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                } else {
                    vector<valoriMenu> menuCartelle;
                    for (const auto& c : cartelle) {
                        menuCartelle.push_back({c, false});
                    }
                    menuCartelle.push_back({get_string("POPUP_ANNULLA"), false});

                    // --- SCORRIMENTO A FINESTRA FISSA (20 RIGHE) PER LE CARTELLE ---
                    size_t indiceInizio = 0;
                    const size_t righeVisibili = 20;
                    int posCartella = 1;
                    int sceltaCartella = -1;

                    system("cls");
                    cout << BOLD << get_string("POS_CURRENT") << fs::current_path().string() << RESET << endl << endl;
                    cout << DIM << "[Usa le Frecce Su/Giu, Invio per selezionare | E = Esci]" << RESET << endl << endl;

                    // Salviamo la posizione corrente del cursore per evitare sfarfallio
                    cout << SAVE_CURSOR;

                    while (true) {
                        cout << RESTORE_CURSOR;

                        size_t fine = min(indiceInizio + righeVisibili, menuCartelle.size());
                        for (size_t i = indiceInizio; i < fine; ++i) {
                            cout << CLEAR_LINE << "\r";
                            bool isSelezionato = (posCartella == (int)(i + 1));
                            cout << (isSelezionato ? "> " GREEN LAMP : "  ");
                            cout << left << setw(60) << menuCartelle[i].testo << RESET << endl;
                        }

                        // Pulisci eventuali righe residue se la pagina ha meno elementi
                        for (size_t i = (fine - indiceInizio); i < righeVisibili; ++i) {
                            cout << CLEAR_LINE << "\r\n";
                        }

                        int tasto1 = 0;
                        #ifdef _WIN32
                        tasto1 = _getch();
                        if (tasto1 == 0 || tasto1 == 224) {
                            int tasto2 = _getch();
                            if (tasto2 == 72) { // Freccia Su
                                if (posCartella > 1) {
                                    posCartella--;
                                    if (posCartella - 1 < (int)indiceInizio && indiceInizio > 0) {
                                        indiceInizio--;
                                    }
                                }
                            } else if (tasto2 == 80) { // Freccia Giù
                                if (posCartella < (int)menuCartelle.size()) {
                                    posCartella++;
                                    if (posCartella - 1 >= (int)(indiceInizio + righeVisibili)) {
                                        indiceInizio++;
                                    }
                                }
                            }
                        } else {
                            tasto1 = tolower(tasto1);
                            if (tasto1 == 'e') {
                                sceltaCartella = menuCartelle.size(); // Corrisponde ad "Annulla"
                                break;
                            } else if (tasto1 == '\r' || tasto1 == '\n') { // Invio
                                sceltaCartella = posCartella;
                                break;
                            }
                        }
                        #endif
                    }

                    // Se l'utente ha scelto una cartella valida (escludendo l'ultima voce "Annulla")
                    if (sceltaCartella > 0 && sceltaCartella <= (int)cartelle.size()) {
                        string cartellaScelta = cartelle[sceltaCartella - 1];
                        try {
                            fs::current_path(fs::current_path() / cartellaScelta);
                        } catch (const fs::filesystem_error& e) {
                            cout << RED << get_string("ERRORE_ACCESSO") << RESET << endl;
                            PAUSE(get_string("PROMPT_CONTINUA"));
                        }
                    }
                }
                break;
            }
            
            case INDIETRO:
                try {
                    fs::current_path(fs::current_path().parent_path());
                } catch (const fs::filesystem_error& e) {
                    cout << RED << get_string("ERR_NO_DIR_BACK") << RESET << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                }
                break;

            case DECISIONE: {
                cout << BOLD << endl << get_string("BANNER_PERCORSO") << RESET << endl;
                cout << get_string("INSERISCI_PERCORSO");
                string nuovoPath;
                getline(cin, nuovoPath);
                if (nuovoPath.empty()) {
                    getline(cin, nuovoPath);
                }

                try {
                    if (fs::exists(nuovoPath) && fs::is_directory(nuovoPath)) {
                        fs::current_path(nuovoPath);
                    } else {
                        cout << RED << get_string("AVVISO_MISS_DIRECTORY") << RESET << endl << endl;
                        PAUSE(get_string("PROMPT_CONTINUA"));
                    }
                } catch (const fs::filesystem_error& e) {
                    cout << RED << get_string("AVVISO_MISS_DIRECTORY") << RESET << endl << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                }
                break;
            }

            case TORNA_AL_MAIN:
                return;
        }
    }
}

void applica_rimuovi_spazi(string& nome) {
    nome.erase(remove(nome.begin(), nome.end(), ' '), nome.end());
}

void applica_rimuovi_spazi_ripetuti(string& nome) {
    string nuovoNome = "";
    bool ultimoSpazio = false;
    for (size_t i = 0; i < nome.length(); ) {
        unsigned char c = nome[i];
        int len = 1;
        if (c >= 0xC0 && c < 0xE0) len = 2;
        else if (c >= 0xE0 && c < 0xF0) len = 3;
        else if (c >= 0xF0) len = 4;

        string carattere = nome.substr(i, len);
        if (carattere == " ") {
            if (!ultimoSpazio) {
                nuovoNome += " ";
                ultimoSpazio = true;
            }
        } else {
            nuovoNome += carattere;
            ultimoSpazio = false;
        }
        i += len;
    }
    nome = nuovoNome;
}

void applica_rimuovi_numeri(string& nome) {
    nome.erase(remove_if(nome.begin(), nome.end(), ::isdigit), nome.end());
}

void applica_minuscole(string& nome) {
    nome = to_lowercase(nome);
}

void applica_maiuscole(string& nome) {
    nome = to_uppercase(nome);
}

void applica_rimuovi_parole(string& nome) {
    for (const string& parola : paroleDaRimuovere) {
        string nome_lower = to_lowercase(nome);
        string parola_lower = to_lowercase(parola);
        size_t pos = nome_lower.find(parola_lower);

        while (pos != string::npos) {
            nome.erase(pos, parola.length());
            nome_lower = to_lowercase(nome);
            pos = nome_lower.find(parola_lower);
        }
    }
}

void applica_rimozione_generica(string& nome, const vector<valoriMenu>& menuSotto, const string& targetChars, bool isSimboliASCII) {
    bool conservaRipetuto = menuSotto.at(menuSotto.size() - 4).decisione;
    bool sostituisciSpazio = menuSotto.at(menuSotto.size() - 3).decisione;

    string nuovoNome = "";
    size_t i = 0;
    while (i < nome.length()) {
        unsigned char c = nome[i];
        int len = 1;
        if (c >= 0xC0 && c < 0xE0) len = 2;
        else if (c >= 0xE0 && c < 0xF0) len = 3;
        else if (c >= 0xF0) len = 4;

        string sottostringa = nome.substr(i, len);
        bool daRimuovere = false;

        if (isSimboliASCII) {
            for (size_t j = 0; j < menuSotto.size() - 4; j++) {
                if (menuSotto.at(j).decisione && !menuSotto.at(j).testo.empty()) {
                    char simboloMenu = menuSotto.at(j).testo.front();
                    if (sottostringa.length() == 1 && sottostringa[0] == simboloMenu) {
                        daRimuovere = true;
                        break;
                    }
                }
            }
        } else {
            if (len == 1) {
                for (size_t j = 0; j < targetChars.length(); j++) {
                    if (menuSotto.at(j).decisione && targetChars[j] == sottostringa[0]) {
                        daRimuovere = true;
                        break;
                    }
                }
            }
        }

        if (!daRimuovere) {
            nuovoNome += sottostringa;
            i += len;
        } else {
            int runCount = 0;
            size_t tempI = i;

            while (tempI < nome.length()) {
                unsigned char cc = nome[tempI];
                int clen = 1;
                if (cc >= 0xC0 && cc < 0xE0) clen = 2;
                else if (cc >= 0xE0 && cc < 0xF0) clen = 3;
                else if (cc >= 0xF0) clen = 4;

                string csottostringa = nome.substr(tempI, clen);
                bool cDaRimuovere = false;

                if (isSimboliASCII) {
                    for (size_t j = 0; j < menuSotto.size() - 4; j++) {
                        if (menuSotto.at(j).decisione && !menuSotto.at(j).testo.empty()) {
                            char simboloMenu = menuSotto.at(j).testo.front();
                            if (csottostringa.length() == 1 && csottostringa[0] == simboloMenu) {
                                cDaRimuovere = true;
                                break;
                            }
                        }
                    }
                } else {
                    if (clen == 1) {
                        for (size_t j = 0; j < targetChars.length(); j++) {
                            if (menuSotto.at(j).decisione && targetChars[j] == csottostringa[0]) {
                                cDaRimuovere = true;
                                break;
                            }
                        }
                    }
                }

                if (cDaRimuovere) {
                    runCount++;
                    tempI += clen;
                } else {
                    break;
                }
            }

            if (sostituisciSpazio && conservaRipetuto) {
                nuovoNome += " ";
            } 
            else if (sostituisciSpazio) {
                for (int k = 0; k < runCount; k++) {
                    nuovoNome += " ";
                }
            } 
            else if (conservaRipetuto) {
                if (runCount > 1) {
                    nuovoNome += sottostringa;
                }
            }

            i = tempI;
        }
    }
    nome = nuovoNome;
}

void applica_rimuovi_punteggiatura(string& nome) {
    const string simboli = ",;.:-_!?'^\"";
    applica_rimozione_generica(nome, menuPunteggiatura, simboli, false);
}

void applica_rimuovi_parentesi(string& nome) {
    const string parentesi = "<>()[]{}";
    applica_rimozione_generica(nome, menuParentesi, parentesi, false);
}

void applica_rimuovi_simboli(string& nome) {
    applica_rimozione_generica(nome, menuSimboli, "", true);
}

void sostituisci_estensioni() {
    system("cls");
    cout << BOLD << RED << "ATTENZIONE: SOSTITUZIONE ESTENSIONE" << RESET << endl << endl;
    cout << YELLOW << "Questa operazione modifichera' l'estensione di tutti i file target selezionati." << RESET << endl;
    cout << "Estensione di output attuale: " << YELLOW << estensioneOutput << RESET << endl << endl;
    cout << "Inserisci la nuova estensione (es. txt, pdf, jpg) o lascia vuoto per disattivare:" << endl << "> ";
    
    string nuovaExt;
    getline(cin, nuovaExt);

    if (!nuovaExt.empty()) {
        if (nuovaExt[0] == '.') nuovaExt.erase(0, 1);
        estensioneOutput = to_lowercase(nuovaExt);
        mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_ESTENSIONE) - 1).decisione = true;
        cout << GREEN << "\nEstensione di output aggiornata con successo a '." << estensioneOutput << "'." << RESET << endl;
    } else {
        estensioneOutput = "Non Impostata";
        mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_ESTENSIONE) - 1).decisione = false;
        cout << YELLOW << "\nSostituzione estensione disattivata (valore reimpostato a 'Non Impostata')." << RESET << endl;
    }
    PAUSE("Premi un tasto per continuare...");
}

void gestisci_menu_parole() {
    system("cls");
    cout << BOLD << get_string("BANNER_DEF_PAROLE") << RESET << endl << endl;
    if (!paroleDaRimuovere.empty()) {
        cout << get_string("MSG_PAROLE_LISTA");
        for (const auto& p : paroleDaRimuovere) cout << "[" << YELLOW << p << RESET << "] ";
        cout << endl << endl;
    }

    cout << get_string("PROMPT_INSERISCI_PAROLE") << endl << "> ";
    string linea, parola;
    getline(cin, linea);

    stringstream ss(linea);
    paroleDaRimuovere.clear();
    while (ss >> parola) {
        paroleDaRimuovere.push_back(parola);
    }

    mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_PAROLE) - 1).decisione = !paroleDaRimuovere.empty();
}

void gestisci_menu_estensioni_esporta() {
    system("cls");
    cout << BOLD << get_string("BANNER_DEF_EXT_ESP") << RESET << endl << endl;
    if (!estensioniEsportate.empty()) {
        cout << get_string("MSG_EXT_ATTUALI");
        for (const auto& e : estensioniEsportate) cout << "[" << YELLOW << e << RESET << "] ";
        cout << endl << endl;
    } else {
        cout << get_string("MSG_NO_EXT") << endl << endl;
    }

    cout << get_string("PROMPT_INSERISCI_EST") << endl << "> ";
    string linea, ext;
    getline(cin, linea);

    stringstream ss(linea);
    estensioniEsportate.clear();
    while (ss >> ext) {
        if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
        estensioniEsportate.push_back(to_lowercase(ext));
    }

    subMenu_regoleCerca.at(1).decisione = !estensioniEsportate.empty();
}

void gestisci_menu_sottostringhe_esporta() {
    system("cls");
    cout << BOLD << get_string("BANNER_DEF_SOTTO_ESP") << RESET << endl << endl;
    if (!sottostringheEsportate.empty()) {
        cout << get_string("MSG_SOTTO_ATTUALI");
        for (const auto& s : sottostringheEsportate) cout << "[" << YELLOW << s << RESET << "] ";
        cout << endl << endl;
    } else {
        cout << get_string("MSG_NO_SOTTO") << endl << endl;
    }

    cout << get_string("PROMPT_INSERISCI_SOTTO") << endl << "> ";
    string linea, sottos;
    getline(cin, linea);

    stringstream ss(linea);
    sottostringheEsportate.clear();
    while (ss >> sottos) {
        sottostringheEsportate.push_back(sottos);
    }

    subMenu_regoleCerca.at(2).decisione = !sottostringheEsportate.empty();
}

void gestisci_menu_estensioni_target() {
    system("cls");
    cout << BOLD << "DEFINISCI ESTENSIONI TARGET" << RESET << endl << endl;
    if (!estensioniTargetMultiple.empty()) {
        cout << "Estensioni attuali: ";
        for (const auto& e : estensioniTargetMultiple) cout << "[" << YELLOW << e << RESET << "] ";
        cout << endl << endl;
    } else {
        cout << "Nessuna estensione specifica definita." << endl << endl;
    }

    cout << "Inserisci le estensioni separate da spazio (es. txt pdf doc):" << endl << "> ";
    string linea, ext;
    getline(cin, linea);
    cout << endl;

    stringstream ss(linea);
    vector<string> nuoveEstensioni;
    while (ss >> ext) {
        if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
        nuoveEstensioni.push_back(to_lowercase(ext));
    }

    if (nuoveEstensioni.empty()) {
        subMenu_regoleTarget.at(1).decisione = false; // CORRETTO (Indice 1)
        return;
    }

    vector<string> estensioniValide;
    for (const auto& e : nuoveEstensioni) {
        bool trovata = false;
        string targetExt = "." + e;
        try {
            for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                if (entry.is_regular_file()) {
                    string fileExt = to_lowercase(entry.path().extension().string());
                    if (fileExt == targetExt) {
                        trovata = true;
                        break;
                    }
                }
            }
        } catch (...) {}

        if (trovata) {
            estensioniValide.push_back(e);
        } else {
            cout << endl << YELLOW << "Attenzione: L'estensione '" << e << "' non e' presente in nessun file nella cartella corrente." << RESET << endl;
        }
    }

    if (!estensioniValide.empty()) {
        estensioniTargetMultiple = estensioniValide;
        subMenu_regoleTarget.at(1).decisione = true; // CORRETTO (Indice 1)
        subMenu_regoleTarget.at(0).decisione = false;
    } else {
        cout << RED << "Nessuna delle estensioni inserite e' presente nella cartella." << RESET << endl;
        subMenu_regoleTarget.at(1).decisione = false; // CORRETTO (Indice 1)
        PAUSE("Premi un tasto per continuare...");
    }
}

void gestisci_menu_sottostringhe_target() {
    system("cls");
    cout << BOLD << "DEFINISCI SOTTOSTRINGHE TARGET" << RESET << endl << endl;
    if (!sottostringheTarget.empty()) {
        cout << "Sottostringhe attuali: ";
        for (const auto& s : sottostringheTarget) cout << "[" << YELLOW << s << RESET << "] ";
        cout << endl << endl;
    }

    cout << "Inserisci le sottostringhe separate da spazio:" << endl << "> ";
    string linea, sottos;
    getline(cin, linea);

    stringstream ss(linea);
    sottostringheTarget.clear();
    while (ss >> sottos) {
        sottostringheTarget.push_back(sottos);
    }

    subMenu_regoleTarget.at(2).decisione = !sottostringheTarget.empty(); // CORRETTO (Indice 2)
    if (!sottostringheTarget.empty()) subMenu_regoleTarget.at(0).decisione = false;
}

void gestisci_menu_cifre_target() {
    system("cls");
    cout << BOLD << "DEFINISCI CIFRE TARGET" << RESET << endl << endl;
    if (!cifreTarget.empty()) {
        cout << "Cifre attuali: ";
        for (const auto& c : cifreTarget) cout << "[" << YELLOW << c << RESET << "] ";
        cout << endl << endl;
    }

    cout << "Inserisci le cifre o i numeri da cercare separati da spazio:" << endl << "> ";
    string linea, cifra;
    getline(cin, linea);

    stringstream ss(linea);
    cifreTarget.clear();
    while (ss >> cifra) {
        cifreTarget.push_back(cifra);
    }

    subMenu_regoleTarget.at(4).decisione = !cifreTarget.empty(); // CORRETTO (Indice 4)
    if (!cifreTarget.empty()) subMenu_regoleTarget.at(0).decisione = false;
}

void gestisci_menu_inizia_con() {
    system("cls");
    cout << BOLD << "INIZIA CON..." << RESET << endl << endl;
    if (!iniziaConTarget.empty()) {
        cout << "Prefissi attuali: ";
        for (const auto& s : iniziaConTarget) cout << "[" << YELLOW << s << RESET << "] ";
        cout << endl << endl;
    }

    cout << "Inserisci i prefissi separati da spazio:" << endl << "> ";
    string linea, pref;
    getline(cin, linea);

    stringstream ss(linea);
    iniziaConTarget.clear();
    while (ss >> pref) {
        iniziaConTarget.push_back(pref);
    }

    subMenu_regoleTarget.at(7).decisione = !iniziaConTarget.empty(); // CORRETTO (Indice 7)
    if (!iniziaConTarget.empty()) subMenu_regoleTarget.at(0).decisione = false;
}

void gestisci_menu_finisce_con() {
    system("cls");
    cout << BOLD << "FINISCE CON..." << RESET << endl << endl;
    if (!finisceConTarget.empty()) {
        cout << "Suffissi attuali: ";
        for (const auto& s : finisceConTarget) cout << "[" << YELLOW << s << RESET << "] ";
        cout << endl << endl;
    }

    cout << "Inserisci i suffissi separati da spazio:" << endl << "> ";
    string linea, suff;
    getline(cin, linea);

    stringstream ss(linea);
    finisceConTarget.clear();
    while (ss >> suff) {
        finisceConTarget.push_back(suff);
    }

    subMenu_regoleTarget.at(8).decisione = !finisceConTarget.empty(); // CORRETTO (Indice 8)
    if (!finisceConTarget.empty()) subMenu_regoleTarget.at(0).decisione = false;
}

void gestisci_menu_selezione_file() {
    fs::path percorso = fs::current_path();

    vector<SelezioneFileItem> nuoviFileTrovati;
    // Se "Tutto" è attivo, i nuovi file di default partono da true
    bool statoDefaultNuovi = subMenu_regoleTarget.at(0).decisione;

    try {
        for (const auto& entry : fs::directory_iterator(percorso)) {
            if (!entry.is_regular_file()) continue;

            string nomeCompleto = entry.path().filename().string();
            string nomeSenzaExt = entry.path().stem().string();
            string estensioneFile = to_lowercase(entry.path().extension().string());
            if (!estensioneFile.empty() && estensioneFile[0] == '.') estensioneFile.erase(0, 1);

            bool incluso = true;

            // Applicazione dei filtri di subMenu_regoleTarget
            if (!subMenu_regoleTarget.at(0).decisione) { // Se "Tutto" non è attivo
                
                // 1. Definisci Estensioni (Indice 1)
                if (subMenu_regoleTarget.at(1).decisione) {
                    bool matchExt = false;
                    for (const auto& e : estensioniTargetMultiple) {
                        if (estensioneFile == e) { matchExt = true; break; }
                    }
                    if (!matchExt) incluso = false;
                }

                // 2. Definisci Sottostringhe (Indice 2)
                if (subMenu_regoleTarget.at(2).decisione && incluso) {
                    bool matchSub = false;
                    string nomeLower = to_lowercase(nomeCompleto);
                    for (const auto& s : sottostringheTarget) {
                        if (nomeLower.find(to_lowercase(s)) != string::npos) {
                            matchSub = true;
                            break;
                        }
                    }
                    if (!matchSub) incluso = false;
                }

                // 3. Con Cifre (Indice 3)
                if (subMenu_regoleTarget.at(3).decisione && incluso) {
                    bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                        return isdigit(c);
                    });
                    if (!haCifre) incluso = false;
                }

                // 4. Definisci Cifre (Indice 4)
                if (subMenu_regoleTarget.at(4).decisione && incluso) {
                    bool matchCifre = false;
                    for (const auto& cif : cifreTarget) {
                        if (nomeCompleto.find(cif) != string::npos) {
                            matchCifre = true;
                            break;
                        }
                    }
                    if (!matchCifre) incluso = false;
                }

                // 5. Data files (Indice 5)
                if (subMenu_regoleTarget.at(5).decisione && incluso) {
                    try {
                        auto ftime = entry.last_write_time();
                        auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                        );
                        time_t cftime = chrono::system_clock::to_time_t(sct);
                        struct tm* timeinfo = localtime(&cftime);
                        if (timeinfo) {
                            struct tm tmFile = *timeinfo;
                            tmFile.tm_hour = 0; tmFile.tm_min = 0; tmFile.tm_sec = 0;
                            time_t tFile = mktime(&tmFile);

                            int g = stoi(dataTarget.substr(0, 2));
                            int m = stoi(dataTarget.substr(3, 2));
                            int a = stoi(dataTarget.substr(6, 4));

                            struct tm tmTarget = {};
                            tmTarget.tm_mday = g;
                            tmTarget.tm_mon = m - 1;
                            tmTarget.tm_year = a - 1900;
                            tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
                            time_t tTarget = mktime(&tmTarget);

                            if (dataModalitaTarget == Tempi::Ieri_Oggi && tFile > tTarget) incluso = false;
                            else if (dataModalitaTarget == Tempi::Oggi && tFile != tTarget) incluso = false;
                            else if (dataModalitaTarget == Tempi::Oggi_Domani && tFile < tTarget) incluso = false;
                        } else {
                            incluso = false;
                        }
                    } catch (...) {
                        incluso = false;
                    }
                }

                // 6. Dimensione (Indice 6)
                if (subMenu_regoleTarget.at(6).decisione && incluso) {
                    uintmax_t sizeBytes = fs::file_size(entry);
                    if (dimTipoTarget == DirezioneDim::Maggiori && sizeBytes < dimValoreTarget) incluso = false;
                    else if (dimTipoTarget == DirezioneDim::Minori && sizeBytes > dimValoreTarget) incluso = false;
                }

                // 7. Inizia con (Indice 7)
                if (subMenu_regoleTarget.at(7).decisione && incluso) {
                    bool matchInizia = false;
                    string nomeLower = to_lowercase(nomeSenzaExt);
                    for (const auto& pref : iniziaConTarget) {
                        string prefLower = to_lowercase(pref);
                        if (nomeLower.rfind(prefLower, 0) == 0) {
                            matchInizia = true;
                            break;
                        }
                    }
                    if (!matchInizia) incluso = false;
                }

                // 8. Finisce con (Indice 8)
                if (subMenu_regoleTarget.at(8).decisione && incluso) {
                    bool matchFinisce = false;
                    string nomeLower = to_lowercase(nomeSenzaExt);
                    for (const auto& suff : finisceConTarget) {
                        string suffLower = to_lowercase(suff);
                        if (nomeLower.length() >= suffLower.length() &&
                            nomeLower.compare(nomeLower.length() - suffLower.length(), suffLower.length(), suffLower) == 0) {
                            matchFinisce = true;
                            break;
                        }
                    }
                    if (!matchFinisce) incluso = false;
                }
            }

            if (incluso) {
                // Manteniamo lo stato scelto dall'utente se il file era già presente nella lista
                bool trovatoPrecedente = false;
                bool statoPrecedente = statoDefaultNuovi;

                for (const auto& vecchioItem : fileSelezionatiManualmente) {
                    if (vecchioItem.nomeCompleto == nomeCompleto) {
                        trovatoPrecedente = true;
                        statoPrecedente = vecchioItem.selezionato;
                        break;
                    }
                }

                nuoviFileTrovati.push_back({nomeCompleto, statoPrecedente});
            }
        }
    } catch (...) {}

    fileSelezionatiManualmente = nuoviFileTrovati;

    if (fileSelezionatiManualmente.empty()) {
        system("cls");
        cout << YELLOW << "Nessun file trovato con i filtri attuali." << RESET << endl << endl;
        PAUSE("Premi un tasto per continuar...");
        return;
    }

    int maxLenNome = 20;
    for (const auto& item : fileSelezionatiManualmente) {
        int realChars = 0;
        size_t idxStr = 0;
        while (idxStr < item.nomeCompleto.length()) {
            unsigned char c = item.nomeCompleto[idxStr];
            int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;
            realChars++;
            idxStr += len;
        }
        if (realChars > maxLenNome) maxLenNome = realChars;
    }

    size_t indiceInizio = 0;
    const size_t righeVisibili = 20;
    int posFile = 1;

    system("cls");
    cout << BOLD << "MENU SELEZIONE FILE MANUALI" << RESET << endl;
    cout << DIM << "Usa le Frecce Su/Giu per scorrere, INVIO per attivare/disattivare, Q = Tutti On/Off, E = Esci" << RESET << endl << endl;
    
    cout << left << setw(maxLenNome + 4) << "Nome File" << "     " << setw(15) << "Stato" << endl;
    cout << string(maxLenNome + 21, '-') << endl;

    cout << SAVE_CURSOR;

    while (true) {
        cout << RESTORE_CURSOR;

        size_t fine = min(indiceInizio + righeVisibili, fileSelezionatiManualmente.size());
        for (size_t i = indiceInizio; i < fine; ++i) {
            cout << CLEAR_LINE << "\r";
            bool isSelezionatoCorsore = (posFile == (int)(i + 1));
            cout << (isSelezionatoCorsore ? "> " GREEN LAMP : "  ");
            
            int byteLen = fileSelezionatiManualmente[i].nomeCompleto.length();
            int realChars = 0;
            size_t idx = 0;
            while (idx < byteLen) {
                unsigned char c = fileSelezionatiManualmente[i].nomeCompleto[idx];
                int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;
                realChars++;
                idx += len;
            }
            int utf8ExtraBytes = byteLen - realChars;
            int larghezzaAdattata = maxLenNome + utf8ExtraBytes;

            char carattere_stampato = '.';
            cout << left << setfill(carattere_stampato);
            cout << setw(larghezzaAdattata + 4) << fileSelezionatiManualmente[i].nomeCompleto;
            cout << setfill(' ');

            if (fileSelezionatiManualmente[i].selezionato) {
                cout << " = " << GREEN << "TRUE" << RESET;
            } else {
                cout << " = " << RED   << "FALSE" << RESET;
            }
            cout << endl;
        }
        
        for (size_t i = (fine - indiceInizio); i < righeVisibili; ++i) {
            cout << CLEAR_LINE << "\r\n";
        }

        cout << string(maxLenNome + 21, '-') << endl;

        int tasto1 = 0;
        #ifdef _WIN32
        tasto1 = _getch();
        if (tasto1 == 0 || tasto1 == 224) {
            int tasto2 = _getch();
            if (tasto2 == 72) { // Freccia Su
                if (posFile > 1) {
                    posFile--;
                    if (posFile - 1 < (int)indiceInizio && indiceInizio > 0) indiceInizio--;
                }
            } else if (tasto2 == 80) { // Freccia Giù
                if (posFile < (int)fileSelezionatiManualmente.size()) {
                    posFile++;
                    if (posFile - 1 >= (int)(indiceInizio + righeVisibili)) indiceInizio++;
                }
            }
        } else {
            tasto1 = tolower(tasto1);
            if (tasto1 == 'e') {
                break; 
            } else if (tasto1 == 'q') { // Tasto Q: Toggle globale di tutti i file
                if (!fileSelezionatiManualmente.empty()) {
                    bool tuttiAttivi = true;
                    for (const auto& item : fileSelezionatiManualmente) {
                        if (!item.selezionato) { tuttiAttivi = false; break; }
                    }
                    bool nuovoStato = !tuttiAttivi; 
                    for (auto& item : fileSelezionatiManualmente) {
                        item.selezionato = nuovoStato;
                    }
                    if (!nuovoStato) {
                        subMenu_regoleTarget.at(0).decisione = false;
                    } else {
                        // Se tutti i file tornano a true, si può sincronizzare con "Tutto"
                        bool filtriAttivi = false;
                        for (size_t i = 1; i <= 8; i++) {
                            if (subMenu_regoleTarget.at(i).decisione) { filtriAttivi = true; break; }
                        }
                        if (!filtriAttivi) subMenu_regoleTarget.at(0).decisione = true;
                    }
                }
            } else if (tasto1 == '\r' || tasto1 == '\n') { // Invio: inverte il singolo file
                if (!fileSelezionatiManualmente.empty()) {
                    fileSelezionatiManualmente[posFile - 1].selezionato = !fileSelezionatiManualmente[posFile - 1].selezionato;
                    
                    // Se disattiviamo anche un solo file, "Tutto" perde di significato e va a false
                    if (!fileSelezionatiManualmente[posFile - 1].selezionato) {
                        subMenu_regoleTarget.at(0).decisione = false;
                    }
                }
            }
        }
        #endif
    }

    // Aggiorniamo la spunta della voce "Menu Selezione" nel sottomenu regole target
    bool almenoUnoSelezionato = false;
    for (const auto& f : fileSelezionatiManualmente) {
        if (f.selezionato) { almenoUnoSelezionato = true; break; }
    }
    subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::MENU_SELEZIONE) - 1).decisione = almenoUnoSelezionato;
}

void gestisci_menu_target() {
    int pos = 1;
    if (modAttivi(subMenu_regoleTarget) == 0) {
        subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::TUTTO) - 1).decisione = true;
    }

    // Assicuriamoci che "Menu Selezione" sia sempre rigorosamente a true
    subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::MENU_SELEZIONE) - 1).decisione = true;

    while (1) {
        system("cls");
        cout << BOLD << "CONFIGURAZIONE REGOLE TARGET" << RESET << endl << endl;
        cout << DIM << "Seleziona i criteri per filtrare i file da rinominare:" << RESET << endl << endl;
        
        int sceltaMenu = menuScelta(pos, subMenu_regoleTarget);
        pos = sceltaMenu;

        if (sceltaMenu == (int)subMenu_regoleTarget.size() || sceltaMenu == static_cast<int>(SubMenu_regoleTarget::TORNA_AL_MENU)) {
            break;
        }
        
        SubMenu_regoleTarget sceltaEnum = static_cast<SubMenu_regoleTarget>(sceltaMenu);

        switch (sceltaEnum) {
            case SubMenu_regoleTarget::TUTTO: {
                bool statoCorrente = subMenu_regoleTarget.at(0).decisione;
                bool nuovoStatoTutto = !statoCorrente;
                subMenu_regoleTarget.at(0).decisione = nuovoStatoTutto;
                
                if (nuovoStatoTutto) {
                    for (size_t i = 1; i < subMenu_regoleTarget.size() - 3; i++) {
                        subMenu_regoleTarget.at(i).decisione = false;
                    }
                    estensioniTargetMultiple.clear();
                    sottostringheTarget.clear();
                    cifreTarget.clear();
                    iniziaConTarget.clear();
                    finisceConTarget.clear();
                    dataTarget = "";
                    dimValoreTarget = 0.0;
                    
                    // Quando si attiva "Tutto", tutti i file in Menu Selezione tornano a true
                    for (auto& item : fileSelezionatiManualmente) {
                        item.selezionato = true;
                    }
                } else { 
                    for (size_t i = 1; i < subMenu_regoleTarget.size() - 3; i++) {
                        subMenu_regoleTarget.at(i).decisione = false;
                    }
                    estensioniTargetMultiple.clear();
                    sottostringheTarget.clear();
                    cifreTarget.clear();
                    iniziaConTarget.clear();
                    finisceConTarget.clear();
                    dataTarget = "";
                    dimValoreTarget = 0.0;
                }
                break;
            }
            case SubMenu_regoleTarget::MENU_SELEZIONE:
                gestisci_menu_selezione_file();
                break;
            case SubMenu_regoleTarget::DEFINISCI_ESTENSIONI:
                gestisci_menu_estensioni_target();
                // Se attivato, portiamo tutti i file a true come richiesto
                for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                break;
            case SubMenu_regoleTarget::DEFINISCI_SOTTOSTRINGHE:
                gestisci_menu_sottostringhe_target();
                for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                break;
            case SubMenu_regoleTarget::CON_CIFRE:
                subMenu_regoleTarget.at(sceltaMenu - 1).decisione = !subMenu_regoleTarget.at(sceltaMenu - 1).decisione;
                if (subMenu_regoleTarget.at(sceltaMenu - 1).decisione) {
                    subMenu_regoleTarget.at(0).decisione = false;
                    for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                }
                break;
            case SubMenu_regoleTarget::DEFINISCI_CIFRE:
                gestisci_menu_cifre_target();
                for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                break;
            case SubMenu_regoleTarget::DATA_FILES: {
                system("cls");
                subMenu_regoleTarget.at(sceltaMenu - 1).decisione = !subMenu_regoleTarget.at(sceltaMenu - 1).decisione;
                if (subMenu_regoleTarget.at(sceltaMenu - 1).decisione) {
                    subMenu_regoleTarget.at(0).decisione = false;
                    cout << BOLD << get_string("BANNER_DEF_DATA") << RESET << endl;
                    cout << get_string("PROMPT_INSERISCI_DATA") << endl << "> ";
                    cin >> dataTarget;
                    bool find = true;
                    for (int i = 0; i < (int)dataTarget.size(); i++) {
                        if (i == 2 || i == 5) continue;
                        if (!isdigit(dataTarget[i])) { find = false; break; }
                    }
                    clearBuffer();
                    
                    if (dataTarget.empty() || to_lowercase(dataTarget) == "stop" || 
                        dataTarget.length() != 10 || dataTarget[2] != '/' || dataTarget[5] != '/' || !find) {
                        subMenu_regoleTarget.at(sceltaMenu - 1).decisione = false;
                        dataTarget = "";
                    } else {
                        for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                        cout << "\n" << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                        dataModalitaTarget = static_cast<Tempi>(menuScelta(1, 3, 
                            get_string("POPUP_DATA_ESATTA").c_str(), 
                            get_string("POPUP_FINO_DATA").c_str(), 
                            get_string("POPUP_DA_DATA").c_str()
                        ));
                    }
                } else {
                    dataTarget = "";
                }
                break;
            }
            case SubMenu_regoleTarget::DIMENSIONE: {
                system("cls");
                int idxDim = static_cast<int>(SubMenu_regoleTarget::DIMENSIONE) - 1;
                subMenu_regoleTarget.at(idxDim).decisione = !subMenu_regoleTarget.at(idxDim).decisione;
                if (subMenu_regoleTarget.at(idxDim).decisione) {
                    subMenu_regoleTarget.at(0).decisione = false;
                    for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                    cout << BOLD << get_string("BANNER_DEF_DIM") << RESET << endl;
                    cout << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                    
                    dimTipoTarget = static_cast<DirezioneDim>(menuScelta(1, 3, 
                        get_string("POPUP_MAGGIORE").c_str(), 
                        get_string("POPUP_COMPRESO").c_str(), 
                        get_string("POPUP_MINORE").c_str()
                    ));
                    
                    double dimMin = 0.0, dimMax = 0.0;
                    if (dimTipoTarget == DirezioneDim::Compresi) {
                        cout << "\nInserisci il valore MINIMO: " << endl << "> ";
                        cin >> dimMin;
                        cout << "Inserisci il valore MASSIMO: " << endl << "> ";
                        cin >> dimMax;
                    } else {
                        cout << "\n" << get_string("PROMPT_VALORE_NUMERICO") << endl << "> ";
                        cin >> dimValoreTarget;
                    }
                    clearBuffer();

                    cout << "\n" << get_string("PROMPT_UNITA_MISURA") << endl;
                    int sceltaUnita = menuScelta(1, 4, 
                        get_string("POPUP_BYTE").c_str(), 
                        get_string("POPUP_KB").c_str(), 
                        get_string("POPUP_MB").c_str(), 
                        get_string("POPUP_GB").c_str()
                    );

                    double moltiplicatore = 1.0;
                    if (sceltaUnita == 2) moltiplicatore = 1024.0;
                    else if (sceltaUnita == 3) moltiplicatore = pow(1024.0, 2);
                    else if (sceltaUnita == 4) moltiplicatore = pow(1024.0, 3);

                    if (dimTipoTarget == DirezioneDim::Compresi) {
                        dimValoreTarget = dimMin * moltiplicatore;
                    } else {
                        dimValoreTarget *= moltiplicatore;
                    }

                    if (dimValoreTarget <= 0) {
                        subMenu_regoleTarget.at(idxDim).decisione = false;
                        dimValoreTarget = 0.0;
                    }
                } else {
                    dimValoreTarget = 0.0;
                }
                break;
            }
            case SubMenu_regoleTarget::INIZIA_CON:
                gestisci_menu_inizia_con();
                for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                break;
            case SubMenu_regoleTarget::FINISCE_CON:
                gestisci_menu_finisce_con();
                for (auto& item : fileSelezionatiManualmente) item.selezionato = true;
                break;
            default:
                break;
        }

        // CONTROLLO AUTOMATICO: se tutti i filtri (da 1 a 8) sono false, "Tutto" torna a true
        bool tuttiFiltriFalse = true;
        for (size_t i = 1; i <= 8; i++) {
            if (subMenu_regoleTarget.at(i).decisione) {
                tuttiFiltriFalse = false;
                break;
            }
        }

        if (tuttiFiltriFalse) {
            subMenu_regoleTarget.at(0).decisione = true;
            // QUESTO DEVE RIMANERE DISATTIVATO
            // for (auto& item : fileSelezionatiManualmente) {
            //     item.selezionato = true;
            // }
        }
        
        // "Menu Selezione" rimane sempre rigorosamente a true
        subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::MENU_SELEZIONE) - 1).decisione = true;
    }

    bool attivoAlmenoUno = false;
    for (size_t i = 1; i < subMenu_regoleTarget.size() - 2; i++) {
        if (subMenu_regoleTarget.at(i).decisione) {
            attivoAlmenoUno = true;
            break;
        }
    }
    mainMenu.at(2).decisione = attivoAlmenoUno;
}

void mostra_lista() {
    int pos = 1;
    subMenu_regoleCerca.at(0).decisione = true; 

    for (size_t i = 1; i < subMenu_regoleCerca.size(); i++) {
        if (subMenu_regoleCerca.at(i).testo != get_string("SUB_ESEGUI") && 
            subMenu_regoleCerca.at(i).testo != get_string("SUB_INDIETRO") && 
            subMenu_regoleCerca.at(i).testo != " ") {
            subMenu_regoleCerca.at(i).decisione = false;
        }
    }

    estensioniEsportate.clear();
    sottostringheEsportate.clear();
    dataEsportate = "";
    dimValoreEsporta = 0.0;

    while (1) {
        system("cls");
        cout << BOLD << get_string("BANNER_CONFIG_FILTRI") << RESET << endl << endl;
        cout << DIM << get_string("HINT_ATTIVA_FILTRI") << RESET << endl << endl;
        
        int sceltaMenu = menuScelta(pos, subMenu_regoleCerca);
        pos = sceltaMenu;

        if (sceltaMenu == (int)subMenu_regoleCerca.size()) {
            return;
        }
        
        SubMenu_regoleCerca sceltaEnum = static_cast<SubMenu_regoleCerca>(sceltaMenu);

        switch (sceltaEnum) {
            case SubMenu_regoleCerca::TUTTO: {
                subMenu_regoleCerca.at(0).decisione = !subMenu_regoleCerca.at(0).decisione;
                if (subMenu_regoleCerca.at(0).decisione) {
                    for (size_t i = 1; i < subMenu_regoleCerca.size() - 2; i++) {
                        subMenu_regoleCerca.at(i).decisione = false;
                    }
                    estensioniEsportate.clear();
                    sottostringheEsportate.clear();
                }
                break;
            }
            case SubMenu_regoleCerca::DEFINISCI_ESTENSIONI:
                gestisci_menu_estensioni_esporta();
                if (!estensioniEsportate.empty()) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::CERCA_SOTTOSTRINGHE:
                gestisci_menu_sottostringhe_esporta();
                if (!sottostringheEsportate.empty()) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::CERCA_CIFRE:
                subMenu_regoleCerca.at(3).decisione = !subMenu_regoleCerca.at(3).decisione;
                if (subMenu_regoleCerca.at(3).decisione) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::DATA_FILES: {
                system("cls");
                subMenu_regoleCerca.at(4).decisione = !subMenu_regoleCerca.at(4).decisione;
                if (subMenu_regoleCerca.at(4).decisione) {
                    subMenu_regoleCerca.at(0).decisione = false;
                    cout << BOLD << get_string("BANNER_DEF_DATA") << RESET << endl;
                    cout << get_string("PROMPT_INSERISCI_DATA") << endl << "> ";
                    cin >> dataEsportate;
                    bool find = true;
                    for (int i = 0; i < (int)dataEsportate.size(); i++) {
                        if (i == 2 || i == 5) continue;
                        if (!isdigit(dataEsportate[i])) { find = false; break; }
                    }
                    clearBuffer();
                    
                    if (dataEsportate.empty() || to_lowercase(dataEsportate) == "stop" || 
                        dataEsportate.length() != 10 || dataEsportate[2] != '/' || dataEsportate[5] != '/' || !find) {
                        subMenu_regoleCerca.at(4).decisione = false;
                    } else {
                        cout << "\n" << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                        dataModalitaEsporta = static_cast<Tempi>(menuScelta(1, 3, 
                            get_string("POPUP_DATA_ESATTA").c_str(), 
                            get_string("POPUP_FINO_DATA").c_str(), 
                            get_string("POPUP_DA_DATA").c_str()
                        ));
                    }
                } else {
                    dataEsportate = "";
                }
                break;
            }
            case SubMenu_regoleCerca::DIMENSIONE: {
                system("cls");
                subMenu_regoleCerca.at(5).decisione = !subMenu_regoleCerca.at(5).decisione;
                if (subMenu_regoleCerca.at(5).decisione) {
                    subMenu_regoleCerca.at(0).decisione = false;
                    cout << BOLD << get_string("BANNER_DEF_DIM") << RESET << endl;
                    cout << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                    dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 3, 
                        get_string("POPUP_MAGGIORE").c_str(), 
                        get_string("POPUP_COMPRESO").c_str(), 
                        get_string("POPUP_MINORE").c_str()
                    ));
                    
                    double dimMin = 0.0, dimMax = 0.0;
                    if (dimTipoEsporta == DirezioneDim::Compresi) {
                        cout << "\nInserisci il valore MINIMO: " << endl << "> ";
                        cin >> dimMin;
                        cout << "Inserisci il valore MASSIMO: " << endl << "> ";
                        cin >> dimMax;
                    } else {
                        cout << "\n" << get_string("PROMPT_VALORE_NUMERICO") << endl << "> ";
                        cin >> dimValoreEsporta;
                    }
                    clearBuffer();

                    cout << "\n" << get_string("PROMPT_UNITA_MISURA") << endl;
                    int sceltaUnita = menuScelta(1, 4, 
                        get_string("POPUP_BYTE").c_str(), 
                        get_string("POPUP_KB").c_str(), 
                        get_string("POPUP_MB").c_str(), 
                        get_string("POPUP_GB").c_str()
                    );

                    double moltiplicatore = 1.0;
                    if (sceltaUnita == 2) moltiplicatore = 1024.0;
                    else if (sceltaUnita == 3) moltiplicatore = pow(1024.0, 2);
                    else if (sceltaUnita == 4) moltiplicatore = pow(1024.0, 3);

                    if (dimTipoEsporta == DirezioneDim::Compresi) {
                        dimValoreEsporta = dimMin * moltiplicatore;
                    } else {
                        dimValoreEsporta *= moltiplicatore;
                    }

                    if (dimValoreEsporta <= 0) subMenu_regoleCerca.at(5).decisione = false;
                } else {
                    dimValoreEsporta = 0.0;
                }
                break;
            }
            case SubMenu_regoleCerca::ESEGUI: {
                bool attivoAlmenoUno = false;
                for (const auto& item : subMenu_regoleCerca) {
                    if (item.decisione) { attivoAlmenoUno = true; break; }
                }
                if (!attivoAlmenoUno) {
                    cout << endl;
                    cout <<           "  ==========================================================================    " << endl;
                    cout << YELLOW << "    " << get_string("AVVISO_NO_FILTRI") << "    "   << RESET << endl;
                    cout <<           "  ==========================================================================    " << endl;
                    PAUSE("  " + get_string("PROMPT_CONTINUA"));
                    continue;
                }
                goto fineConfigCerca;
            }
            default:
                break;
        }
    }
    fineConfigCerca:

    struct InfoFile {
        string nomeVisualizzato;
        string pesoStr;
        string dataStr;
        string statoStr;
        bool isFile;
    };

    vector<InfoFile> fileTrovati;
    int countFiles = 0;
    int countDirs = 0;

    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            string nomeCompleto = entry.path().filename().string();
            string estensioneFile = to_lowercase(entry.path().extension().string());
            if (!estensioneFile.empty() && estensioneFile[0] == '.') estensioneFile.erase(0, 1);

            bool incluso = true;

            if (!subMenu_regoleCerca.at(0).decisione) {
                if (subMenu_regoleCerca.at(1).decisione) {
                    bool matchExt = false;
                    for (const auto& e : estensioniEsportate) {
                        if (estensioneFile == e) { matchExt = true; break; }
                    }
                    if (!matchExt) incluso = false;
                }

                if (subMenu_regoleCerca.at(2).decisione && incluso) {
                    bool matchSub = false;
                    string nomeLower = to_lowercase(nomeCompleto);
                    for (const auto& s : sottostringheEsportate) {
                        if (nomeLower.find(to_lowercase(s)) != string::npos) {
                            matchSub = true;
                            break;
                        }
                    }
                    if (!matchSub) incluso = false;
                }

                if (subMenu_regoleCerca.at(3).decisione && incluso) {
                    bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                        return isdigit(c);
                    });
                    if (!haCifre) incluso = false;
                }

                if (subMenu_regoleCerca.at(4).decisione && incluso) {
                    try {
                        auto ftime = entry.last_write_time();
                        auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                        );
                        time_t cftime = chrono::system_clock::to_time_t(sct);
                        struct tm* timeinfo = localtime(&cftime);
                        
                        if (timeinfo) {
                            struct tm tmFile = *timeinfo;
                            tmFile.tm_hour = 0; tmFile.tm_min = 0; tmFile.tm_sec = 0;
                            time_t tFile = mktime(&tmFile);

                            int g = stoi(dataEsportate.substr(0, 2));
                            int m = stoi(dataEsportate.substr(3, 2));
                            int a = stoi(dataEsportate.substr(6, 4));

                            struct tm tmTarget = {};
                            tmTarget.tm_mday = g;
                            tmTarget.tm_mon = m - 1;
                            tmTarget.tm_year = a - 1900;
                            tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
                            time_t tTarget = mktime(&tmTarget);

                            if (dataModalitaEsporta == Tempi::Ieri_Oggi && tFile > tTarget) incluso = false;
                            else if (dataModalitaEsporta == Tempi::Oggi && tFile != tTarget) incluso = false;
                            else if (dataModalitaEsporta == Tempi::Oggi_Domani && tFile < tTarget) incluso = false;
                        } else {
                            incluso = false;
                        }
                    } catch (...) {
                        incluso = false;
                    }
                }

                if (subMenu_regoleCerca.at(5).decisione && incluso) {
                    if (!entry.is_regular_file()) {
                        incluso = false;
                    } else {
                        uintmax_t sizeBytes = fs::file_size(entry);
                        if (dimTipoEsporta == DirezioneDim::Maggiori && sizeBytes < dimValoreEsporta) incluso = false;
                        else if (dimTipoEsporta == DirezioneDim::Minori && sizeBytes > dimValoreEsporta) incluso = false;
                        else if (dimTipoEsporta == DirezioneDim::Compresi) {
                            // Se implementi max, verifica qui l'intervallo
                        }
                    }
                }
            }

            if (!incluso) continue;

            InfoFile info;
            string nomeVisualizzato = "";
            int charCount = 0;
            size_t i = 0;
            while (i < nomeCompleto.length()) {
                unsigned char c = nomeCompleto[i];
                int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;

                if (charCount >= 57) {
                    nomeVisualizzato += "...";
                    break;
                }

                nomeVisualizzato.append(nomeCompleto, i, len);
                i += len;
                charCount++;
            }
            
            info.nomeVisualizzato = nomeVisualizzato;
            info.pesoStr = "-";
            info.isFile = entry.is_regular_file();
            if (info.isFile) {
                uintmax_t sizeBytes = fs::file_size(entry);
                if (sizeBytes < 1024) info.pesoStr = to_string(sizeBytes) + " B";
                else if (sizeBytes < 1024 * 1024) info.pesoStr = to_string(sizeBytes / 1024) + " KB";
                else info.pesoStr = to_string(sizeBytes / (1024 * 1024)) + " MB";
                countFiles++;
            } else if (entry.is_directory()) {
                info.pesoStr = "<DIR>";
                countDirs++;
            }

            info.dataStr = "N/D";
            try {
                auto ftime = entry.last_write_time();
                auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                );
                time_t cftime = chrono::system_clock::to_time_t(sct);
                char buf[80];
                struct tm* timeinfo = localtime(&cftime);
                if (timeinfo) {
                    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
                    info.dataStr = buf;
                }
            } catch (...) {}

            bool modificabile = true;
            bool nascosto = false;

            try {
                auto perms = entry.status().permissions();
                modificabile = ((perms & fs::perms::owner_write) != fs::perms::none);
            } catch (...) {}

            #ifdef _WIN32
            DWORD attrs = GetFileAttributesA(entry.path().string().c_str());
            if (attrs != INVALID_FILE_ATTRIBUTES) nascosto = (attrs & FILE_ATTRIBUTE_HIDDEN);
            #else
            if (!nomeCompleto.empty() && nomeCompleto[0] == '.') nascosto = true;
            #endif

            info.statoStr = (modificabile ? "Mod." : "Blocc.") + string(nascosto ? " / Nasc." : " / Vis.");
            fileTrovati.push_back(info);
        }
    } catch (const fs::filesystem_error& e) {
        cout << RED << get_string("ERR_DIR") << e.what() << RESET << endl;
    }

    size_t indiceInizio = 0;
    const size_t righeVisibili = 20;

    system("cls");
    cout << BOLD << get_string("BANNER_LISTA_FILE") << RESET << endl;
    cout << get_string("LABEL_PERCORSO") << YELLOW << fs::current_path().string() << RESET << endl << endl;

    cout << left << setw(60) << get_string("COL_NOME") << "  " 
         << setw(12) << get_string("COL_PESO") << "  " 
         << setw(22) << get_string("COL_ULTIMA_MOD") << "  " 
         << setw(15) << get_string("COL_STATO") << endl;
    cout << string(113, '-') << endl;

    while (true) {
        cout << "\033[6H";

        if (fileTrovati.empty()) {
            cout << YELLOW << left << setw(113) << get_string("MSG_NO_RISULTATI") << RESET << endl;
        } else {
            size_t fine = min(indiceInizio + righeVisibili, fileTrovati.size());
            for (size_t i = indiceInizio; i < fine; ++i) {
                const auto& f = fileTrovati[i];
                cout << left << setw(60) << f.nomeVisualizzato << "  " 
                     << setw(12) << f.pesoStr << "  " 
                     << setw(22) << f.dataStr << "  " 
                     << setw(15) << f.statoStr << "\n";
            }

            for (size_t i = (fine - indiceInizio); i < righeVisibili; ++i) {
                cout << string(113, ' ') << "\n";
            }
        }

        cout << string(113, '-') << endl;
        cout << get_string("MSG_TOTALE_ELEMENTI") << countFiles << get_string("MSG_FILE_CARTELLE") << countDirs << get_string("MSG_SOLO_CARTELLE") << "                   \n";
        cout << DIM << "[Usa le Frecce Su/Giu per scorrere | E = Esci]                 " << RESET << endl;

        int tasto1 = 0;
        #ifdef _WIN32
        tasto1 = _getch();
        if (tasto1 == 0 || tasto1 == 224) {
            int tasto2 = _getch();
            if (tasto2 == 72) { 
                if (indiceInizio > 0) indiceInizio--;
            } else if (tasto2 == 80) { 
                if (indiceInizio + righeVisibili < fileTrovati.size()) indiceInizio++;
            }
        } else {
            tasto1 = tolower(tasto1);
            if (tasto1 == 'e') {
                break;
            }
        }
        #endif
    }
}

string formattaDimensione(uintmax_t sizeBytes) {
    if (sizeBytes < 1024) {
        return to_string(sizeBytes) + " B";
    } else if (sizeBytes < 1024ULL * 1024) {
        return to_string(sizeBytes / 1024) + " KB";
    } else if (sizeBytes < 1024ULL * 1024 * 1024) {
        return to_string(sizeBytes / (1024 * 1024)) + " MB";
    } else {
        return to_string(sizeBytes / (1024ULL * 1024 * 1024)) + " GB";
    }
}

void esporta_lista() {
    int pos = 1;

    subMenu_regoleCerca.at(0).decisione = true; 
    for (size_t i = 1; i < subMenu_regoleCerca.size(); i++) {
        if (subMenu_regoleCerca.at(i).testo != get_string("SUB_ESEGUI") && 
            subMenu_regoleCerca.at(i).testo != get_string("SUB_INDIETRO") && 
            subMenu_regoleCerca.at(i).testo != " ") {
            subMenu_regoleCerca.at(i).decisione = false;
        }
    }
    estensioniEsportate.clear();
    sottostringheEsportate.clear();
    dataEsportate = "";
    dimValoreEsporta = 0.0;

    while (1) {
        inizioConfig:
        system("cls");
        cout << BOLD << get_string("BANNER_CONFIG_ESPORTAZIONE") << RESET << endl << endl;
        cout << DIM << get_string("HINT_GESTISCI_CRITERI") << RESET << endl << endl;
        
        int sceltaMenu = menuScelta(pos, subMenu_regoleCerca);
        pos = sceltaMenu;

        if (sceltaMenu == (int)subMenu_regoleCerca.size()) return;
        
        SubMenu_regoleCerca sceltaEnum = static_cast<SubMenu_regoleCerca>(sceltaMenu);

        switch (sceltaEnum) {
            case SubMenu_regoleCerca::TUTTO: {
                subMenu_regoleCerca.at(0).decisione = !subMenu_regoleCerca.at(0).decisione;
                if (subMenu_regoleCerca.at(0).decisione) {
                    for (size_t i = 1; i < subMenu_regoleCerca.size() - 2; i++) subMenu_regoleCerca.at(i).decisione = false;
                    estensioniEsportate.clear();
                    sottostringheEsportate.clear();
                }
                break;
            }
            case SubMenu_regoleCerca::DEFINISCI_ESTENSIONI:
                gestisci_menu_estensioni_esporta();
                if (!estensioniEsportate.empty()) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::CERCA_SOTTOSTRINGHE:
                gestisci_menu_sottostringhe_esporta();
                if (!sottostringheEsportate.empty()) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::CERCA_CIFRE:
                subMenu_regoleCerca.at(3).decisione = !subMenu_regoleCerca.at(3).decisione;
                if (subMenu_regoleCerca.at(3).decisione) subMenu_regoleCerca.at(0).decisione = false;
                break;
            case SubMenu_regoleCerca::DATA_FILES: {
                system("cls");
                subMenu_regoleCerca.at(4).decisione = !subMenu_regoleCerca.at(4).decisione;
                if (subMenu_regoleCerca.at(4).decisione) {
                    subMenu_regoleCerca.at(0).decisione = false;
                    cout << BOLD << get_string("BANNER_DEF_DATA") << RESET << endl;
                    cout << get_string("PROMPT_INSERISCI_DATA") << endl << "> ";
                    cin >> dataEsportate;
                    bool find = true;
                    for (int i = 0; i < (int)dataEsportate.size(); i++) {
                        if (i == 2 || i == 5) continue;
                        if (!isdigit(dataEsportate[i])) { find = false; break; }
                    }
                    clearBuffer();
                    
                    if (dataEsportate.empty() || to_lowercase(dataEsportate) == "basta" || 
                        dataEsportate.length() != 10 || dataEsportate[2] != '/' || dataEsportate[5] != '/' || !find) {
                        subMenu_regoleCerca.at(4).decisione = false;
                    } else {
                        cout << "\n" << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                        dataModalitaEsporta = static_cast<Tempi>(menuScelta(1, 3, 
                            get_string("POPUP_DATA_ESATTA").c_str(), 
                            get_string("POPUP_FINO_DATA").c_str(), 
                            get_string("POPUP_DA_DATA").c_str()
                        ));
                    }
                } else {
                    dataEsportate = "";
                }
                break;
            }
            case SubMenu_regoleCerca::DIMENSIONE: {
                system("cls");
                subMenu_regoleCerca.at(5).decisione = !subMenu_regoleCerca.at(5).decisione;
                if (subMenu_regoleCerca.at(5).decisione) {
                    subMenu_regoleCerca.at(0).decisione = false;
                    cout << BOLD << get_string("BANNER_DEF_DIM") << RESET << endl;
                    cout << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                    dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 3, 
                        get_string("POPUP_MAGGIORE").c_str(), 
                        get_string("POPUP_COMPRESO").c_str(), 
                        get_string("POPUP_MINORE").c_str()
                    ));
                    
                    double dimMin = 0.0, dimMax = 0.0;
                    if (dimTipoEsporta == DirezioneDim::Compresi) {
                        cout << "\nInserisci il valore MINIMO: " << endl << "> ";
                        cin >> dimMin;
                        cout << "Inserisci il valore MASSIMO: " << endl << "> ";
                        cin >> dimMax;
                    } else {
                        cout << "\n" << get_string("PROMPT_VALORE_NUMERICO") << endl << "> ";
                        cin >> dimValoreEsporta;
                    }
                    clearBuffer();

                    cout << "\n" << get_string("PROMPT_UNITA_MISURA") << endl;
                    int sceltaUnita = menuScelta(1, 4, 
                        get_string("POPUP_BYTE").c_str(), 
                        get_string("POPUP_KB").c_str(), 
                        get_string("POPUP_MB").c_str(), 
                        get_string("POPUP_GB").c_str()
                    );

                    double moltiplicatore = 1.0;
                    if (sceltaUnita == 2) moltiplicatore = 1024.0;
                    else if (sceltaUnita == 3) moltiplicatore = pow(1024.0, 2);
                    else if (sceltaUnita == 4) moltiplicatore = pow(1024.0, 3);

                    if (dimTipoEsporta == DirezioneDim::Compresi) {
                        dimValoreEsporta = dimMin * moltiplicatore;
                    } else {
                        dimValoreEsporta *= moltiplicatore;
                    }

                    if (dimValoreEsporta <= 0) subMenu_regoleCerca.at(5).decisione = false;
                } else {
                    dimValoreEsporta = 0.0;
                }
                break;
            }
            case SubMenu_regoleCerca::ESEGUI: {
                bool attivoAlmenoUno = false;
                for (const auto& item : subMenu_regoleCerca) {
                    if (item.decisione) { attivoAlmenoUno = true; break; }
                }
                if (!attivoAlmenoUno) {
                    cout << YELLOW << "\n" << get_string("AVVISO_NO_FILTRI") << RESET << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                    continue;
                }
                goto fineConfigEsporta;
            }
            default:
                break;
        }
    }
    fineConfigEsporta:

    cout << "\n" << get_string("PROMPT_COME_ESPORTARE") << endl;
    int sceltaFormato = menuScelta(1, 4, 
        get_string("POPUP_TXT").c_str(), 
        get_string("POPUP_CSV").c_str(), 
        get_string("POPUP_JSON").c_str(), 
        get_string("POPUP_ANNULLA").c_str()
    );

    if (sceltaFormato == 4) goto inizioConfig;

    vector<string> righeTemporanee;
    int countScelti = 0;
    bool primoJson = true;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (!entry.is_regular_file()) continue;

        string nomeCompleto = entry.path().filename().string();
        string estensione = to_lowercase(entry.path().extension().string());
        if (!estensione.empty() && estensione[0] == '.') estensione.erase(0, 1);

        bool incluso = true;

        if (!subMenu_regoleCerca.at(0).decisione) {
            if (subMenu_regoleCerca.at(1).decisione) {
                bool matchExt = false;
                for (const auto& e : estensioniEsportate) {
                    if (estensione == e) { matchExt = true; break; }
                }
                if (!matchExt) incluso = false;
            }

            if (subMenu_regoleCerca.at(2).decisione && incluso) {
                bool matchSub = false;
                string nomeLower = to_lowercase(nomeCompleto);
                for (const auto& s : sottostringheEsportate) {
                    if (nomeLower.find(to_lowercase(s)) != string::npos) {
                        matchSub = true;
                        break;
                    }
                }
                if (!matchSub) incluso = false;
            }

            if (subMenu_regoleCerca.at(3).decisione && incluso) {
                bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                    return isdigit(c);
                });
                if (!haCifre) incluso = false;
            }

            if (subMenu_regoleCerca.at(4).decisione && incluso) {
                try {
                    auto ftime = entry.last_write_time();
                    auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                        ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                    );
                    time_t cftime = chrono::system_clock::to_time_t(sct);
                    struct tm* timeinfo = localtime(&cftime);
                    
                    if (timeinfo) {
                        struct tm tmFile = *timeinfo;
                        tmFile.tm_hour = 0; tmFile.tm_min = 0; tmFile.tm_sec = 0;
                        time_t tFile = mktime(&tmFile);

                        int g = stoi(dataEsportate.substr(0, 2));
                        int m = stoi(dataEsportate.substr(3, 2));
                        int a = stoi(dataEsportate.substr(6, 4));

                        struct tm tmTarget = {};
                        tmTarget.tm_mday = g;
                        tmTarget.tm_mon = m - 1;
                        tmTarget.tm_year = a - 1900;
                        tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
                        time_t tTarget = mktime(&tmTarget);

                        if (dataModalitaEsporta == Tempi::Ieri_Oggi && tFile > tTarget) incluso = false;
                        else if (dataModalitaEsporta == Tempi::Oggi && tFile != tTarget) incluso = false;
                        else if (dataModalitaEsporta == Tempi::Oggi_Domani && tFile < tTarget) incluso = false;
                    } else {
                        incluso = false;
                    }
                } catch (...) {
                    incluso = false;
                }
            }

            if (subMenu_regoleCerca.at(5).decisione && incluso) {
                uintmax_t sizeBytes = fs::file_size(entry);
                if (dimTipoEsporta == DirezioneDim::Maggiori && sizeBytes < dimValoreEsporta) incluso = false;
                else if (dimTipoEsporta == DirezioneDim::Minori && sizeBytes > dimValoreEsporta) incluso = false;
            }
        }

        if (!incluso) continue;

        string nomeSenzaExt = entry.path().stem().string();
        uintmax_t sizeBytes = fs::file_size(entry);
        string dimensioneDinamica = formattaDimensione(sizeBytes);

        string nomePulito = nomeSenzaExt;
        string annoStr = "";
        
        std::regex regexAnno("(.*?)\\s*[\\(\\[]?\\b(19\\d{2}|20\\d{2})\\b[\\)\\]]?\\s*(.*)");
        std::smatch matchAnno;
        if (std::regex_match(nomeSenzaExt, matchAnno, regexAnno)) {
            string partePrima = matchAnno[1];
            annoStr = matchAnno[2];
            string parteDopo = matchAnno[3];
            
            nomePulito = partePrima;
            if (!partePrima.empty() && !parteDopo.empty()) nomePulito += " ";
            nomePulito += parteDopo;
            
            while (!nomePulito.empty() && nomePulito.front() == ' ') nomePulito.erase(nomePulito.begin());
            while (!nomePulito.empty() && nomePulito.back() == ' ') nomePulito.pop_back();
        }

        string rigaFormattata = "";
        if (sceltaFormato == 1) { 
            rigaFormattata = nomePulito + (annoStr.empty() ? "" : " (" + annoStr + ")");
        } 
        else if (sceltaFormato == 2) { 
            rigaFormattata = "\"" + nomePulito + "\"," + estensione + ",\"" + annoStr + "\"," + dimensioneDinamica;
        } 
        else if (sceltaFormato == 3) { 
            if (!primoJson) rigaFormattata += ",\n";
            rigaFormattata += "  {\n    \"nome\": \"" + nomePulito + "\",\n    \"estensione\": \"" + estensione + "\",\n    \"anno\": \"" + annoStr + "\",\n    \"dimensione\": \"" + dimensioneDinamica + "\"\n  }";
            primoJson = false;
        }

        righeTemporanee.push_back(rigaFormattata);
        countScelti++;
    }

    if (countScelti == 0) {
        cout << endl << YELLOW << get_string("MSG_ESPORTAZIONE_ANNULLATA") << RESET << endl << endl;
        PAUSE(get_string("PROMPT_CONTINUA"));
        goto inizioConfig;
    }

    string nome_file_base = "lista_esportata.txt";
    if (sceltaFormato == 2) nome_file_base = "lista_esportata.csv";
    else if (sceltaFormato == 3) nome_file_base = "lista_esportata.json";

    fs::path percorsoCompletoFile = fs::current_path() / nome_file_base;

    ofstream file(percorsoCompletoFile, ios::trunc);
    if (!file.is_open()) {
        cout << endl << RED << get_string("ERR_MAKE_FILE") << RESET << endl;
        PAUSE(get_string("PROMPT_CONTINUA"));
        return;
    }

    if (sceltaFormato == 2) {
        file << "NomeFile,Estensione,Anno,Dimensione\n";
    } else if (sceltaFormato == 3) {
        file << "[\n";
    }

    for (const auto& riga : righeTemporanee) file << riga << "\n";

    if (sceltaFormato == 3) file << "]\n";

    file.close();

    cout << endl << GREEN << get_string("POP_SCC") << RESET << get_string("MSG_SUCCESSO_ESPORTAZIONE") << endl;
    cout << get_string("MSG_FILE_SALVATO") << YELLOW << percorsoCompletoFile.string() << RESET << endl;
    cout << get_string("MSG_ELEMENTI_ESPORTATI") << BOLD << countScelti << RESET << endl << endl;
    
    PAUSE(get_string("PROMPT_CONTINUA"));
}

void esegui_rinomina() {
    if (modAttivi(mainMenu) == 0) {
        cout << endl <<   "  ================================================================  " << endl;
        cout << YELLOW << "    " << get_string("AVVISO_NO_FILTRI") << "  "     << RESET << endl;
        cout <<           "  ================================================================  " << endl;
        PAUSE("  " + get_string("PROMPT_CONTINUA"));
        return;
    }

    fs::path percorso = fs::current_path();

    struct AnteprimaRinomina {
        fs::path vecchioPercorso;
        fs::path nuovoPercorso;
        string vecchioNomeCompleto;
        string nuovoNomeCompleto;
    };

    vector<AnteprimaRinomina> listaModifiche;

    for (const auto& entry : fs::directory_iterator(percorso)) {
        if (!entry.is_regular_file()) continue;

        string nomeCompleto = entry.path().filename().string();
        string nomeSenzaExt = entry.path().stem().string();
        string estensioneFile = to_lowercase(entry.path().extension().string());
        if (!estensioneFile.empty() && estensioneFile[0] == '.') estensioneFile.erase(0, 1);

        bool incluso = true;

        // Applicazione dei filtri di subMenu_regoleTarget
        if (!subMenu_regoleTarget.at(0).decisione) { // Se "Tutto" non è attivo
            
            // 1. Estensioni multiple
            if (subMenu_regoleTarget.at(2).decisione) {
                bool matchExt = false;
                for (const auto& e : estensioniTargetMultiple) {
                    if (estensioneFile == e) { matchExt = true; break; }
                }
                if (!matchExt) incluso = false;
            }

            // 2. Sottostringhe
            if (subMenu_regoleTarget.at(3).decisione && incluso) {
                bool matchSub = false;
                string nomeLower = to_lowercase(nomeCompleto);
                for (const auto& s : sottostringheTarget) {
                    if (nomeLower.find(to_lowercase(s)) != string::npos) {
                        matchSub = true;
                        break;
                    }
                }
                if (!matchSub) incluso = false;
            }

            // 3. Con Cifre (qualsiasi cifra)
            if (subMenu_regoleTarget.at(4).decisione && incluso) {
                bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                    return isdigit(c);
                });
                if (!haCifre) incluso = false;
            }

            // 4. Definisci Cifre specifiche
            if (subMenu_regoleTarget.at(5).decisione && incluso) {
                bool matchCifre = false;
                for (const auto& cif : cifreTarget) {
                    if (nomeCompleto.find(cif) != string::npos) {
                        matchCifre = true;
                        break;
                    }
                }
                if (!matchCifre) incluso = false;
            }

            // 5. Data files
            if (subMenu_regoleTarget.at(6).decisione && incluso) {
                try {
                    auto ftime = entry.last_write_time();
                    auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                        ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                    );
                    time_t cftime = chrono::system_clock::to_time_t(sct);
                    struct tm* timeinfo = localtime(&cftime);
                    if (timeinfo) {
                        struct tm tmFile = *timeinfo;
                        tmFile.tm_hour = 0; tmFile.tm_min = 0; tmFile.tm_sec = 0;
                        time_t tFile = mktime(&tmFile);

                        int g = stoi(dataTarget.substr(0, 2));
                        int m = stoi(dataTarget.substr(3, 2));
                        int a = stoi(dataTarget.substr(6, 4));

                        struct tm tmTarget = {};
                        tmTarget.tm_mday = g;
                        tmTarget.tm_mon = m - 1;
                        tmTarget.tm_year = a - 1900;
                        tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
                        time_t tTarget = mktime(&tmTarget);

                        if (dataModalitaTarget == Tempi::Ieri_Oggi && tFile > tTarget) incluso = false;
                        else if (dataModalitaTarget == Tempi::Oggi && tFile != tTarget) incluso = false;
                        else if (dataModalitaTarget == Tempi::Oggi_Domani && tFile < tTarget) incluso = false;
                    } else {
                        incluso = false;
                    }
                } catch (...) {
                    incluso = false;
                }
            }

            // 6. Dimensione
            if (subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::DIMENSIONE) - 1).decisione && incluso) {
                uintmax_t sizeBytes = fs::file_size(entry);
                if (dimTipoTarget == DirezioneDim::Maggiori && sizeBytes < dimValoreTarget) incluso = false;
                else if (dimTipoTarget == DirezioneDim::Minori && sizeBytes > dimValoreTarget) incluso = false;
                else if (dimTipoTarget == DirezioneDim::Compresi) {
                    // Se usi un intervallo min-max, escludi se fuori range (supponendo dimValoreTarget = min e dimValoreMaxTarget = max)
                    // if (sizeBytes < dimValoreTarget || sizeBytes > dimValoreMaxTarget) incluso = false;
                }
            }

            // 7. Inizia con
            if (subMenu_regoleTarget.at(8).decisione && incluso) {
                bool matchInizia = false;
                string nomeLower = to_lowercase(nomeSenzaExt);
                for (const auto& pref : iniziaConTarget) {
                    string prefLower = to_lowercase(pref);
                    if (nomeLower.rfind(prefLower, 0) == 0) {
                        matchInizia = true;
                        break;
                    }
                }
                if (!matchInizia) incluso = false;
            }

            // 8. Finisce con
            if (subMenu_regoleTarget.at(9).decisione && incluso) {
                bool matchFinisce = false;
                string nomeLower = to_lowercase(nomeSenzaExt);
                for (const auto& suff : finisceConTarget) {
                    string suffLower = to_lowercase(suff);
                    if (nomeLower.length() >= suffLower.length() &&
                        nomeLower.compare(nomeLower.length() - suffLower.length(), suffLower.length(), suffLower) == 0) {
                        matchFinisce = true;
                        break;
                    }
                }
                if (!matchFinisce) incluso = false;
            }
        }

        // controllo finale per scartare i file impostati su FALSE
        if (subMenu_regoleTarget.at(static_cast<int>(SubMenu_regoleTarget::MENU_SELEZIONE) - 1).decisione && incluso) {
            bool trovatoInSelezione = false;
            for (const auto& itemSel : fileSelezionatiManualmente) {
                if (itemSel.nomeCompleto == nomeCompleto) {
                    if (itemSel.selezionato) trovatoInSelezione = true;
                    break;
                }
            }
            if (!trovatoInSelezione) incluso = false;
        }

        // Se dopo tutti i filtri (incluso il menu manuale) il file è ancora incluso, procediamo
        if (!incluso) continue;

        string nome_originale = nomeSenzaExt;
        string nuovo_nome = nome_originale;

        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_PUNTEGGIATURA) - 1).decisione) applica_rimuovi_punteggiatura(nuovo_nome);
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_PARENTESI) - 1).decisione) applica_rimuovi_parentesi(nuovo_nome);
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI) - 1).decisione) applica_rimuovi_spazi(nuovo_nome);
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SPAZI_RIPETUTI) - 1).decisione) applica_rimuovi_spazi_ripetuti(nuovo_nome);
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_NUMERI) - 1).decisione) applica_rimuovi_numeri(nuovo_nome);
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_SIMBOLI) - 1).decisione) applica_rimuovi_simboli(nuovo_nome);    
        if (mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MAIUSCOLE) - 1).decisione) applica_maiuscole(nuovo_nome);           
        if (mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_MINUSCOLE) - 1).decisione) applica_minuscole(nuovo_nome);           
        if (mainMenu.at(static_cast<int>(MenuMain::RIMUOVI_PAROLE) - 1).decisione && !paroleDaRimuovere.empty()) applica_rimuovi_parole(nuovo_nome);

        if (mainMenu.at(4).decisione) {
            size_t doppioSpazio = nuovo_nome.find("  ");
            while (doppioSpazio != string::npos) {
                nuovo_nome.erase(doppioSpazio, 1);
                doppioSpazio = nuovo_nome.find("  ");
            }
        }
        if (!nuovo_nome.empty() && nuovo_nome.front() == ' ') nuovo_nome.erase(0, 1);
        if (!nuovo_nome.empty() && nuovo_nome.back() == ' ') nuovo_nome.pop_back();

        string nuova_estensione;
        if (mainMenu.at(static_cast<int>(MenuMain::SOSTITUISCI_ESTENSIONE) - 1).decisione) { 
            nuova_estensione = "." + to_lowercase(estensioneOutput);
        } else {
            nuova_estensione = entry.path().extension().string();
        }

        bool nomeCambiato = (!nuovo_nome.empty() && nuovo_nome != nome_originale);
        bool estensioneCambiata = (nuova_estensione != entry.path().extension().string());

        if (nomeCambiato || estensioneCambiata) {
            if (nuovo_nome.empty()) nuovo_nome = nome_originale;

            fs::path nuovo_percorso = entry.path().parent_path() / (nuovo_nome + nuova_estensione);

            AnteprimaRinomina item;
            item.vecchioPercorso = entry.path();
            item.nuovoPercorso = nuovo_percorso;
            item.vecchioNomeCompleto = entry.path().filename().string();
            item.nuovoNomeCompleto = nuovo_percorso.filename().string();

            listaModifiche.push_back(item);
        }
    }

    system("cls");
    if (listaModifiche.empty()) {
        cout << YELLOW << get_string("MSG_NO_MODIFICHE") << RESET << endl << endl;
        PAUSE(get_string("PROMPT_CONTINUA"));
        return;
    }

    cout << BOLD << RED << get_string("BANNER_CONFERMA_BATCH") << RESET << endl;
    cout << get_string("MSG_CARTELLA") << YELLOW << percorso.string() << RESET << endl;
    cout << get_string("MSG_VERRANNO_MODIFICATI") << BOLD << GREEN << listaModifiche.size() << RESET << get_string("MSG_FILE_REGOLE_ATTIVE") << endl << endl;

    cout << left << setw(45) << get_string("COL_NOME_ORIGINALE") << " -> " << setw(45) << get_string("COL_NUOVO_NOME") << endl;
    cout << string(95, '-') << endl;

    for (const auto& item : listaModifiche) {
        string vNome = item.vecchioNomeCompleto;
        string nNome = item.nuovoNomeCompleto;
        if (vNome.length() > 42) vNome = vNome.substr(0, 39) + "...";
        if (nNome.length() > 42) nNome = nNome.substr(0, 39) + "...";

        cout << left << setw(45) << vNome << " -> " << GREEN << setw(45) << nNome << RESET << endl;
    }

    cout << string(95, '-') << endl << endl;
    cout << BOLD << get_string("PROMPT_SICURO_PROCEDERE") << RESET << endl;

    int sceltaConferma = menuScelta(1, 2, 
        get_string("POPUP_SI_APPLICA").c_str(), 
        get_string("POPUP_NO_ANNULLA").c_str()
    );
    
    if (sceltaConferma == 2) {
        cout << endl << YELLOW << get_string("MSG_OPERAZIONE_ANNULLATA") << RESET << endl << endl;
        PAUSE(get_string("PROMPT_CONTINUA"));
        return;
    }

    system("cls");
    cout << BOLD << get_string("BANNER_ESECUZIONE_RINOMINA") << RESET << endl << endl;

    int successi = 0;
    int errori = 0;

    for (const auto& item : listaModifiche) {
        try {
            fs::rename(item.vecchioPercorso, item.nuovoPercorso);
            cout << GREEN << "[OK] " << RESET << item.vecchioNomeCompleto << " -> " << GREEN << item.nuovoNomeCompleto << RESET << endl;
            successi++;
        } catch (const fs::filesystem_error& e) {
            cout << RED << get_string("POP_ERR") << RESET << item.vecchioNomeCompleto << endl;
            errori++;
        }
    }

    cout << endl << string(60, '=') << endl;
    cout << BOLD << get_string("MSG_OPERAZIONE_COMPLETATA") << GREEN << successi << RESET;
    if (errori > 0) cout << get_string("MSG_ERRORI") << RED << errori << RESET;
    cout << endl << endl;

    PAUSE(get_string("PROMPT_CONTINUA"));
}

void aboutMe() {
    system("cls");

    cout << CYAN << BOLD;
    cout << R"(
      _____       ______ _ _          
     |  __ \     |  ____(_) |         
     | |__) |___ | |__   _| | ___  ___ 
     |  _  // _ \|  __| | | |/ _ \/ __|
     | | \ \  __/| |    | | |  __/\__ \
     |_|  \_\___||_|    |_|_|\___||___/
    )" << RESET << endl;

    cout << string(60, '=') << endl;
    cout << BOLD << "                " << get_string("TITLE_ABOUT") << RESET << endl;
    cout << string(60, '=') << endl << endl;

    cout << BOLD << "  " << get_string("SECTION_INFO") << RESET << endl;
    cout << "  " << get_string("INFO_VERSIONE") << GREEN << "v1.2" << RESET << endl;
    cout << "  " << get_string("INFO_SVILUPPATORE") << YELLOW << "Raizen" << RESET << endl;
    cout << "  " << get_string("INFO_LICENZA") << DIM << "MIT Open Source" << RESET << endl << endl;

    cout << BOLD << "  " << get_string("SECTION_GUIDA") << RESET << endl;
    cout << "  " << get_string("GUIDA_1") << endl;
    cout << "  " << get_string("GUIDA_2") << endl;
    cout << "  " << get_string("GUIDA_3") << endl;
    cout << "  " << get_string("GUIDA_4") << endl;
    cout << "  " << get_string("GUIDA_5") << endl;
    cout << "  " << get_string("GUIDA_6") << endl;

    cout << endl << string(60, '-') << endl;
    PAUSE(get_string("PROMPT_ABOUT_EXIT"));
}

int conta_file_target() {
    int count = 0;
    fs::path percorso = fs::current_path();
    try {
        for (const auto& entry : fs::directory_iterator(percorso)) {
            if (!entry.is_regular_file()) continue;

            string nomeCompleto = entry.path().filename().string();
            string nomeSenzaExt = entry.path().stem().string();
            string estensioneFile = to_lowercase(entry.path().extension().string());
            if (!estensioneFile.empty() && estensioneFile[0] == '.') estensioneFile.erase(0, 1);

            bool incluso = true;

            // Applicazione filtri di subMenu_regoleTarget
            if (!subMenu_regoleTarget.at(0).decisione) { // Se "Tutto" non è attivo
                
                // 1. Estensioni multiple
                if (subMenu_regoleTarget.at(1).decisione) {
                    bool matchExt = false;
                    for (const auto& e : estensioniTargetMultiple) {
                        if (estensioneFile == e) { matchExt = true; break; }
                    }
                    if (!matchExt) incluso = false;
                }

                // 2. Sottostringhe
                if (subMenu_regoleTarget.at(2).decisione && incluso) {
                    bool matchSub = false;
                    string nomeLower = to_lowercase(nomeCompleto);
                    for (const auto& s : sottostringheTarget) {
                        if (nomeLower.find(to_lowercase(s)) != string::npos) {
                            matchSub = true;
                            break;
                        }
                    }
                    if (!matchSub) incluso = false;
                }

                // 3. Con Cifre
                if (subMenu_regoleTarget.at(3).decisione && incluso) {
                    bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                        return isdigit(c);
                    });
                    if (!haCifre) incluso = false;
                }

                // 4. Definisci Cifre
                if (subMenu_regoleTarget.at(4).decisione && incluso) {
                    bool matchCifre = false;
                    for (const auto& cif : cifreTarget) {
                        if (nomeCompleto.find(cif) != string::npos) {
                            matchCifre = true;
                            break;
                        }
                    }
                    if (!matchCifre) incluso = false;
                }

                // 5. Data files
                if (subMenu_regoleTarget.at(5).decisione && incluso) {
                    try {
                        auto ftime = entry.last_write_time();
                        auto sct = chrono::time_point_cast<chrono::system_clock::duration>(
                            ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
                        );
                        time_t cftime = chrono::system_clock::to_time_t(sct);
                        struct tm* timeinfo = localtime(&cftime);
                        if (timeinfo) {
                            struct tm tmFile = *timeinfo;
                            tmFile.tm_hour = 0; tmFile.tm_min = 0; tmFile.tm_sec = 0;
                            time_t tFile = mktime(&tmFile);

                            int g = stoi(dataTarget.substr(0, 2));
                            int m = stoi(dataTarget.substr(3, 2));
                            int a = stoi(dataTarget.substr(6, 4));

                            struct tm tmTarget = {};
                            tmTarget.tm_mday = g;
                            tmTarget.tm_mon = m - 1;
                            tmTarget.tm_year = a - 1900;
                            tmTarget.tm_hour = 0; tmTarget.tm_min = 0; tmTarget.tm_sec = 0;
                            time_t tTarget = mktime(&tmTarget);

                            if (dataModalitaTarget == Tempi::Ieri_Oggi && tFile > tTarget) incluso = false;
                            else if (dataModalitaTarget == Tempi::Oggi && tFile != tTarget) incluso = false;
                            else if (dataModalitaTarget == Tempi::Oggi_Domani && tFile < tTarget) incluso = false;
                        } else {
                            incluso = false;
                        }
                    } catch (...) {
                        incluso = false;
                    }
                }

                // 6. Dimensione
                if (subMenu_regoleTarget.at(6).decisione && incluso) {
                    uintmax_t sizeBytes = fs::file_size(entry);
                    if (dimTipoTarget == DirezioneDim::Maggiori && sizeBytes < dimValoreTarget) incluso = false;
                    else if (dimTipoTarget == DirezioneDim::Minori && sizeBytes > dimValoreTarget) incluso = false;
                }

                // 7. Inizia con
                if (subMenu_regoleTarget.at(7).decisione && incluso) {
                    bool matchInizia = false;
                    string nomeLower = to_lowercase(nomeSenzaExt);
                    for (const auto& pref : iniziaConTarget) {
                        string prefLower = to_lowercase(pref);
                        if (nomeLower.rfind(prefLower, 0) == 0) {
                            matchInizia = true;
                            break;
                        }
                    }
                    if (!matchInizia) incluso = false;
                }

                // 8. Finisce con
                if (subMenu_regoleTarget.at(8).decisione && incluso) {
                    bool matchFinisce = false;
                    string nomeLower = to_lowercase(nomeSenzaExt);
                    for (const auto& suff : finisceConTarget) {
                        string suffLower = to_lowercase(suff);
                        if (nomeLower.length() >= suffLower.length() &&
                            nomeLower.compare(nomeLower.length() - suffLower.length(), suffLower.length(), suffLower) == 0) {
                            matchFinisce = true;
                            break;
                        }
                    }
                    if (!matchFinisce) incluso = false;
                }
            }

            // Controllo Menu Selezione manuale
            if (incluso && subMenu_regoleTarget.at(9).decisione) {
                bool trovatoInSelezione = false;
                bool selezionato = true;
                for (const auto& itemSel : fileSelezionatiManualmente) {
                    if (itemSel.nomeCompleto == nomeCompleto) {
                        trovatoInSelezione = true;
                        selezionato = itemSel.selezionato;
                        break;
                    }
                }
                if (trovatoInSelezione && !selezionato) incluso = false;
            }

            if (incluso) count++;
        }
    } catch (...) {}
    return count;
}