/*
 *  NOME:   Petru Marcel Marincas 
 *  CORSO:  Architettura degli elaboratori
 *  DATA:   10/12/2021
 * 
 *  DESCRIZIONE:
 *          Programma che compila i file asm in file hack per nand2tetris
 * 
 */

/*
    ALTRE CARATTERISTICHE

    Il programma dovrebbe eseguito con:

        ./compiler *.asm
    
    Il primo argomento fa riferimento al file da compilare
        Deve avere necessariamente ".asm" come estensione


    NOTA: Funzioni consentite per le stringhe:
            strlen, strcpy, strcat e strcmp

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./compiler.h"

int main (int argc, char *argv[]) {

    int i,j; // Contatori, dichiaro ora per non doverli dichiarare dopo   
    
    int returnValue = 0;

    char * asmFileName, * hackFileName;

    if (!(argc == 2 && controlloArgv1(argv))) {
        // Se gli argomenti inseriti non vanno bene, verranno ritornati degli errori
        printf("C'e stato un errore di inserimento degli argomenti!\n");
        printf(" - Dovresti inserire come primo argomento il file da compilare del formato \"*.asm\"\n");
        returnValue = -1; // -1 come return indica errore
    } else {
        // Imposta i nomi dei file
        asmFileName = argv[1];

        // Alloco la memoria del nuovo nome, cioè
        // *.hack al posto di *.asm
        hackFileName = (char*) calloc(strlen(asmFileName)+2, sizeof(char));
        // copio fino al .
        for(j = 0; j < strlen(asmFileName)-3; j ++) {
            hackFileName[j] = asmFileName[j];
        }
        hackFileName[j] = 'h';
        hackFileName[j+1] = 'a';
        hackFileName[j+2] = 'c';
        hackFileName[j+3] = 'k';
        hackFileName[j+4] = '\0';

        // Apertura del file di input per la prima passata

        FILE * fileInput, * fileOutput;
        
        fileInput = fopen(asmFileName, "r");

        // Se non riesce ad aprire nulla, allora ritorna un errore
        if (fileInput == NULL) {
            printf("Il file da compilare (%s) non e' stato trovato.\n", asmFileName);
            returnValue = -1;
        } else {
            // Adesso che sappiamo che il nome del file è corretto
            // E lo abbiamo aperto e sappiamo che esiste
            // Si passa alla compilazione

            // Inizializzazione di dati utili
            elenco_address addressdef = createStandardAddress();
            possible_cinstruction cinstructions = initializeCInst();

            // Contatore della riga "effettiva", cioè quella del risultato .hack
            size_t numRows = 0;
            unsigned long long currentRow = 1; // Stesso tipo di size_t, ma non size_t in quanto concettualmente sbagliato
            char * stringTempInp; // forse da togliere malloc(1)
            
            /* Se non posso compilarlo e non è un commento, allora errore di compilazione*/
            bool erroreDiCompilazione = false;
            
            /* Se ho un commento multiline non chiuso, questo diventerà vero */
            bool flagOfComment = false;

            // Prima passata
            // Lettura e aggiunta dei label

            do {
                // Prendo la riga dal file
                stringTempInp = leggiStringaDaFile(fileInput);

                currentRow++;
                if (stringTempInp != NULL && stringTempInp[0] != '\0') {

                    // Tolgo commenti
                    stringTempInp = i_eliminaCommentoMultiline(stringTempInp, &flagOfComment);    
                    stringTempInp = eliminaCommentoInLine(stringTempInp);
                    
                    // Gli tolgo eventuali ' ', '\t', '\n' e '\r'
                    stringTempInp = pulisciString(stringTempInp);
                    
                    // Se linea risulta vuota, vuol dire che aveva solo commenti o caratteri 
                    // "non utili", cioè quelli tolti da pulsciString, quindi la ignoro e 
                    // passo alla lettura di quella dopo se non sono arrivato alla fine del file
                    if (stringTempInp[0] != '\0') {
                        if(isLInstruction(stringTempInp)) {
                            // Imposta tabella L instruction
                            stringTempInp = getLabel(stringTempInp);
                            if(ottieniAddress(addressdef, stringTempInp) == -1) {
                                aggiungiLabel(&addressdef, stringTempInp, numRows);
                            }

                            // Clean della stringa per indicare che non va aggiunta all'addressdef.elenco delle stringhe
                            // lette dal file successivamente.
                            free(stringTempInp);
                            stringTempInp = (char *) calloc(1, sizeof(char));
                            stringTempInp[0] = '\0';
                        } else if (isAInstruction(stringTempInp) || isCInstruction(cinstructions, stringTempInp)) {
                            numRows++;
                        } else {
                            // Se la stringa non è vuota, ma non è neanche un label, un a Ainst o una Cinst, allora è un errore
                            erroreDiCompilazione = true;

                            free(stringTempInp);
                            stringTempInp = (char *) calloc(1, sizeof(char));
                            stringTempInp[0] = '\0';
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
            

            // Seconda passata, dove compila
            if (!erroreDiCompilazione) {
                // Riapro il file
                fclose(fileInput);
                fileInput = fopen(asmFileName, "r");
                fileOutput = fopen(hackFileName, "w");  

                // Rileggo linea per linea fino alla fine
                do {
                    // Prendo la riga dal file
                    stringTempInp = leggiStringaDaFile(fileInput);

                    if (stringTempInp != NULL && stringTempInp[0] != '\0') {

                        stringTempInp = i_eliminaCommentoMultiline(stringTempInp, &flagOfComment);    
                        stringTempInp = eliminaCommentoInLine(stringTempInp);
                        
                        // Gli tolgo eventuali ' ', '\t', '\n' e '\r'
                        stringTempInp = pulisciString(stringTempInp);

                        if (stringTempInp[0] != '\0') {
                            if(isLInstruction(stringTempInp)) {

                                // Clean della stringa per indicare che non va aggiunta all'addressdef.elenco delle stringhe
                                // lette dal file successivamente.
                                free(stringTempInp);
                                stringTempInp = (char *) calloc(1, sizeof(char));
                                stringTempInp[0] = '\0';
                            } else if (isAInstruction(stringTempInp)) {
                                // Traduci stringTempInp to bits
                                stringTempInp = getAInstruction(stringTempInp); // Rimuovo @
                                // Se non è presente nell'elenco di quelle già salvate
                                if(ottieniAddress(addressdef, stringTempInp) == -1) { 
                                    // Devo capire se aggiungerla come variabile o se già indica un indrizzo di memoria
                                    if(stringIsNumber(stringTempInp)) {
                                        // Se è già un numero, allora la traduco nei bit dell'AInst e poi lo stampo nel file
                                        int temp1 = stringToInt(stringTempInp);
                                        char * temp2 = (char *) calloc (17, sizeof(char));
                                        temp2[0] = '0';
                                        temp2[1] = '\0';
                                        char * temp3 = intToBinary(temp1);
                                        strcat(temp2,temp3);
                                        fprintf(fileOutput, "%s\n", temp2);
                                        free(temp2);
                                        free(temp3);
                                    } else {
                                        // Se non lo ho ancora, lo aggiungo come variabile e poi 
                                        // lo traduco con una funzione e poi lo stampo nel file
                                        aggiungiVariabile(&addressdef, stringTempInp);
                                        char * stampato = ottieniAInstBits(addressdef,stringTempInp);
                                        fprintf(fileOutput, "%s\n", stampato);
                                        free(stampato);
                                    }
                                } else {
                                    // Sennò ho già l'indirizzo salvato, quindi lo traduco e lo stampo nel file
                                    char * stampato = ottieniAInstBits(addressdef,stringTempInp);
                                    fprintf(fileOutput, "%s\n", stampato);
                                    free(stampato);
                                }
                            } else if (isCInstruction(cinstructions, stringTempInp)) {
                                // Traduci stringTempInp in bit e lo stampa nel file
                                char * stampato =  ottieniCInstBits(cinstructions,stringTempInp);
                                fprintf(fileOutput, "%s\n",stampato);
                                free(stampato);
                            }
                            // Non dovrebbe arrivare ad un altro else, perché i controlli sono stati fatti precedentemente
                            // In generale, nella seconda passata, non si dovrebbero incontrare errori di compilazione.
                        }
                        free(stringTempInp);
                    } else {
                        free(stringTempInp);
                    }
                } while (!feof(fileInput));

                // Dopo aver finito di scrivere, chiudo il file di output
                fclose(fileOutput);
            }

            // "Libero" tutta la memoria che ho usato per gli elenchi di cinstruction

            for(i = 0; i < NUMOFINST_COMP; i++) {
                free(cinstructions.comp[i].bits);
                free(cinstructions.comp[i].contenuto);
            }
            free(cinstructions.comp);
            for(i = 0; i < NUMOFINST_JUMP; i++) {
                free(cinstructions.jump[i].bits);
                free(cinstructions.jump[i].contenuto);
            }
            free(cinstructions.jump);
            for(i = 0; i < NUMOFINST_DEST; i++) {
                free(cinstructions.dest[i].bits);
                free(cinstructions.dest[i].contenuto);
            }
            free(cinstructions.dest);

            // "Libero" la memoria anche del nome del file di output
            free(hackFileName);

            // Alla fine libero la memoria degli address
            for (i = 0; i < addressdef.len; i++) {
                free(addressdef.elenco[i].name);
            }
            free(addressdef.elenco);
        }
        // Chiusura dei file dopo la loro eventuale apertura
        fclose(fileInput);
    }
    return returnValue; // 0 se va a buon fine, -1 se non va a buon fine
}
