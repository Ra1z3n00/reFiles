#pragma once
#include <string>

enum class Lingua {
    Italiano,
    Inglese
};

// Funzione per impostare la lingua corrente
void imposta_lingua(Lingua lang);
Lingua ottieni_lingua_corrente();

// Funzione "tabella" che restituisce la stringa tradotta
std::string get_string(const std::string& chiave);