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

    // Allocation de la mémoire pour plateau
    int **plateau;
    plateau = malloc(taille_plateau * sizeof(int *));
    if (plateau == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for plateau.\n");
        exit(-1);
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        plateau[i] = malloc(taille_plateau * sizeof(int));
        if (plateau[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for plateau row %d.\n", i);
            exit(-1);
        }
    }

    initialize_plate(taille_plateau, plateau);

    // Allocation et initialisation des navires
    Liste_Navire liste;
    liste = initialisation_plateau(plateau, taille_plateau);

    printf("\n\nAprès:\n");
    copier_navires(prop, plateau, taille_plateau);

    //game loop
    int points = 11; //11 points attributed to the user
    bool repeat = true; //be used when positioning ships in the begining and to repeat the game procedure

    //INSTEAD OF HAVING THIS TEST< MAKE A FUNCTION THAT REMOVES A NAVIRE FROM THE LISTE CHAINEE AND AS NEW TEST WE CHECK IF liste == NULL
    int ships_positioned_counter = 0; //how many ships have been positioned in the begining

    int i, j; //coordinates used for ships position
    int round = 1; //used to show the number of the round
    bool coordinates = true; //used to ask the use new coordinates for a vessel if the previous ones are not inside the specified limits mentioned on the instructions

    printf("SCORE\n");
    printf("Your initial score is %d\n\n", points);
    printf("RULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. The goal is to find the hiden vessels coordinates. If you do not find it, you will lose one point from the total of %d\n\n", taille_plateau, taille_plateau, points);
    printf("The coordinates x & y can take any value between 1 and %d\n", taille_plateau, taille_plateau);
    printf("================================================================\n");
    while(repeat)
    {
        printf("------------------------------------------------\n");
        printf("Round No %d\n\n", round);
        while (coordinates)
        {
            printf("Postion x: ");
            scanf("%d", &i);
            printf("\n");
            printf("Postion y: ");
            scanf("%d", &j);
            printf("\n");

            if (!(i < 1 || i > taille_plateau + 1 || j < 1 || j > taille_plateau + 1)) //coordinates verification out of limit
            {
                
                coordinates = false;
            }
            else
            {
                printf("The coordinates x & y can take any value between 1 and %d. TRY AGAIN!\n", taille_plateau);
            }
        }

        coordinates = true;
        
        //USE THE NEW FUNCTIONS FOUND ON THE END OF PROGRAMMES FILE AND THE COORDINATES
        if (position_available_or_not(grille, i, j))
        {
            //means that no ship found -> marking with 'x'
            grille[i-1][j-1] = 2; //we need -1 because user gives visual coordinates
            points --; //updating the score
        }
        else 
        {
            //means that the user found a ship -> markig with 'o'
            grille[i-1][j-1] = 3; //we need -1 because user gives visual coordinates
            ships_positioned_counter --;
        }

        printing_the_grille_v3(prop, taille_plateau);

        //loop's logic
        if (points == 0)
        {
            repeat = false;
            printf("=================================\n");
            printf("=========== Game over ===========\n");
            printf("=================================\n");
            printf("           YOU LOSE              \n");

            return 1; //returns 1 if the user lost all of his points - it also works as the while(repeat) stopper
        }
        if (ships_positioned_counter == 0 )
        {
            printf("=================================\n");
            printf("========= Game finished =========\n");
            printf("=================================\n");
            printf("            YOU WIN              \n");

            return 0; //returns 0 if the user foudn all the ships - it also works as the while(repeat) stopper
        }

        round ++; //updating round number
    }

    return 10; //game code closed
}