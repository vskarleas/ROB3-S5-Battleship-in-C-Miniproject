// #############################################################################
// # File programmes.h
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#ifndef _FUNCTIONS_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"

/* Waiting for keypress before moving to the rest of the program */
void waitForKeypress();

/* Clearing the terminal screen for more optimised visualisations */
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

/* Verifies if a navire can be placed on a specific location with a specific size and orientation */
bool est_valide_pro(int **table_navire, Navire nav, int taille_plateau);

/* Deletes a specific navire from a liste chainee of navires */
void suprimer_navire(Cellule_Liste_Navire *principal, Liste_Navire *liste);

/* create a list cell with a navire v
    returns the pointer to the created list cell
    the function stops if the creation could not be done */
Cellule_Liste_Navire *creer_element_liste_Navire(Navire v);

/* Creates an empty lise chainee of navires (allocation) */
Liste_Navire creer_liste_Navire_vide();

/* add the navire e at the end of the list L, returns the modified list L */
void ajouter_element_liste_Navire(Liste_Navire *L, Navire e);

/* Checks if a navire was found by scanning the game's plate with an advanced logic and an optimised algorithm
The idea was to remove any navire that was found from this liste chainne L in order to have more optimised checking times */
bool navire_found(int **prop, Liste_Navire L);

/* Updating the specific coordinates that the user chose (x,y) according to their state  */
void update_prop(int **prop, int x, int y, int mode);

/* Decrypting the informations of a point of the type numer+letter (returns for instance for a point 2A the following (1,0))*/
int point_decryption(char *input, int *number_returned, int *letter_returned);

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav, int language);

/* Printing a loading indicator */
void printProgress(double percentage);

/* Allocating and initialising the navires */
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau, int number_of_navires, int language);

/* Any navire that was placed customly CUSTOMNAVIRE is now updated to NAVIRE */
void update_game_table_before_launch(int **prop, int taille_plateau);

/* Allocating and initialising the navires customly */
Liste_Navire initialisation_plateau_custom(int **plateau, int taille_plateau, int number_of_navires, int language);

/* AI logic for spark mode that simly chooses points aleartory that it hasn't visited before */
void tour_ia_random_v1(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue);

/* Calls the function taht verfies if a navire was found from the AI. If this is the case then it adds a point to the AI's score */
void proposition_ai(int **prop, int taille_plateau, Liste_Navire L, int *NbNav);

/* Decides if we need to find a new point or not for the next round */
bool play_point_and_decide(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue, int x, int y);

/* Advanced AI logic V1
void next_point(int **table, int taille_plateau, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens, int *state);

Advanced AI logic V2
void next_point_v2(int **table, int taille_plateau, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens); */

/* Checks if the chosen point can be accesed - Function to check if a point is within the limits of game table */
int is_valid_point(int x, int y, int table_size);

/* Function to update the search direction based on the current direction (inverses them) */
void update_direction(int *sens, int *deep_sens);

/* Advanced AI logic V3 */
void next_point_v3(int **table, int table_size, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens);

/* create a list cell with a point v
    returns the pointer to the created list cell
    the function stops if the creation could not be done */
Cellule_Liste_Point *creer_element_liste_Point(int x, int y);

/* Creates an empty lise chainee of points (allocation) */
Liste_Point creer_liste_Point_vide();

/* add the point (x,y) e at the end of the list L, returns the modified list L */
void ajouter_element_liste_Point(Liste_Point *L, int x, int y);

/* suppression de tous les �l�ments de la liste, renvoie la liste L vide */
Liste_Point supprimer_liste_Point(Liste_Point L);

/* cr�er une s�quence de points sous forme d'un tableau de points
   � partir de la liste de points L */
Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L);

/* Provides some waiting time with a message for the user */
void waitTime(int seconds, char *message, int colour1, int colour2, char *name1, char *name2);

/* Waiting some time and clearing the screen */
void clearScreenWait(double seconds);

#endif