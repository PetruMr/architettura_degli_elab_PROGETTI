/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   18/02/2021
 * 
 *  DESCRIZIONE:
 *          File contenente il main per il progetto 6.
 *          Questo programma traduce un file .vm in un file .asm.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VMtranslator_functions.h"
 
 
int main (int argc, char *argv[]) {

    int i, j; // Contatori usati nel programma

    int returnValue = 0; // 0 se va a buon fine, -1 se non va a buon fine

    if (!(argc == 2 && controlloArgv1(argv[1]))) {
        // Se gli argomenti inseriti non vanno bene, verranno ritornati degli errori
        printf("C'e stato un errore di inserimento degli argomenti!\n");
        printf(" - Dovresti inserire come primo argomento il file da compilare del formato \"*.vm\"\n");
        returnValue = -1; // -1 come return indica errore
    } else {
        char * vmFileName, * asmFileName;

        // Imposta i nomi dei file

        // Questo lo faccio per comodità, in modo da non dover usare argv[1] 
        // tutte le volte, ma un nome più significativo
        vmFileName = argv[1]; 
        
        // Alloco la memoria del nuovo nome, cioè
        // *.asm al posto di *.vm
        asmFileName = (char*) calloc(strlen(vmFileName)+2, sizeof(char));
        // copio fino al .
        for(j = 0; j < strlen(vmFileName)-2; j ++) {
            asmFileName[j] = vmFileName[j];
        }
        asmFileName[j] = 'a';
        asmFileName[j+1] = 's';
        asmFileName[j+2] = 'm';
        asmFileName[j+4] = '\0';

        // Apertura del file di input

        FILE * fileInput, * fileOutput;
                
        fileInput = fopen(vmFileName, "r");

        // Se non riesce ad aprire nulla, allora ritorna un errore
        if (fileInput == NULL) {
            printf("Il file da compilare (%s) non e' stato trovato.\n", vmFileName);
            returnValue = -1;
        } else {            
            unsigned long long currentRow = 1; // Stesso tipo di size_t, ma non size_t in quanto concettualmente sbagliato
            char * stringTempInp; // Stringa dove viene caricato l'input della riga del file
            splitString tempInp; // Conterrà un array delle stringhe che si ottengono

            /* Se non posso compilarlo e non è un commento, allora errore di compilazione*/
            bool erroreDiCompilazione = false;
            
            /* Se ho un commento multiline non chiuso, questo diventerà vero */
            bool flagOfComment = false;    

            do {
                // Prendo la riga dal file
                stringTempInp = leggiStringaDaFile(fileInput);

                if (stringTempInp != NULL && stringTempInp[0] != '\0') {

                    // Tolgo commenti
                    stringTempInp = i_eliminaCommentoMultiline(stringTempInp, &flagOfComment);    
                    stringTempInp = eliminaCommentoInLine(stringTempInp);
                    
                    // Divide la stringa in un elenco di sottostringhe separate da ' ', '\t', '\n' e '\r'
                    tempInp = splitString_creator(stringTempInp);
                    
                    // Se linea risulta vuota, vuol dire che aveva solo commenti o caratteri 
                    // "non utili", cioè quelli tolti da splitString, quindi la ignoro e 
                    // passo alla lettura di quella dopo se non sono arrivato alla fine del file
                    if (tempInp.num_of_el != 0) {
                        printf("Lettura stringa originale a riga %llu: %s", currentRow, stringTempInp);
                        for(i = 0; i < tempInp.num_of_el; i++) {
                            printf("\tLettura stringa %d di lunghezza %d: %s\n", i,strlen(tempInp.content[i]), tempInp.content[i]);
                        }
                    }

                    if (erroreDiCompilazione) {
                        // Mi fermo dalla compilazione
                        printf("Compiler error!\nAt line number %llu there is a mistake\n", currentRow);
                    }

                    free(stringTempInp);
                } else {
                    free(stringTempInp);
                }
            } while (!feof(fileInput) && !erroreDiCompilazione);

        }

        
        fclose(fileInput);

    }

    return returnValue; // 0 se va a buon fine, -1 se non va a buon fine
}