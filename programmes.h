#ifndef _FUNCTIONS_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"

/* Introducing a controled delay whenever we call this function */
int msleep(long tms);

/* Starting the random engine */
void init_nb_aleatoire();

/*Generates random numbers min and max given as arguments*/
int nb_random(int min, int max);

/* It prints the plate - Treats table of any size and makes sure that the printing is done correctly */
void printing_the_grille(int **table, int taille_plateau);

/* Printing the game's table in a more UI friendly way */
void printing_the_grille_v3(int **table, int taille_plateau);

/* Just puts a 0 on every cell for initialization */
void initialize_plate(int taille_plateau, int **table);

/*Verifies if a navire can be placed on a specific location with a specific size and orientation */
bool est_valide_pro(int **table_navire, int x, int y, int sens, int taille, int taille_plateau);

bool est_valide_pro_v2(int **table_navire, int taille_plateau, Tmp_navire *navire);

/* Creation of a navire/boat */
Navire *creer_navire(int taille, int taille_plateau, int **table_navire, int id);

/* Adding navires randomly on the game's plate */
void initialisation_plateau(int **table_navire, int taille_plateau, Navire **liste_of_navires, int **boats_checklist);

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
void proposition_joueur(int **plateau, int **prop, int *NbTouche, int *NbJoue, int *NbToucheNav, int taille_plateau);

#endif