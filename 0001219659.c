#include<stdio.h>
#include<stdlib.h>
#define EXIT_SUCCESS 0
/*
    Faccetta Matteo,
    0001219659,
    Classe B,
    matteo.faccetta@studio.unibo.it
*/

/*visto che uso array ad una dimensione uso un indicizzatore per muovermi come se fosse una matrice*/
#define INDEX_FOR_1d_MATRIX(i, j, M) (i)*(M)+(j)

/*struttura dati per rappresentare il circuito
*ho deciso di utilizzare degli uptree, in questo modo ogni piazzola collegata e' rappresentata da un albero, 
*e due piazzole sono collegate se hanno lo stesso rappresentante */
typedef struct Circuit{
    int* rappresentant;
    int* rank;
    int size;
} Circuit;

/*dichiarazioni delle funzioni di che andrò ad utilizzare*/
static Circuit initialize_circuit(int rows, int cols);

static void check_connection(Circuit* c, int i1, int j1, int i2, int j2, int cols);

static void create_connection(Circuit* c, int i1, int j1, int i2, int j2, int cols);

/*dichiarazione dei metodi necessari per la gestione dell'uptree, find e union*/
static int find(Circuit* c, int piazzola);
static void union_rank(Circuit* c, int i, int j);

int main(int argc, char const *argv[])
{
    /*dichiarazione delle variabile e delle strutture dati*/
    FILE* input_file;
    int rows, cols;
    Circuit circuit;
    char command;
    int i1, j1, i2, j2;


    input_file = fopen(argv[1], "r");
    /*prendo dal file la grandezza della griglia*/
    fscanf(input_file, "%d %d", &rows, &cols);
    
    circuit = initialize_circuit(rows, cols);
    /*main loop, leggo dal file di input il comando e le piazzole da verificare. se command == L creo il collegamento, se invece e' = Q verifico se sono collegate
    * mi accorgo che e' finito il file quando fscanf non mi restituisce piu' valori*/
    while(fscanf(input_file, " %c", &command) == 1){
        fscanf(input_file, "%d %d %d %d", &i1, &j1, &i2, &j2);
        switch(command){
            case 'L':
                create_connection(&circuit, i1, j1, i2, j2, cols);
                break;
            case 'Q':
                check_connection(&circuit ,i1, j1, i2, j2, cols);
                break;
            /*non gestisco il default perchè da specifica progetto assumo che l'output sia sempre corretto*/
        }

    }

    /*deallocazione dello spazio occupato nell'heap e chisura del file*/    
    fclose(input_file);
    free(circuit.rappresentant);
    free(circuit.rank);
    return EXIT_SUCCESS;
}

/*inizializzo la struttura del circuito, tra cui la grandezza della "matrice" (anche se un array 1d grazie all'indizzatore posso ragionare come se fossi in 2D)"
*il rango a zero e creo l'array in cui ad ogni casella verrà memorizzato il suo rappresentate
*/
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

/*metodo degli uptree, find. semplicemente salvo e ritorno il rappresentante. Inoltre implemento la path compression (ricorsivamente)*/
static int find(Circuit* c, int piazzola){
    if(piazzola != c->rappresentant[piazzola])
        c->rappresentant[piazzola] = find(c, c->rappresentant[piazzola]); 
    return c->rappresentant[piazzola];
}

/*altro metodo degli uptree. union by rank c -> unisco i due insiemi disgiunti utilizzando il rango*/
static void union_rank(Circuit* c, int i, int j){
    int rappresentant_i, rappresentant_j;

    rappresentant_i = find(c, i);
    rappresentant_j = find(c, j);

    if(c->rank[rappresentant_i] > c->rank[rappresentant_j])
        c->rappresentant[rappresentant_j] = rappresentant_i;
    else{
        c->rappresentant[rappresentant_i] = rappresentant_j;
        if(c->rank[rappresentant_i] == c->rank[rappresentant_j])
            c->rank[rappresentant_j]++;
    }
}

/*visto che le piste possono essere o verticali od orizzontali se:
*è verticale scorro le righe e finchè l'indice (che parte da i1) è minore di i2 faccio una union
*è orizzontale scorro le colonne e finchè l'indice (che parte le j1) è minore di j2 faccio union*/
static void create_connection(Circuit* c, int i1, int j1, int i2, int j2, int cols){
    int index;
    if(i1 == i2){ /*pista orizzontale*/
        for(index = INDEX_FOR_1d_MATRIX(i1, j1,cols); index < INDEX_FOR_1d_MATRIX(i1, j2, cols); index++){
            union_rank(c, index, index+1);
        }
    }else if(j1 == j2) /*pista verticale*/
    {
       for(index = INDEX_FOR_1d_MATRIX(i1, j1,cols); index < INDEX_FOR_1d_MATRIX(i2, j1, cols); index+= cols){
            union_rank(c, index, index+cols);
        } 
    }
}

/*avendo fatto path compression per controllare che la pista esista mi basta vedere se hanno lo stesso rappresentate 
*(sempre per il fatto che le piste sono o orizzontali o verticali)
*se non avessi fatto path compression sarei dovuto risalire fino al rappresentate per tutto l'uptree percorrendo tutto il percorso, rendendo tutto più lento*/
static void check_connection(Circuit* c, int i1, int j1, int i2, int j2, int cols){
    int piazzola1 = INDEX_FOR_1d_MATRIX(i1, j1, cols);
    int piazzola2 = INDEX_FOR_1d_MATRIX(i2, j2, cols);
    
    printf("Q %d %d %d %d ", i1, j1, i2, j2);
    find(c, piazzola1) == find(c, piazzola2) ? printf("C\n") : printf("NC\n");
}

