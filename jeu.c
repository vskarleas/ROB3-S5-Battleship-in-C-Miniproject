#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "programmes.h"
#include "user_input.h"

int main(int argc, char **argv) {
    init_nb_aleatoire();
    int taille_plateau = get_user_input();

    // Allocation de la mémoire pour prop
    int **prop = malloc(taille_plateau * sizeof(int *));
    if (prop == NULL) {
        fprintf(stderr, "Failed to allocate memory for prop.\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++) {
        prop[i] = malloc(taille_plateau * sizeof(int));
        if (prop[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for prop row %d.\n", i);
            exit(-1);
        }
    }

    initialize_plate(taille_plateau, prop);
    printing_the_grille(prop, taille_plateau);

    // Allocation de la mémoire pour plateau
    int **plateau = malloc(taille_plateau * sizeof(int *));
    if (plateau == NULL) {
        fprintf(stderr, "Failed to allocate memory for plateau.\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++) {
        plateau[i] = malloc(taille_plateau * sizeof(int));
        if (plateau[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for plateau row %d.\n", i);
            exit(-1);
        }
    }

    initialize_plate(taille_plateau, plateau);

    // Allocation et initialisation des navires
    Navire *boats = malloc(sizeof(Navire) * 6);
    if (boats == NULL) {
        fprintf(stderr, "Failed to allocate memory for boats.\n");
        exit(-1);
    }

    initialisation_plateau(plateau, taille_plateau, &boats);

    printf("\n\nAprès:\n");
    for (int i=0; i<taille_plateau; i++)
	{
		for (int j=0; j<taille_plateau; j++)
		{
			prop[i][j] = plateau[i][j];
		}
	}

    printing_the_grille(prop, taille_plateau); // Après copie


    return 0;
}