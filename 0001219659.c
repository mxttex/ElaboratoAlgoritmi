#include<stdio.h>
#include<stdlib.h>
#define EXIT_SUCCESS 0
/*
    Matteo Faccetta,
    0001219659,
    Classe B,
    matteo.faccetta@studio.unibo.it
*/

/*struttura dati per rappresentare il circuito
*ho deciso di utilizzare degli uptree, in questo modo ogni piazzole collegato e' rappresentato da un albero, 
*e due piazzole sono collegate se hanno lo stesso rappresentante */
typedef struct Circuit{
    int* rappresentant;
    int* rank;
    int size;
} Circuit;

static Circuit initialize_circuit(int rows, int cols);

/*
* TODO = find e unionByRank, metodi CHECKCONNECTION e CREATECONNECTION
*/

int main(int argc, char const *argv[])
{
    /*dichiarazione delle variabile e delle strutture dati*/
    FILE* input_file;
    int rows, cols;
    Circuit circuit;
    char command;

    input_file = fopen(argv[1], "r");
    /*prendo dal file la grandezza della griglia*/
    fscanf(input_file, "%d %d", &rows, &cols);
    
    circuit = initialize_circuit(rows, cols);
    /*main loop, leggo dal file di input il comando e le piazzole da verificare. se command == L creo il collegamento, se invece e' = Q verifico se sono collegate
    * mi accorgo che e' finito il file quando fscanf non mi restituisce piu' valori*/
    while(fscanf(input_file, " %c", &command) == 1){
        int i1, j1, i2, j2;
        fscanf(input_file, "%d %d %d %d", &i1, &j1, &i2, &j2);
        switch(command){
            case 'L':
                printf("creo pista che connette %d, %d, %d, %d \n", i1, j1, i2, j2);
                break;
            case 'Q':
                printf("verifico collegamento %d, %d, %d, %d \n", i1, j1, i2, j2);
                break;
        }

    }

    fclose(input_file);

    
    /*deallocazione dello spazio occupato nell'heap*/
    free(circuit.rappresentant);
    free(circuit.rank);
    return EXIT_SUCCESS;
}

static Circuit initialize_circuit(int rows, int cols){
    Circuit c;
    int i;

    c.size = rows*cols;
    c.rappresentant = (int*)malloc(c.size * sizeof(int));
    c.rank = (int*)malloc(c.size * sizeof(int));

    for(i = 0; i<c.size; i++){
        c.rappresentant[i] = i;
        c.rank[i] = 0;
    }
    return c;
}