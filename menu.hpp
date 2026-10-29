#pragma once

#include "utilities.hpp"

struct valoriMenu {
    string testo;
    bool decisione;
};

enum class MenuMain {
    MOSTRA_LISTA = 1,
    CAMBIA_PATH,
    RIMUOVI_PUNTEGGIATURA,
    RIMUOVI_PARENTESI,
    RIMUOVI_SPAZI,
    RIMUOVI_SPAZI_RIPETUTI,
    RIMUOVI_NUMERI,
    RIMUOVI_SIMBOLI,
    SOSTITUISCI_MAIUSCOLE,
    SOSTITUISCI_MINUSCOLE,
    RIMUOVI_PAROLE,
    DEFINISCI_ESTENSIONI,
    SOSTITUISCI_ESTENSIONE,
    ESPORTA_LISTA,
    ESGUI_RINOMINA,
    RIGA_VUOTA,
    ABOUT_ME,
    CAMBIA_LINGUA,
    CHIUDI
};

extern vector<valoriMenu> mainMenu;
extern vector<valoriMenu> subMenu;
extern vector<valoriMenu> menuPunteggiatura;
extern vector<valoriMenu> menuParentesi;
extern vector<valoriMenu> menuSimboli;
extern string estensioneTarget;
extern string estensioneOutput;

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
void aggiorna_testi_menu();