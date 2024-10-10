#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>

#define MAX 50           // Numero massimo di libri nella libreria
#define CONST 256        // Costante per la dimensione del buffer

// Definizione di un enum per rappresentare le diverse categorie di libri
typedef enum
{
    Narrativa,
    Saggistica,
    Scienza,
    Arte,
    Romanzo, 
    CategoriaMax    // Usato per contare il numero totale di categorie
} Categoria;

// Struttura per rappresentare un libro
typedef struct
{
    char titolo[40];    
    char autore[40];    
    int annoPub;        
    float prezzo;       
    Categoria genere;   
} Libro;

Libro libreria[MAX];    // Array per memorizzare i libri della libreria

// Array di stringhe per mappare le categorie a stringhe leggibili
const char* categoriaToString[] = {"Narrativa", "Saggistica", "Scienza", "Arte", "Romanzo"};

// Funzione per convertire una stringa in un valore di tipo Categoria
Categoria stringToCategoria(const char* categoriaStr) 
{
    for (int i = 0; i < CategoriaMax; i++) 
    {
        if (strcasecmp(categoriaStr, categoriaToString[i]) == 0) // Usa strcasecmp per confronto case-insensitive
        {
            return (Categoria)i;
        }
    }
    // Se non viene trovata una corrispondenza, restituisce Narrativa come valore di default
    return Narrativa;
}

// Funzione per leggere i dati dei libri da un file CSV
int leggiCSV(const char* nomeFile) 
{
    FILE* file = fopen(nomeFile, "r");
    if (file == NULL) 
    {
        printf("Errore nell'aprire il file.\n");
        exit(-1);  // Termina il programma in caso di errore nell'apertura del file
    }

    char buffer[CONST];  // Buffer per la lettura del file
    int i = 0;

    fgets(buffer, sizeof(buffer), file);  // Legge la prima riga (intestazione) del CSV

    while (fgets(buffer, sizeof(buffer), file) && i < MAX) 
    {
        char* token = strtok(buffer, ";");  // Estrae il titolo del libro
        strcpy(libreria[i].titolo, token);

        token = strtok(NULL, ";");          // Estrae l'autore del libro
        strcpy(libreria[i].autore, token);

        token = strtok(NULL, ";");          // Estrae l'anno di pubblicazione
        libreria[i].annoPub = atoi(token);

        token = strtok(NULL, ";");          // Estrae il prezzo del libro
        libreria[i].prezzo = atof(token);

        token = strtok(NULL, ";");          // Estrae la categoria del libro
        if (token != NULL) 
        {
            token[strcspn(token, "\r\n")] = 0;  // Rimuovi i caratteri di nuova linea
            libreria[i].genere = stringToCategoria(token);
        } 
        else 
        {
            libreria[i].genere = Narrativa; // Imposta valore di default se il token è NULL
        }

        i++;
    }

    fclose(file);  // Chiude il file una volta terminata la lettura
    return i;      // Restituisce il numero di libri letti dal file
}

// Funzione per stampare i dettagli dei libri
void Stampa(const char *nomeFile)
{
    FILE* file = fopen(nomeFile, "r");
    if (file == NULL) 
    {
        printf("Errore nell'aprire il file.\n");
        exit(-1);  // Termina il programma in caso di errore nell'apertura del file
    }

    int numLibri = leggiCSV(nomeFile);  // Legge i libri dal file

    if (numLibri > 0) 
    {
        for (int i = 0; i < numLibri; i++) 
        {
            printf("[Titolo]: %s - [Autore]: %s - [Anno]: %d - [Prezzo]: %.2f euro - [Categoria]: %s\n", 
            libreria[i].titolo, libreria[i].autore, libreria[i].annoPub, libreria[i].prezzo, categoriaToString[libreria[i].genere]);
        }
    }
}

// Funzione per cercare un libro per titolo nella libreria
int CercaLibro(const char* titolo) 
{
    for (int i = 0; i < MAX; i++) 
    {
        if (strcasecmp(libreria[i].titolo, titolo) == 0) // Confronto case-insensitive per il titolo del libro
        { 
            return i;  // Restituisce l'indice del libro se trovato
        }
    }
    return -1;  // Restituisce -1 se il libro non è trovato
}

// Funzione per stampare i libri di una determinata categoria
void StampaPerCategoria(Categoria categoria)
{
    int libriTrovati = 0;  // Contatore per i libri trovati

    for (int i = 0; i < MAX; i++) 
    {
        // Verifica se il libro ha un titolo valido e se appartiene alla categoria specificata
        if (libreria[i].genere == categoria && libreria[i].annoPub != 0 && libreria[i].prezzo != 0.0 && strlen(libreria[i].titolo) > 0) 
        {
            printf("[Titolo]: %s - [Autore]: %s - [Anno]: %d - [Prezzo]: %.2f euro - [Categoria]: %s\n", 
            libreria[i].titolo, libreria[i].autore, libreria[i].annoPub, libreria[i].prezzo, categoriaToString[libreria[i].genere]);
            libriTrovati++;
        }
    }

    if (libriTrovati == 0) 
    {
        printf("Nessun libro trovato per la categoria selezionata.\n");
    }
}

int main(int argc, char *argv[])
{
    int scelta; 
    do
    {
        printf("======LIBRERIA======\n");
        printf("[1] STAMPA LIBRERIA\n");
        printf("[2] RICERCA LIBRO\n");
        printf("[3] STAMPA LIBRI PER CATEGORIA\n");
        printf("[0] ESCI DALLA LIBRERIA\n");
        printf("INSERISCI UN OPZIONE:\n");
        scanf("%d", &scelta);
        
        switch (scelta)
        {
            case 1:
                Stampa(argv[1]);
                printf("\n");
                break;
            case 2:
                char titolo[50];
                printf("Inserisci il titolo del libro che vuoi cercare:\n");
                scanf(" %[^\n]s", titolo);
                int i = CercaLibro(titolo);
                if (i != -1) 
                {
                    printf("Libro trovato: [Titolo]: %s - [Autore]: %s - [Anno]: %d - [Prezzo]: %.2f euro - [Categoria]: %s\n",
                           libreria[i].titolo, libreria[i].autore, libreria[i].annoPub, libreria[i].prezzo, categoriaToString[libreria[i].genere]);
                } 
                else 
                {
                    printf("Libro non trovato.\n");
                }
                printf("\n");
                break;
            case 3: 
                char categoriaStr[20];
                printf("Inserisci la categoria (Narrativa, Saggistica, Scienza, Arte, Romanzo):\n");
                scanf(" %s", categoriaStr);
                Categoria categoria = stringToCategoria(categoriaStr);
                StampaPerCategoria(categoria);
                printf("\n");
                break;
            case 0: 
                printf("Sei uscito dal programma.\n");
                break;
            default:  // Gestione di un'opzione non valida
                printf("Opzione non valida.\n");
        }

    } while(scelta != 0);

    return 0;
}
