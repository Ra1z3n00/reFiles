# reFiles - Batch File Management & Cleaning Tool

**reFiles** è uno strumento CLI (Command Line Interface) avanzato sviluppato in C++20, progettato per la gestione, la pulizia batch e l'esportazione strutturata di file nel file system locale.

Grazie a un'interfaccia terminale interattiva e a un'architettura modulare a 7 file, **reFiles** permette di manipolare nomi di file in blocco applicando regole dinamiche (rimozione di punteggiatura, parentesi, simboli ASCII, cifre, spazi e parole chiave) e di esportare elenchi dettagliati nei formati **TXT**, **CSV** e **JSON**.

---

## 🚀 Caratteristiche Principali

* **Interfaccia CLI Interattiva**: Navigazione fluida tramite tasti direzionali (`SU` / `GIÙ`), scorciatoie rapide per attivare/disattivare regole (`Q`, `E`) e supporto a colori ANSI.
* **Architettura Modulare**: Codice suddiviso in 7 file separati con gestione dei menu tramite `enum class` fortemente tipizzate.
* **Rinomina Batch in Sicurezza (Safe-Run)**: Anteprima dettagliata a schermo di tutte le modifiche pianificate prima dell'esecuzione effettiva sul file system.
* **Sistema di Filtraggio Avanzato**: Filtra i file da mostrare o esportare per:
  * Estensione target
  * Sottostringhe nel nome
  * Presenza di cifre numeriche
  * Data di modifica (confronti $<$, $=$, $>$)
  * Dimensione del file (B, KB, MB, GB)
* **Esportazione Intelligente**:
  * **Formattazione Dinamica delle Dimensioni**: Ridimensiona automaticamente i byte in B, KB, MB o GB.
  * **Estrazione Regex dell'Anno**: Riconosce gli anni (es. `2006`, `1998`) presenti tra parentesi nei file multimediali, rimuove le parentesi superflue e separa l'anno in una colonna/campo dedicato in CSV e JSON.

---

## 📁 Struttura del Progetto (7 File)

Il progetto è suddiviso in 7 file per separare nettamente le responsabilità di utility, interazione e manipolazione file:

| File | Descrizione |
| :--- | :--- |
| `main.cpp` | Entry point del programma con il ciclo principale basato su `MenuMain`. |
| `utilities.h` | Macro dei colori ANSI, prototipi cross-platform e utility UTF-8 (`maxStringUTF8`). |
| `utilities.cpp` | Implementazione delle funzioni di gestione console e compatibilità (`getch`, `clearBuffer`). |
| `menu.hpp` | Strutture dati (`valoriMenu`), enumerazione `MenuMain` e dichiarazioni dei menu. |
| `menu.cpp` | Vettori dei sottomenu e implementazione del gestore generico `gestisci_sottomenu_generico`. |
| `file_operations.hpp` | Prototipi delle operazioni sui file, regex, trasformazioni stringhe e filtri. |
| `file_operations.cpp` | Implementazione delle funzioni di rinomina batch, esportazione (TXT/CSV/JSON) e banner `aboutMe`. |

---

## 🛠️ Compilazione e Installazione

Richiede un compilatore C++ con supporto allo standard **C++20** (es. `g++` 10+ o `clang`).

### Compilazione tramite GCC (Linux / MinGW su Windows)

```bash
g++ main.cpp menu.cpp file_operations.cpp utilities.cpp -o reFiles -std=c++20
```

### Esecuzione

```bash
./reFiles
```

---

## ⌨️ Guida Rapida ai Comandi da Tastiera

| Tasto | Azione |
| :--- | :--- |
| `Freccia SU` / `GIÙ` | Spostati tra le opzioni del menu |
| `INVIO` | Attiva / Disattiva una regola o seleziona un sottomenu |
| `Q` | Seleziona / Deseleziona tutte le opzioni in un sottomenu |
| `E` | Esci da un sottomenu e torna indietro |

---

## 📊 Esempio di Esportazione

Dato un file denominato `Silent Hill (2006).mp4` (dimensione: 1.5 GB):

### CSV (`lista_esportata.csv`)
```csv
NomeFile,Estensione,Anno,Dimensione
"Silent Hill",mp4,"2006",1.5 GB
```

### JSON (`lista_esportata.json`)
```json
[
  {
    "nome": "Silent Hill",
    "estensione": "mp4",
    "anno": "2006",
    "dimensione": "1.5 GB"
  }
]
```

---

## 📜 Licenza

Questo progetto è distribuito sotto licenza **MIT Open Source**.