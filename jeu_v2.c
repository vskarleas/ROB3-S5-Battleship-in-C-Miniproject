#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "ui.h"
#include "api.h"

#define DELAY 200000 // Delay in milliseconds (200000 us = 0.2 seconds)

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define HORIZONTAL 1
#define VERTICAL 2

#define MULTIPLAYER 2
#define COMPUTER 1
#define AI 3
#define LOAD 5

#define VIDE 0

#define AUCUN_NAVIRE -1        // deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2        // one point was found
#define NAVIRE_TROUVE_PLUS_1 3 // one point of a navire was refound (it was choosen before)

#define COULE 10 // congratsulations

int main(int argc, char **argv)
{
    init_nb_aleatoire();
    int language = choose_language();
    clearScreen();
    const char *welcome_message[4] = {"Battleship game", "", "Bataille Navale", ""};
    const char *welcome_message_parameters[4] = {"Now is the time to configure the game before choosing the desired mode", "", "C'est le moment de parametriser le jeu avant choisir le mode souhaité", ""};

    printf("\n\e[0;103m%s V2.1\e[0m\n--------------------------------\n%s\n", welcome_message[language], welcome_message_parameters[language]);
    int taille_plateau_jeu = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25, language);
    clearScreenWait(0.8);
    printf("\n\n\e[0;103mBataille Navale V2.0\e[0m\n--------------------------------\nC'est le moment de parametriser le jeu avant choisir le mode souhaité\n");
    int number_of_navires_jeu = get_user_input("Combien de bateaux souhaitez-vous inclure dans le jeu ? ", "Il doit être au minimum 1 pour un jeu optimisé. Réessayez : ", "Pour un jeu qui respecte les règles, vous pouvez placer jusqu'à 6 navires. Essayez à nouveau : ", 1, 6, language);
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
    Liste_Navire liste2;

    int coulle1 = 0;
    int *NbNav1 = &coulle1;

    int round_global = 3; // used to show the number of the round (multiplayer & AI)
    int *NbJoue_global = &round_global;

    int round = 1; // used to show the number of the round (solo version & load)
    int *NbJoue = &round;

    int coulle2 = 0;
    int *NbNav2 = &coulle2;

    int mode = game_mode_menu();
    if (mode == COMPUTER)
    {
        int mode_solo = game_mode_solo();

        clearScreen();
        printf("\n\e[0;102mBienvenue au SOLO mode\e[0m\n");

        bool repeat = true; // be used when positioning ships in the begining and to repeat the game procedure

        clearScreen();
        liste1 = initialisation_plateau(prop1, taille_plateau_jeu, number_of_navires_jeu);

        if (mode_solo == 1)
        {
            int max_rounds;
            ajuster_tours(taille_plateau_jeu, &max_rounds, number_of_navires_jeu, 1);

            // game loop
            rules_interface(max_rounds, taille_plateau_jeu);
            msleep(100);
            waitForKeypress();
            waitForKeypress();
            clearScreen();
            rules_reminder(max_rounds, taille_plateau_jeu);

            while (repeat)
            {
                // loop's logic
                new_round_graphics(*NbJoue, taille_plateau_jeu, prop1, 0, "", -1);
                if (waitForMenuKeypress())
                {
                    if (midle_game_menu(max_rounds, taille_plateau_jeu, 2, COMPUTER) == 1) // 1 is internal code for saving the progress and continuing another time
                    {
                        api_save_game(number_of_navires_jeu, taille_plateau_jeu, coulle1, round, prop1, liste1);
                        error_graphics(5);
                    }
                }
                else
                {
                    if (proposition_joueur(prop1, NbJoue, liste1, taille_plateau_jeu, NbNav1)) // NbNav and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 0, "");
                    }
                    clearScreen();

                    // decision making if the user wins or loses the game
                    if (round == max_rounds && *NbNav1 < number_of_navires_jeu)
                    {
                        repeat = false;
                        lost_graphics(1);
                        return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                    }
                    if (*NbNav1 == number_of_navires_jeu)
                    {
                        win_graphics(taille_plateau_jeu, prop1, *NbJoue - 1, 1, "");
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
            rules_interface_temps(duree_limite, taille_plateau_jeu);
            msleep(100);
            waitForKeypress();
            waitForKeypress();
            clearScreen();
            rules_reminder_temps(duree_limite, taille_plateau_jeu);
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
                new_round_graphics(*NbJoue, taille_plateau_jeu, prop1, 1, "", temps_restant);
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
                    midle_game_menu_saving_unavailable_temps(duree_limite, taille_plateau_jeu, 2, &diff_time); // case solo option temps for mode = 2
                }
                else
                {
                    if (proposition_joueur(prop1, NbJoue, liste1, taille_plateau_jeu, NbNav1)) // NbNav and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 0, "");
                    }
                    clearScreen();

                    if (*NbNav1 == number_of_navires_jeu)
                    {
                        win_graphics(taille_plateau_jeu, prop1, *NbJoue - 1, 1, "");
                        return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                    }
                }
            }
        }
        else
        {
            error_graphics(4);
        }
    }

    if (mode == MULTIPLAYER)
    {
        int mode_multi = game_mode_multi();

        clearScreen();
        printf("\n\e[0;102mBienvenue au MULTIPLAYER mode\e[0m\n");
        char *player1 = get_user_name("Comment s'appelle le joueur n°1 ? ", language);
        clearScreenWait(0.8);
        char *player2 = get_user_name("Comment s'appelle le joueur n°2 ? ", language);
        clearScreenWait(0.8);

        bool repeat_multi = true; // be used when positioning ships in the begining and to repeat the game procedure

        clearScreen();

        int max_rounds_multi;
        ajuster_tours(taille_plateau_jeu, &max_rounds_multi, number_of_navires_jeu, 2);

        if (mode_multi == 2) // autmatically create plates for every user
        {
            rules_interface(max_rounds_multi, taille_plateau_jeu);
            msleep(100);
            waitForKeypress();
            waitForKeypress();
            clearScreen();
            rules_reminder(max_rounds_multi, taille_plateau_jeu);

            clearScreen();
            liste1 = initialisation_plateau(prop1, taille_plateau_jeu, number_of_navires_jeu);
            liste2 = initialisation_plateau(prop2, taille_plateau_jeu, number_of_navires_jeu);

            while (repeat_multi)
            {
                // loop's logic
                new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop2, 2, player1, -1);
                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                    if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1);
                    }
                }

                clearScreen();
                new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop1, 3, player2, -1);

                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa
                    if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_jeu, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav2, 1, player2);
                    }
                }

                // decision making if the user wins or loses the game
                if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
                {
                    repeat_multi = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav1 == number_of_navires_jeu || *NbNav2 == number_of_navires_jeu)
                {
                    if (*NbNav1 > *NbNav2)
                    {
                        win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else if (mode_multi == 1) // creating custom plates
        {
            clearScreen();
            waitTime(3, "creer maintenant le plateu pour", 34, 36, player1, player2);
            liste1 = initialisation_plateau_custom(prop1, taille_plateau_jeu, number_of_navires_jeu);
            waitTime(3, "creer maintenant le plateu pour", 34, 36, player2, player1);
            liste2 = initialisation_plateau_custom(prop2, taille_plateau_jeu, number_of_navires_jeu);

            rules_interface(max_rounds_multi, taille_plateau_jeu);
            msleep(100);
            waitForKeypress();
            waitForKeypress();
            clearScreen();
            rules_reminder(max_rounds_multi, taille_plateau_jeu);
            while (repeat_multi)
            {
                // loop's logic
                new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop2, 2, player1, -1);
                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                    if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1);
                    }
                }

                clearScreen();
                new_round_graphics(*NbJoue_global, taille_plateau_jeu, prop1, 3, player2, -1);

                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_jeu, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa
                    if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_jeu, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        game_mode_graphics_congratulations(prop1, taille_plateau_jeu, number_of_navires_jeu, *NbNav2, 1, player2);
                    }
                }

                // decision making if the user wins or loses the game
                if (round_global == max_rounds_multi && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
                {
                    repeat_multi = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav1 == number_of_navires_jeu || *NbNav2 == number_of_navires_jeu)
                {
                    if (*NbNav1 > *NbNav2)
                    {
                        win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else
        {
            error_graphics(3);
        }
    }
    if (mode == AI)
    {
        int mode_AI = game_mode_ai();

        clearScreen();
        printf("\n\e[0;102mBienvenue au IA mode\e[0m\n");
        char *player1 = get_user_name("Quel est ton nom ? ", language);
        clearScreenWait(0.8);
        char player2[8] = "DrixAI";

        bool repeat_multi_custom = true; // be used when positioning ships in the begining and to repeat the game procedure

        waitTime(3, "creer maintenant le plateu pour", 34, 36, player1, player2);
        liste1 = initialisation_plateau_custom(prop1, taille_plateau_jeu, number_of_navires_jeu);
        clearScreen();
        liste2 = initialisation_plateau(prop2, taille_plateau_jeu, number_of_navires_jeu);
        printf("\n%s has created the game plate for you as well %s\n", player2, player1);
        clearScreenWait(2);

        int max_rounds_multi_custom;
        ajuster_tours(taille_plateau_jeu, &max_rounds_multi_custom, number_of_navires_jeu, 2);
        rules_interface(max_rounds_multi_custom, taille_plateau_jeu);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi_custom, taille_plateau_jeu);

        if (mode_AI == 2) // mode Spark
        {
            while (repeat_multi_custom)
            {
                // loop's logic
                printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing\n\n", (*NbJoue_global) / 2, player1); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printing_the_grille_v2(prop2, taille_plateau_jeu);

                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1);
                }

                clearScreen();
                printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;33m%s\e[0m is playing\n\n", ((*NbJoue_global) / 2) - 1, player2); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

                // APPEL AU FONCTION AI
                tour_ia_random_v1(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global);

                // printing the evolutuon of AI
                printing_the_grille_v2(prop1, taille_plateau_jeu);

                // decision making if the user wins or loses the game
                if (round_global == max_rounds_multi_custom && (*NbNav1 < number_of_navires_jeu || *NbNav2 < number_of_navires_jeu))
                {
                    repeat_multi_custom = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav1 == number_of_navires_jeu || *NbNav2 == number_of_navires_jeu)
                {
                    if (*NbNav1 > *NbNav2)
                    {
                        win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                    }

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
            printf("%s as already played once!\n", player2);

            bool repeat_generator = true;
            bool not_skip_action;
            not_skip_action = play_point_and_decide(prop1, taille_plateau_jeu, liste1, NbNav2, NbJoue_global, x_now, y_now);

            while (repeat_multi_custom)
            {
                // loop's logic
                printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing.\n\n", (*NbJoue_global) / 2, player1); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printing_the_grille_v2(prop2, taille_plateau_jeu);

                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_jeu, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    game_mode_graphics_congratulations(prop2, taille_plateau_jeu, number_of_navires_jeu, *NbNav1, 1, player1);
                }

                clearScreen();
                printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;33m%s\e[0m is playing\n\n", ((*NbJoue_global) / 2) - 1, player2); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

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
                        win_graphics(taille_plateau_jeu, prop2, (*NbJoue_global - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_jeu, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else
        {
            error_graphics(6);
        }
    }
    if (mode == LOAD)
    {
        clearScreen();
        printf("\n\e[0;102mBienvenue au LOAD mode\e[0m\n");
        int round_load;
        int coulle_load;
        int number_of_navires_load;
        int taille_plateau_load;

        int *number = &number_of_navires_load;
        int *taille = &taille_plateau_load;
        int *NbNav_load = &coulle_load;
        int *NbJoue_load = &round_load;

        int api_response = api_table_size("filecodec239012V1.txt");

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
        game_loaded_graphics(max_rounds_load, taille_plateau_load);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_load, taille_plateau_load);

        while (repeat_load)
        {
            // loop's logic
            printf("\n\e[4;32mRound No %d\e[0m\n\n", *NbJoue_load);
            printing_the_grille_v2(prop_load, taille_plateau_load);
            if (waitForMenuKeypress())
            {
                if (midle_game_menu(max_rounds_load, taille_plateau_load, 2, COMPUTER) == 1) // 1 is internal code foe saving the progress and continuing another time
                {
                    api_save_game(number_of_navires_load, taille_plateau_load, coulle_load, round_load, prop_load, liste_load);
                    error_graphics(5);
                }
            }
            else
            {
                if (proposition_joueur(prop_load, NbJoue_load, liste_load, taille_plateau_load, NbNav_load))
                {
                    game_mode_graphics_congratulations(prop_load, taille_plateau_load, number_of_navires_load, *NbNav_load, 0, "");
                }
                clearScreen();

                // decision making if the user wins or loses the game
                if (round_load == max_rounds_load && *NbNav_load < number_of_navires_load)
                {
                    repeat_load = false;
                    lost_graphics(1);
                    api_delete_game_file();
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav_load == number_of_navires_load)
                {
                    win_graphics(taille_plateau_load, prop_load, *NbJoue_load - 1, 1, "");
                    api_delete_game_file();
                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
    }

    return 1;
}