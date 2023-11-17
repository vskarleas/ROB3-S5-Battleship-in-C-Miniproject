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

    /*
    bool repeater = true;

    int taille_plateau;
    printf("Donner la taille du tableau: ");
    scanf("%d", &taille_plateau);
    if (taille_plateau < 4)
    {
        while (repeater)
        {
            printf("\033[0;33mATTENTION!\033[1;0m La taille doit etre au minimum 4. Redonner la taille: ");
            scanf("%d", &taille_plateau);
            printf("\n");
            if (taille_plateau >= 4)
            {
                repeater = false;
            }
        }
    }
    printf("C'est parti!\n\n");
    */ //EVERYTHING IS PASSED ON user_input FILE

    int taille_plateau = get_user_input();

    /* Dynamic creation of the table */
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
        
    //list of navires created
    Navire * boats ;
    boats = malloc(sizeof(Navire *)*6);

    if (boats == NULL) {
        fprintf(stderr, "failed to allocate memory for navires list.\n");
        exit(-1);
    }


    initialisation_plateau(grille_ref, taille_plateau, &boats);
    copier_grille_de_reference_vers_la_grille_de_jeu(grille_ref, grille, taille_plateau);
    printing_the_grille(grille, taille_plateau);
    

    return 1;
}