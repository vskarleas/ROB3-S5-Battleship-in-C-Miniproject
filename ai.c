// #############################################################################
// # File ai.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define AUCUN_NAVIRE -1        // deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2        // one point was found
#define NAVIRE_TROUVE_PLUS_1 3 // one point of a navire was refound (it was choosen before)
#define COULE 10               // congratsulations you find the whole navire

#include "ai.h"
#include "programmes.h"
#include "ui.h"

int ai(int **prop1, int **prop2, Liste_Navire liste1, Liste_Navire liste2, int *NbNav1, int *NbNav2, int *NbJoue_global, int coulle1, int coulle2, int round_global, int taille_plateau_jeu, int number_of_navires_jeu, int language)
{
    char *round_txt[4] = {"Round", "is playing", "Tour", "est en train de jouer"};
    char *msg_ai[4] = {"now create the board for", "has created the game plate for you as well", "creer maintenant le plateu pour", "a également créé le plateau de jeu pour vous"};
    char *name_msg[4] = {"What is your name ? ", "has already played once", "Quel est ton prenom ? ", "a déjà joué une fois"};

    int mode_AI = game_mode_ai(language);

    clearScreen();
    printf("\n\e[0;102mAI mode\e[0m\n");

    char *player1 = get_user_name(name_msg[language], language);
    clearScreenWait(0.8);
    char player2[8] = "DrixAI";

    bool repeat_multi_custom = true; // be used when positioning ships in the begining and to repeat the game procedure

    waitTime(3, msg_ai[language], 34, 36, player1, player2);
    liste1 = initialisation_plateau_custom(prop1, taille_plateau_jeu, number_of_navires_jeu, language);
    clearScreen();
    liste2 = initialisation_plateau(prop2, taille_plateau_jeu, number_of_navires_jeu, language);
    printf("\n%s %s %s\n", player2, msg_ai[language + 1], player1);
    clearScreenWait(2);

    int max_rounds_multi_custom;
    ajuster_tours(taille_plateau_jeu, &max_rounds_multi_custom, number_of_navires_jeu, 2);
    rules_interface(max_rounds_multi_custom, taille_plateau_jeu, language);
    msleep(100);
    waitForKeypress();
    waitForKeypress();
    clearScreen();
    rules_reminder(max_rounds_multi_custom, taille_plateau_jeu, language);

    if (mode_AI == 2) // mode Spark
    {
        while (repeat_multi_custom)
        {
            // loop's logic
            printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;32m%s\e[0m %s\n\n", round_txt[language], (*NbJoue_global) / 2, player1, round_txt[language + 1]); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
            printing_the_grille_v2(prop2, taille_plateau_jeu);

            // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
            if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
            {
                game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1, language);
            }

            clearScreen();
            printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;33m%s\e[0m %s\n\n", round_txt[language], ((*NbJoue_global) / 2) - 1, player2, round_txt[language + 1]); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

            // APPEL AU FONCTION AI
            tour_ia_random_v1(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global);

            // printing the evolutuon of AI
            printing_the_grille_v2(prop1, taille_plateau_jeu);

            // decision making if the user wins or loses the game
            if (round_global == max_rounds_multi_custom && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
            {
                repeat_multi_custom = false;
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
    else if (mode_AI == 1) // mode Fireball
    {
        int x_now, y_now;
        int previous_sens = -1;
        int sens_mode = -1;
        int deep_sens = -1;

        x_now = nb_random(0, taille_plateau_jeu - 1);
        y_now = nb_random(0, taille_plateau_jeu - 1);

        int x_prev, y_prev;
        printf("%s %s!\n", player2, name_msg[language + 1]);

        bool repeat_generator = true;
        bool not_skip_action;
        not_skip_action = play_point_and_decide(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global, x_now, y_now);

        while (repeat_multi_custom)
        {
            // loop's logic
            printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;32m%s\e[0m %s.\n\n", round_txt[language], (*NbJoue_global) / 2, player1, round_txt[language + 1]); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
            printing_the_grille_v2(prop2, taille_plateau_jeu);

            // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
            if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1, language)) // NbNav_global and NbJoue are updated on the function's core via pointers
            {
                game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1, language);
            }

            clearScreen();
            printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;33m%s\e[0m %s\n\n", round_txt[language], ((*NbJoue_global) / 2) - 1, player2, round_txt[language + 1]); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

            // Calling AI V2
            proposition_ai(prop1, taille_plateau_jeu, liste1, NbNav2);
            if (not_skip_action)
            {
                x_now = nb_random(0, taille_plateau_jeu - 1);
                y_now = nb_random(0, taille_plateau_jeu - 1);
                while (repeat_generator)
                {
                    if (prop1[x_now][y_now] == AUCUN_NAVIRE || prop1[x_now][y_now] == NAVIRE_TROUVE || prop1[x_now][y_now] == NAVIRE_TROUVE_PLUS_1 || prop1[x_now][y_now] == COULE)
                    {
                        x_now = nb_random(0, taille_plateau_jeu - 1);
                        y_now = nb_random(0, taille_plateau_jeu - 1);
                    }
                    else
                    {
                        repeat_generator = false;
                    }
                }

                repeat_generator = true; // re-initialize the variable to be used in another round

                not_skip_action = play_point_and_decide(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global, x_now, y_now);
            }
            else
            {
                x_prev = x_now;
                y_prev = y_now;

                next_point_v3(prop1, taille_plateau_jeu, x_prev, y_prev, &x_now, &y_now, &previous_sens, &sens_mode, &deep_sens);

                not_skip_action = play_point_and_decide(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global, x_now, y_now);
            }

            // printing the evolutuon of AI
            printing_the_grille_v2(prop1, taille_plateau_jeu);

            // decision making if the user wins or loses the game
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
        error_graphics(6, language);
    }

#ifdef __APPLE__
    system("killall afplay");
#endif
    return 1;
}