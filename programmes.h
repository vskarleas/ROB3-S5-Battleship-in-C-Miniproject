#ifndef _FUNCTIONS_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"

void waitForKeypress();

void clearScreen();

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

Cellule_Liste_Navire *creer_element_liste_Navire(Navire v);

Liste_Navire creer_liste_Navire_vide();

void ajouter_element_liste_Navire(Liste_Navire *L, Navire e);

bool navire_found(int **prop, Liste_Navire L);

void update_prop(int **prop, int x, int y);

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav);

void printProgress(double percentage);

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau);

void copier_navires(int **prop, int **plateau, int taille_plateau);

#endif