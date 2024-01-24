// #############################################################################
// # File solo.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "solo.h"
#include "programmes.h"
#include "ui.h"
#include "api.h"

int solo(int **prop1, Liste_Navire liste1, int *NbNav1, int *NbJoue, int coulle1, int taille_plateau_jeu, int number_of_navires_jeu, int language)
{
    int mode_solo = game_mode_solo(language);

    clearScreen();
    printf("\n\e[0;102mSOLO mode\e[0m\n");

    bool repeat = true; // be used when positioning ships in the begining and to repeat the game procedure

    clearScreen();
    liste1 = initialisation_plateau(prop1, taille_plateau_jeu, number_of_navires_jeu, language);

    if (mode_solo == 1)
    {
        int max_rounds;
        ajuster_tours(taille_plateau_jeu, &max_rounds, number_of_navires_jeu, 1);

        // game loop
        rules_interface(max_rounds, taille_plateau_jeu, language, 1); //mode game COMPUTER = 1
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds, taille_plateau_jeu, language);

        while (repeat)
        {
            // loop's logic
            new_round_graphics(*NbJoue, taille_plateau_jeu, prop1, 0, "", -1, language);
            if (waitForMenuKeypress(language))
            {
                if (midle_game_menu(max_rounds, taille_plateau_jeu, 2, 1, language) == 1) // 1 is internal code for saving the progress and continuing another time
                {
                    api_save_game(number_of_navires_jeu, taille_plateau_jeu, (*NbNav1), (*NbJoue), prop1, liste1, language);

                    error_graphics(5, language);
                }
            }
            else
            {
                if (proposition_joueur(prop1, NbJoue, liste1, taille_plateau_jeu, NbNav1, language)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 0, "", language);
                }
                clearScreen();

                // decision making if the user wins or loses the game
                if (*NbJoue-1 == max_rounds && *NbNav1 < number_of_navires_jeu)
                {
                    repeat = false;
                    lost_graphics(1, language);

                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav1 == number_of_navires_jeu)
                {
                    win_graphics(taille_plateau_jeu, prop1, *NbJoue - 1, 1, "", language);

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
    }
    else if (mode_solo == 2) // 2 equals to mode temps
    {
        int duree_limite;
        ajuster_temps(taille_plateau_jeu, &duree_limite);

        // game loop
        rules_interface_temps(duree_limite, taille_plateau_jeu, language); 
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder_temps(duree_limite, taille_plateau_jeu, language);
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
            new_round_graphics(*NbJoue, taille_plateau_jeu, prop1, 1, "", temps_restant, language);
            // decision making if the user wins or loses the game
            if (temps_restant <= 0)
            {
                clearScreen();
                repeat = false;
                lost_graphics(2, language);

                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }

            if (waitForMenuKeypress(language))
            {
                midle_game_menu_saving_unavailable_temps(duree_limite, taille_plateau_jeu, 2, &diff_time, language); // case solo option temps for mode = 2
            }
            else
            {
                if (proposition_joueur(prop1, NbJoue, liste1, taille_plateau_jeu, NbNav1, language)) // NbNav and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 0, "", language);
                }
                clearScreen();

                if (*NbNav1 == number_of_navires_jeu)
                {
                    win_graphics(taille_plateau_jeu, prop1, *NbJoue - 1, 1, "", language);

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
    }
    else
    {
        error_graphics(4, language);
    }

    return 1;
}