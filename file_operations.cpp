#include "file_operations.hpp"

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
    cout << BOLD << "=== DEFINISCI PAROLE DA RIMUOVERE ===" << RESET << endl << endl;
    if (!paroleDaRimuovere.empty()) {
        cout << "Parole attualmente in lista: ";
        for (const auto& p : paroleDaRimuovere) cout << "[" << YELLOW << p << RESET << "] ";
        cout << endl << endl;
    }

    cout << "Inserisci le parole separate da spazio (oppure premi Invio per resettare):" << endl << "> ";
    string linea, parola;
    getline(cin, linea);

    stringstream ss(linea);
    paroleDaRimuovere.clear();
    while (ss >> parola) {
        paroleDaRimuovere.push_back(parola);
    }

    mainMenu.at(9).decisione = !paroleDaRimuovere.empty();
}

void gestisci_menu_estensioni_esporta() {
    system("cls");
    cout << BOLD << "=== DEFINISCI ESTENSIONI DA ESPORTARE ===" << RESET << endl << endl;
    if (!estensioniEsportate.empty()) {
        cout << "Estensioni attualmente in lista: ";
        for (const auto& e : estensioniEsportate) cout << "[" << YELLOW << e << RESET << "] ";
        cout << endl << endl;
    } else {
        cout << "Nessuna estensione impostata (verranno considerate tutte)." << endl << endl;
    }

    cout << "Inserisci le estensioni separate da spazio (oppure premi Invio per resettare):" << endl << "> ";
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
    cout << BOLD << "=== DEFINISCI SOTTOSTRINGHE DA ESPORTARE ===" << RESET << endl << endl;
    if (!sottostringheEsportate.empty()) {
        cout << "Sottostringhe attualmente in lista: ";
        for (const auto& s : sottostringheEsportate) cout << "[" << YELLOW << s << RESET << "] ";
        cout << endl << endl;
    } else {
        cout << "Nessuna sottostringa impostata." << endl << endl;
    }

    cout << "Inserisci le parole chiave separate da spazio (oppure premi Invio per resettare):" << endl << "> ";
    string linea, sottos;
    getline(cin, linea);

    stringstream ss(linea);
    sottostringheEsportate.clear();
    while (ss >> sottos) {
        sottostringheEsportate.push_back(sottos);
    }

    subMenu.at(2).decisione = !sottostringheEsportate.empty();
}

void imposta_estensioni() {
    system("cls");
    cout << BOLD << "=== DEFINISCI ESTENSIONE TARGET ===" << RESET << endl << endl;
    cout << "Estensione attuale: " << YELLOW << estensioneTarget << RESET << endl;
    cout << "Inserisci la nuova estensione (senza punto, es: mp3, pdf, txt, cpp): ";
    cin >> estensioneTarget;
    estensioneOutput = estensioneTarget;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    mainMenu.at(11).decisione = false;
}

void sostituisci_estensioni() {
    system("cls");
    cout << BOLD << "=== DEFINISCI ESTENSIONE OUTPUT ===" << RESET << endl << endl;
    cout << "Estensione attuale: " << YELLOW << estensioneOutput << RESET << endl;
    cout << "Inserisci la nuova estensione (senza punto, es: mp3, pdf, txt, cpp): ";
    cin >> estensioneOutput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    mainMenu.at(11).decisione = (to_lowercase(estensioneOutput) != to_lowercase(estensioneTarget));
}

// --- VISUALIZZAZIONE LISTA FILE ---
void mostra_lista() {
    int pos = 1;
    for (auto& item : subMenu) {
        if (item.testo != "Esegui" && item.testo != "Torna al menu principale" && item.testo != " ") {
            item.decisione = false;
        }
    }
    estensioniEsportate.clear();
    sottostringheEsportate.clear();
    dataEsportate = "";
    dimValoreEsporta = 0.0;

    while (1) {
        system("cls");
        cout << BOLD << "=== MOSTRA LISTA: CONFIGURAZIONE FILTRI ===" << RESET << endl << endl;
        cout << DIM << "Attiva le opzioni con [Invio] e seleziona [Esegui] per visualizzare." << RESET << endl << endl;
        
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
                cout << BOLD << "\n=== DEFINISCI DATA ===" << RESET << endl;
                cout << "Inserisci la data esatta in formato GG/MM/AAAA: ";
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
                    cout << "\nScegli il tipo di confronto temporale:" << endl;
                    dataModalitaEsporta = static_cast<Tempi>(menuScelta(1, 3, "...fino a questa data (<=)", "Data esatta", "Da questa data in poi... (>=)"));
                }
            } else {
                dataEsportate = "";
            }
        }
        else if (sceltaMenu == 6) {
            subMenu.at(5).decisione = !subMenu.at(5).decisione;
            if (subMenu.at(5).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n=== DEFINISCI DIMENSIONE ===" << RESET << endl;
                cout << "Scegli il tipo di confronto:" << endl;
                dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 2, "Maggiore o uguale (>=)", "Minore o uguale (<=)"));
                
                cout << "\nInserisci il valore numerico: ";
                cin >> dimValoreEsporta;
                clearBuffer();

                cout << "\nScegli l'unita di misura:" << endl;
                int sceltaUnita = menuScelta(1, 4, "Byte (B)", "Kilobyte (KB)", "MegaByte (MB)", "GigaByte (GB)");

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
                cout << YELLOW << "    [AVVISO] Nessun filtro attivo! Seleziona almeno un criterio o 'Tutto'.    "   << RESET << endl;
                cout <<           "  ==========================================================================    " << endl;
                PAUSE("  Premere un tasto per continuare . . . ");
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
        cout << RED << "[ERRORE] Impossibile leggere la cartella: " << e.what() << RESET << endl;
    }

    system("cls");
    cout << BOLD << "=== LISTA FILE E CARTELLA CORRENTE ===" << RESET << endl;
    cout << "Percorso: " << YELLOW << fs::current_path().string() << RESET << endl << endl;

    cout << left << setw(60) << "Nome" << "  " 
         << setw(12) << "Peso" << "  " 
         << setw(22) << "Ultima Modifica" << "  " 
         << setw(15) << "Stato" << endl;
    cout << string(113, '-') << endl;

    if (fileTrovati.empty()) {
        cout << YELLOW << left << setw(113) << "  Nessun Risultato trovato con i criteri selezionati." << RESET << endl;
    } else {
        for (const auto& f : fileTrovati) {
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
                 << setw(15) << f.statoStr << endl;
        }
    }

    cout << string(113, '-') << endl;
    cout << "Totale elementi filtrati: " << countFiles << " file, " << countDirs << " cartelle." << endl << endl;
    PAUSE("Premere un tasto per continuare . . . ");
}

// --- ESPORTAZIONE LISTA FILE ---
void esporta_lista() {
    int pos = 1;
    while (1) {
        inizioConfig:
        system("cls");
        cout << BOLD << "=== CONFIGURAZIONE FILTRI ESPORTAZIONE ===" << RESET << endl << endl;
        cout << DIM << "Gestisci i criteri e seleziona [Esegui] per procedere." << RESET << endl << endl;
        
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
                cout << BOLD << "\n=== DEFINISCI DATA ===" << RESET << endl;
                cout << "Inserisci la data esatta in formato GG/MM/AAAA: ";
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
                    cout << "\nScegli il tipo di confronto temporale:" << endl;
                    dataModalitaEsporta = static_cast<Tempi>(menuScelta(1, 3, "Data esatta", "...fino a questa data (<=)", "Da questa data in poi... (>=)"));
                }
            } else {
                dataEsportate = "";
            }
        }
        else if (sceltaMenu == 6) {
            subMenu.at(5).decisione = !subMenu.at(5).decisione;
            if (subMenu.at(5).decisione) {
                subMenu.at(0).decisione = false;
                cout << BOLD << "\n=== DEFINISCI DIMENSIONE ===" << RESET << endl;
                cout << "Scegli il tipo di confronto:" << endl;
                dimTipoEsporta = static_cast<DirezioneDim>(menuScelta(1, 2, "Maggiore o uguale (>=)", "Minore o uguale (<=)"));
                
                cout << "\nInserisci il valore numerico: ";
                cin >> dimValoreEsporta;
                clearBuffer();

                cout << "\nScegli l'unita di misura:" << endl;
                int sceltaUnita = menuScelta(1, 4, "Byte (B)", "Kilobyte (KB)", "MegaByte (MB)", "GigaByte (GB)");

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
                cout << YELLOW << "\n[AVVISO] Nessun filtro attivo! Seleziona almeno un criterio o 'Tutto'." << RESET << endl;
                PAUSE("Premere un tasto per continuare . . . ");
                continue;
            }
            break;
        }
    }

    cout << "\nCome vuoi esportare la lista?" << endl;
    int sceltaFormato = menuScelta(1, 4, "TXT (Testo semplice)", "CSV (Valori separati da virgola)", "JSON (Formato strutturato)", "Annulla");

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

        string rigaFormattata = "";
        if (sceltaFormato == 1) {
            rigaFormattata = nomeCompleto;
        } 
        else if (sceltaFormato == 2) {
            rigaFormattata = "\"" + nomeCompleto + "\"," + estensione + "," + to_string(fs::file_size(entry)) + "B";
        } 
        else if (sceltaFormato == 3) {
            if (!primoJson) rigaFormattata += ",\n";
            rigaFormattata += "  {\n    \"nome\": \"" + nomeCompleto + "\",\n    \"estensione\": \"" + estensione + "\",\n    \"dimensione_bytes\": " + to_string(fs::file_size(entry)) + "\n  }";
            primoJson = false;
        }

        righeTemporanee.push_back(rigaFormattata);
        countScelti++;
    }

    if (countScelti == 0) {
        cout << endl << YELLOW << "[AVVISO]: Nessun file trovato con i criteri selezionati. Esportazione annullata." << RESET << endl << endl;
        PAUSE("Premere un tasto per continuare . . . ");
        goto inizioConfig;
    }

    string nome_file_base = "lista_esportata.txt";
    if (sceltaFormato == 2) nome_file_base = "lista_esportata.csv";
    else if (sceltaFormato == 3) nome_file_base = "lista_esportata.json";

    fs::path percorsoCompletoFile = fs::current_path() / nome_file_base;

    ofstream file(percorsoCompletoFile, ios::trunc);
    if (!file.is_open()) {
        cout << RED << "[ERRORE]: Impossibile creare il file!" << RESET << endl;
        PAUSE("Premere un tasto per continuare . . . ");
        return;
    }

    if (sceltaFormato == 2) file << "NomeFile,Estensione,Dimensione\n";
    else if (sceltaFormato == 3) file << "[\n";

    for (const auto& riga : righeTemporanee) file << riga << "\n";

    if (sceltaFormato == 3) file << "]\n";

    file.close();

    cout << endl << GREEN << "[SUCCESSO] " << RESET << "Esportazione completata!" << endl;
    cout << "File salvato in: " << YELLOW << percorsoCompletoFile.string() << RESET << endl;
    cout << "Elementi esportati: " << BOLD << countScelti << RESET << endl << endl;
    
    PAUSE("Premere un tasto per continuare . . . ");
}

// --- RINOMINA BATCH ---
void esegui_rinomina() {
    if (modAttivi(mainMenu) == 0) {
        cout << endl <<   "  ================================================================  " << endl;
        cout << YELLOW << "    [AVVISO]: Nessun criterio selezionato. Rinomina annullata.  "     << RESET << endl;
        cout <<           "  ================================================================  " << endl;
        PAUSE("  Premere un tasto per continuare . . . ");
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
        cout << YELLOW << "[AVVISO]: Nessun file verrà modificato (nessun file corrisponde ai criteri o alla target extension)." << RESET << endl << endl;
        PAUSE("Premere un tasto per continuare . . . ");
        return;
    }

    cout << BOLD << RED << "=== [ATTENZIONE] CONFERMA MODIFICHE BATCH ===" << RESET << endl;
    cout << "Cartella: " << YELLOW << percorso.string() << RESET << endl;
    cout << "Verranno modificati " << BOLD << GREEN << listaModifiche.size() << RESET << " file in base alle regole attive:" << endl << endl;

    cout << left << setw(45) << "Nome Originale" << " -> " << setw(45) << "Nuovo Nome" << endl;
    cout << string(95, '-') << endl;

    for (const auto& item : listaModifiche) {
        string vNome = item.vecchioNomeCompleto;
        string nNome = item.nuovoNomeCompleto;
        if (vNome.length() > 42) vNome = vNome.substr(0, 39) + "...";
        if (nNome.length() > 42) nNome = nNome.substr(0, 39) + "...";

        cout << left << setw(45) << vNome << " -> " << GREEN << setw(45) << nNome << RESET << endl;
    }

    cout << string(95, '-') << endl << endl;
    cout << BOLD << "Sei sicuro di voler procedere con la rinomina effettiva sui file?" << RESET << endl;

    int sceltaConferma = menuScelta(1, 2, "Sì, applica le modifiche", "No, annulla e torna al menu");

    if (sceltaConferma == 2) {
        cout << endl << YELLOW << "[OPERAZIONE ANNULLATA]: Nessun file è stato modificato." << RESET << endl << endl;
        PAUSE("Premere un tasto per continuare . . . ");
        return;
    }

    system("cls");
    cout << BOLD << "=== ESECUZIONE RINOMINA IN CORSO... ===" << RESET << endl << endl;

    int successi = 0;
    int errori = 0;

    for (const auto& item : listaModifiche) {
        try {
            fs::rename(item.vecchioPercorso, item.nuovoPercorso);
            cout << GREEN << "[OK] " << RESET << item.vecchioNomeCompleto << " -> " << GREEN << item.nuovoNomeCompleto << RESET << endl;
            successi++;
        } catch (const fs::filesystem_error& e) {
            cout << RED << "[ERRORE] " << RESET << item.vecchioNomeCompleto << endl;
            errori++;
        }
    }

    cout << endl << string(60, '=') << endl;
    cout << BOLD << "Operazione completata. File rinominati: " << GREEN << successi << RESET;
    if (errori > 0) cout << ", Errori: " << RED << errori << RESET;
    cout << endl << endl;

    PAUSE("Premere un tasto per continuare . . . ");
}

// --- INFORMATIONS ---
void aboutMe() {
    system("cls");

    cout << CYAN << BOLD;
    cout << R"(
   ______ _                             ________ _
  / ____/| |                           |  ____(_) |          
 | |     | | ___  __ _ _ __   ___ _ __ | |__   _| | ___  ___ 
 | |     | |/ _ \/ _` | '_ \ / _ \ '__||  __| | | |/ _ \/ __|
 | |____ | |  __/ (_| | | | |  __/ |   | |    | | |  __/\__ \
  \_____/|_|\___|\__,_|_| |_|\___|_|   |_|    |_|_|\___||___/
    
    )" << RESET << endl;

    cout << string(60, '=') << endl;
    cout << BOLD << "                CLEANER FILES BATCH TOOL" << RESET << endl;
    cout << string(60, '=') << endl << endl;

    cout << BOLD << "  [ INFORMAZIONI SOFTWARE ]" << RESET << endl;
    cout << "  • Versione:       " << GREEN << "v1.2 (C++20)" << RESET << endl;
    cout << "  • Sviluppatore:   " << YELLOW << "Raizen" << RESET << endl;
    cout << "  • Licenza:        " << DIM << "MIT Open Source" << RESET << endl << endl;

    cout << BOLD << "  [ GUIDA RAPIDA ]" << RESET << endl;
    cout << "  • " << YELLOW << "[Freccia SU / GIÙ]" << RESET << "     Spostati nel menu" << endl;
    cout << "  • " << YELLOW << "[INVIO]" << RESET << "                Attiva / Disattiva regola" << endl;
    cout << "  • " << YELLOW << "[Q]" << RESET << "                    Seleziona / Deseleziona tutto" << endl;
    cout << "  • " << YELLOW << "[E]" << RESET << "                    Esci dal SottoMenu" << endl;
    cout << "  • " << YELLOW << "[Esegui Rinomina]" << RESET << "      Applica tutte le regole TRUE in sequenza" << endl;
    cout << "  • " << YELLOW << "[Esporta lista Files]" << RESET << "  Salva in TXT, CSV o JSON i file attuali" << endl;

    cout << endl << string(60, '-') << endl;
    PAUSE("Premi INVIO per tornare al menu principale...");
}