/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   10/12/2021
 * 
 *  DESCRIZIONE:
 *          .h di compiler.c, file che contine funzioni utili all'assembler
 * 
 */

// Inserisco anche qua le varie librerie standard che vado poi ad usare
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

/**
 * @brief Controlal se l'argv 1 è stato inserito correttamente,
 * cioè se argv 1 finisce con ".asm". Ritorna vero se corretto,
 * falso se non lo è.
 * 
 * @param argv  L'array contenente le stringhe di argomenti usati per chiamare la funzione
 * @return bool Vero se è corretto, falso se è sbagliato
 */
bool controlloArgv1(char *argv[]) {
    int i = 0;
    bool corretto = false; 
    if(!(argv[1][0] == '\0' || argv[1][1] == '\0' || argv[1][2] == '\0' || argv[1][3] == '\0')) {
        while(argv[1][i+3] != '\0' && !corretto) {
            if (argv[1][i] == '.' && argv[1][i+1] == 'a' && argv[1][i+2] == 's' && argv[1][i+3] == 'm') {
                corretto = true;
            }
            i++;
        }
    }
    return corretto;
}

/**
 * @brief Imposto un cinst_content, cioè gli imposto la stringa dei bits e del contenuto
 *        In quanto questa istruzione la userò come stringhe costanti.
 * 
 * @param dato      La variabile nella quale vanno inserite le due stringhe 'bits' e 'contenuto'
 * @param bits      Stringa che contiene una sequenza di 0 e 1
 * @param contenuto Stringa che contiene uno dei possibili comandi delle C inst (dest, comp o jump)
 */
void inserisci_instruction(cinst_content * dato, char * bits, char * contenuto) {
    (*dato).bits =  (char*) calloc(strlen(bits)+1, sizeof(char));
    strcpy((*dato).bits, bits);
    (*dato).contenuto =  (char*) calloc(strlen(contenuto)+1, sizeof(char));
    strcpy((*dato).contenuto, contenuto);
}

/**
 * @brief Inizializza tutte le parti di una Cinstruction, cioè la dest, la comp e il jump
 * e restituisce la struttura dati che contiene queste informazioni.
 * 
 * Queste informazioni verranno successivamente usate per tradurre le C-Instruction che 
 * si troveranno nel file.
 * 
 * @return possible_cinstruction Una struttura dati contenente tutte le parti dest, comp 
 *                               e jump e le loro traduzioni 
 */
possible_cinstruction initializeCInst() {
    // alloco lo spazio dei vari array di dati usando le costanti del numero di possibili inst.
    possible_cinstruction mapOfInstructions;
    mapOfInstructions.dest = (cinst_content *) calloc(NUMOFINST_DEST, sizeof(cinst_content));
    mapOfInstructions.comp = (cinst_content *) calloc(NUMOFINST_COMP, sizeof(cinst_content));
    mapOfInstructions.jump = (cinst_content *) calloc(NUMOFINST_JUMP, sizeof(cinst_content));

    // le "dest"
    int i = 0;
    inserisci_instruction(&mapOfInstructions.dest[i++],"000", "");
    inserisci_instruction(&mapOfInstructions.dest[i++],"001", "M=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"010", "D=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"011", "MD=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"100", "A=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"101", "AM=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"110", "AD=");
    inserisci_instruction(&mapOfInstructions.dest[i++],"111", "AMD=");

    // le "jump"
    i = 0;
    inserisci_instruction(&mapOfInstructions.jump[i++],"000", "");
    inserisci_instruction(&mapOfInstructions.jump[i++],"001", ";JGT");
    inserisci_instruction(&mapOfInstructions.jump[i++],"010", ";JEQ");
    inserisci_instruction(&mapOfInstructions.jump[i++],"011", ";JGE");
    inserisci_instruction(&mapOfInstructions.jump[i++],"100", ";JLT");
    inserisci_instruction(&mapOfInstructions.jump[i++],"101", ";JNE");
    inserisci_instruction(&mapOfInstructions.jump[i++],"110", ";JLE");
    inserisci_instruction(&mapOfInstructions.jump[i++],"111", ";JMP");

    // le "comp"
    i = 0;
    inserisci_instruction(&mapOfInstructions.comp[i++],"0101010", "0");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0111111", "1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0111010", "-1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0001100", "D");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0110000", "A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0001101", "!D");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0110001", "!A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0001111", "-D");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0110011", "-A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0011111", "D+1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0110111", "A+1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0001110", "D-1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0110010", "A-1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0000010", "D+A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0010011", "D-A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0000111", "A-D");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0000000", "D&A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"0010101", "D|A");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1110000", "M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1110001", "!M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1110011", "-M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1110111", "M+1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1110010", "M-1");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1000010", "D+M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1010011", "D-M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1000111", "M-D");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1000000", "D&M");
    inserisci_instruction(&mapOfInstructions.comp[i++],"1010101", "D|M");

    return mapOfInstructions;
}

/**
 * @brief Inizializzo l'elenco di address con le address standard, cioè: Inizializzazione degli address predefiniti:
 *      -   SP      0       0x0000
 *      -   LCL     1       0x0001
 *      -   ARG     2       0x0002
 *      -   THIS    3       0x0003
 *      -   THAT    4       0x0004
 *      -   R0-R15  0-15    0x0000-0x0010
 *      -   SCREEN  16384   0x4000
 *      -   KBD     24576   0x6000  
 * 
 * Potrei averlo fatto più corto usando "aggiungiLabel()", ma ormai l'avevo
 * scritto prima di fare la funzione, quindi l'ho lasciato così.
 * 
 * @return elenco_address L'elenco di address, con le address standard inizializzate
 */
elenco_address createStandardAddress() {
    elenco_address addressdef;
    
    addressdef.curvar = 16;
    addressdef.len = 23;
    addressdef.elenco = (name_and_address * ) calloc(addressdef.len, sizeof(name_and_address));
    
    int i = 0;
    // SP - 0
    addressdef.elenco[i].address = 0;
    addressdef.elenco[i].name = (char *) calloc(strlen("SP")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "SP");
    // LCL - 1
    addressdef.elenco[i].address = 1;
    addressdef.elenco[i].name = (char *) calloc(strlen("LCL")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "LCL");
    // ARG - 2
    addressdef.elenco[i].address = 2;
    addressdef.elenco[i].name = (char *) calloc(strlen("ARG")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "ARG");
    // THIS - 3
    addressdef.elenco[i].address = 3;
    addressdef.elenco[i].name = (char *) calloc(strlen("THIS")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "THIS");
    // THAT - 4
    addressdef.elenco[i].address = 4;
    addressdef.elenco[i].name = (char *) calloc(strlen("THAT")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "THAT");
    // SCREEN - 16384
    addressdef.elenco[i].address = 16384;
    addressdef.elenco[i].name = (char *) calloc(strlen("SCREEN")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "SCREEN");
    // KBD - 24576
    addressdef.elenco[i].address = 24576;
    addressdef.elenco[i].name = (char *) calloc(strlen("KBD")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "KBD");

    // R0 - R15 - 0 - 15
    addressdef.elenco[i].address = 0;
    addressdef.elenco[i].name = (char *) calloc(strlen("R0")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R0");
    
    addressdef.elenco[i].address = 1;
    addressdef.elenco[i].name = (char *) calloc(strlen("R1")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R1");

    addressdef.elenco[i].address = 2;
    addressdef.elenco[i].name = (char *) calloc(strlen("R2")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R2");

    addressdef.elenco[i].address = 3;
    addressdef.elenco[i].name = (char *) calloc(strlen("R3")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R3");    

    addressdef.elenco[i].address = 4;
    addressdef.elenco[i].name = (char *) calloc(strlen("R4")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R4");

    addressdef.elenco[i].address = 5;
    addressdef.elenco[i].name = (char *) calloc(strlen("R5")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R5");

    addressdef.elenco[i].address = 6;
    addressdef.elenco[i].name = (char *) calloc(strlen("R6")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R6"); 

    addressdef.elenco[i].address = 7;
    addressdef.elenco[i].name = (char *) calloc(strlen("R7")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R7");

    addressdef.elenco[i].address = 8;
    addressdef.elenco[i].name = (char *) calloc(strlen("R8")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R8");

    addressdef.elenco[i].address = 9;
    addressdef.elenco[i].name = (char *) calloc(strlen("R9")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R9");

    addressdef.elenco[i].address = 10;
    addressdef.elenco[i].name = (char *) calloc(strlen("R10")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R10");

    addressdef.elenco[i].address = 11;
    addressdef.elenco[i].name = (char *) calloc(strlen("R11")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R11");

    addressdef.elenco[i].address = 12;
    addressdef.elenco[i].name = (char *) calloc(strlen("R12")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R12");

    addressdef.elenco[i].address = 13;
    addressdef.elenco[i].name = (char *) calloc(strlen("R13")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R13");

    addressdef.elenco[i].address = 14;
    addressdef.elenco[i].name = (char *) calloc(strlen("R14")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R14");

    addressdef.elenco[i].address = 15;
    addressdef.elenco[i].name = (char *) calloc(strlen("R15")+1, sizeof(char));
    strcpy(addressdef.elenco[i++].name, (char *) "R15");

    return addressdef;
}

/**
 * @brief Aggiunge un nuovo label all'elenco degli indirizzi nominati
 * 
 * @param addressdef    L'elenco al quale aggiungere l'indirizzo
 * @param nome          Il nome del label
 * @param indirizzo     Il nome dell'indirizzo
 */
void aggiungiLabel(elenco_address * addressdef, char * nome, int indirizzo) {
    (*addressdef).elenco = (name_and_address *) realloc((*addressdef).elenco, ((*addressdef).len+1) * sizeof(name_and_address));
    (*addressdef).elenco[(*addressdef).len].address = indirizzo;
    (*addressdef).elenco[(*addressdef).len].name = (char *) calloc(strlen(nome)+1, sizeof(char));
    strcpy((*addressdef).elenco[(*addressdef).len++].name, nome); // incremento len qua
}

/**
 * @brief Aggiungo una nuova variabile all'elenco degli indirizzi nominati.
 * Userò l'attributo della struttura "curvar" e lo andrò a incrementare dopo
 * aver inserito la variabile.
 * 
 * @param addressdef    L'elenco al quale aggiunge la variabile
 * @param nome          Il nome della variabile
 */
void aggiungiVariabile(elenco_address * addressdef, char * nome) {
    (*addressdef).elenco = (name_and_address *) realloc((*addressdef).elenco, ((*addressdef).len+1) * sizeof(name_and_address));
    (*addressdef).elenco[(*addressdef).len].address = (*addressdef).curvar++; // Incremento il contantore del numero della variabile attuale
    (*addressdef).elenco[(*addressdef).len].name = (char *) calloc(strlen(nome)+1, sizeof(char));
    strcpy((*addressdef).elenco[(*addressdef).len++].name, nome); // incremento len qua
}


/**
 * @brief Ritorna l'indirizzo di un determinato label o di una variabile
 * già salvata precedentemente.
 * Se non è esistente nell'elenco di address, allora ritornerà -1;
 * 
 * @param addressdef    L'elenco nel quale cerca l'indirizzo
 * @param nome          Il nome dell'indirizzo
 * @return int          L'indirizzo con il nome inserito. -1 se non esistente.
 */
int ottieniAddress(elenco_address addressdef, char * nome) {
    int i = 0;
    int returnValue = -1;
    while(i < addressdef.len && returnValue == -1) {
        if (strcmp(addressdef.elenco[i].name, nome) == 0) {
            returnValue = addressdef.elenco[i].address;
        }
        i++;
    }
    return returnValue;
}



/**
 * @brief "Pulisce" una stringa, cioè rimuove i caratteri inutili per la sua
 * traduzione in codice macchina. Questi caratteri sono ' ', '\t' e '\n'.
 * 
 * Verrà quindi usata la stringa in input per creare una nuova stringa senza questi
 * caratteri. Quindi, la stringa inserita verrà cancellata dalla memoria e verrà ritornata
 * la stringa pulita dalla funzione. In questo modo non ci sono memory leak.
 * 
 * Questo tipo di operazioen viene fatta anche dal CPUEmulator quando viene inserito
 * del codice .asm. Infatti se noi inseriamo (PRO VA), questo sarà considerato
 * uguale a (PROVA) successivamente. Inoltre @PRO VA e @PROVA sono identici una
 * volta che si trovano nel CPUEmulator, cioè andranno a puntare allo stesso 
 * indirizzo.
 * 
 * In generale, in quanto dopo la compilazione che esegue il CPUEmulator del .asm
 * gli spazi non vengono considerati, anche io faccio altrettanto.
 * 
 * @param stringa   La stringa che deve venire "pulita". Verrà liberata dalla memoria.
 * @return char*    La stringa "pulita".
 */
char * pulisciString (char * stringa) {
    // Se la stringa è nulla, non c'è nulla da pulire
    if (stringa != NULL) {
        char * tmp;     // La nuova stringa da creare
        int counter = 0;    // Contatore del numero di caratteri non "inutili" della stringa
        int i = 0;
        // Conto i caratteri utili della stringa
        while (stringa[i] != '\0') {
            if (stringa[i] != ' ' && stringa[i] != '\t' && stringa[i] != '\n' && stringa[i] != '\r') {
                counter++;
            }
            i++;
        }
        // Alloco la giusta quantità di spazio per la stringa. +1 per \0
        tmp = (char*) calloc(counter+1, sizeof(char));
        i =0;
        int j = 0;
        // Copio solo i caratteri utili dentro la nuova stringa tmp;
        while (stringa[i] != '\0') {
            if (stringa[i] != ' ' && stringa[i] != '\t' && stringa[i] != '\n' && stringa[i] != '\r') {
                tmp[j] = stringa[i];
                j++;
            }
            i++;
        }
        tmp[j] = '\0';
        // Libero lo spazio occupato dalla stringa in input.
        // Lo faccio qua per evitare giri strani, e necessari, quando chiamo la funzione
        free(stringa); 
        stringa = NULL;
        return tmp;
    }
    return NULL;
}


/**
 * @brief Controlla se la stringa ha il carattere \n e restituisce true
 * se lo trova, false se non lo trova.
 * 
 * @param stringa La stringa da controllare
 * @return bool   Se contiene \n (true) oppure no (false)  
 */
bool controllaSeHaEndLine(char * stringa) {
    int i = 0;
    bool contieneEndOfLine = false;
    while (stringa[i] != '\0' && !contieneEndOfLine) {
        if (stringa[i] == '\n') {
            contieneEndOfLine = true;
        }
        i++;
    }
    return contieneEndOfLine;
}

/**
 * @brief Legge 5 caratteri alla volta del file in input, allocando
 * più memoria per la stringa se ancora non si ha trovato /n. 
 * 
 * Scelgo proprio 5 caratteri alla volta perché ho notato che l'istruzione
 * più comune che ho usato è una C instruction del tipo "X=Y" che,
 * includendo '\n' e '\0', sono 5 caratteri.
 * 
 * Inoltre, mantenendo una dimensione piccola, evito di allocare troppa
 * memoria non effettivamente utilizzata, anche se vado a sacrificare
 * la velocità di esecuzione.
 * 
 * Successivamente, quando la stringa verrà pulita, andrò a rendere
 * la stringa che leggo dell'esatta dimensione che mi è necessaria
 * 
 * @param inputFile Il file dal quale leggere le stringhe
 * @return char*    La stringa che ha appena letto dal file
 */
char * leggiStringaDaFile (FILE * inputFile) {
    const int dis_num_col = 5; 
    int cur_num_col = dis_num_col;
    char * stringInput = (char*) calloc(cur_num_col, sizeof(char));
    stringInput[0] = '\0';
    char * tmpInput = (char*) calloc(dis_num_col, sizeof(char));
    tmpInput[0] = '\0';

    bool trovatoEOL = false; // Trovato end of line
    bool trovatoEOF = false; // Se end of file 
    do {
        if (!feof(inputFile)) {
            free(tmpInput);
            tmpInput = (char*) calloc(dis_num_col, sizeof(char));
            fgets(tmpInput, dis_num_col, inputFile);
            stringInput = (char *) realloc(stringInput, cur_num_col * sizeof(char));
            strcat(stringInput, tmpInput);
            trovatoEOL = controllaSeHaEndLine(stringInput); 
            if (!trovatoEOL) {
                cur_num_col += dis_num_col;
            }
        } else {
            // Se ho trovato EOF, allora concludo la lettura della stringa
            trovatoEOF = true;
        }            
    } while (!trovatoEOL && !trovatoEOF);

    free(tmpInput);
    return stringInput;
}



/**
 * @brief pulisce la stringa da commenti multilinea.
 * Se il flag in input è true, allora vuol dire che c'è stato un commento
 * multilinea precedentemente iniziato e ancora non concluso.
 * 
 * Il flag diventa true se non si chiude l'ultimo commento, o se non ci 
 * sono in generale i caratteri dei commenti ed era true prima. 
 * 
 * @param stringa   la stringa da pulire
 * @param flag      vero se c'è un commento non chiuso nella stessa riga 
 * @return char*    la stringa senza commenti multilinea
 */
char * i_eliminaCommentoMultiline(char * stringa, bool * flag) {

    char * tmp = (char *) calloc(strlen(stringa)+1, sizeof(char));
    int i = 0, j = 0;
    while (stringa[i] != '\0') {
        if (!(*flag)) {
            if (stringa[i+1] != '\0' && stringa[i] == '/' && stringa[i+1] == '*' ) {
                *flag = true;
                i++;
            } else {
                tmp[j] = stringa[i];
                j++;
            }
        } else if (stringa[i] == '*' && stringa[i+1] == '/') {
            *flag = false;
            i++;
        }
        i++;
    }

    // Metto il carattere terminatore in tmp
    tmp[j] = '\0';
    free(stringa);

    // Riutilizzo la stringa inserita in input come returnvalue
    // In modo da poter allocare la memoria esatta necessaria alla stringa
    stringa = (char*) calloc(strlen(tmp)+1, sizeof(char));
    strcpy(stringa, tmp);
    free(tmp);

    return stringa;
}

/**
 * @brief Elimina il commento inline che si trova nella stringa inserita.
 * Questo vuol dire che ritornerà tutta la stringa che si trova prima dei
 * due caratteri //. Dopo averli incontrati, libererà la memoria della vecchia
 * stringa e allocherà la giusta quantità per la nuova stringa.
 * 
 * Infine, ritornerà la stringa senza il commento inline.
 * 
 * Se non ci fosse un commento inline, non eliminerà nulla e ritorna
 * la stringa così come inserita, però riallocata.
 * 
 * @param stringa   la stringa da "pulire"
 * @return char*    la stringa pulita da commenti in-line
 */
char * eliminaCommentoInLine(char * stringa) {
    int i = 0;
    char * tmp = (char *) calloc (strlen(stringa)+1, sizeof(char));
    bool trovatoInizio = false;
    while(stringa[i] != '\0' && !trovatoInizio) {
        if(stringa[i+1] != '\0') {
            if (stringa[i] == '/' && stringa[i+1] == '/') {
                trovatoInizio = true;
            } else {
                tmp[i] = stringa[i];
            }
        } else {
            tmp[i] = stringa[i];
        }
        i++;
    }
    tmp[i] = '\0';
    free(stringa);
    stringa = (char *) calloc(strlen(tmp)+1, sizeof(char));
    strcpy(stringa, tmp);
    free(tmp);
    return stringa;
}

/**
 * @brief Controlla se la stringa inserita come input è una L instructioc,
 * cioè se indica un label. Uso la diccitura Linstruction per comodità.
 * Il controllo consiste nel verificare se la stringa è del formato:
 * '(' + [...] + ')'
 *      (Dove + indica concatenazione e 
 *      [...] qualsiasi carattere/insieme di caratteri valido,
 *      cioè da 'a' a 'z', da 'A' a 'Z', da '0' a '9' e '$','_' e '.')
 * 
 * @param stringa   una stringa che si vuole verificare sia una Linstruction
 * @return true     Se è una Linstruction valida
 * @return false    Se non è una Linstruction valida
 */
bool isLInstruction(char * stringa) {
    bool returnValue = true;
    // Il minimo per un label sono (*), quindi deve avere i caratteri almeno i primi 3 caratteri
    if(stringa[0] != '\0' && stringa[1] != '\0' && stringa[2] != '\0') {
        if (stringa[0] == '(' && stringa[strlen(stringa)-1] == ')') {
            int i;
            if (!(stringa[1]<='9' || stringa[1]>='0')) {
                returnValue = false;
            }
            for(i = 1; i < strlen(stringa)-1; i++) {
                if (!((stringa[i] <= 'Z' && stringa[i] >= 'A') || (stringa[i] <= 'z' && stringa[i] >= 'a') || (stringa[i] <= '9' && stringa[i] >= '0') || stringa[i] == '_' || stringa[i] == '.' || stringa[i] == '$')) {
                    returnValue = false;
                }
            }
        } else {
            returnValue = false;
        }
    } else {
        returnValue = false;
    }
    return returnValue;
}

/**
 * @brief Restituisce la scritta contenuta in un label.
 * Questa funzione da per scontato che il label inserito per input sia 
 * scritto con una sintassi correnta, cioè '(' + [...] + ')'.  
 * //  Per capire la sintassi del "corretto", vedere la funzione isLInstruction
 * 
 * @param stringa Il label
 * @return char*  Il label senza parentesi
 */
char * getLabel(char * stringa) {
    int i = 1;
    char * tmp = (char *) calloc(strlen(stringa)-1, sizeof(char));
    while(stringa[i] != ')') {
        tmp[i-1] = stringa[i];
        i++;
    }
    tmp[i-1] = '\0';
    free(stringa);
    return tmp;
}

/**
 * @brief Ritorna vero se la stringa in input è una A-instruction, cioè del formato:
 * @ + [...]    (Dove + è la concatenazione e [...] indica qualsiasi carattere
 *               o insieme di caratteri possibili, cioè da 'a' a 'z', da 'A' a 'Z', 
 *               da '0' a '9' e '$','_' e '.').
 * 
 * Nel caso dopo @ ci fosse un numero, darò un errore se a seguire non ci sono
 * solo numeri, in quanto @123ABC non dovrebbe essere possibile, ma solo @ABC123
 * 
 * @param stringa   La stringa da controllare
 * @return true     Se ha una sintassi corretta per una A instruction
 * @return false    Se non ha una sintassi corretta per una A instruction
 */
bool isAInstruction(char * stringa) {
    bool returnValue = true;
    bool variabileolabel = false;
    if (strlen(stringa) > 0) {
        if(stringa[0] == '@' && stringa[1] != '\0') {
            int i;
            // Se il primo carattere è un numero, allora il resto deve essere un numero. Sennò è un label o una variabile
            if (!(stringa[1]<='9' && stringa[1]>='0')) {
                variabileolabel = true;
            }
            if (variabileolabel) {
                for(i = 1; i < strlen(stringa); i++) {
                    if (!((stringa[i] <= 'Z' && stringa[i] >= 'A') || (stringa[i] <= 'z' && stringa[i] >= 'a') || (stringa[i] <= '9' && stringa[i] >= '0') || stringa[i] == '_' || stringa[i] == '.' || stringa[i] == '$')) {
                        returnValue = false;
                    }
                }
            } else {
                for(i = 1; i < strlen(stringa); i++) {
                    if (!(stringa[i] <= '9' && stringa[i] >= '0')) {
                        returnValue = false;
                    }
                }
            }            
        } else {
            returnValue = false;
        }
    } else {
        returnValue = false;
    }
    return returnValue;
}

/**
 * @brief Ritorna la stringa in input senza @ iniziale.
 * Si considera che la stringa in input è una AInstruction valida, cioè
 * già precedentemente verificata.
 * 
 * "stringa" verrà liberata e verrà restituita una nuova stringa 
 * della dimensione giusta.
 * 
 * @param stringa   L'AInstruction dove togliere l'@ iniziale
 * @return char*    La stringa senza l'@ iniziale, con un numero preciso di bytes allocati
 */
char * getAInstruction(char * stringa) {
    char * tmp = (char *) calloc(strlen(stringa), sizeof(char));
    int i = 0;
    while(stringa[i] != '\0') {
        tmp[i] = stringa[i+1];
        i++;
    }
    free(stringa);
    return tmp;
}

/**
 * @brief Traduco un valore intero in una stringa che rappresenta 
 * i suoi bit. Questa stringa di bit sarà lunga 16, cioè avrà 15 
 * caratteri che sarnno 0 o 1 e il carattere terminatore, cioè '\0'.
 * 
 * Se necessita più di 15 numeri per essere rappresentato in binario,
 * la conversione risulterà in un overflow, cioè, per esempio, il 
 * numero 32768, che in binario è 1000000000000000, verrà convertito
 * nella stringa "000000000000000".
 * 
 * Inoltre, se il numero tradotto necessita meno cifre di 15 per
 * la sua rappresentazione in binario, allora verranno aggiunti 0 
 * dopo la cifra più significativa nella stringa.
 * 
 * La stringa avrà la cifra più significativa a sinistra.
 * 
 * @param n         L'intero da convertire in binario
 * @return char*    Stringa che rappresenta l'intero in binario
 */
char * intToBinary(int n) {
    char * tmp = (char *) calloc (16, sizeof(char));
    int i = 15;
    tmp[i] = '\0';
    for(i = i-1; i >= 0; i--) {
        tmp[i] = (n%2) + 48;
        n /= 2;
    }
    return tmp;
}

/**
 * @brief Ritorna true se la stringa inserita in input
 * ha solo caratteri compresi tra 0 e 9, oltre al carattere
 * terminatore, cioè '\0'.
 * 
 * @param stringa   La stringa da controllare
 * @return true     Se ha solo le cifre da 0 a 9 prima del '\0'
 * @return false    Se ha altri caratteri oltre a 0 e 9 prima del '\0'
 */
bool stringIsNumber(char * stringa) {
    int i = 0;
    bool returnValue = true;
    while(stringa[i] != '\0') {
        if(!(stringa[i] >= '0' && stringa[i] <= '9')) {
            returnValue = false;
        }
        i++;
    }
    return returnValue;
}

/**
 * @brief Converte la stringa inserita in un intero.
 * Si considera che la stringa in input sia un numero, cioè
 * sia già stata precedentemente controllata prima di essere 
 * stata inserita come input in questa funzione.
 * 
 * @param stringa   La stringa da convertire 
 * @return int      L'intero che si trovava nella stringa
 */
int stringToInt(char * stringa) {
    int i = 0;
    int temp = 0; // La variabile che verrà usata per contenere il nunero calcolato
    while(stringa[i] != '\0') {
        int j, pow = 1;
        // La cifra che sto guardando, sarà corrispondente a cifra * 10 ^ (qualcosa)
        // quel qualcosa è "l'inverso" di i rispetto alla lunghezza della stringa.
        for(j = 0; j < (strlen(stringa) - i - 1); j++) {
            pow*=10;
        }
        // Compongo il numero finale
        temp += (stringa[i] - 48)*pow;

        i++;
    }
    return temp;
}

/**
 * @brief Inserendo le address di cui si conosce un nume e una stringa
 * che rappresenta uno di questi nomi, ottengo i bits che compongono la
 * AInstruction come return value.
 * 
 * @param addressdef    Elenco di address di cui conosco un nome
 * @param stringa       Stringa che rappresenta un nome di un address
 * @return char*        I bits della AInstruction
 */
char * ottieniAInstBits(elenco_address addressdef, char * stringa) {
    char * tmp = (char *) calloc (17, sizeof(char));
    // Metto lo 0 prima degli altri 15 bit
    tmp[0] = '0';
    tmp[1] = '\0';
    // Ottengo i restanti bit
    char * bits =intToBinary(ottieniAddress(addressdef, stringa));
    // Concateno le due stringhe
    strcat(tmp, bits);
    free(bits);
    return tmp;
}

/**
 * @brief Verifica se una stringa letta è uan Cinstruction possibile, cioè
 * se è possibile comporla usando le CInstruction che ho salvato.
 * 
 * Per eseguire questo controllo, quindi, andrò a fare tutte le combinazioni possibili
 * di dest + comp + jump. Se una di queste combinazioni è uguale alla stringa inserita
 * come parametro, allora risulta essere una C instruction possibile, quindi ritornerò
 * true. Sennò, se non verrà trovata alcuna combinazione uguale alla stringa in input,
 * ritornerò false.              
 * 
 * @param listofpossible    Struttura dati che contiene tutte le possible componenti di CInst.
 * @param stringa           La stringa da controllare se sia una Cinst.
 * @return true             Se è una CInst possibile
 * @return false            Se non è una CInst possibile
 */
bool isCInstruction(possible_cinstruction listofpossible, char * stringa) {
    int i, j, k;
    bool trovato = false;
    i = 0;
    char * comparable = (char *) calloc(15, sizeof(char));
    while(i < NUMOFINST_DEST && !trovato) {
        j = 0;
        while(j < NUMOFINST_COMP && !trovato) {
            k = 0;
            while(k < NUMOFINST_JUMP && !trovato) {
                comparable[0] = '\0';
                strcat(comparable, listofpossible.dest[i].contenuto);
                strcat(comparable, listofpossible.comp[j].contenuto);
                strcat(comparable, listofpossible.jump[k].contenuto);
                if(strcmp(comparable, stringa) == 0) {
                    trovato = true;
                }
                k++;
            }
            j++;
        }
        i++;
    }
    free(comparable);

    return trovato;
}

/**
 * @brief Traduce la C instruction "stringa", che si considera essere valida,
 * nei bit della sua traduzione in machine language. Questo avviene provando
 * tutte le combinazioni di dest, comp e jump e poi, se una combinazione è uguale
 * alla stringa inserita, imposterà la stringa che contiene i bit ordinati secondo 
 * come devono venire stampati.
 * 
 * Ritornerà quindi i bit a cui la Cinstruction "stringa" corrisponde sotto forma 
 * di stringa di dimensione 17, cioè di 16 bit e il carattere terminatore '\0'.
 * 
 * La stringa si considera valida in quanto è stata precedentemente controllata
 * nel main.
 * 
 * @param listofpossible Struttura dati che contiene le possibili dest, comp e jump
 * @param stringa        La Cinstruction da tradurre
 * @return char*         Stringa che contiene i bit a cui corrisponde la Cinst inserita
 */
char * ottieniCInstBits(possible_cinstruction listofpossible, char * stringa) {
    int i, j, k;
    bool trovato = false;
    char * tmp = (char *) calloc (17, sizeof(char));
    tmp[0] = '1';
    tmp[1] = '1';
    tmp[2] = '1';
    tmp[3] = '\0';
    i = 0;
    char * comparable = (char *) calloc(15, sizeof(char));
    while(i < NUMOFINST_DEST && !trovato) {
        j = 0;
        while(j < NUMOFINST_COMP && !trovato) {
            k = 0;
            while(k < NUMOFINST_JUMP && !trovato) {
                comparable[0] = '\0';
                strcat(comparable, listofpossible.dest[i].contenuto);
                strcat(comparable, listofpossible.comp[j].contenuto);
                strcat(comparable, listofpossible.jump[k].contenuto);
                if(strcmp(comparable, stringa) == 0) {
                    trovato = true;
                    strcat(tmp, listofpossible.comp[j].bits);
                    strcat(tmp, listofpossible.dest[i].bits);
                    strcat(tmp, listofpossible.jump[k].bits);
                }
                k++;
            }
            j++;
        }
        i++;
    }
    free(comparable);
    return tmp;
}
