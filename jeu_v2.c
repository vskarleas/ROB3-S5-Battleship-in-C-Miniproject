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
    int mode = game_mode_menu();
    if (mode == COMPUTER)
    {
        int mode_solo = game_mode_solo();

        clearScreen();
        printf("\n\e[0;102mBienvenue au SOLO mode\e[0m\n");
        init_nb_aleatoire();
        int taille_plateau = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
        msleep(1500);
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
                printf("\nRound No %d\n\n", *NbJoue);
                printing_the_grille_v3(prop, taille_plateau);
                if (waitForMenuKeypress())
                {
                    if (midle_game_menu(max_rounds, taille_plateau, 2, COMPUTER) == 1) // 1 is internal code foe saving the progress and continuing another time
                    {
                        api_save_game(number_of_navires, taille_plateau, coulle, round, prop, liste);
                        clearScreen();
                        printf("\n\e[0;32mThe game has been saved succesfully on server!\e[0m\n");
                        exit(5);
                    }
                }
                else
                {
                    if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav)) // NbNav and NbJoue are updated on the function's core via pointers
                    {
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n\n", *NbNav, number_of_navires);
                        printing_the_grille_v3(prop, taille_plateau);
                        msleep(3000);
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
                printf("\nRound No %d | Temps restant: %d secondes\n\n", *NbJoue, temps_restant);
                printing_the_grille_v3(prop, taille_plateau);
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
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n\n", *NbNav, number_of_navires);
                        printing_the_grille_v3(prop, taille_plateau);
                        msleep(3000);
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
            printf("Error entering a sub-mode on solo\n");
            exit(-4);
        }
    }

    if (mode == MULTIPLAYER)
    {
        int mode_multi = game_mode_multi();

        clearScreen();
        printf("\n\e[0;102mBienvenue au MULTIPLAYER mode\e[0m\n");
        init_nb_aleatoire();
        int taille_plateau_multi = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
        msleep(1500);
        clearScreen();
        int number_of_navires_multi = get_user_input("How many boats do you want to be included on the game ? ", "It has to be minimum 1 for an optimized game. Try again: ", "For a game that respects the rules, you can place up to 6 navires. Try again: ", 1, 6);
        msleep(1500);
        clearScreen();
        char *player1 = get_user_name("What's the name of the player No 1 ? ");
        msleep(1500);
        clearScreen();
        char *player2 = get_user_name("What's the name of the player No 2 ? ");
        msleep(1500);
        clearScreen();

        // Allocation de la mémoire pour prop1
        int **prop1;
        prop1 = malloc(taille_plateau_multi * sizeof(int *));
        if (prop1 == NULL)
        {
            allocation_error_print_general("prop_lmuti");
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

        rules_interface(max_rounds_multi, taille_plateau_multi);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi, taille_plateau_multi);

        if (mode_multi == 2)
        {
            clearScreen();
            liste1 = initialisation_plateau(prop1, taille_plateau_multi, number_of_navires_multi);
            liste2 = initialisation_plateau(prop2, taille_plateau_multi, number_of_navires_multi);

            while (repeat_multi)
            {
                // loop's logic
                printf("\nRound No %d\n\n", (*NbJoue_global) / 2); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printf("\n\e[1;32m%s\e[0m is playing\n", player1);
                printing_the_grille_v3(prop2, taille_plateau_multi);
                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                    if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_multi, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player1, *NbNav1, number_of_navires_multi);
                        printing_the_grille_v3(prop2, taille_plateau_multi);
                        msleep(3000);
                    }
                }

                clearScreen();
                printf("\nRound No %d\n\n", ((*NbJoue_global) / 2) - 1); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma
                printf("\n\e[1;33m%s\e[0m is playing\n", player2);
                printing_the_grille_v3(prop1, taille_plateau_multi);

                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa
                    if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_multi, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player2, *NbNav2, number_of_navires_multi);
                        printing_the_grille_v3(prop1, taille_plateau_multi);
                        msleep(3000);
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
                        win_graphics(taille_plateau_multi, prop1, (*NbJoue_global - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_multi, prop1, (*NbJoue_global - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else if (mode_multi == 1)
        {
            clearScreen();
            printf("\n\e[0;34m%s\e[0m creer maintenant le plateu pour \e[0;36m%s\e[0m\n", player1, player2);
            liste1 = initialisation_plateau_custom(prop1, taille_plateau_multi, number_of_navires_multi);
            clearScreen();
            printf("\n\e[0;36m%s\e[0m creer maintenant le plateu pour \e[0;34m%s\e[0m\n", player2, player1);
            liste2 = initialisation_plateau_custom(prop2, taille_plateau_multi, number_of_navires_multi);
            while (repeat_multi)
            {
                // loop's logic
                printf("\nRound No %d\n\n", (*NbJoue_global) / 2); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printf("\n\e[1;32m%s\e[0m is playing\n", player1);
                printing_the_grille_v3(prop2, taille_plateau_multi);
                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                    if (proposition_joueur(prop2, NbJoue_global, liste2, taille_plateau_multi, NbNav1)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player1, *NbNav1, number_of_navires_multi);
                        printing_the_grille_v3(prop2, taille_plateau_multi);
                        msleep(3000);
                    }
                }

                clearScreen();
                printf("\nRound No %d\n\n", ((*NbJoue_global) / 2) - 1); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma
                printf("\n\e[1;33m%s\e[0m is playing\n", player2);
                printing_the_grille_v3(prop1, taille_plateau_multi);

                if (waitForMenuKeypress())
                {
                    midle_game_menu_saving_unavailable(max_rounds_multi, taille_plateau_multi, 1); // case mupliplayer for mode = 1
                }
                else
                {
                    // prop2 is created by the player2 for the player1 and vice versa
                    if (proposition_joueur(prop1, NbJoue_global, liste1, taille_plateau_multi, NbNav2)) // NbNav_global and NbJoue are updated on the function's core via pointers
                    {
                        clearScreen();
                        printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player2, *NbNav2, number_of_navires_multi);
                        printing_the_grille_v3(prop1, taille_plateau_multi);
                        msleep(3000);
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
                        win_graphics(taille_plateau_multi, prop1, (*NbJoue_global - 1) / 2, 2, player1);
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
            clearScreen();
            printf("Error entering a sub-mode on multiplayer\n");
            exit(-3);
        }
    }
    if (mode == AI)
    {
        int mode_AI = game_mode_ai();

        clearScreen();
        printf("\n\e[0;102mBienvenue au IA mode\e[0m\n");
        init_nb_aleatoire();
        int taille_plateau_multi = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
        msleep(1500);
        clearScreen();
        int number_of_navires_multi = get_user_input("How many boats do you want to be included on the game ? ", "It has to be minimum 1 for an optimized game. Try again: ", "For a game that respects the rules, you can place up to 6 navires. Try again: ", 1, 6);
        msleep(1500);
        clearScreen();
        char *player1 = get_user_name("What's your name ? ");
        msleep(1500);
        clearScreen();
        char player2[8] = "DrixAI";

        // Allocation de la mémoire pour prop1
        int **prop11;
        prop11 = malloc(taille_plateau_multi * sizeof(int *));
        if (prop11 == NULL)
        {
            allocation_error_print_general("prop11");
        }

        for (int i = 0; i < taille_plateau_multi; i++)
        {
            prop11[i] = malloc(taille_plateau_multi * sizeof(int));
            if (prop11[i] == NULL)
            {
                allocation_error_print_with_id("prop11 row", i);
            }
        }

        initialize_plate(taille_plateau_multi, prop11);

        // Allocation de la mémoire pour prop2
        int **prop22;
        prop22 = malloc(taille_plateau_multi * sizeof(int *));
        if (prop22 == NULL)
        {
            allocation_error_print_general("prop22");
        }

        for (int i = 0; i < taille_plateau_multi; i++)
        {
            prop22[i] = malloc(taille_plateau_multi * sizeof(int));
            if (prop22[i] == NULL)
            {
                allocation_error_print_with_id("prop22 row", i);
            }
        }

        initialize_plate(taille_plateau_multi, prop22);
        Liste_Navire liste11;
        Liste_Navire liste22;

        int coulle11 = 0;
        int *NbNav11 = &coulle11;

        int round_global_custom = 3; // used to show the number of the round
        int *NbJoue_global_custom = &round_global_custom;

        int coulle22 = 0;
        int *NbNav22 = &coulle22;

        bool repeat_multi_custom = true; // be used when positioning ships in the begining and to repeat the game procedure

        clearScreen();
        printf("\n\e[0;34m%s\e[0m creer maintenant le plateu pour \e[0;36m%s\e[0m\n", player1, player2);
        liste11 = initialisation_plateau_custom(prop11, taille_plateau_multi, number_of_navires_multi);
        clearScreen();
        liste22 = initialisation_plateau(prop22, taille_plateau_multi, number_of_navires_multi);
        printf("%s has created the game plate for you as well %s\n", player2, player1);
        msleep(2000);
        clearScreen();

        int max_rounds_multi_custom;
        ajuster_tours(taille_plateau_multi, &max_rounds_multi_custom, number_of_navires_multi, 2);
        rules_interface(max_rounds_multi_custom, taille_plateau_multi);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_multi_custom, taille_plateau_multi);

        if (mode_AI == 2) // mode Spark
        {
            while (repeat_multi_custom)
            {
                // loop's logic
                printf("\nRound No %d\n\n", (*NbJoue_global_custom) / 2); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printf("\n\e[1;32m%s\e[0m is playing\n", player1);
                printing_the_grille_v3(prop22, taille_plateau_multi);

                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop22, NbJoue_global_custom, liste22, taille_plateau_multi, NbNav11)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    clearScreen();
                    printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player1, *NbNav11, number_of_navires_multi);
                    printing_the_grille_v3(prop22, taille_plateau_multi);
                    msleep(3000);
                }

                clearScreen();
                printf("\nRound No %d\n\n", ((*NbJoue_global_custom) / 2) - 1); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma
                printf("\n\e[1;33m%s\e[0m is playing\n", player2);

                // APPEL AU FONCTION AI
                tour_ia_random_v1(prop11, taille_plateau_multi, liste11, NbNav22, NbJoue_global_custom);

                // printing the evolutuon of AI
                printing_the_grille_v3(prop11, taille_plateau_multi);

                // decision making if the user wins or loses the game
                if (round_global_custom == max_rounds_multi_custom && (*NbNav11 < number_of_navires_multi || *NbNav22 < number_of_navires_multi))
                {
                    repeat_multi_custom = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav11 == number_of_navires_multi || *NbNav22 == number_of_navires_multi)
                {
                    if (*NbNav11 > *NbNav22)
                    {
                        win_graphics(taille_plateau_multi, prop11, (*NbJoue_global_custom - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_multi, prop11, (*NbJoue_global_custom - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else if (mode_AI == 1) // mode Firewall
        {
            Liste_Point our_list;
            our_list = creer_liste_Point_vide();

            int x_now, y_now;
            int state;

            x_now = nb_random(0, taille_plateau_multi - 1);
            y_now = nb_random(0, taille_plateau_multi - 1);
            ajouter_element_liste_Point(&our_list, x_now, y_now);

            int x_next, y_next, x_prev, y_prev;

            tour_ia_random_v1(prop11, taille_plateau_multi, liste11, NbNav22, NbJoue_global_custom); // v1 is used here because we want just to start the game
            printf("%s as already played once!\n", player2);

            bool repeat_generator = true;
            bool not_skip_action = true;

            while (repeat_multi_custom)
            {
                // loop's logic
                printf("\nRound No %d\n\n", (*NbJoue_global_custom) / 2); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
                printf("\n\e[1;32m%s\e[0m is playing.\n", player1);
                printing_the_grille_v3(prop22, taille_plateau_multi);

                // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
                if (proposition_joueur(prop22, NbJoue_global_custom, liste22, taille_plateau_multi, NbNav11)) // NbNav_global and NbJoue are updated on the function's core via pointers
                {
                    clearScreen();
                    printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", player1, *NbNav11, number_of_navires_multi);
                    printing_the_grille_v3(prop22, taille_plateau_multi);
                    msleep(3000);
                }

                clearScreen();
                printf("\nRound No %d\n\n", ((*NbJoue_global_custom) / 2) - 1); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma
                printf("\n\e[1;33m%s\e[0m is playing\n", player2);

                // Calling AI V2
                if (not_skip_action)
                {
                    x_prev = x_now;
                    y_prev = y_now;

                    while (repeat_generator)
                    {
                        state = 0;
                        x_now = nb_random(0, taille_plateau_multi - 1);
                        y_now = nb_random(0, taille_plateau_multi - 1);
                        Tableau_Point T = sequence_points_liste_vers_tableau(our_list); // loto_numbers liste to a table

                        // verifying the uniqueness
                        for (int i = 0; i < T.taille; i++)
                        {
                            if (prop11[(T.tab[i]).x][(T.tab[i]).y] != VIDE)
                            {
                                state = 1; // action 1 means that this number already found before
                            }
                        }

                        if (state != 1)
                        {
                            repeat_generator = false;
                        }

                        // when exited from the previous loop, it means that we found a unique number
                    }

                    repeat_generator = true; // re-initialize the variable to be used in another round

                    ajouter_element_liste_Point(&our_list, x_now, y_now);
                    not_skip_action = tour_ia_random_v2(prop11, taille_plateau_multi, liste11, NbNav11, NbJoue_global_custom, x_prev, y_prev, x_now, y_now, &x_next, &y_next);
                }
                else
                {
                    x_prev = x_now;
                    y_prev = y_now;

                    x_now = x_next; //was updated from the previous
                    y_now = y_next;

                    not_skip_action = tour_ia_random_v2(prop11, taille_plateau_multi, liste11, NbNav11, NbJoue_global_custom, x_prev, y_prev, x_now, y_now, &x_next, &y_next);
                }


                // printing the evolutuon of AI
                printing_the_grille_v3(prop11, taille_plateau_multi);

                // decision making if the user wins or loses the game
                if (round_global_custom == max_rounds_multi_custom && (*NbNav11 < number_of_navires_multi || *NbNav22 < number_of_navires_multi))
                {
                    repeat_multi_custom = false;
                    lost_graphics(1);
                    return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
                }
                if (*NbNav11 == number_of_navires_multi || *NbNav22 == number_of_navires_multi)
                {
                    if (*NbNav11 > *NbNav22)
                    {
                        win_graphics(taille_plateau_multi, prop11, (*NbJoue_global_custom - 1) / 2, 2, player1);
                    }
                    else
                    {
                        win_graphics(taille_plateau_multi, prop11, (*NbJoue_global_custom - 1) / 2, 2, player2);
                    }

                    return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
                }
            }
        }
        else
        {
            printf("Error entering a sub mode in AI\n");
            exit(-6);
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
        printf("\n\033[1;36mThe previous game has been loaded from the server succesfully! For your reference, here are the rules:\033[0m\n");
        rules_interface(max_rounds_load, taille_plateau_load);
        msleep(100);
        waitForKeypress();
        waitForKeypress();
        clearScreen();
        rules_reminder(max_rounds_load, taille_plateau_load);

        while (repeat_load)
        {
            // loop's logic
            printf("\nRound No %d\n\n", *NbJoue_load);
            printing_the_grille_v3(prop_load, taille_plateau_load);
            if (waitForMenuKeypress())
            {
                if (midle_game_menu(max_rounds_load, taille_plateau_load, 2, COMPUTER) == 1) // 1 is internal code foe saving the progress and continuing another time
                {
                    api_save_game(number_of_navires_load, taille_plateau_load, coulle_load, round_load, prop_load, liste_load);
                    clearScreen();
                    printf("\n\e[0;32mThe game has been saved succesfully on the server!\e[0m\n");
                    exit(5);
                }
            }
            else
            {
                if (proposition_joueur(prop_load, NbJoue_load, liste_load, taille_plateau_load, NbNav_load))
                {
                    clearScreen();
                    printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n\n", *NbNav_load, number_of_navires_load);
                    printing_the_grille_v3(prop_load, taille_plateau_load);
                    msleep(3000);
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