#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#define GENES                    6   //Ciudades
#define INDIVIDUOS               100
#define GENERACIONES             50
#define PROBABILIDAD_CRUZAMIENTO 70
#define PROBABILIDAD_MUTACION    5

int costos[GENES][GENES] = {
    {0, 10, 15, 20, 25, 30},
    {10, 0, 35, 25, 15, 5},
    {15, 35, 0, 30, 20, 10},
    {20, 25, 30, 0, 5, 10},
    {25, 15, 20, 5, 0, 40},
    {30, 5, 10, 10, 40, 0}
};

struct Individuo {
    int cromosoma[GENES];
    int aptitud;
};

typedef struct Individuo Ind;

int evaluarSolucion(int cromosoma[GENES]) {
    //La aptitud es el costo total de recorrer todas las ciudades.
    int aptitud = 0;

    for(int i = 1; i < GENES; i++) {
        aptitud += costos[cromosoma[i - 1]][cromosoma[i]];
    }

    return aptitud;
}

Ind generarSolucion() {
    //La solucion no debe repetir ciudades.
    Ind individuo;

    bool disponibles[GENES];

    for(int i = 0; i < GENES; i++) {
        disponibles[i] = true;
    }

    for(int i = 0; i < GENES; i++) {
        individuo.cromosoma[i] = rand() % GENES;

        while(!disponibles[individuo.cromosoma[i]]) {
            individuo.cromosoma[i] = rand() % GENES;
        }

        disponibles[individuo.cromosoma[i]] = false;
    }

    individuo.aptitud = evaluarSolucion(individuo.cromosoma);

    return individuo;
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

Ind cruzamiento(Ind madre, Ind padre) {
    Ind hijo;

    //Seleccion de un punto de cruzamiento.
    int punto_cruza = rand() % GENES;
    int cont = punto_cruza;

    bool disponibles[GENES];

    for(int i = 0; i < GENES; i++) {
        disponibles[i] = true;
    }

    //El hijo va conservar la primer parte de la madre.
    for(int i = 0; i < punto_cruza; i++) {
        hijo.cromosoma[i] = madre.cromosoma[i];
        disponibles[hijo.cromosoma[i]] = false;
    }

    /*
        Se añaden las ciudades que aún no se encutran en el hijo,
        conservando el orden que aparecen en el padre.
    */
    for(int i = 0; i < GENES; i++) {
        if(disponibles[padre.cromosoma[i]]) {
            hijo.cromosoma[cont] = padre.cromosoma[i];
            disponibles[hijo.cromosoma[i]] = false;
            cont++;
        }
    }

    return hijo;
}

void mutacion(Ind *individuo) {
    //Se realiza un mutación por intercambio, donde se seleccionan
    // dos genes y se intercambian.

    int gen_1 = rand() % GENES;
    int gen_2 = rand() % GENES;
    int aux;

    while(gen_1 == gen_2) {
        gen_2 = rand() % GENES;
    }

    aux = individuo->cromosoma[gen_1];
    individuo->cromosoma[gen_1] = individuo->cromosoma[gen_2];
    individuo->cromosoma[gen_2] = aux;

    individuo->aptitud = evaluarSolucion(individuo->cromosoma);
}

void mostrarGeneracion(Ind poblacion[INDIVIDUOS]) {
    int mejor = 0;
    float media = 0;
    int peor = 0;

    for(int i = 0; i < INDIVIDUOS; i++) {
        for(int j = 0; j < GENES; j++) {
            printf("%i ", poblacion[i].cromosoma[j]);
        }
        printf("\n");

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
    printf("Mejor individuo: ");

    for(int i = 0; i < GENES; i++) {
        printf("%i ", poblacion[mejor].cromosoma[i]);
    }
    printf("-> %i", poblacion[mejor].aptitud);

    printf("\nPeor individuo: ");

    for(int i = 0; i < GENES; i++) {
        printf("%i ", poblacion[peor].cromosoma[i]);
    }
    printf("-> %i", poblacion[peor].aptitud);

    printf("\n");
}

int main() {
    //Semilla
    srand(time(NULL));

    //Inicio del algoritmo.
    Ind poblacion[INDIVIDUOS];
    Ind poblacionNueva[INDIVIDUOS];

    for(int i = 0; i < INDIVIDUOS; i++) {
        poblacion[i] = generarSolucion();
    }

    printf("Poblacion Inicial\n");
    mostrarGeneracion(poblacion);

    for(int g = 0; g < GENERACIONES; g++) {
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
                hijo_1 = cruzamiento(poblacion[madre], poblacion[padre]);
                hijo_2 = cruzamiento(poblacion[padre], poblacion[madre]);

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
    }

    system("pause");
    return 0;
}