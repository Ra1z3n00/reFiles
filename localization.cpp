#include "localization.hpp"
#include <unordered_map>

static Lingua linguaAttiva = Lingua::Italiano;

static const std::unordered_map<std::string, std::string> dizionarioIT = {
    // Menu Principale
    {"MENU_MOSTRA", "Mostra Lista Files"},
    {"MENU_POSIZIONE", "Cambia Posizione"},
    {"MENU_TARGET", "Definisci il Target"},
    {"MENU_PUNTEGGIATURA", "Rimuovi Punteggiatura"},
    {"MENU_PARENTESI", "Rimuovi Parentesi"},
    {"MENU_SPAZI", "Rimuovi Spazi"},
    {"MENU_SPAZI_RIPETUTI", "Rimuovi Spazi Ripetuti"},
    {"MENU_NUMERI", "Rimuovi Numeri"},
    {"MENU_SIMBOLI", "Rimuovi Simboli"},
    {"MENU_MAIUSCOLE", "Sostituisci con Maiuscole"},
    {"MENU_MINUSCOLE", "Sostituisci con Minuscole"},
    {"MENU_PAROLE", "Rimuovi Parole"},
    {"MENU_DEF_EXT", "Definisci estensioni"},
    {"MENU_SOS_EXT", "Sostituisci estensione"},
    {"MENU_ESPORTA", "Esporta lista file"},
    {"MENU_ESGUI", "[ ESEGUI RINOMINA ]"},
    {"MENU_ABOUT", "About Me"},
    {"MENU_LINGUA", "Lingua: Italiano"},
    {"MENU_CHIUDI", "Chiudi"},

    // Sottomenu Comuni e Filtri
    {"SUB_TUTTO", "Tutto"},
    {"SUB_ESTENSIONI", "Estensioni Specifiche"},
    {"SUB_SOTTOSTRINGHE", "Cerca Sottostringhe"},
    {"SUB_CIFRE", "Cerca Cifre"},
    {"SUB_DATA", "Data specifica (GG/MM/AAAA)"},
    {"SUB_DIMENSIONE", "Dimensione"},
    {"SUB_ESEGUI", "Esegui"},
    {"SUB_INDIETRO", "Torna al menu principale"},
    {"SUB_STRINGHE","Definisci Sottostringhe"},
    {"SUB_CON_CIFRE","Con Cifre"},
    {"SUB_DEF_CIFRE","Definisci Cifre"},
    {"SUB_DATA_FILES","Data files (GG/MM/AAAA)"},
    {"SUB_INIZIA_CON","Inizia con"},
    {"SUB_FINISCE_CON","Finisce con"},
    {"SUB_M_SELEZIONE","Menu Selezione"},
    
    // menu infoEsporta
    {"SUB_MENU_INFO_EXP_NOME","Nome"},
    {"SUB_MENU_INFO_EXP_EXT","Estensione"},
    {"SUB_MENU_INFO_EXP_DIM","Dimensione"},
    {"SUB_MENU_INFO_EXP_YEAR","Anno (Se Disponibile)"},
    {"SUB_MENU_INFO_EXP_EXE","Esegui"},
    {"SUB_MENU_INFO_EXP_BACK","Torna Indietro"},
    
    // menu formatoFile
    {"SUB_MENU_FORM_FILE_TXT","TXT (Testo semplice)"},
    {"SUB_MENU_FORM_FILE_CSV","CSV (Valori separati da virgola)"},
    {"SUB_MENU_FORM_FILE_JSON","JSON (Formato strutturato)"},
    {"SUB_MENU_FORM_FILE_EXE","[Esporta]"},
    {"SUB_MENU_FORM_FILE_BACK","Torna Indietro"},

    // Opzioni Comuni Sottomenu Rimozione
    {"OP_CONSERVA_RIPETUTO", "Conserva un Ripetuto"},
    {"OP_SOSTITUISCI_SPAZIO", "Sostituisci con Spazio"},

    // Menu Punteggiatura
    {"PUNT_VIRGOLA", ",  (Virgola)"},
    {"PUNT_PUNTO_VIRGOLA", ";  (Punto e virgola)"},
    {"PUNT_PUNTO", ".  (Punto)"},
    {"PUNT_DUE_PUNTI", ":  (Due punti)"},
    {"PUNT_TRATTINO", "-  (Trattino)"},
    {"PUNT_UNDERSCORE", "_  (Underscore)"},
    {"PUNT_ESCLAMATIVO", "!  (Punto esclamativo)"},
    {"PUNT_INTERROGATIVO", "?  (Punto interrogativo)"},
    {"PUNT_APOSTROFO", "'  (Apostrofo)"},
    {"PUNT_CIRCONFLESSO", "^  (Accento circonflesso)"},
    {"PUNT_VIRGOLETTE", "\"  (Virgolette)"},

    // Menu Parentesi
    {"PAR_ANG_SX", "< (Angolare sinistra)"},
    {"PAR_ANG_DX", "> (Angolare destra)"},
    {"PAR_ROT_SX", "( (Rotonda sinistra)"},
    {"PAR_ROT_DX", ") (Rotonda destra)"},
    {"PAR_QUAD_SX", "[ (Quadrata sinistra)"},
    {"PAR_QUAD_DX", "] (Quadrata destra)"},
    {"PAR_GRAF_SX", "{ (Graffa sinistra)"},
    {"PAR_GRAF_DX", "} (Graffa destra)"},

    // Menu Simboli
    {"SIMB_TILDE", "[~] (Tilde)"},
    {"SIMB_BACKSLASH", "[\\] (Barra rovesciata)"},
    {"SIMB_STERLINA", "[£] (Sterlina)"},
    {"SIMB_DOLLARO", "[$] (Dollaro)"},
    {"SIMB_PERCENTUALE", "[%] (Percentuale)"},
    {"SIMB_AMPERSAND", "[&] (E commerciale)"},
    {"SIMB_DIVISO", "[/] (Diviso)"},
    {"SIMB_UGUALE", "[=] (Uguale)"},
    {"SIMB_SOMMA", "[+] (Somma)"},
    {"SIMB_MOLTIPLICAZIONE", "[*] (Moltiplicazione)"},
    {"SIMB_E_ACC_GRAVE", "[è] (E accentata) "},
    {"SIMB_E_ACC_ACUTA", "[é] (E acuta) "},
    {"SIMB_CHIOCCIOLA", "[@] (Chiocciola)"},
    {"SIMB_C_CEDIGLIA", "[ç] (C cediglia) "},
    {"SIMB_O_ACC_GRAVE", "[ò] (O accentata) "},
    {"SIMB_A_ACC_GRAVE", "[à] (A accentata) "},
    {"SIMB_GRADO", "[°] (Grado)"},
    {"SIMB_CANCELLETTO", "[#] (Cancelletto)"},
    {"SIMB_U_ACC_GRAVE", "[ù] (U accentata) "},
    {"SIMB_PARAGRAFO", "[§] (Paragrafo)"},
    {"SIMB_I_ACC_GRAVE", "[ì] (I accentata) "},
    {"SIMB_BARRA_VERT", "[|] (Barra verticale)"},

    // Banner Sottomenu
    {"BANNER_PUNTEGGIATURA", "=== CONFIGURAZIONE PUNTEGGIATURA ==="},
    {"BANNER_PARENTESI", "=== CONFIGURAZIONE PARENTESI ==="},
    {"BANNER_SIMBOLI", "=== CONFIGURAZIONE SIMBOLI ==="},
    {"BANNER_PERCORSO","=== DEFINISCI PERCORSO ==="},

    // Messaggi e Avvisi
    {"AVVISO_NO_FILTRI", "[AVVISO] Nessun filtro attivo! Seleziona almeno un criterio o 'Tutto'."},
    {"PROMPT_CONTINUA", "Premere un tasto per continuare . . . "},
    {"POP_ERR", "[ERRORE]"},
    {"POP_SCC", "[SUCCESSO]"},
    {"ERRORE_ACCESSO","[ERRORE] Impossibile accedere alla cartella (permessi negati)."},
    {"AVVISO_NO_CARTELLA","Nessuna Cartella Trovata"},
    {"AVVISO_MISS_DIRECTORY","Posizione Errata o Inesistente"},
    {"ERR_NO_DIR_BACK","[ERRORE] Impossibile risalire alla cartella superiore."},
    {"INSERISCI_PERCORSO","Inserisci il percorso desiderato: "},
    {"LAB_CAMBIA_PATH","[Usa le Frecce Su/Giu, Invio per selezionare | E = Esci]"},

    // Menu Popup / Inline (menuScelta)
    {"POPUP_AVANTI", "Avanti"},
    {"POPUP_INDIETRO", "Indietro"},
    {"POPUP_DEFINISCI", "Definisci"},

    {"POPUP_DATA_ESATTA", "Data esatta"},
    {"POPUP_FINO_DATA", "...fino a questa data (a <= b)"},
    {"POPUP_DA_DATA", "Da questa data in poi... (a >= b)"},

    {"POPUP_MAGGIORE", "Maggiore o uguale (a >= b)"},
    {"POPUP_COMPRESO","Compreso tra (a > x < b)"},
    {"POPUP_MINORE", "Minore o uguale (a <= b)"},
    
    {"POPUP_BYTE", "Byte (B)"},
    {"POPUP_KB", "Kilobyte (KB)"},
    {"POPUP_MB", "MegaByte (MB)"},
    {"POPUP_GB", "GigaByte (GB)"},
    
    {"POPUP_TXT", "TXT (Testo semplice)"},
    {"POPUP_CSV", "CSV (Valori separati da virgola)"},
    {"POPUP_JSON", "JSON (Formato strutturato)"},
    {"POPUP_ANNULLA", "Annulla"},
    
    {"POPUP_SI_APPLICA", "Sì, applica le modifiche"},
    {"POPUP_NO_ANNULLA", "No, annulla e torna al menu"},

    // Etichette dinamiche menuScelta
    {"LBL_TUTTI_FILE", " =          (Tutti i file: "},
    {"LBL_CONVERTI_IN", " (Converti in: "},
    {"LBL_ELEM_ATTIVO", " elemento attivo)"},
    {"LBL_ELEM_ATTIVI", " elementi attivi)"},
    {"LBL_PAROLA_INSERITA"," parola inserita)"},
    {"LBL_PAROLE_INSERITE"," parole inserite)"},
    {"LBL_RIPETUTO_GUIDA", "  (Ne mantiene uno se consecutivo)"},
    {"LBL_SPAZIO_GUIDA", "  (Sostituisce i caratteri rimossi con spazio)"},

    // Gestione Parole ed Estensioni (file_operations.cpp)
    {"BANNER_DEF_PAROLE", "=== DEFINISCI PAROLE DA RIMUOVERE ==="},
    {"MSG_PAROLE_LISTA", "Parole attualmente in lista: "},
    {"PROMPT_INSERISCI_PAROLE", "Inserisci le parole separate da spazio (oppure premi Invio per resettare):"},
    
    {"BANNER_DEF_TARGET", "=== DEFINISCI ESTENSIONE TARGET ==="},
    {"MSG_ESTENSIONE_ATTUALE", "Estensione attuale: "},
    {"PROMPT_NUOVA_ESTENSIONE", "Inserisci la nuova estensione (senza punto, es: mp3, pdf, txt, cpp): "},
    
    {"BANNER_DEF_OUTPUT", "=== DEFINISCI ESTENSIONE OUTPUT ==="},

    // Esportazione e Filtri (file_operations.cpp)
    {"BANNER_DEF_EXT_ESP", "=== DEFINISCI ESTENSIONI DA ESPORTARE ==="},
    {"MSG_EXT_ATTUALI", "Estensioni attualmente in lista: "},
    {"MSG_NO_EXT", "Nessuna estensione impostata (verranno considerate tutte)."},
    {"PROMPT_INSERISCI_EST", "Inserisci le estensioni separate da spazio (oppure premi Invio per resettare):"},
    {"PROMPT_SELECT_INFO","Seleziona le informazioni che desideri includere nel file:"},
    {"HINT_SELECT_ONE_INFO","Seleziona almeno un'informazione da esportare!"},
    {"MSG_SELECT_AND_EXPORT","Seleziona uno o piu formati desiderati e premi [Esporta]:"},
    {"BANNER_INFO_ESPORTA","=== SELEZIONE INFORMAZIONI DA ESPORTARE ==="},
    {"BANNER_FORMATI_EXP","=== SELEZIONA I FORMATI DI ESPORTAZIONE ==="},
    {"HINT_SELECT_ONE_FORMAT","Seleziona almeno un formato di file!"},
    {"DISPONIBILE","NoN Disponibile"},
    
    {"BANNER_DEF_SOTTO_ESP", "=== DEFINISCI SOTTOSTRINGHE DA ESPORTARE ==="},
    {"MSG_SOTTO_ATTUALI", "Sottostringhe attualmente in lista: "},
    {"MSG_NO_SOTTO", "Nessuna sottostringa impostata."},
    {"PROMPT_INSERISCI_SOTTO", "Inserisci le parole chiave separate da spazio (oppure premi Invio per resettare):"},
    
    {"BANNER_CONFIG_FILTRI", "=== MOSTRA LISTA: CONFIGURAZIONE FILTRI ==="},
    {"HINT_ATTIVA_FILTRI", "Attiva le opzioni con [Invio] e seleziona [Esegui] per visualizzare."},
    {"BANNER_DEF_DATA", "=== DEFINISCI DATA ==="},
    {"PROMPT_INSERISCI_DATA", "Inserisci la data esatta in formato GG/MM/AAAA: "},
    {"BANNER_DEF_DIM", "=== DEFINISCI DIMENSIONE ==="},
    {"PROMPT_SCEGLI_CONFRONTO", "Scegli il tipo di confronto:"},
    {"PROMPT_VALORE_NUMERICO", "Inserisci il valore numerico: "},
    {"PROMPT_UNITA_MISURA", "Scegli l'unita di misura:"},

    {"POPUP_ADD_PAROLA","Aggiungi nuova parola"}, 
    {"POPUP_EDIT_PAROLA","Modifica una parola"}, 
    {"POPUP_CANC_PAROLA","Rimuovi una parola a scelta"}, 
    {"POPUP_ERASE_ALL_PAROLE","Rimuovi tutte le parole"}, 

    {"MSG_SELEZIONA_PAROLE_MODIFICA","Seleziona la parola che vuoi modificare"},
    {"POPUP_PAROLA_ATT","Parola Attuale: "},
    {"POPUP_INS_VALORE","Inserisci il nuovo valore: "},
    {"MGS_SCC_EDIT_PAROLA","Parola modificata con successo!"},
    {"MSG_EDIT_ANNULLATA","Modifica annullata (nessun testo inserito)."},
    {"BANNER_SELECT_PAROLA_REMOVE","Seleziona la parola che vuoi rimuovere:"},
    {"MSG_ALL_WORDS_REMOVED","Tutte le parole sono state rimosse."},
    {"WORD","Parola"},
    {"SUCC_REMOVED","rimossa con successo!"},
    
    {"BANNER_LISTA_FILE", "=== LISTA FILE E CARTELLA CORRENTE ==="},
    {"LABEL_PERCORSO", "Percorso: "},
    {"COL_NOME", "Nome"},
    {"COL_DIM", "Dimensione"},
    {"COL_ULTIMA_MOD", "Ultima Modifica"},
    {"COL_STATO", "Stato"},
    {"COL_ESTENSIONE","Estensione"},
    {"COL_ANNO","Anno"},
    {"MSG_NO_RISULTATI", "  Nessun Risultato trovato con i criteri selezionati."},
    {"MSG_TOTALE_ELEMENTI", "Totale elementi filtrati: "},
    {"MSG_FILE_CARTELLE", " file, "},
    {"MSG_SOLO_CARTELLE", " cartelle."},
    {"ERR_DIR","[ERRORE] Impossibile leggere la cartella: "},
    {"POS_CURRENT","Posizione Corrente: "},
    
    {"BANNER_CONFIG_ESPORTAZIONE", "=== CONFIGURAZIONE FILTRI ESPORTAZIONE ==="},
    {"HINT_GESTISCI_CRITERI", "Gestisci i criteri e seleziona [Esegui] per procedere."},
    {"PROMPT_COME_ESPORTARE", "Come vuoi esportare la lista?"},
    {"MSG_ESPORTAZIONE_ANNULLATA", "[AVVISO]: Nessun file trovato con i criteri selezionati. Esportazione annullata."},
    {"MSG_SUCCESSO_ESPORTAZIONE", "Esportazione completata!"},
    {"MSG_FILE_SALVATO", "File salvato in: "},
    {"MSG_ELEMENTI_ESPORTATI", "Elementi esportati: "},
    {"ERR_MAKE_FILE","[ERRORE]: Impossibile creare il file!"},
    
    {"BANNER_CONFERMA_BATCH", "=== [ATTENZIONE] CONFERMA MODIFICHE BATCH ==="},
    {"MSG_CARTELLA", "Cartella: "},
    {"MSG_VERRANNO_MODIFICATI", "Verranno modificati "},
    {"MSG_FILE_REGOLE_ATTIVE", " file in base alle regole attive:"},
    {"COL_NOME_ORIGINALE", "Nome Originale"},
    {"COL_NUOVO_NOME", "Nuovo Nome"},
    {"PROMPT_SICURO_PROCEDERE", "Sei sicuro di voler procedere con la rinomina effettiva sui file?"},
    {"MSG_OPERAZIONE_ANNULLATA", "[OPERAZIONE ANNULLATA]: Nessun file è stato modificato."},
    {"MSG_NO_MODIFICHE", "[AVVISO]: Nessun file verrà modificato."},
    {"BANNER_ESECUZIONE_RINOMINA", "=== ESECUZIONE RINOMINA IN CORSO... ==="},
    {"MSG_OPERAZIONE_COMPLETATA", "Operazione completata. File rinominati: "},
    {"MSG_ERRORI", ", Errori: "},

    {"FILE_NAME_EXP","lista_esportata"},
    
    // About Me
    {"TITLE_ABOUT", "RE(set)_FILES BATCH TOOL"},
    {"SECTION_INFO", "[ INFORMAZIONI SOFTWARE ]"},
    {"INFO_VERSIONE", "• Versione:       "},
    {"INFO_SVILUPPATORE", "• Sviluppatore:   "},
    {"INFO_LICENZA", "• Licenza:        "},
    {"SECTION_GUIDA", "[ GUIDA RAPIDA ]"},
    {"GUIDA_1", "• [Freccia SU / GIÙ]     Spostati nel menu"},
    {"GUIDA_2", "• [INVIO]                Attiva / Disattiva regola"},
    {"GUIDA_3", "• [Q]                    Seleziona / Deseleziona tutto"},
    {"GUIDA_4", "• [E]                    Esci dal SottoMenu"},
    {"GUIDA_5", "• [Esegui Rinomina]      Applica tutte le regole TRUE in sequenza"},
    {"GUIDA_6", "• [Esporta lista Files]  Salva in TXT, CSV o JSON i file attuali"},
    {"PROMPT_ABOUT_EXIT", "Premi INVIO per tornare al menu principale..."},

    // Sostituzione Estensione
    {"BANNER_SOS_EST", "=== ATTENZIONE: SOSTITUZIONE ESTENSIONE ==="},
    {"MSG_INFO_SOS_EXT", "Questa operazione modifichera' l'estensione di tutti i file target selezionati."},
    {"MSG_EST_OUTPUT_ATTUALE", "Estensione di output attuale: "},
    {"PROMPT_INSERISCI_NUOVA_EXT", "Inserisci la nuova estensione (es. txt, pdf, jpg) o lascia vuoto per disattivare:"},
    {"MSG_EST_AGGIORNATA", "Estensione di output aggiornata con successo a '."},
    {"MSG_SOS_EST_DISATTIVATA", "Sostituzione estensione disattivata (valore reimpostato a 'Non Impostata')."},
    {"EXT_SET_NAME","Non Impostata"},

    // Estensioni Target
    {"BANNER_DEF_EXT_TARGET", "=== DEFINISCI ESTENSIONI TARGET ==="},
    {"MSG_EXT_TARGET_ATTUALI", "Estensioni attuali: "},
    {"MSG_NO_EXT_TARGET", "Nessuna estensione specifica definita."},
    {"PROMPT_INSERISCI_EXT_TARGET", "Inserisci le estensioni separate da spazio (es. txt pdf doc):"},
    {"AVVISO_EXT_ASSENTE", "Attenzione: L'estensione '"},
    {"AVVISO_EXT_ASSENTE_2", "' non e' presente in nessun file nella cartella corrente."},
    {"AVVISO_NESSUNA_EXT_PRESENTE", "Nessuna delle estensioni inserite e' presente nella cartella."},

    // Sottostringhe Target
    {"BANNER_DEF_SOTTO_TARGET", "=== DEFINISCI SOTTOSTRINGHE TARGET ==="},
    {"MSG_SOTTO_TARGET_ATTUALI", "Sottostringhe attuali: "},
    {"PROMPT_INSERISCI_SOTTO_TARGET", "Inserisci le sottostringhe separate da spazio:"},

    // Cifre Target
    {"BANNER_DEF_CIFRE_TARGET", "=== DEFINISCI CIFRE TARGET ==="},
    {"MSG_CIFRE_TARGET_ATTUALI", "Cifre attuali: "},
    {"PROMPT_INSERISCI_CIFRE_TARGET", "Inserisci le cifre o i numeri da cercare separati da spazio:"},

    // Inizia con / Finisce con
    {"BANNER_INIZIA_CON", "=== INIZIA CON... ==="},
    {"MSG_PREFISSI_ATTUALI", "Prefissi attuali: "},
    {"PROMPT_INSERISCI_PREFISSI", "Inserisci i prefissi separati da spazio:"},
    {"BANNER_FINISCE_CON", "=== FINISCE CON... ==="},
    {"MSG_SUFFISSI_ATTUALI", "Suffissi attuali: "},
    {"PROMPT_INSERISCI_SUFFISSI", "Inserisci i suffissi separati da spazio:"},

    // Selezione File Manuale & Target Menu
    {"AVVISO_NO_FILE_FILTRI", "Nessun file trovato con i filtri attuali."},
    {"BANNER_SELEZIONE_MANUALE", "=== MENU SELEZIONE FILE MANUALI ==="},
    {"HINT_SELEZIONE_MANUALE", "Usa le Frecce Su/Giu per scorrere, INVIO per attivare/disattivare, Q = Tutti On/Off, E = Esci"},
    {"COL_NOME_FILE", "Nome File"},
    {"BANNER_CONFIG_REGOLE_TARGET", "=== CONFIGURAZIONE REGOLE TARGET ==="},
    {"HINT_CRITERI_TARGET", "Seleziona i criteri per filtrare i file da rinominare:"},
    {"PROMPT_VALORE_MINIMO", "Inserisci il valore MINIMO: "},
    {"PROMPT_VALORE_MASSIMO", "Inserisci il valore MASSIMO: "},
    {"LBL_FILE_SELEZIONATO", " File Selezionato"},
    {"LBL_FILE_SELEZIONATI", " File Selezionati"},
};

static const std::unordered_map<std::string, std::string> dizionarioEN = {
    // Main Menu
    {"MENU_MOSTRA", "Show Files List"},
    {"MENU_POSIZIONE", "Change Path"},
    {"MENU_TARGET", "Define the Target"},
    {"MENU_PUNTEGGIATURA", "Remove Punctuation"},
    {"MENU_PARENTESI", "Remove Parentheses"},
    {"MENU_SPAZI", "Remove Spaces"},
    {"MENU_SPAZI_RIPETUTI", "Remove Repeated Spaces"},
    {"MENU_NUMERI", "Remove Numbers"},
    {"MENU_SIMBOLI", "Remove Symbols"},
    {"MENU_MAIUSCOLE", "Replace with Uppercase"},
    {"MENU_MINUSCOLE", "Replace with Lowercase"},
    {"MENU_PAROLE", "Remove Words"},
    {"MENU_DEF_EXT", "Define extensions"},
    {"MENU_SOS_EXT", "Replace extension"},
    {"MENU_ESPORTA", "Export files list"},
    {"MENU_ESGUI", "[ RUN RENAME ]"},
    {"MENU_ABOUT", "About Me"},
    {"MENU_LINGUA", "Language: English"},
    {"MENU_CHIUDI", "Exit"},

    // Common Submenu & Filters
    {"SUB_TUTTO", "All"},
    {"SUB_ESTENSIONI", "Specified extensions"},
    {"SUB_SOTTOSTRINGHE", "Search Substrings"},
    {"SUB_CIFRE", "Search Digits"},
    {"SUB_DATA", "Specific date (DD/MM/YYYY)"},
    {"SUB_DIMENSIONE", "Size"},
    {"SUB_ESEGUI", "Execute"},
    {"SUB_INDIETRO", "Back to main menu"},
    {"SUB_STRINGHE","Define Substrings"},
    {"SUB_CON_CIFRE","With Digits"},
    {"SUB_DEF_CIFRE","Define Digits"},
    {"SUB_DATA_FILES","Data files (DD/MM/YYYY)"},
    {"SUB_INIZIA_CON","Starts with"},
    {"SUB_FINISCE_CON","Ends with"},
    {"SUB_M_SELEZIONE","Selection Menu"},

    // menu_infoEsporta
    {"SUB_MENU_INFO_EXP_NOME","Name"},
    {"SUB_MENU_INFO_EXP_EXT","Extension"},
    {"SUB_MENU_INFO_EXP_DIM","Size"},
    {"SUB_MENU_INFO_EXP_YEAR","Year (if Available)"},
    {"SUB_MENU_INFO_EXP_EXE","Execute"},
    {"SUB_MENU_INFO_EXP_BACK","Go Back"},
    
    // menu_formatoFile
    {"SUB_MENU_FORM_FILE_TXT","TXT (Plain text)"},
    {"SUB_MENU_FORM_FILE_CSV","CSV (Comma-separated values)"},
    {"SUB_MENU_FORM_FILE_JSON","JSON (Structured format)"},
    {"SUB_MENU_FORM_FILE_EXE","[Export]"},
    {"SUB_MENU_FORM_FILE_BACK","Go Back"},

    // Common Removal Submenu Options
    {"OP_CONSERVA_RIPETUTO", "Keep One Repeated"},
    {"OP_SOSTITUISCI_SPAZIO", "Replace with Space"},

    // Punctuation Menu
    {"PUNT_VIRGOLA", ",  (Comma)"},
    {"PUNT_PUNTO_VIRGOLA", ";  (Semicolon)"},
    {"PUNT_PUNTO", ".  (Period)"},
    {"PUNT_DUE_PUNTI", ":  (Colon)"},
    {"PUNT_TRATTINO", "-  (Hyphen)"},
    {"PUNT_UNDERSCORE", "_  (Underscore)"},
    {"PUNT_ESCLAMATIVO", "!  (Exclamation mark)"},
    {"PUNT_INTERROGATIVO", "?  (Question mark)"},
    {"PUNT_APOSTROFO", "'  (Apostrophe)"},
    {"PUNT_CIRCONFLESSO", "^  (Circumflex accent)"},
    {"PUNT_VIRGOLETTE", "\"  (Quotes)"},

    // Parentheses Menu
    {"PAR_ANG_SX", "< (Left angle bracket)"},
    {"PAR_ANG_DX", "> (Right angle bracket)"},
    {"PAR_ROT_SX", "( (Left parenthesis)"},
    {"PAR_ROT_DX", ") (Right parenthesis)"},
    {"PAR_QUAD_SX", "[ (Left bracket)"},
    {"PAR_QUAD_DX", "] (Right bracket)"},
    {"PAR_GRAF_SX", "{ (Left brace)"},
    {"PAR_GRAF_DX", "} (Right brace)"},

    // Symbols Menu
    {"SIMB_TILDE", "[~] (Tilde)"},
    {"SIMB_BACKSLASH", "[\\] (Backslash)"},
    {"SIMB_STERLINA", "[£] (Pound sign)"},
    {"SIMB_DOLLARO", "[$] (Dollar sign)"},
    {"SIMB_PERCENTUALE", "[%] (Percent sign)"},
    {"SIMB_AMPERSAND", "[&] (Ampersand)"},
    {"SIMB_DIVISO", "[/] (Slash)"},
    {"SIMB_UGUALE", "[=] (Equals sign)"},
    {"SIMB_SOMMA", "[+] (Plus sign)"},
    {"SIMB_MOLTIPLICAZIONE", "[*] (Asterisk)"},
    {"SIMB_E_ACC_GRAVE", "[è] (E with grave accent) "},
    {"SIMB_E_ACC_ACUTA", "[é] (E with acute accent) "},
    {"SIMB_CHIOCCIOLA", "[@] (At sign)"},
    {"SIMB_C_CEDIGLIA", "[ç] (C cedilla) "},
    {"SIMB_O_ACC_GRAVE", "[ò] (O with grave accent) "},
    {"SIMB_A_ACC_GRAVE", "[à] (A with grave accent) "},
    {"SIMB_GRADO", "[°] (Degree symbol)"},
    {"SIMB_CANCELLETTO", "[#] (Hash / Pound)"},
    {"SIMB_U_ACC_GRAVE", "[ù] (U with grave accent) "},
    {"SIMB_PARAGRAFO", "[§] (Section sign)"},
    {"SIMB_I_ACC_GRAVE", "[ì] (I with grave accent) "},
    {"SIMB_BARRA_VERT", "[|] (Vertical bar)"},

    // Submenu Banners
    {"BANNER_PUNTEGGIATURA", "=== PUNCTUATION CONFIGURATION ==="},
    {"BANNER_PARENTESI", "=== PARENTHESES CONFIGURATION ==="},
    {"BANNER_SIMBOLI", "=== SYMBOLS CONFIGURATION ==="},
    {"BANNER_PERCORSO","=== DEFINE ROUTE ==="},

    // Messages & Warnings
    {"AVVISO_NO_FILTRI", "[WARNING] No active filters! Select at least one criteria or 'All'."},
    {"PROMPT_CONTINUA", "Press any key to continue . . . "},
    {"POP_ERR", "[ERROR]"},
    {"POP_SCC", "[SUCCESS]"},
    {"ERRORE_ACCESSO","[ERROR] Unable to access the folder (permission denied)."},
    {"AVVISO_NO_CARTELLA","No Folder Found"},
    {"AVVISO_MISS_DIRECTORY","Incorrect or Non-existent Location"},
    {"ERR_NO_DIR_BACK","[ERROR] Unable to navigate to the parent folder."},
    {"INSERISCI_PERCORSO","Enter the desired path: "},
    {"LAB_CAMBIA_PATH","[Use Up/Down Arrows, Enter to select | E = Exit]"},

    // Popup / Inline Menus (menuScelta)
    {"POPUP_AVANTI", "Next"},
    {"POPUP_INDIETRO", "Back"},
    {"POPUP_DEFINISCI", "Define path"},

    {"POPUP_DATA_ESATTA", "Specific date"},
    {"POPUP_FINO_DATA", "...up to this date (<=)"},
    {"POPUP_DA_DATA", "From this date onwards... (>=)"},
    
    {"POPUP_MAGGIORE", "Greater than or equal (>=)"},
    {"POPUP_COMPRESO","Between (a > x < b)"},
    {"POPUP_MINORE", "Less than or equal (<=)"},
    
    {"POPUP_BYTE", "Byte (B)"},
    {"POPUP_KB", "Kilobyte (KB)"},
    {"POPUP_MB", "MegaByte (MB)"},
    {"POPUP_GB", "GigaByte (GB)"},
    
    {"POPUP_TXT", "TXT (Plain text)"},
    {"POPUP_CSV", "CSV (Comma-separated values)"},
    {"POPUP_JSON", "JSON (Structured format)"},
    {"POPUP_ANNULLA", "Cancel"},
    
    {"POPUP_SI_APPLICA", "Yes, apply changes"},
    {"POPUP_NO_ANNULLA", "No, cancel and return to menu"},

    // Dynamic labels menuScelta
    {"LBL_TUTTI_FILE", " =          (All files: "},
    {"LBL_CONVERTI_IN", " (Convert to: "},
    {"LBL_ELEM_ATTIVO", " active element)"},
    {"LBL_ELEM_ATTIVI", " active elements)"},
    {"LBL_PAROLA_INSERITA"," word Entered)"},
    {"LBL_PAROLE_INSERITE"," words Entered)"},
    {"LBL_RIPETUTO_GUIDA", "  (Keeps one if consecutive)"},
    {"LBL_SPAZIO_GUIDA", "  (Replaces the removed characters with space)"},

    // Words and Extensions Management (file_operations.cpp)
    {"BANNER_DEF_PAROLE", "=== DEFINE WORDS TO REMOVE ==="},
    {"MSG_PAROLE_LISTA", "Words currently in list: "},
    {"PROMPT_INSERISCI_PAROLE", "Enter words separated by space (or press Enter to reset):"},
    
    {"BANNER_DEF_TARGET", "=== DEFINE TARGET EXTENSION ==="},
    {"MSG_ESTENSIONE_ATTUALE", "Current extension: "},
    {"PROMPT_NUOVA_ESTENSIONE", "Enter the new extension (without dot, e.g., mp3, pdf, txt, cpp): "},
    
    {"BANNER_DEF_OUTPUT", "=== DEFINE OUTPUT EXTENSION ==="},

    // Export and Filters (file_operations.cpp)
    {"BANNER_DEF_EXT_ESP", "=== DEFINE EXTENSIONS TO EXPORT ==="},
    {"MSG_EXT_ATTUALI", "Extensions currently in list: "},
    {"MSG_NO_EXT", "No extensions set (all will be considered)."},
    {"PROMPT_INSERISCI_EST", "Enter extensions separated by space (or press Enter to reset):"},
    {"PROMPT_SELECT_INFO","Select the information you wish to include in the file:"},
    {"HINT_SELECT_ONE","Select at least one piece of information to export!"},
    {"MSG_SELECT_AND_EXPORT","Select one or more desired formats and press [Export]:"},
    {"BANNER_INFO_ESPORTA","=== SELECTION OF INFORMATION TO EXPORT ==="},
    {"BANNER_FORMATI_EXP","=== SELECT EXPORT FORMATS ==="},
    {"HINT_SELECT_ONE_FORMAT","Select at least one file format!"},
    {"DISPONIBILE","Not Available"},
    
    {"BANNER_DEF_SOTTO_ESP", "=== DEFINE SUBSTRINGS TO EXPORT ==="},
    {"MSG_SOTTO_ATTUALI", "Substrings currently in list: "},
    {"MSG_NO_SOTTO", "No substrings set."},
    {"PROMPT_INSERISCI_SOTTO", "Enter keywords separated by space (or press Enter to reset):"},
    
    {"BANNER_CONFIG_FILTRI", "=== SHOW LIST: FILTER CONFIGURATION ==="},
    {"HINT_ATTIVA_FILTRI", "Activate options with [Enter] and select [Execute] to view."},
    {"BANNER_DEF_DATA", "=== DEFINE DATE ==="},
    {"PROMPT_INSERISCI_DATA", "Enter exact date in DD/MM/YYYY format: "},
    {"BANNER_DEF_DIM", "=== DEFINE SIZE ==="},
    {"PROMPT_SCELGI_CONFRONTO", "Choose comparison type:"},
    {"PROMPT_VALORE_NUMERICO", "Enter numeric value: "},
    {"PROMPT_UNITA_MISURA", "Choose unit of measurement:"},

    {"POPUP_ADD_PAROLA","Add a new word"}, 
    {"POPUP_EDIT_PAROLA","Edit a word"}, 
    {"POPUP_CANC_PAROLA","Remove a specific word"}, 
    {"POPUP_ERASE_ALL_PAROLE","Remove all words"}, 

    {"MSG_SELEZIONA_PAROLE_MODIFICA","Select the word you want to modify"},
    {"POPUP_PAROLA_ATT","Current word: "},
    {"POPUP_INS_VALORE","Enter the new value: "},
    {"MGS_SCC_EDIT_PAROLA","Word successfully modified!"},
    {"MSG_EDIT_ANNULLATA","Edit cancelled (no text entered)."},
    {"BANNER_SELECT_PAROLA_REMOVE","Select the word you want to remove:"},
    {"MSG_ALL_WORDS_REMOVED","All words have been removed."},
    {"WORD","Word"},
    {"SUCC_REMOVED","successfully removed!"},
    
    {"BANNER_LISTA_FILE", "=== FILES LIST AND CURRENT FOLDER ==="},
    {"LABEL_PERCORSO", "Path: "},
    {"COL_NOME", "Name"},
    {"COL_DIM", "Size"},
    {"COL_ULTIMA_MOD", "Last Modified"},
    {"COL_STATO", "Status"},
    {"COL_ESTENSIONE","Extension"},
    {"COL_ANNO","Year"},
    {"MSG_NO_RISULTATI", "  No results found matching the selected criteria."},
    {"MSG_TOTALE_ELEMENTI", "Total filtered items: "},
    {"MSG_FILE_CARTELLE", " files, "},
    {"MSG_SOLO_CARTELLE", " folders."},
    {"ERR_DIR","[ERROR] Unable to read the folder: "},
    {"POS_CURRENT","Current Location: "},
    
    {"BANNER_CONFIG_ESPORTAZIONE", "=== EXPORT FILTER CONFIGURATION ==="},
    {"HINT_GESTISCI_CRITERI", "Manage criteria and select [Execute] to proceed."},
    {"PROMPT_COME_ESPORTARE", "How do you want to export the list?"},
    {"MSG_ESPORTAZIONE_ANNULLATA", "[WARNING]: No files found with selected criteria. Export cancelled."},
    {"MSG_SUCCESSO_ESPORTAZIONE", "Export completed!"},
    {"MSG_FILE_SALVATO", "File saved to: "},
    {"MSG_ELEMENTI_ESPORTATI", "Exported items: "},
    {"ERR_MAKE_FILE","[ERROR]: Unable to create the file!"},
    
    {"BANNER_CONFERMA_BATCH", "=== [WARNING] BATCH CHANGES CONFIRMATION ==="},
    {"MSG_CARTELLA", "Folder: "},
    {"MSG_VERRANNO_MODIFICATI", "About to modify "},
    {"MSG_FILE_REGOLE_ATTIVE", " files based on active rules:"},
    {"COL_NOME_ORIGINALE", "Original Name"},
    {"COL_NUOVO_NOME", "New Name"},
    {"PROMPT_SICURO_PROCEDERE", "Are you sure you want to proceed with the actual file renaming?"},
    {"MSG_OPERAZIONE_ANNULLATA", "[OPERATION CANCELLED]: No files were modified."},
    {"MSG_NO_MODIFICHE", "[WARNING]: No files will be modified."},
    {"BANNER_ESECUZIONE_RINOMINA", "=== EXECUTING RENAME... ==="},
    {"MSG_OPERAZIONE_COMPLETATA", "Operation completed. Renamed files: "},
    {"MSG_ERRORI", ", Errors: "},

    {"FILE_NAME_EXP","exported_list"},
    
    // About Me
    {"TITLE_ABOUT", "RE(set)_FILES BATCH TOOL"},
    {"SECTION_INFO", "[ SOFTWARE INFORMATION ]"},
    {"INFO_VERSIONE", "• Version:        "},
    {"INFO_SVILUPPATORE", "• Developer:      "},
    {"INFO_LICENZA", "• License:        "},
    {"SECTION_GUIDA", "[ QUICK GUIDE ]"},
    {"GUIDA_1", "• [Arrow UP / DOWN]     Navigate menu"},
    {"GUIDA_2", "• [ENTER]               Toggle rule on/off"},
    {"GUIDA_3", "• [Q]                   Select / Deselect all"},
    {"GUIDA_4", "• [E]                   Exit SubMenu"},
    {"GUIDA_5", "• [Execute Rename]      Apply all TRUE rules in sequence"},
    {"GUIDA_6", "• [Export Files List]   Save current files to TXT, CSV, or JSON"},
    {"PROMPT_ABOUT_EXIT", "Press ENTER to return to main menu..."},

    // Extension Replacement
    {"BANNER_SOS_EST", "=== WARNING: EXTENSION REPLACEMENT ==="},
    {"MSG_INFO_SOS_EXT", "This operation will modify the extension of all selected target files."},
    {"MSG_EST_OUTPUT_ATTUALE", "Current output extension: "},
    {"PROMPT_INSERISCI_NUOVA_EXT", "Enter new extension (e.g., txt, pdf, jpg) or leave empty to disable:"},
    {"MSG_EST_AGGIORNATA", "Output extension successfully updated to '."},
    {"MSG_SOS_EST_DISATTIVATA", "Extension replacement disabled (reset to 'Not Set')."},
    {"EXT_SET_NAME","Not Set"},

    // Target Extensions
    {"BANNER_DEF_EXT_TARGET", "=== DEFINE TARGET EXTENSIONS ==="},
    {"MSG_EXT_TARGET_ATTUALI", "Current extensions: "},
    {"MSG_NO_EXT_TARGET", "No specific extension defined."},
    {"PROMPT_INSERISCI_EXT_TARGET", "Enter extensions separated by space (e.g., txt pdf doc):"},
    {"AVVISO_EXT_ASSENTE", "Warning: Extension '"},
    {"AVVISO_EXT_ASSENTE_2", "' is not present in any file in the current folder."},
    {"AVVISO_NESSUNA_EXT_PRESENTE", "None of the entered extensions are present in the folder."},

    // Target Substrings
    {"BANNER_DEF_SOTTO_TARGET", "=== DEFINE TARGET SUBSTRINGS ==="},
    {"MSG_SOTTO_TARGET_ATTUALI", "Current substrings: "},
    {"PROMPT_INSERISCI_SOTTO_TARGET", "Enter substrings separated by space:"},

    // Target Digits
    {"BANNER_DEF_CIFRE_TARGET", "=== DEFINE TARGET DIGITS ==="},
    {"MSG_CIFRE_TARGET_ATTUALI", "Current digits: "},
    {"PROMPT_INSERISCI_CIFRE_TARGET", "Enter digits or numbers to search separated by space:"},

    // Starts with / Ends with
    {"BANNER_INIZIA_CON", "=== STARTS WITH... ==="},
    {"MSG_PREFISSI_ATTUALI", "Current prefixes: "},
    {"PROMPT_INSERISCI_PREFISSI", "Enter prefixes separated by space:"},
    {"BANNER_FINISCE_CON", "=== ENDS WITH... ==="},
    {"MSG_SUFFISSI_ATTUALI", "Current suffixes: "},
    {"PROMPT_INSERISCI_SUFFISSI", "Enter suffixes separated by space:"},

    // Manual File Selection & Target Menu
    {"AVVISO_NO_FILE_FILTRI", "No files found with current filters."},
    {"BANNER_SELEZIONE_MANUALE", "=== MANUAL FILE SELECTION MENU ==="},
    {"HINT_SELEZIONE_MANUALE", "Use Arrow Up/Down to scroll, ENTER to toggle, Q = All On/Off, E = Exit"},
    {"COL_NOME_FILE", "File Name"},
    {"BANNER_CONFIG_REGOLE_TARGET", "=== TARGET RULES CONFIGURATION ==="},
    {"HINT_CRITERI_TARGET", "Select criteria to filter files to rename:"},
    {"PROMPT_VALORE_MINIMO", "Enter MINIMUM value: "},
    {"PROMPT_VALORE_MASSIMO", "Enter MAXIMUM value: "},
    {"LBL_FILE_SELEZIONATO", " Selected File"},
    {"LBL_FILE_SELEZIONATI", " Selected Files"},
};

void imposta_lingua(Lingua lang) {
    linguaAttiva = lang;
}

Lingua ottieni_lingua_corrente() {
    return linguaAttiva;
}

std::string get_string(const std::string& chiave) {
    if (linguaAttiva == Lingua::Italiano) {
        auto it = dizionarioIT.find(chiave);
        if (it != dizionarioIT.end()) return it->second;
    } else {
        auto it = dizionarioEN.find(chiave);
        if (it != dizionarioEN.end()) return it->second;
    }
    return chiave;
}