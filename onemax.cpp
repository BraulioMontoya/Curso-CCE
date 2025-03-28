#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#define GENES                    10
#define INDIVIDUOS               20
#define GENERACIONES             5
#define PROBABILIDAD_CRUZAMIENTO 70
#define PROBABILIDAD_MUTACION    5

struct Individuo {
    int cromosoma[GENES];
    int aptitud;
};

typedef struct Individuo Ind;

int evaluarSolucion(int cromosoma[GENES]) {
    int aptitud = 0;

    for(int i = 0; i < GENES; i++) {
        aptitud += cromosoma[i];
    }

    return aptitud;
}

Ind generarSolucion() {
    Ind Individuo;

    //Solución aleatoria.
    for(int i = 0; i < GENES; i++) {
        Individuo.cromosoma[i] = rand() % 2;
    }

    Individuo.aptitud = evaluarSolucion(Individuo.cromosoma);

    return Individuo;
}

int seleccion(Ind poblacion[INDIVIDUOS]) {
    //Supuesto mejor individuo.
    int mejor = rand() % INDIVIDUOS;
    int competidor;

    //Subgrupo de individuos.
    for(int i = 0; i < 5; i++) {
        //Competidor.
        competidor = rand() % INDIVIDUOS;

        if(poblacion[competidor].aptitud > poblacion[mejor].aptitud) {
            //Si el competidor tiene mejor aptitud que el supuesto mejor
            // individuo, este pasa a ser el supuesto mejor.
            mejor = competidor;
        }
    }

    //Retornamos al mejor individuo del subgrupo.
    return mejor;
}

void cruzamiento(Ind madre, Ind padre, Ind *hijo_1, Ind *hijo_2) {
    //Punto de cruzamiento.
    int punto_cruza = rand() % GENES;

    for(int i = 0; i < GENES; i++) {
        if(i < punto_cruza) {
            //Primer parte del cruzamiento.
            hijo_1->cromosoma[i] = madre.cromosoma[i];
            hijo_2->cromosoma[i] = padre.cromosoma[i];
        } else {
            //Segunda parte del cruzamiento.
            hijo_1->cromosoma[i] = padre.cromosoma[i];
            hijo_2->cromosoma[i] = madre.cromosoma[i];
        }
    }

    hijo_1->aptitud = evaluarSolucion(hijo_1->cromosoma);
    hijo_2->aptitud = evaluarSolucion(hijo_2->cromosoma);
}

void mutacion(Ind *Individuo) {
    //Gen a modificar.
    int gen = rand() % GENES;

    //Mutación del cromosoma.
    /*
    if(Individuo->cromosoma[gen] == 1) {
        Individuo->cromosoma[gen] = 0;
    } else {
        Individuo->cromosoma[gen] = 1;
    }
    */

    Individuo->cromosoma[gen] = 1 - Individuo->cromosoma[gen];
    Individuo->aptitud = evaluarSolucion(Individuo->cromosoma);
}

void mostrarGeneracion(Ind poblacion[INDIVIDUOS]) {
    int mejor = 0;
    float media = 0;
    int min = GENES;

    for(int i = 0; i < INDIVIDUOS; i++) {
        for(int j = 0; j < GENES; j++) {
            printf("%i ", poblacion[i].cromosoma[j]);
        }
        printf("\n");

        if(poblacion[i].aptitud > poblacion[mejor].aptitud) {
            mejor = i;
        }

        media += poblacion[i].aptitud;

        if(poblacion[i].aptitud < poblacion[min].aptitud) {
            min = i;
        }
    }
    
    media /= INDIVIDUOS;

    printf("Media: %f\n", media);
    printf("Mejor individuo: ");

    for(int i = 0; i < GENES; i++) {
        printf("%i ", poblacion[mejor].cromosoma[i]);
    }

    printf("\nPeor individuo: ");

    for(int i = 0; i < GENES; i++) {
        printf("%i ", poblacion[min].cromosoma[i]);
    }

    printf("\n");
}

int main() {
    //Semilla.
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
            if(poblacion[i].aptitud == GENES) {
                printf("La mejor solución se encontro en la generacion %i\n", g + 1);
                fin = true;
            }
        }
    }

    system("pause");
    return 0;
}
