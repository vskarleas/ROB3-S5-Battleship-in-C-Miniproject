//#############################################################################
//# File jeu.c (main program)
//# UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
//# Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
//#############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "programmes.h"
#include "user_input.h"

int main(int argc, char **argv)
{
    init_nb_aleatoire();
    int taille_plateau = get_user_input();

    /* Dynamic creation of the game table */
    int **grille;
    grille = malloc(taille_plateau * sizeof (int **));

    if (grille == NULL) {
        fprintf(stderr, "failed to allocate memory for plate of the game.\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        grille[i] = malloc(taille_plateau * sizeof(int));
    }

    initialize_plate(taille_plateau, grille);
    printing_the_grille(grille, taille_plateau);

    int **grille_ref;
    grille_ref = malloc(taille_plateau * sizeof (int **));

    if (grille_ref == NULL) {
        fprintf(stderr, "failed to allocate memory for the navires table (table of reference).\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        grille_ref[i] = malloc(taille_plateau * sizeof(int));
    }

    initialize_plate(taille_plateau, grille_ref);

    int **boats_checlist;
    boats_checlist = malloc(6 * sizeof (int **));

    if (boats_checlist == NULL) {
        fprintf(stderr, "failed to allocate memory for the boats checklist table.\n");
        exit(-1);
    }

    for (int i = 0; i < 6; i++)
    {
        boats_checlist[i] = malloc(2 * sizeof(int));
    }
        
    //list of navires created (reference matrix of placed boats initially)
    Navire * boats ;
    boats = malloc(sizeof(Navire *)*6);


    if (boats == NULL) {
        fprintf(stderr, "failed to allocate memory for navires list.\n");
        exit(-1);
    }


    initialisation_plateau(grille_ref, taille_plateau, &boats, boats_checlist);
    //copier_grille_de_reference_vers_la_grille_de_jeu(grille_ref, grille, taille_plateau); //TO BE FIXED!!!
    

    return 1;
}