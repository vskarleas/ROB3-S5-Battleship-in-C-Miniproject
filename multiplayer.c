#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "ui.h"
#include "api.h"

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define MULTIPLAYER 2
#define COMPUTER 1
#define AI 3
#define LOAD 5

#define VIDE 0

int main(int argc, char **argv)
{

    int mode_multi = game_mode_multi();

    clearScreen();
    printf("\n\e[0;102mBienvenue au MULTIPLAYER mode\e[0m\n");
    init_nb_aleatoire();
    int taille_plateau_multi = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
    clearScreenWait(0.8);
    int number_of_navires_multi = get_user_input("Combien de bateaux souhaitez-vous inclure dans le jeu ? ", "Il doit être au minimum 1 pour un jeu optimisé. Réessayez : ", "Pour un jeu qui respecte les règles, vous pouvez placer jusqu'à 6 navires. Essayez à nouveau : ", 1, 6);
    clearScreenWait(0.8);
    char *player1 = get_user_name("Comment s'appelle le joueur n°1 ? ");
    clearScreenWait(0.8);
    char *player2 = get_user_name("Comment s'appelle le joueur n°2 ? ");
    clearScreenWait(0.8);

    // Allocation de la mémoire pour prop1
    int **prop1;
    prop1 = malloc(taille_plateau_multi * sizeof(int *));
    if (prop1 == NULL)
    {
        allocation_error_print_general("prop_muti");
    }

    for (int i = 0; i < taille_plateau_multi; i++)
    {
        prop1[i] = malloc(taille_plateau_multi * sizeof(int));
        if (prop1[i] == NULL)
        {
            allocation_error_print_with_id("prop row", i);
        }
    }

    initialize_plate(taille_plateau_multi, prop1);

    // Allocation de la mémoire pour prop2
    int **prop2;
    prop2 = malloc(taille_plateau_multi * sizeof(int *));
    if (prop2 == NULL)
    {
        allocation_error_print_general("prop2");
    }

    for (int i = 0; i < taille_plateau_multi; i++)
    {
        prop2[i] = malloc(taille_plateau_multi * sizeof(int));
        if (prop2[i] == NULL)
        {
            allocation_error_print_with_id("prop2 row", i);
        }
    }

    initialize_plate(taille_plateau_multi, prop2);
    Liste_Navire liste1;
    Liste_Navire liste2;

    int coulle1 = 0;
    int *NbNav1 = &coulle1;

    int round_global = 3; // used to show the number of the round
    int *NbJoue_global = &round_global;

    int coulle2 = 0;
    int *NbNav2 = &coulle2;

    bool repeat_multi = true; // be used when positioning ships in the begining and to repeat the game procedure

    clearScreen();

    int max_rounds_multi;
    ajuster_tours(taille_plateau_multi, &max_rounds_multi, number_of_navires_multi, 2);

    if (mode_multi == 2) // autmatically create plates for every user
    {
        rules_interface(max_rounds_multi, taille_plateau_multi);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi, taille_plateau_multi);

        clearScreen();
        liste1 = initialisation_plateau(prop1, taille_plateau_multi, number_of_navires_multi);
        liste2 = initialisation_plateau(prop2, taille_plateau_multi, number_of_navires_multi);

        while (repeat_multi)
        {
            // loop's logic
            new_round_graphics(*NbJoue_global, taille_plateau_multi, prop2, 2, player1, -1);
            if (waitForMenuKeypress())
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_multi, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_multi, number_of_navires_multi, *NbNav1, 1, player1);
                }
            }

            clearScreen();
            new_round_graphics(*NbJoue_global, taille_plateau_multi, prop1, 3, player2, -1);

            if (waitForMenuKeypress())
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa
                if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_multi, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_multi, number_of_navires_multi, *NbNav2, 1, player2);
                }
            }

            // decision making if the user wins or loses the game
            if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_multi || *NbNav2 < number_of_navires_multi))
            {
                repeat_multi = false;
                lost_graphics(1);
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav1 == number_of_navires_multi || *NbNav2 == number_of_navires_multi)
            {
                if (*NbNav1 > *NbNav2)
                {
                    win_graphics(taille_plateau_multi, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                }
                else
                {
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                }

                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }
    else if (mode_multi == 1) // creating custom plates
    {
        clearScreen();
        waitTime(3, "creer maintenant le plateu pour", 34, 36, player1, player2);
        liste1 = initialisation_plateau_custom(prop1, taille_plateau_multi, number_of_navires_multi);
        waitTime(3, "creer maintenant le plateu pour", 34, 36, player2, player1);
        liste2 = initialisation_plateau_custom(prop2, taille_plateau_multi, number_of_navires_multi);

        rules_interface(max_rounds_multi, taille_plateau_multi);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi, taille_plateau_multi);
        while (repeat_multi)
        {
            // loop's logic
            new_round_graphics(*NbJoue_global, taille_plateau_multi, prop2, 2, player1, -1);
            if (waitForMenuKeypress())
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_multi, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_multi, number_of_navires_multi, *NbNav1, 1, player1);
                }
            }

            clearScreen();
            new_round_graphics(*NbJoue_global, taille_plateau_multi, prop1, 3, player2, -1);

            if (waitForMenuKeypress())
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa
                if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_multi, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_multi, number_of_navires_multi, *NbNav2, 1, player2);
                }
            }

            // decision making if the user wins or loses the game
            if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_multi || *NbNav2 < number_of_navires_multi))
            {
                repeat_multi = false;
                lost_graphics(1);
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav1 == number_of_navires_multi || *NbNav2 == number_of_navires_multi)
            {
                if (*NbNav1 > *NbNav2)
                {
                    win_graphics(taille_plateau_multi, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                }
                else
                {
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                }

                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }
    else
    {
        error_graphics(3);
    }

    return 1;
}