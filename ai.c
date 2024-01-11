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

    int mode_AI = game_mode_ai();

    clearScreen();
    printf("\n\e[0;102mBienvenue au IA mode\e[0m\n");
    init_nb_aleatoire();
    int taille_plateau_multi = get_user_input("Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", 4, 25);
    clearScreenWait(0.8);
    int number_of_navires_multi = get_user_input("Combien de bateaux souhaitez-vous inclure dans le jeu ? ", "Il doit être au minimum 1 pour un jeu optimisé. Réessayez : ", "Pour un jeu qui respecte les règles, vous pouvez placer jusqu'à 6 navires. Essayez à nouveau : ", 1, 6);
    clearScreenWait(0.8);
    char *player1 = get_user_name("Quel est ton nom ? ");
    clearScreenWait(0.8);
    char player2[8] = "DrixAI";

    // Allocation de la mémoire pour prop1
    int **prop1;
    prop1 = malloc(taille_plateau_multi * sizeof(int *));
    if (prop1 == NULL)
    {
        allocation_error_print_general("prop1");
    }

    for (int i = 0; i < taille_plateau_multi; i++)
    {
        prop1[i] = malloc(taille_plateau_multi * sizeof(int));
        if (prop1[i] == NULL)
        {
            allocation_error_print_with_id("prop1 row", i);
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
    Liste_Navire liste11;
    Liste_Navire liste22;

    int coulle11 = 0;
    int *NbNav11 = &coulle11;

    int round_global_custom = 3; // used to show the number of the round
    int *NbJoue_global_custom = &round_global_custom;

    int coulle22 = 0;
    int *NbNav22 = &coulle22;

    bool repeat_multi_custom = true; // be used when positioning ships in the begining and to repeat the game procedure

    waitTime(3, "creer maintenant le plateu pour", 34, 36, player1, player2);
    liste11 = initialisation_plateau_custom(prop1, taille_plateau_multi, number_of_navires_multi);
    clearScreen();
    liste22 = initialisation_plateau(prop2, taille_plateau_multi, number_of_navires_multi);
    printf("%s has created the game plate for you as well %s\n", player2, player1);
    clearScreenWait(2);

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
            printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing\n\n", (*NbJoue_global_custom) / 2, player1); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
            printing_the_grille_v2(prop2, taille_plateau_multi);

            // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
            if (proposition_joueur(prop2, NbJoue_global_custom, liste22, taille_plateau_multi, NbNav11)) // NbNav_global and NbJoue are updated on the function's core via pointers
            {
                game_mode_graphics_congratulations(prop2, taille_plateau_multi, number_of_navires_multi, *NbNav11, 1, player1);
            }

            clearScreen();
            printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;33m%s\e[0m is playing\n\n", ((*NbJoue_global_custom) / 2) - 1, player2); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

            // APPEL AU FONCTION AI
            tour_ia_random_v1(prop1, taille_plateau_multi, liste11, NbNav22, NbJoue_global_custom);

            // printing the evolutuon of AI
            printing_the_grille_v2(prop1, taille_plateau_multi);

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
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global_custom - 1) / 2, 2, player1);
                }
                else
                {
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global_custom - 1) / 2, 2, player2);
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
        tour_ia_random_v2(prop1, taille_plateau_multi, liste11, NbNav11, NbJoue_global_custom, 0, 0, x_now, y_now, &x_next, &y_next, 1);
        printf("%s as already played once!\n", player2);

        bool repeat_generator = true;
        bool not_skip_action = true;

        while (repeat_multi_custom)
        {
            // loop's logic
            printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing.\n\n", (*NbJoue_global_custom) / 2, player1); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
            printing_the_grille_v2(prop2, taille_plateau_multi);

            // prop2 is created by the player2 for the player1 and vice versa (the same for liste2)
            if (proposition_joueur(prop2, NbJoue_global_custom, liste22, taille_plateau_multi, NbNav11)) // NbNav_global and NbJoue are updated on the function's core via pointers
            {
                game_mode_graphics_congratulations(prop2, taille_plateau_multi, number_of_navires_multi, *NbNav11, 1, player1);
            }

            clearScreen();
            printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;33m%s\e[0m is playing\n\n", ((*NbJoue_global_custom) / 2) - 1, player2); // in that case there is also -1 because there was an iteration that increased the number of NbJoue_global before really changing the number of current round. It's based on the principles of the euclidian division in c that takes the part before the comma

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
                        if (prop1[(T.tab[i]).x][(T.tab[i]).y] != VIDE)
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
                not_skip_action = tour_ia_random_v2(prop1, taille_plateau_multi, liste11, NbNav11, NbJoue_global_custom, x_prev, y_prev, x_now, y_now, &x_next, &y_next, 0);
            }
            else
            {
                x_prev = x_now;
                y_prev = y_now;

                x_now = x_next; // was updated from the previous
                y_now = y_next;

                not_skip_action = tour_ia_random_v2(prop1, taille_plateau_multi, liste11, NbNav11, NbJoue_global_custom, x_prev, y_prev, x_now, y_now, &x_next, &y_next, 0);
            }

            // printing the evolutuon of AI
            printing_the_grille_v2(prop1, taille_plateau_multi);

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
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global_custom - 1) / 2, 2, player1);
                }
                else
                {
                    win_graphics(taille_plateau_multi, prop1, (*NbJoue_global_custom - 1) / 2, 2, player2);
                }

                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }
    else
    {
        error_graphics(6);
    }
    return 1;
}