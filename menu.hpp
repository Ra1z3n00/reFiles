#pragma once

#include "utilities.hpp"

struct valoriMenu {
    string testo;
    bool decisione;
};

enum class MenuMain {
    MOSTRA_LISTA = 1,
    CAMBIA_PATH,
    REGOLE_TARGET,
    RIMUOVI_PUNTEGGIATURA,
    RIMUOVI_PARENTESI,
    RIMUOVI_SPAZI,
    RIMUOVI_SPAZI_RIPETUTI,
    RIMUOVI_NUMERI,
    RIMUOVI_SIMBOLI,
    RIMUOVI_PAROLE,
    SOSTITUISCI_MAIUSCOLE,
    SOSTITUISCI_MINUSCOLE,
    SOSTITUISCI_ESTENSIONE,
    ESPORTA_LISTA,
    ESGUI_RINOMINA,
    RIGA_VUOTA,
    ABOUT_ME,
    CAMBIA_LINGUA,
    CHIUDI
};

enum class SubMenu_regoleCerca {
    TUTTO = 1,
    DEFINISCI_ESTENSIONI,
    CERCA_SOTTOSTRINGHE,
    CERCA_CIFRE,
    DATA_FILES,
    DIMENSIONE,
    ESEGUI,
    RIGA_VUOTA,
    TORNA_AL_MENU,
};

enum class SubMenu_regoleTarget {
    TUTTO = 1,                 
    DEFINISCI_ESTENSIONI,        
    DEFINISCI_SOTTOSTRINGHE,     
    CON_CIFRE,
    DEFINISCI_CIFRE,             
    DATA_FILES,     
    DIMENSIONE,                  
    INIZIA_CON,                  
    FINISCE_CON,       
    MENU_SELEZIONE,              
    RIGA_VUOTA,                           
    TORNA_AL_MENU,
};

struct SelezioneFileItem {
    string nomeCompleto;
    bool selezionato;
};

extern int nFiles;
extern vector<valoriMenu> mainMenu;
extern vector<valoriMenu> subMenu_regoleCerca;
extern vector<valoriMenu> subMenu_regoleTarget;
extern vector<valoriMenu> menuPunteggiatura;
extern vector<valoriMenu> menuParentesi;
extern vector<valoriMenu> menuSimboli;
extern vector<valoriMenu> menu_infoEsporta;
extern vector<valoriMenu> menu_formatoFile;
extern vector<string> estensioniTargetMultiple;
extern string estensioneTarget;
extern string estensioneOutput;
extern vector<SelezioneFileItem> fileSelezionatiManualmente;

// Prototipi
int menuScelta(unsigned pos, unsigned elements, ...);
int menuScelta(int pos, const vector<valoriMenu>& menu);
void switcher(int scelta);
void mutuaEsclusione(int scelta);
int modAttivi(const vector<valoriMenu>& menu);
void gestisci_sottomenu_generico(vector<valoriMenu>& menuTarget, const string& titoloBanner, int indiceMainMenu);
void gestisci_menu_punteggiatura();
void gestisci_menu_parentesi();
void gestisci_menu_simboli();
void gestisci_menu_target();
void aggiorna_testi_menu();