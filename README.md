# reFiles - Batch File Management & Cleaning Tool

**reFiles** è uno strumento CLI (Command Line Interface) avanzato sviluppato in C++20, progettato per la gestione, la pulizia batch, il filtraggio mirato e l'esportazione strutturata di file nel file system locale.

Grazie a un'interfaccia terminale interattiva e a una struttura modulare completa, **reFiles** permette di manipolare nomi di file in blocco applicando regole dinamiche (rimozione di punteggiatura, parentesi, simboli ASCII, cifre, spazi e parole chiave), filtrando i bersagli (target) tramite criteri specifici e selezionando anche i singoli file da includere o escludere manualmente.

---

## 🚀 Caratteristiche Principali

* **Interfaccia CLI Interattiva**: Navigazione fluida tramite tasti direzionali (`SU` / `GIÙ`), scorrimento a finestra fissa per cartelle e elenchi numerosi, e supporto completo ai colori ANSI.
* **Internazionalizzazione (i18n)**: Supporto multilingua nativo (Italiano e Inglese) selezionabile direttamente dal menu principale con aggiornamento dinamico delle etichette e delle opzioni.
* **Selezione Target Flessibile**:
  * Filtro per estensioni multiple, sottostringhe, presenza di cifre, prefissi (*Inizia con*) e suffissi (*Finisce con*).
  * Filtro temporale (data di modifica esatta, fino a una data o da una data in poi).
  * Filtro per dimensione (maggiore, minore o intervalli compresi in B, KB, MB, GB).
  * **Menu di Selezione Manuale**: Elenco interattivo con supporto per attivare/disattivare singoli file o invertire lo stato di tutti gli elementi in tempo reale.
* **Rinomina Batch in Sicurezza (Safe-Run)**: Anteprima dettagliata a schermo con tabella comparativa (*Nome Originale -> Nuovo Nome*) prima di applicare le modifiche effettive.
* **Esportazione Intelligente (TXT, CSV, JSON)**:
  * Formattazione dinamica delle dimensioni dei file.
  * Estrazione automatica dell'anno (tramite regex) per separare l'anno di produzione dai titoli di file multimediali e documenti.

---

## 📁 Struttura del Progetto

Il codice sorgente è organizzato nei seguenti file per garantire massima manutenibilità e modularità:

| File | Descrizione |
| :--- | :--- |
| `main.cpp` | Entry point del programma con il ciclo principale dell'interfaccia terminale. |
| `utilities.hpp` | Macro dei colori ANSI, inclusioni di sistema, definizioni cross-platform e funzioni di supporto UTF-8. |
| `utilities.cpp` | Implementazione delle funzioni console, gestione input, pausamento e buffer. |
| `menu.hpp` | Strutture dati (`valoriMenu`, `SelezioneFileItem`), enumerazioni dei menu (`MenuMain`, `SubMenu_regoleTarget`, ecc.) e prototipi. |
| `menu.cpp` | Definizione dei vettori di menu, aggiornamento dinamico dei testi in base alla lingua e rendering dell'interfaccia. |
| `file_operations.hpp` | Prototipi delle operazioni sui file, trasformazioni stringhe, filtri e rinomina. |
| `file_operations.cpp` | Logic core per la manipolazione delle stringhe, gestione delle cartelle, filtri target, rinomina batch ed esportazione. |
| `localization.hpp` | Prototipi per il sistema di localizzazione e selettore lingua. |
| `localization.cpp` | Dizionari di traduzione per Italiano (`dizionarioIT`) e Inglese (`dizionarioEN`). |

---

## 🛠️ Compilazione e Installazione

Richiede un compilatore C++ con supporto allo standard **C++20** (es. `g++` 10+ o `clang`).

### Compilazione tramite GCC (Linux / MinGW su Windows)

```bash
g++ main.cpp menu.cpp file_operations.cpp utilities.cpp localization.cpp -o reFiles -std=c++20
```

### Esecuzione

```bash
./reFiles
```

---

## ⌨️ Guida Rapida ai Comandi da Tastiera

### Navigazione Menu Principali e Sottomenu
| Tasto | Azione |
| :--- | :--- |
| `Freccia SU` / `GIÙ` | Spostati tra le opzioni del menu |
| `INVIO` | Attiva / Disattiva una regola o conferma una selezione |
| `Q` | Seleziona / Deseleziona tutte le opzioni (nei menu punteggiatura, parentesi, simboli e selezione manuale) |
| `E` | Esci dal sottomenu e torna indietro |

### Menu Selezione Manuale File e Gestione Cartelle
| Tasto | Azione |
| :--- | :--- |
| `Freccia SU` / `GIÙ` | Scorri la lista dei file/cartelle |
| `INVIO` | Attiva/Disattiva l'inclusione del singolo file o entra nella cartella |
| `Q` | Inverti lo stato (*TRUE/FALSE*) di tutti i file in lista |
| `E` | Conferma la selezione ed esci dal menu |

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