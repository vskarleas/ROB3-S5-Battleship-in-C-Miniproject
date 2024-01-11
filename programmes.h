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

int random_color();

/* It prints the plate - Treats table of any size and makes sure that the printing is done correctly */
void printing_the_grille(int **table, int taille_plateau);

/* Printing the game's table in a more UI friendly way */
void printing_the_grille_v2(int **table, int taille_plateau);

/* Just puts a 0 on every cell for initialization */
void initialize_plate(int taille_plateau, int **table);

/*Verifies if a navire can be placed on a specific location with a specific size and orientation */
bool est_valide_pro(int **table_navire, Navire nav, int taille_plateau);

bool est_valide_pro_custom(int **table_navire, Navire nav, int taille_plateau);

void suprimer_navire(Cellule_Liste_Navire *principal, Liste_Navire *liste);

Cellule_Liste_Navire *creer_element_liste_Navire(Navire v);

Liste_Navire creer_liste_Navire_vide();

void ajouter_element_liste_Navire(Liste_Navire *L, Navire e);

bool navire_found(int **prop, Liste_Navire L);

void update_prop(int **prop, int x, int y);

int point_decryption(char *input, int *number_returned, int *letter_returned);

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav);

void printProgress(double percentage);

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau, int number_of_navires);

void update_game_table_before_launch(int **prop, int taille_plateau);

Liste_Navire initialisation_plateau_custom(int **plateau, int taille_plateau, int number_of_navires);

void tour_ia_random_v1(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue);

bool random_choice(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue, int x, int y);

/* cr�er une cellule de liste avec l'�l�ment v 
   renvoie le pointeur sur la cellule de liste cr��e
   la fonction s'arrete si la cr�ation n'a pas pu se faire */
Cellule_Liste_Point *creer_element_liste_Point(int x, int y);

/* cr�er une liste vide */
Liste_Point creer_liste_Point_vide();

/* ajouter l'�l�ment e en fin de la liste L, renvoie la liste L modifi�e */
void ajouter_element_liste_Point(Liste_Point *L, int x, int y);

/* suppression de tous les �l�ments de la liste, renvoie la liste L vide */
Liste_Point supprimer_liste_Point(Liste_Point L);

/* cr�er une s�quence de points sous forme d'un tableau de points 
   � partir de la liste de points L */
Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L);

bool tour_ia_random_v2(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue, int x_prev, int y_prev, int x_now, int y_now, int *x_next, int *y_next, int mode);

void waitTime(int seconds, char *message, int colour1, int colour2, char *name1, char *name2);

void clearScreenWait(double seconds);

#endif