// #############################################################################
// # File multipllayer.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "multiplayer.h"
#include "programmes.h"
#include "ui.h"

int multiplayer(int **prop1, int **prop2, Liste_Navire liste1, Liste_Navire liste2, int *NbNav1, int *NbNav2, int *NbJoue_global, int coulle1, int coulle2, int round_global, int taille_plateau_jeu, int number_of_navires_jeu, int language)
{
    char *msg_ai[4] = {"now create the board for", "has created the game plate for you as well", "creer maintenant le plateu pour", "a également créé le plateau de jeu pour vous"};
    char *msg_multi[4] = {"What is the name of player number 1 ? ", "What is the name of player number 2 ? ", "Comment s'appelle le joueur n°1 ? ", "Comment s'appelle le joueur n°2 ? "};

    int mode_multi = game_mode_multi(language);

    clearScreen();
    printf("\n\e[0;102mMULTI mode\e[0m\n");
    char *player1 = get_user_name(msg_multi[language], language);
    clearScreenWait(0.8);
    char *player2 = get_user_name(msg_multi[language + 1], language);
    clearScreenWait(0.8);

    bool repeat_multi = true; // be used when positioning ships in the begining and to repeat the game procedure

    clearScreen();

    int max_rounds_multi;
    ajuster_tours(taille_plateau_jeu, &max_rounds_multi, number_of_navires_jeu, 2);

    if (mode_multi == 2) // autmatically create plates for every user
    {
        rules_interface(max_rounds_multi, taille_plateau_jeu, language);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi, taille_plateau_jeu, language);

        clearScreen();
        liste1 = initialisation_plateau(prop1, taille_plateau_jeu, number_of_navires_jeu, language);
        liste2 = initialisation_plateau(prop2, taille_plateau_jeu, number_of_navires_jeu, language);

        while (repeat_multi)
        {
            // loop's logic
            new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop2, 2, player1, -1, language);
            if (waitForMenuKeypress(language))
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1, language); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1, language);
                }
            }

            clearScreen();
            new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop1, 3, player2, -1, language);

            if (waitForMenuKeypress(language))
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1, language); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa
                if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_jeu, NbNav2, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav2, 1, player2, language);
                }
            }

            // decision making if the user wins or loses the game
            if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
            {
                repeat_multi = false;
                lost_graphics(1, language);

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav1 == number_of_navires_jeu || *NbNav2 == number_of_navires_jeu)
            {
                if (*NbNav1 > *NbNav2)
                {
                    win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1, language);
                }
                else
                {
                    win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2, language);
                }

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }
    else if (mode_multi == 1) // creating custom plates
    {
        clearScreen();
        waitTime(3, msg_ai[language], 34, 36, player1, player2);
        liste1 = initialisation_plateau_custom(prop1, taille_plateau_jeu, number_of_navires_jeu, language);
        waitTime(3, msg_ai[language], 34, 36, player2, player1);
        liste2 = initialisation_plateau_custom(prop2, taille_plateau_jeu, number_of_navires_jeu, language);

        rules_interface(max_rounds_multi, taille_plateau_jeu, language);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi, taille_plateau_jeu, language);
        while (repeat_multi)
        {
            // loop's logic
            new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop2, 2, player1, -1, language);
            if (waitForMenuKeypress(language))
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1, language); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1, language);
                }
            }

            clearScreen();
            new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop1, 3, player2, -1, language);

            if (waitForMenuKeypress(language))
            {
                midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1, language); // case mupliplayer for mode = 1
            }
            else
            {
                // prop2 is created by the player2 for the player1 and vice versa
                if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_jeu, NbNav2, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav2, 1, player2, language);
                }
            }

            // decision making if the user wins or loses the game
            if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
            {
                repeat_multi = false;
                lost_graphics(1, language);

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav1 == number_of_navires_jeu || *NbNav2 == number_of_navires_jeu)
            {
                if (*NbNav1 > *NbNav2)
                {
                    win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1, language);
                }
                else
                {
                    win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2, language);
                }

#ifdef __APPLE__
                system("killall afplay");
#endif
                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }
    else
    {
#ifdef __APPLE__
        system("killall afplay");
#endif
        error_graphics(3, language);
    }

#ifdef __APPLE__
    system("killall afplay");
#endif
    return 1;
}