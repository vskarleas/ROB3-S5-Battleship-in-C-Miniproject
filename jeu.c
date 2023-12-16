#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "programmes.h"
#include "user_input.h"

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define ROUNDS 40 //how many rounds we want per game ?

int main(int argc, char **argv)
{
    init_nb_aleatoire();
    int taille_plateau = get_user_input();

    // Allocation de la mémoire pour prop
    int **prop;
    prop = malloc(taille_plateau * sizeof(int *));
    if (prop == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for prop.\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        prop[i] = malloc(taille_plateau * sizeof(int));
        if (prop[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for prop row %d.\n", i);
            exit(-1);
        }
    }

    initialize_plate(taille_plateau, prop);

    // Allocation et initialisation des navires
    Liste_Navire liste;
    liste = initialisation_plateau(prop, taille_plateau);

    //game loop
    int coulle = 0;
    int *NbNav = &coulle;
    bool repeat = true; //be used when positioning ships in the begining and to repeat the game procedure

    int round = 1; //used to show the number of the round
    int *NbJoue = &round;
    rules_interface(ROUNDS, taille_plateau);
    msleep(100);
    waitForKeypress();
    waitForKeypress();

    while(repeat)
    {
        //loop's logic
        printf("\nRound No %d\n\n", *NbJoue);
        printing_the_grille_v3(prop, taille_plateau);
        if(proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) //NbNav and NbJoue are updated on the function's core via pointers
        {
            clearScreen();
            printf("\033[0;36m\n=====================  Congratsulations, you found a navire!!!  =====================\033[0m\n");
        }

        if (round == ROUNDS && coulle < 6)
        {
            repeat = false;
            printf("=================================\n");
            printf("=========== Game over ===========\n");
            printf("=================================\n");
            printf("           YOU LOSE              \n");

            return 1; //returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
        }
        if (coulle == 6)
        {
            clearScreen();
            printf("Round No %d\n\n", *NbJoue-1);
            printing_the_grille_v3(prop, taille_plateau);
            printf("=================================\n");
            printf("========= Game finished =========\n");
            printf("=================================\n");
            printf("            YOU WIN              \n");

            return 0; //returns 0 if the user foudn all the ships - it also works as the while(repeat) stopper
        }
    }

    return 10; //game code closed
}