#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "structures.h"

// Add this function to programmes.c
void save_game_state(int **table, int taille_plateau, Navire **liste_of_navires, int **boats_checklist, int NbJoue, int NbTouche)
{
    FILE *file = fopen("saved_game.txt", "w");

    if (file == NULL)
    {
        fprintf(stderr, "Failed to open the file for saving.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", taille_plateau);

    for (int i = 0; i < taille_plateau; i++)
    {
        for (int j = 0; j < taille_plateau; j++)
        {
            fprintf(file, "%d ", table[i][j]);
        }
        fprintf(file, "\n");
    }

    // Add more information to save if needed (e.g., Navires, boats_checklist, NbJoue, NbTouche)

    fclose(file);
    printf("Game state saved successfully.\n");
}
