// #############################################################################
// # File load.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "load.h"
#include "structures.h"
#include "api.h"
#include "ui.h"
#include "programmes.h"

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define MULTIPLAYER 2
#define COMPUTER 1
#define AI 3
#define LOAD 5

#define VIDE 0

int load(int language)
{
    char *round_txt[4] = {"Round", "is playing", "Tour", "est en train de jouer"};
    clearScreen();

    clearScreen();
    printf("\n\e[0;102mLOAD mode\e[0m\n");
    int round_load;
    int coulle_load;
    int number_of_navires_load;
    int taille_plateau_load;

    int *number = &number_of_navires_load;
    int *taille = &taille_plateau_load;
    int *NbNav_load = &coulle_load;
    int *NbJoue_load = &round_load;

    int api_response = api_table_size("filecodec239012V1.txt", language);

    int **prop_load;
    prop_load = malloc(api_response * sizeof(int *));
    if (prop_load == NULL)
    {
        allocation_error_print_general("prop_load");
    }

    for (int i = 0; i < api_response; i++)
    {
        prop_load[i] = malloc(api_response * sizeof(int));
        if (prop_load[i] == NULL)
        {
            allocation_error_print_with_id("prop_load row", i);
        }
    }

    Liste_Navire liste_load;

    liste_load = api_load_game("filecodec239012V1.txt", number, taille, NbNav_load, NbJoue_load, prop_load);
    bool repeat_load = true;

    int max_rounds_load;
    ajuster_tours(taille_plateau_load, &max_rounds_load, number_of_navires_load, 1);

    clearScreen();

    // game loop
    game_loaded_graphics(max_rounds_load, taille_plateau_load, language);
    waitForKeypress();
    waitForKeypress();
    clearScreen();
    rules_reminder(max_rounds_load, taille_plateau_load, language);

    while (repeat_load)
    {
        // loop's logic
        printf("\n\e[4;32m%s No %d\e[0m\n\n", round_txt[language], *NbJoue_load);
        printing_the_grille_v2(prop_load, taille_plateau_load);
        if (waitForMenuKeypress(language))
        {
            if (midle_game_menu(max_rounds_load, taille_plateau_load, 2, COMPUTER, language) == 1) // 1 is internal code foe saving the progress and continuing another time
            {
                api_save_game(number_of_navires_load, taille_plateau_load, coulle_load, round_load, prop_load, liste_load, language);

#ifdef __APPLE__
                system("killall afplay");
#endif
                error_graphics(5, language);
            }
        }
        else
        {
            if (proposition_joueur(prop_load, NbJoue_load, liste_load, taille_plateau_load, NbNav_load, language))
            {
                game_mode_graphics_congratulations(prop_load, taille_plateau_load, number_of_navires_load, *NbNav_load, 0, "", language);
            }
            clearScreen();

            // decision making if the user wins or loses the game
            if (round_load == max_rounds_load && *NbNav_load < number_of_navires_load)
            {
                repeat_load = false;
                lost_graphics(1, language);
                api_delete_game_file(language);

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav_load == number_of_navires_load)
            {
                win_graphics(taille_plateau_load, prop_load, *NbJoue_load - 1, 1, "", language);
                api_delete_game_file(language);

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }

#ifdef __APPLE__
    system("killall afplay");
#endif
    return 1;
}