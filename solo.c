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

    int mode_solo = game_mode_solo();

    clearScreen();
    printf("\n\e[0;102mBienvenue au SOLO mode\e[0m\n");
    init_nb_aleatoire();
    int taille_plateau = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
    msleep(800);
    clearScreen();
    int number_of_navires = get_user_input("How many boats do you want to be included on the game ? ", "It has to be minimum 1 for an optimized game. Try again: ", "For a game that respects the rules, you can place up to 6 navires. Try again: ", 1, 6);

    // Allocation de la mémoire pour prop
    int **prop;
    prop = malloc(taille_plateau * sizeof(int *));
    if (prop == NULL)
    {
        allocation_error_print_general("prop");
    }

    for (int i = 0; i < taille_plateau; i++)
    {
        prop[i] = malloc(taille_plateau * sizeof(int));
        if (prop[i] == NULL)
        {
            allocation_error_print_with_id("prop row", i);
        }
    }

    initialize_plate(taille_plateau, prop);
    Liste_Navire liste;

    int coulle = 0;
    int *NbNav = &coulle;

    int round = 1; // used to show the number of the round
    int *NbJoue = &round;

    bool repeat = true; // be used when positioning ships in the begining and to repeat the game procedure

    clearScreen();
    liste = initialisation_plateau(prop, taille_plateau, number_of_navires);

    if (mode_solo == 1)
    {
        int max_rounds;
        ajuster_tours(taille_plateau, &max_rounds, number_of_navires, 1);

        // game loop
        rules_interface(max_rounds, taille_plateau);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds, taille_plateau);

        while (repeat)
        {
            // loop's logic
            new_round_graphics(*NbJoue, taille_plateau, prop, 0, "", -1);
            if (waitForMenuKeypress())
            {
                if (midle_game_menu(max_rounds, taille_plateau, 2, COMPUTER) == 1) // 1 is internal code for saving the progress and continuing another time
                {
                    api_save_game(number_of_navires, taille_plateau, coulle, round, prop, liste);
                    error_graphics(5);
                }
            }
            else
            {
                if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop, taille_plateau, number_of_navires, *NbNav, 0, "");
                }
                clearScreen();

                // decision making if the user wins or loses the game
                if (round == max_rounds && *NbNav < number_of_navires)
                {
                    repeat = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav == number_of_navires)
                {
                    win_graphics(taille_plateau, prop, *NbJoue - 1, 1, "");
                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
    }
    else if (mode_solo == 2) // 2 equals to mode temps
    {
        int duree_limite;
        ajuster_temps(taille_plateau, &duree_limite);

        // game loop
        rules_interface_temps(duree_limite, taille_plateau);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder_temps(duree_limite, taille_plateau);
        time_t start = time(NULL);
        time_t now;
        int diff_time = 0;

        while (repeat)
        {
            now = time(NULL);
            int temps_ecoule = (int)difftime(now, start);
            int temps_restant = (duree_limite - temps_ecoule) + diff_time;
            diff_time = 0;

            // loop's logic
            new_round_graphics(*NbJoue, taille_plateau, prop, 1, "", temps_restant);
            // decision making if the user wins or loses the game
            if (temps_restant <= 0)
            {
                clearScreen();
                repeat = false;
                lost_graphics(2);
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }

            if (waitForMenuKeypress())
            {
                midle_game_menu_saving_unavailable_temps(duree_limite, taille_plateau, 2, &diff_time); // case solo option temps for mode = 2
            }
            else
            {
                if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop, taille_plateau, number_of_navires, *NbNav, 0, "");
                }
                clearScreen();

                if (*NbNav == number_of_navires)
                {
                    win_graphics(taille_plateau, prop, *NbJoue - 1, 1, "");
                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
    }
    else
    {
        error_graphics(4);
    }
    
    return 1;
}