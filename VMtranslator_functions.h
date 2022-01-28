/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   18/02/2021
 * 
 *  DESCRIZIONE:
 *          File contenente le dichiarazioni per le funzioni usate 
 *          per il progetto 6. Contiene inoltre le dichiarazioni per 
 *          i tipi di dati e le struct.
*/

/* Per comodità userò i bool e le define di true come 1 e false come 0 piuttosto che gli int, gli 1 e gli 0 */
typedef int bool;
#define true 1
#define false 0

struct splitString {
    char ** content;
    int num_of_el;
}; typedef struct splitString splitString;


// Controllo dell'argv 1, il quale dovrebbe essere il nome di un file .vm
int controlloArgv1(char * argv1);

// Funzioni per leggere da file
bool controllaSeHaEndLine(char * stringa);
char * leggiStringaDaFile (FILE * inputFile);

// Pulire la stringa da spazi e altro
splitString splitString_creator (char * stringa);

// Funzioni per eliminare commenti
char * i_eliminaCommentoMultiline(char * stringa, bool * flag);
char * eliminaCommentoInLine(char * stringa);