#ifndef _FUNCTIONS_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"


int nb_random(int min, int max);

void printing_the_grille(int **table, int taille_plateau);

void initialize_plate(int taille_plateau, int **table);

bool valdation_navire(int **table_navire, int x, int y, int sens, int taille, int taille_plateau);

Navire *creer_navire(int taille, int taille_plateau, int **table_navire);

void initialisation_plateau(int **table_navire, int taille_plateau, Navire **liste_of_navires);

void copier_grille_de_reference_vers_la_grille_de_jeu(int **table_navire, int **table_jeu, int taille_tableau);

void proposition_joueur(int **plateau, int **prop, int *NbTouche, int *NbJoue, int *NbToucheNav, int taille_plateau);

#endif