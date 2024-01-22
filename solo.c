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
    const char *welcome_message[4] = {"Battleship game", "It's time to configure the game before choosing the desired mode.", "Bataille Navale", "C'est le moment de parametriser le jeu avant choisir le mode souhaité"};
    char *welcome_message_parameters[4] = {"Now is the time to configure the game before choosing the desired mode", "", "C'est le moment de parametriser le jeu avant choisir le mode souhaité", ""};
    char *txt_1[4] = {"Give the size of the game table: ", " The size must be at least 4. Resize: ", "Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: "};
    char *txt_2[4] = {" You are sure for such a size. For an optimized game we do not recommend having a table size larger than 25. Try again: ", "How many boats do you want to include in the game ? ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", "Combien de bateaux souhaitez-vous inclure dans le jeu ? "};
    char *txt_3[4] = {"It must be at least 1 for an optimized game. Try again : ", "For a game that respects the rules, you can place up to 6 ships. Try again : ", "Il doit être au minimum 1 pour un jeu optimisé. Réessayez : ", "Pour un jeu qui respecte les règles, vous pouvez placer jusqu'à 6 navires. Essayez à nouveau : "};

    init_nb_aleatoire();
    clearScreen();
    int language = choose_language();
    clearScreen();
    printf("\n\e[0;103m%s V2.1\e[0m\n--------------------------------\n%s\n", welcome_message[language], welcome_message_parameters[language]);
    int taille_plateau_jeu = get_user_input(txt_1[language], txt_1[language + 1], txt_2[language], 4, 25, language);
    clearScreenWait(0.8);
    printf("\n\e[0;103m%s V2.0\e[0m\n--------------------------------\n%s\n", welcome_message[language], welcome_message[language + 1]);
    int number_of_navires_jeu = get_user_input(txt_2[language + 1], txt_3[language], txt_3[language + 1], 1, 6, language);
    clearScreenWait(0.8);

    // Allocation de la mémoire pour prop1
    int **prop1;
    prop1 = malloc(taille_plateau_jeu * sizeof(int *));
    if (prop1 == NULL)
    {
        allocation_error_print_general("prop_muti");
    }

    for (int i = 0; i < taille_plateau_jeu; i++)
    {
        prop1[i] = malloc(taille_plateau_jeu * sizeof(int));
        if (prop1[i] == NULL)
        {
            allocation_error_print_with_id("prop row", i);
        }
    }

    initialize_plate(taille_plateau_jeu, prop1);

    // Allocation de la mémoire pour prop2 - Used on modes such as multiplayer and AI
    int **prop2;
    prop2 = malloc(taille_plateau_jeu * sizeof(int *));
    if (prop2 == NULL)
    {
        allocation_error_print_general("prop2");
    }

    for (int i = 0; i < taille_plateau_jeu; i++)
    {
        prop2[i] = malloc(taille_plateau_jeu * sizeof(int));
        if (prop2[i] == NULL)
        {
            allocation_error_print_with_id("prop2 row", i);
        }
    }

    initialize_plate(taille_plateau_jeu, prop2);

    Liste_Navire liste1;

    int coulle1 = 0;
    int *NbNav1 = &coulle1;

    int round = 1; // used to show the number of the round (solo version & load)
    int *NbJoue = &round;

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
        rules_interface(max_rounds, taille_plateau_jeu, language);
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
                if (midle_game_menu(max_rounds, taille_plateau_jeu, 2, COMPUTER, language) == 1) // 1 is internal code for saving the progress and continuing another time
                {
                    api_save_game(number_of_navires_jeu, taille_plateau_jeu, coulle1, round, prop1, liste1, language);
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
                if (round == max_rounds && *NbNav1 < number_of_navires_jeu)
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