/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   10/12/2021
 * 
 *  DESCRIZIONE:
 *          .h di compiler.c, file che contine funzioni utili all'assembler
 * 
 *          Contiene le dichiarazioni dei tipi di dati, dei define utili ovunque e 
 *          delle funzioni.
 */

/* Per comodità userò i bool e le define di true e false piuttosto che gli int, gli 1 e gli 0 */
typedef int bool;
#define true 1
#define false 0

/* Struct usata per label e address */
struct name_and_address {
    char * name;
    int address;
}; typedef struct name_and_address name_and_address;

struct elenco_address {
    name_and_address * elenco;
    int len;
    int curvar;
}; typedef struct elenco_address elenco_address;


/* Variabili per salvare le c instruction possibili */
#define NUMOFINST_DEST 8
#define NUMOFINST_COMP 28
#define NUMOFINST_JUMP 8

struct cinst_content {
    char * contenuto;   // L'istruzione effettiva
    char * bits;        // i bit a cui l'istruzione corrisponde
}; typedef struct cinst_content cinst_content;

struct possible_cinstruction {
    cinst_content* dest;
    cinst_content* comp;
    cinst_content* jump; 
}; typedef struct possible_cinstruction possible_cinstruction;

// Funzioni per inizializzare le Cinst
void inserisci_instruction(cinst_content * dato, char * bits, char * contenuto);
possible_cinstruction initializeCInst();

// Controllo dell'argv 1, il quale indica .asm
int controlloArgv1(char *argv[]);

//  Funzioni per le address, cioè per tutto quello che ha corrispondenze tra un nome e un indirizzo
elenco_address createStandardAddress(); 
void aggiungiLabel(elenco_address * addressdef, char * nome, int indirizzo); 
void aggiungiVariabile(elenco_address * addressdef, char * nome); 
int ottieniAddress(elenco_address addressdef, char * nome); 

// Funzioni per leggere da file
bool controllaSeHaEndLine(char * stringa);
char * leggiStringaDaFile (FILE * inputFile);

// Pulire la stringa da spazi e altro
char * pulisciString (char * stringa);

// Funzioni per eliminare commenti
char * i_eliminaCommentoMultiline(char * stringa, bool * flag);
char * eliminaCommentoInLine(char * stringa);

// Funzioni utili a "capire" se si tratta di, e per tradurre le, istruzioni di vario tipo
bool isLInstruction(char * stringa);
char * getLabel(char * stringa); 
bool isAInstruction(char * stringa);
char * getAInstruction(char * stringa);
char * intToBinary(int a); 
bool stringIsNumber(char * stringa);
int stringToInt(char * stringa);
char * ottieniAInstBits(elenco_address addressdef, char * stringa);
bool isCInstruction(possible_cinstruction listofpossible, char * stringa);
char * ottieniCInstBits(possible_cinstruction listofpossible, char * stringa);
