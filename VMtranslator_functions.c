/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   18/02/2021
 * 
 *  DESCRIZIONE:
 *          File contenente le funzioni utili per la realizzazione del progetto 6.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VMtranslator_functions.h"

/**
 * @brief Controlal se l'argv 1 è stato inserito correttamente,
 * cioè se argv 1 finisce con ".vm". Ritorna vero se corretto,
 * falso se non lo è.
 * 
 * @param argv  Il primo arg
 * @return bool Vero se è corretto, falso se è sbagliato
 */
bool controlloArgv1(char * argv1) {
    int i = 0;
    bool corretto = false; 
    if(!(argv1[0] == '\0' || argv1[1] == '\0' || argv1[2] == '\0')) {
        while(argv1[i+2] != '\0' && !corretto) {
            if (argv1[i] == '.' && argv1[i+1] == 'v' && argv1[i+2] == 'm') {
                corretto = true;
            }
            i++;
        }
    }
    return corretto;
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
 * @brief Legge 10 caratteri alla volta del file in input, allocando
 * più memoria per la stringa se ancora non si ha trovato /n.
 * 
 * Mantenendo una dimensione piccola, evito di allocare troppa
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
    const int dis_num_col = 10; 
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
 * @brief Splitta una stringa, cioè rimuove i caratteri inutili per la sua
 * traduzione in assembly e la divide in tutte le stringhe che la compongono. 
 * I caratteri che "dividono" le varie sottostringhe sono ' ', '\t'. 
 * '\n' indica la fine della stringa, insieme a '\0', e viene anche questo rimosso dalla stringa.
 * 
 * Verrà quindi usata la stringa in input per creare una nuova stringa senza questi
 * caratteri. Quindi, la stringa inserita verrà cancellata dalla memoria e verrà ritornata
 * le stringhe generate dallo splitting. In questo modo non ci sono memory leak.
 * 
 * @param stringa 
 * @return splitString 
 */
splitString splitString_creator (char * stringa) {
    splitString returnValue;
    returnValue.content = NULL;
    returnValue.num_of_el = 0;

    if (stringa != NULL) {
        // Devo contare il numero di singole substring da ottenere da questa stringa
        int i = 0;

        while(stringa[i] != '\0' && stringa[i] != '\n') {
            while(stringa[i] == ' ' || stringa[i] == '\t' || stringa[i] == '\r') {
                i++;
            }
            while(stringa[i] != ' ' && stringa[i] != '\t' && stringa[i] != '\n' && stringa[i] != '\r') {
                i++;
            }
            returnValue.num_of_el++;
        }

        returnValue.content = (char **) calloc(returnValue.num_of_el, sizeof(char *));

        int singleStringSize = 0;
        int currentString = 0;
        int currentChar = 0;
        i = 0;
        while(stringa[i] != '\0' && stringa[i] != '\n') {
            while(stringa[i] == ' ' || stringa[i] == '\t' || stringa[i] == '\r') {
                i++;
            }
            singleStringSize = 0;
            while(stringa[i] != ' ' && stringa[i] != '\t' && stringa[i] != '\n' && stringa[i] != '\r') {
                i++;
                singleStringSize++;
            }
            i -= singleStringSize;
            returnValue.content[currentString] = calloc(singleStringSize+1, sizeof(char));
            currentChar = 0;
            while(stringa[i] != ' ' && stringa[i] != '\t' && stringa[i] != '\n' && stringa[i] != '\r') {
                returnValue.content[currentString][currentChar] = stringa[i];
                i++;
                currentChar++;
            }
            returnValue.content[currentString][currentChar] = '\0';
            currentString++;
        }
    }

    //free(stringa);

    return returnValue;
}