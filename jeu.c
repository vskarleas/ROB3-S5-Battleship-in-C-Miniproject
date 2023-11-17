#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "programmes.h"

int main(int argc, char **argv)
{
    srand(time(0));
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

    /* Dynamic creation of the table */
    int **grille;
    grille = malloc(taille_plateau * sizeof (int **));
    for (int i = 0; i < taille_plateau; i++)
    {
        grille[i] = malloc(taille_plateau * sizeof(int));
    }

    initialize_plate(taille_plateau, grille);

    int **grille_ref;
    grille_ref = malloc(taille_plateau * sizeof (int **));
    for (int i = 0; i < taille_plateau; i++)
    {
        grille_ref[i] = malloc(taille_plateau * sizeof(int));
    }

    initialize_plate(taille_plateau, grille_ref);
    printing_the_grille(grille, taille_plateau);

    
    //list of navires created
    Navire * boats ;
    boats = malloc(sizeof(Navire *)*6);


    initialisation_plateau(grille_ref, taille_plateau, &boats);
    printing_the_grille(grille, taille_plateau);
    

    return 1;
}