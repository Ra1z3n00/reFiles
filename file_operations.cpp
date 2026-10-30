#include "file_operations.hpp"
#include "localization.hpp"


// --- DEFINIZIONE DELLE VARIABILI DI CONFIGURAZIONE ---
string estensioneTarget = "txt";
string estensioneOutput = "txt";
vector<string> paroleDaRimuovere;
vector<string> sottostringheEsportate;
vector<string> estensioniEsportate;
string dataEsportate = "";

Tempi dataModalitaEsporta = Tempi::Oggi;
DirezioneDim dimTipoEsporta = DirezioneDim::Maggiori;
double dimValoreEsporta = 0.0;

// --- TRASFORMAZIONI DI STRINGA ---
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
                    // Gestione di eventuali errori di permessi nella lettura della cartella
                    cout << RED << get_string("ERRORE_ACCESSO") << RESET << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                }

                if (cartelle.empty()) {
                    cout << YELLOW << get_string("AVVISO_NO_CARTELLA") << RESET << endl << endl;
                    PAUSE(get_string("PROMPT_CONTINUA"));
                } else {
                    // Costruiamo dinamicamente il menu delle sottocartelle compatibile con menuScelta
                    vector<valoriMenu> menuCartelle;
                    for (const auto& c : cartelle) {
                        menuCartelle.push_back({c, false});
                    }
                    menuCartelle.push_back({get_string("POPUP_ANNULLA"), false}); // Opzione di sicurezza per uscire

                    int sceltaCartella = menuScelta(1, menuCartelle);

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
                    getline(cin, nuovoPath); // Sicurezza extra per eventuali newline rimasti nel buffer
                }

                try {
                    // Verifica se il percorso inserito esiste ed è effettivamente una directory
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

// --- GESTIONE PAROLE ED ESTENSIONI ---
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

    mainMenu.at(10).decisione = !paroleDaRimuovere.empty();
}

void imposta_estensioni() {
    system("cls");
    cout << BOLD << get_string("BANNER_DEF_TARGET") << RESET << endl << endl;
    cout << get_string("MSG_ESTENSIONE_ATTUALE") << YELLOW << estensioneTarget << RESET << endl;
    cout << get_string("PROMPT_NUOVA_ESTENSIONE");
    cin >> estensioneTarget;
    estensioneOutput = estensioneTarget;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    mainMenu.at(12).decisione = false;
}

void sostituisci_estensioni() {
    system("cls");
    cout << BOLD << get_string("BANNER_DEF_OUTPUT") << RESET << endl << endl;
    cout << get_string("MSG_ESTENSIONE_ATTUALE") << YELLOW << estensioneOutput << RESET << endl;
    cout << get_string("PROMPT_NUOVA_ESTENSIONE");
    cin >> estensioneOutput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    mainMenu.at(12).decisione = (to_lowercase(estensioneOutput) != to_lowercase(estensioneTarget));
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

    subMenu.at(1).decisione = !estensioniEsportate.empty();
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

    subMenu.at(2).decisione = !sottostringheEsportate.empty();
}

// --- VISUALIZZAZIONE LISTA FILE ---
void mostra_lista() {
    int pos = 1;
    // Imposta esplicitamente "Tutto" a true all'apertura del menu
    subMenu.at(0).decisione = true; 

    // Imposta a false tutti gli altri filtri specifici
    for (size_t i = 1; i < subMenu.size(); i++) {
        if (subMenu.at(i).testo != get_string("SUB_ESEGUI") && 
            subMenu.at(i).testo != get_string("SUB_INDIETRO") && 
            subMenu.at(i).testo != " ") {
            subMenu.at(i).decisione = false;
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
        
        int sceltaMenu = menuScelta(pos, subMenu);
        pos = sceltaMenu;

        if (sceltaMenu == (int)subMenu.size()) {
            return;
        }
        
        if (sceltaMenu == 1) {
            subMenu.at(0).decisione = !subMenu.at(0).decisione;
            if (subMenu.at(0).decisione) {
                for (size_t i = 1; i < subMenu.size() - 2; i++) {
                    subMenu.at(i).decisione = false;
                }
                estensioniEsportate.clear();
                sottostringheEsportate.clear();
            }
        }
        else if (sceltaMenu == 2) {
            gestisci_menu_estensioni_esporta();
            if (!estensioniEsportate.empty()) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 3) {
            gestisci_menu_sottostringhe_esporta();
            if (!sottostringheEsportate.empty()) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 4) {
            subMenu.at(3).decisione = !subMenu.at(3).decisione;
            if (subMenu.at(3).decisione) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 5) {
            subMenu.at(4).decisione = !subMenu.at(4).decisione;
            if (subMenu.at(4).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n" << get_string("BANNER_DEF_DATA") << RESET << endl;
                cout << get_string("PROMPT_INSERISCI_DATA");
                cin >> dataEsportate;
                bool find = true;
                for (int i = 0; i < (int)dataEsportate.size(); i++) {
                    if (i == 2 || i == 5) continue;
                    if (!isdigit(dataEsportate[i])) { find = false; break; }
                }
                clearBuffer();
                
                if (dataEsportate.empty() || to_lowercase(dataEsportate) == "stop" || 
                    dataEsportate.length() != 10 || dataEsportate[2] != '/' || dataEsportate[5] != '/' || !find) {
                    subMenu.at(4).decisione = false;
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
        }
        else if (sceltaMenu == 6) {
            subMenu.at(5).decisione = !subMenu.at(5).decisione;
            if (subMenu.at(5).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n" << get_string("BANNER_DEF_DIM") << RESET << endl;
                cout << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 2, 
                    get_string("POPUP_MAGGIORE").c_str(), 
                    get_string("POPUP_MINORE").c_str()
                ));
                
                cout << "\n" << get_string("PROMPT_VALORE_NUMERICO");
                cin >> dimValoreEsporta;
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

                dimValoreEsporta *= moltiplicatore;

                if (dimValoreEsporta <= 0) subMenu.at(5).decisione = false;
            } else {
                dimValoreEsporta = 0.0;
            }
        }
        else if (sceltaMenu == 7) {
            bool attivoAlmenoUno = false;
            for (const auto& item : subMenu) {
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
            break;
        }
    }

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

            if (!subMenu.at(0).decisione) {
                if (subMenu.at(1).decisione) {
                    bool matchExt = false;
                    for (const auto& e : estensioniEsportate) {
                        if (estensioneFile == e) { matchExt = true; break; }
                    }
                    if (!matchExt) incluso = false;
                }

                if (subMenu.at(2).decisione && incluso) {
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

                if (subMenu.at(3).decisione && incluso) {
                    bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                        return isdigit(c);
                    });
                    if (!haCifre) incluso = false;
                }

                if (subMenu.at(4).decisione && incluso) {
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

                if (subMenu.at(5).decisione && incluso) {
                    if (!entry.is_regular_file()) {
                        incluso = false;
                    } else {
                        uintmax_t sizeBytes = fs::file_size(entry);
                        if (dimTipoEsporta == DirezioneDim::Maggiori && sizeBytes < dimValoreEsporta) incluso = false;
                        if (dimTipoEsporta == DirezioneDim::Minori && sizeBytes > dimValoreEsporta) incluso = false;
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
                int len = 1;
                if (c >= 0xC0 && c < 0xE0) len = 2;
                else if (c >= 0xE0 && c < 0xF0) len = 3;
                else if (c >= 0xF0) len = 4;

                if (charCount >= 57) {
                    nomeVisualizzato += "...";
                    break;
                }

                nomeVisualizzato.append(nomeCompleto, i, len);
                i += len;
                charCount++;
            }
            
            int totalRealChars = 0;
            for (size_t idx = 0; idx < nomeCompleto.length(); ) {
                unsigned char c = nomeCompleto[idx];
                int len = 1;
                if (c >= 0xC0 && c < 0xE0) len = 2;
                else if (c >= 0xE0 && c < 0xF0) len = 3;
                else if (c >= 0xF0) len = 4;
                totalRealChars++;
                idx += len;
            }

            info.nomeVisualizzato = (totalRealChars <= 60) ? nomeCompleto : nomeVisualizzato;

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

    // --- VISUALIZZAZIONE FINESTRA FISSA (15 RIGHE) CON GESTIONE FRECCE IN TEMPO REALE ---
    size_t indiceInizio = 0;
    const size_t righeVisibili = 20;

    // Stampa l'intestazione fissa UNA SOLA VOLTA all'inizio
    system("cls");
    cout << BOLD << get_string("BANNER_LISTA_FILE") << RESET << endl;
    cout << get_string("LABEL_PERCORSO") << YELLOW << fs::current_path().string() << RESET << endl << endl;

    cout << left << setw(60) << get_string("COL_NOME") << "  " 
         << setw(12) << get_string("COL_PESO") << "  " 
         << setw(22) << get_string("COL_ULTIMA_MOD") << "  " 
         << setw(15) << get_string("COL_STATO") << endl;
    cout << string(113, '-') << endl;

    while (true) {
        // Riporta il cursore alla riga 7 (subito sotto l'intestazione delle colonne) senza sfarfallio
        cout << "\033[6H";

        if (fileTrovati.empty()) {
            cout << YELLOW << left << setw(113) << get_string("MSG_NO_RISULTATI") << RESET << endl;
        } else {
            size_t fine = min(indiceInizio + righeVisibili, fileTrovati.size());
            for (size_t i = indiceInizio; i < fine; ++i) {
                const auto& f = fileTrovati[i];
                int realCharCount = 0;
                size_t idx = 0;
                while (idx < f.nomeVisualizzato.length()) {
                    unsigned char c = f.nomeVisualizzato[idx];
                    int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;
                    realCharCount++;
                    idx += len;
                }

                int byteLength = f.nomeVisualizzato.length();
                int utf8ExtraBytes = byteLength - realCharCount;
                int larghezzaGuida = 60 + utf8ExtraBytes;

                cout << left << setw(larghezzaGuida) << f.nomeVisualizzato << "  " 
                     << setw(12) << f.pesoStr << "  " 
                     << setw(22) << f.dataStr << "  " 
                     << setw(15) << f.statoStr << "\n";
            }

            // Pulisci eventuali righe residue in basso se la pagina ha meno di 15 elementi
            for (size_t i = (fine - indiceInizio); i < righeVisibili; ++i) {
                cout << string(113, ' ') << "\n";
            }
        }

        // Stampa il footer aggiornato sotto la griglia dei file
        cout << string(113, '-') << endl;
        cout << get_string("MSG_TOTALE_ELEMENTI") << countFiles << get_string("MSG_FILE_CARTELLE") << countDirs << get_string("MSG_SOLO_CARTELLE") << "                   \n";
        cout << DIM << "[Usa le Frecce Su/Giu per scorrere | E = Esci]                 " << RESET << endl;

        // Gestione della pressione dei tasti in tempo reale senza Invio
        int tasto1 = 0;
        #ifdef _WIN32
        tasto1 = _getch();
        if (tasto1 == 0 || tasto1 == 224) {
            int tasto2 = _getch();
            if (tasto2 == 72) { // Freccia Su
                if (indiceInizio > 0) indiceInizio--;
            } else if (tasto2 == 80) { // Freccia Giù
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

// --- ESPORTAZIONE LISTA FILE ---
void esporta_lista() {
    int pos = 1;
    while (1) {
        inizioConfig:
        system("cls");
        cout << BOLD << get_string("BANNER_CONFIG_ESPORTAZIONE") << RESET << endl << endl;
        cout << DIM << get_string("HINT_GESTISCI_CRITERI") << RESET << endl << endl;
        
        int sceltaMenu = menuScelta(pos, subMenu);
        pos = sceltaMenu;

        if (sceltaMenu == (int)subMenu.size()) return;
        
        if (sceltaMenu == 1) {
            subMenu.at(0).decisione = !subMenu.at(0).decisione;
            if (subMenu.at(0).decisione) {
                for (size_t i = 1; i < subMenu.size() - 2; i++) subMenu.at(i).decisione = false;
                estensioniEsportate.clear();
                sottostringheEsportate.clear();
            }
        }
        else if (sceltaMenu == 2) {
            gestisci_menu_estensioni_esporta();
            if (!estensioniEsportate.empty()) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 3) {
            gestisci_menu_sottostringhe_esporta();
            if (!sottostringheEsportate.empty()) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 4) {
            subMenu.at(3).decisione = !subMenu.at(3).decisione;
            if (subMenu.at(3).decisione) subMenu.at(0).decisione = false;
        }
        else if (sceltaMenu == 5) {
            subMenu.at(4).decisione = !subMenu.at(4).decisione;
            if (subMenu.at(4).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n" << get_string("BANNER_DEF_DATA") << RESET << endl;
                cout << get_string("PROMPT_INSERISCI_DATA");
                cin >> dataEsportate;
                bool find = true;
                for (int i = 0; i < (int)dataEsportate.size(); i++) {
                    if (i == 2 || i == 5) continue;
                    if (!isdigit(dataEsportate[i])) { find = false; break; }
                }
                clearBuffer();
                
                if (dataEsportate.empty() || to_lowercase(dataEsportate) == "basta" || 
                    dataEsportate.length() != 10 || dataEsportate[2] != '/' || dataEsportate[5] != '/' || !find) {
                    subMenu.at(4).decisione = false;
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
        }
        else if (sceltaMenu == 6) {
            subMenu.at(5).decisione = !subMenu.at(5).decisione;
            if (subMenu.at(5).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n" << get_string("BANNER_DEF_DIM") << RESET << endl;
                cout << get_string("PROMPT_SCELGI_CONFRONTO") << endl;
                dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 2, 
                    get_string("POPUP_MAGGIORE").c_str(), 
                    get_string("POPUP_MINORE").c_str()
                ));
                
                cout << "\n" << get_string("PROMPT_VALORE_NUMERICO");
                cin >> dimValoreEsporta;
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

                dimValoreEsporta *= moltiplicatore;
                if (dimValoreEsporta <= 0) subMenu.at(5).decisione = false;
            } else {
                dimValoreEsporta = 0.0;
            }
        }
        else if (sceltaMenu == 7) {
            bool attivoAlmenoUno = false;
            for (const auto& item : subMenu) {
                if (item.decisione) { attivoAlmenoUno = true; break; }
            }
            if (!attivoAlmenoUno) {
                cout << YELLOW << "\n" << get_string("AVVISO_NO_FILTRI") << RESET << endl;
                PAUSE(get_string("PROMPT_CONTINUA"));
                continue;
            }
            break;
        }
    }

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

        if (!subMenu.at(0).decisione) {
            if (subMenu.at(1).decisione) {
                bool matchExt = false;
                for (const auto& e : estensioniEsportate) {
                    if (estensione == e) { matchExt = true; break; }
                }
                if (!matchExt) incluso = false;
            }

            if (subMenu.at(2).decisione && incluso) {
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

            if (subMenu.at(3).decisione && incluso) {
                bool haCifre = any_of(nomeCompleto.begin(), nomeCompleto.end(), [](unsigned char c){
                    return isdigit(c);
                });
                if (!haCifre) incluso = false;
            }

            if (subMenu.at(4).decisione && incluso) {
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

            if (subMenu.at(5).decisione && incluso) {
                uintmax_t sizeBytes = fs::file_size(entry);
                if (dimTipoEsporta == DirezioneDim::Maggiori && sizeBytes < dimValoreEsporta) incluso = false;
                if (dimTipoEsporta == DirezioneDim::Minori && sizeBytes > dimValoreEsporta) incluso = false;
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

    string nome_file_base = "lista_esportata.txt"; // TODO devo tradurre anche l'output?
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
        file << "NomeFile,Estensione,Anno,Dimensione\n"; // TODO devo tradurre anche l'output?
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

// --- RINOMINA BATCH ---
void esegui_rinomina() {
    if (modAttivi(mainMenu) == 0) {
        cout << endl <<   "  ================================================================  " << endl;
        cout << YELLOW << "    " << get_string("AVVISO_NO_FILTRI") << "  "     << RESET << endl;
        cout <<           "  ================================================================  " << endl;
        PAUSE("  " + get_string("PROMPT_CONTINUA"));
        return;
    }

    fs::path percorso = fs::current_path();
    string targetExt = "." + to_lowercase(estensioneTarget);

    struct AnteprimaRinomina {
        fs::path vecchioPercorso;
        fs::path nuovoPercorso;
        string vecchioNomeCompleto;
        string nuovoNomeCompleto;
    };

    vector<AnteprimaRinomina> listaModifiche;

    for (const auto& entry : fs::directory_iterator(percorso)) {
        if (entry.is_regular_file() && to_lowercase(entry.path().extension().string()) == targetExt) {
            
            string nome_originale = entry.path().stem().string();
            string nuovo_nome = nome_originale;

            if (mainMenu.at(1).decisione) applica_rimuovi_punteggiatura(nuovo_nome);
            if (mainMenu.at(2).decisione) applica_rimuovi_parentesi(nuovo_nome);
            if (mainMenu.at(3).decisione) applica_rimuovi_spazi(nuovo_nome);
            if (mainMenu.at(4).decisione) applica_rimuovi_spazi_ripetuti(nuovo_nome);
            if (mainMenu.at(5).decisione) applica_rimuovi_numeri(nuovo_nome);
            if (mainMenu.at(6).decisione) applica_rimuovi_simboli(nuovo_nome);    
            if (mainMenu.at(7).decisione) applica_maiuscole(nuovo_nome);           
            if (mainMenu.at(8).decisione) applica_minuscole(nuovo_nome);           
            if (mainMenu.at(9).decisione && !paroleDaRimuovere.empty()) applica_rimuovi_parole(nuovo_nome);

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
            if (mainMenu.at(11).decisione) { 
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

// --- INFORMATIONS ---
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
    cout << "  " << get_string("INFO_VERSIONE") << GREEN << "v1.1.2" << RESET << endl;
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