#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define GENES                    9
#define INDIVIDUOS               800
#define GENERACIONES             1000
#define PROBABILIDAD_CRUZAMIENTO 80
#define PROBABILIDAD_MUTACION    10

struct Individuo {
    int cromosoma[GENES][GENES];
    int aptitud;
};

typedef struct Individuo Ind;

//Sudoku inicial.
int sudoku_inicial[GENES][GENES] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

bool esValido(int cromosoma[GENES][GENES], int i, int j) {
    //Columnas
    for(int k = 0; k < GENES; k++) {
        if(k != i) {
            if(cromosoma[k][j] == cromosoma[i][j]) {
                return false;
            }
        }
    }

    //Subcudricula 3x3.
    int fila = i - (i % 3);
    int columna = j - (j % 3);

    for(int k = fila; k < fila + 3; k++) {
        for(int l = columna; l < columna + 3; l++) {
            if(k != i && l != j) {
                if(cromosoma[k][l] == cromosoma[i][j]) {
                    return false;
                }
            }
        }
    }

    return true;
}

int evaluarSolucion(int cromosoma[GENES][GENES]) {
    int aptitud = 0;

    for(int i = 0; i < GENES; i++) {
        for(int j = 0; j < GENES; j++) {
            if(sudoku_inicial[i][j] == 0) {
                aptitud += !esValido(cromosoma, i, j);
            }
        }
    }

    return aptitud;
}

Ind generarSolucion() {
    Ind solucion;

    int num;
    bool disponibles[GENES];

    for(int i = 0; i < GENES; i++) { //Filas
        for(int k = 0; k < GENES; k++) {
            disponibles[k] = true;
        }

        for(int k = 0; k < GENES; k++) {
            solucion.cromosoma[i][k] = sudoku_inicial[i][k];

            if(sudoku_inicial[i][k] != 0) {
                disponibles[sudoku_inicial[i][k] - 1] = false;
            }
        }

        for(int j = 0; j < GENES; j++) { //Columnas
            if(sudoku_inicial[i][j] == 0) {
                num = rand() % 9;

                while(!disponibles[num]) {
                    num = rand() % 9;
                }

                solucion.cromosoma[i][j] = num + 1;
                disponibles[num] = false;
            }
        }
    }

    solucion.aptitud = evaluarSolucion(solucion.cromosoma);

    return solucion;
}

int seleccion(Ind poblacion[INDIVIDUOS]) {
    //Supuesto mejor individuo.
    int mejor = rand() % INDIVIDUOS;
    int competidor;

    //Subgrupo de individuos.
    for(int i = 0; i < 5; i++) {
        //Competidor.
        competidor = rand() % INDIVIDUOS;

        if(poblacion[competidor].aptitud < poblacion[mejor].aptitud) {
            //Si el competidor tiene mejor aptitud que el supuesto mejor
            // individuo, este pasa a ser el supuesto mejor.
            mejor = competidor;
        }
    }

    //Retornamos al mejor individuo del subgrupo.
    return mejor;
}

void cruzamiento(Ind madre, Ind padre, Ind *hijo_1, Ind *hijo_2) {
    //Punto de cruza.
    int punto_cruza = rand() % GENES;

    for(int i = 0; i < GENES; i++) {
        if(i < punto_cruza) {
            memcpy(hijo_1->cromosoma[i], madre.cromosoma[i], sizeof(int) * GENES);
            memcpy(hijo_2->cromosoma[i], padre.cromosoma[i], sizeof(int) * GENES);
        } else {
            memcpy(hijo_1->cromosoma[i], padre.cromosoma[i], sizeof(int) * GENES);
            memcpy(hijo_2->cromosoma[i], madre.cromosoma[i], sizeof(int) * GENES);
        }
    }

    hijo_1->aptitud = evaluarSolucion(hijo_1->cromosoma);
    hijo_2->aptitud = evaluarSolucion(hijo_2->cromosoma);
}

void mutacion(Ind *Individuo) {
    int fila = rand() % GENES;
    int gen_1 = rand() % GENES;
    int gen_2 = rand() % GENES;
    int aux;

    aux = Individuo->cromosoma[fila][gen_1];
    Individuo->cromosoma[fila][gen_1] = Individuo->cromosoma[fila][gen_2];
    Individuo->cromosoma[fila][gen_2] = aux;

    Individuo->aptitud = evaluarSolucion(Individuo->cromosoma);
}

void mostrarGeneracion(Ind poblacion[INDIVIDUOS]) {
    int mejor = 0;
    float media = 0;
    int peor = 0;

    for(int i = 0; i < INDIVIDUOS; i++) {
        if(poblacion[i].aptitud < poblacion[mejor].aptitud) {
            mejor = i;
        }

        media += poblacion[i].aptitud;

        if(poblacion[i].aptitud > poblacion[peor].aptitud) {
            peor = i;
        }
    }
    
    media /= INDIVIDUOS;

    printf("Media: %f\n", media);
    printf("Mejor individuo:\n");

    for(int i = 0; i < GENES; i++) {
        for(int j = 0; j < GENES; j++) {
            printf("%i ", poblacion[mejor].cromosoma[i][j]);
        }
        printf("\n");
    }
    printf("-> %i", poblacion[mejor].aptitud);

    printf("\nPeor individuo:\n");

    for(int i = 0; i < GENES; i++) {
        for(int j = 0; j < GENES; j++) {
            printf("%i ", poblacion[peor].cromosoma[i][j]);
        }
        printf("\n");
    }
    printf("-> %i", poblacion[peor].aptitud);

    printf("\n");
}

int main() {
    //Semilla
    srand(time(NULL));

    bool fin = false;

    //Inicio del algoritmo.
    Ind poblacion[INDIVIDUOS];
    Ind poblacionNueva[INDIVIDUOS];

    for(int i = 0; i < INDIVIDUOS; i++) {
        poblacion[i] = generarSolucion();
    }

    printf("Poblacion Inicial\n");
    mostrarGeneracion(poblacion);

    for(int g = 0; g < GENERACIONES && !fin; g++) {
        for(int i = 0; i < INDIVIDUOS; i += 2) {
            int madre = seleccion(poblacion);
            int padre = seleccion(poblacion);

            //Evitar que los padres sean el mismo.
            while(madre == padre) {
                padre = seleccion(poblacion);
            }

            //Reproducción.
            Ind hijo_1;
            Ind hijo_2;

            if(PROBABILIDAD_CRUZAMIENTO < rand() % 101) {
                //Los padres se cruzan.
                cruzamiento(poblacion[madre], poblacion[padre], &hijo_1, &hijo_2);

                if(PROBABILIDAD_MUTACION < rand() % 101) {
                    //Los hijos mutan.
                    mutacion(&hijo_1);
                    mutacion(&hijo_2);
                }

                poblacionNueva[i] = hijo_1;
                poblacionNueva[i + 1] = hijo_2;
            } else {
                poblacionNueva[i] = poblacion[madre];
                poblacionNueva[i + 1] = poblacion[padre];
            }
        }

        //Actualizar poblacion;
        for(int i = 0; i < INDIVIDUOS; i++) {
            poblacion[i] = poblacionNueva[i];
        }

        printf("Generacion %i\n", g + 1);
        mostrarGeneracion(poblacion);

        //Se encontro la mejor solución.
        for(int i = 0; i < INDIVIDUOS; i++) {
            if(poblacion[i].aptitud == 0) {
                printf("La mejor solución se encontro en la generacion %i\n", g + 1);
                fin = true;
            }
        }
    }

    system("pause");
    return 0;
}