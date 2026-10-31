#pragma once

#include "utilities.hpp"
#include "menu.hpp"

namespace fs = std::filesystem;

// --- ENUM PER FILTRI E CONFIGURAZIONI ---
enum class Tempi { Ieri_Oggi = 1, Oggi = 2, Oggi_Domani = 3 };
enum class DirezioneDim { Maggiori = 1, Compresi = 2, Minori = 3 };

struct SelezioneFileItem {
    string nomeCompleto;
    bool selezionato; // true o false per la rinomina
};
extern vector<SelezioneFileItem> fileSelezionatiManualmente;

// --- DICHIARAZIONI EXTERN DELLE VARIABILI DI CONFIGURAZIONE ---
extern string estensioneTarget;
extern string estensioneOutput;
extern vector<string> paroleDaRimuovere;
extern vector<string> sottostringheEsportate;
extern vector<string> estensioniEsportate;
extern string dataEsportate;

extern Tempi dataModalitaEsporta;
extern DirezioneDim dimTipoEsporta;
extern double dimValoreEsporta;

// --- PROTOTIPI TRASFORMAZIONE STRINGHE ---
string to_lowercase(string str);
string to_uppercase(string str);

void cambia_path();
void applica_rimuovi_spazi(string& nome);
void applica_rimuovi_spazi_ripetuti(string& nome);
void applica_rimuovi_numeri(string& nome);
void applica_minuscole(string& nome);
void applica_maiuscole(string& nome);
void applica_rimuovi_parole(string& nome);

void applica_rimozione_generica(string& nome, const vector<valoriMenu>& menuSotto, const string& targetChars, bool isSimboliASCII = false);
void applica_rimuovi_punteggiatura(string& nome);
void applica_rimuovi_parentesi(string& nome);
void applica_rimuovi_simboli(string& nome);

// --- PROTOTIPI GESTIONE DIALOGHI / SOTTOMENU CONFIGURAZIONE ---
void gestisci_menu_parole();
void gestisci_menu_estensioni_esporta();
void gestisci_menu_sottostringhe_esporta();
void gestisci_menu_target();
void gestisci_menu_selezione_file();
void sostituisci_estensioni();

// --- PROTOTIPI OPERAZIONI PRINCIPALI ---
void mostra_lista();
void esporta_lista();
void esegui_rinomina();
void aboutMe();