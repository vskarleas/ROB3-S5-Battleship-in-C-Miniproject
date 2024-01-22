// #############################################################################
// # File jeu.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "api.h"
#include "ui.h"

#define UP 0    // y--
#define DOWN 2  // y++
#define LEFT 1  // x--
#define RIGHT 3 // x++

#define ROUNDS 40 // how many rounds we want per game ?

int main(int argc, char **argv)
{
    const char *welcome_message[4] = {"Battleship game", "It's time to configure the game before choosing the desired mode.", "Bataille Navale", "C'est le moment de parametriser le jeu avant choisir le mode souhaité"};
    char *welcome_message_parameters[4] = {"Now is the time to configure the game before choosing the desired mode", "", "C'est le moment de parametriser le jeu avant choisir le mode souhaité", ""};
    char *round_txt[4] = {"Round","is playing","Tour","est en train de jouer"};
    char *txt_1[4] = {"Give the size of the game table: ", " The size must be at least 4. Resize: ", "Donner la taille du tableau: ", " La taille doit etre au minimum 4. Redonner la taille: "};
    char *txt_2[4] = {" You are sure for such a size. For an optimized game we do not recommend having a table size larger than 25. Try again: ", "How many boats do you want to include in the game ? ", "Tu est sur pour une telle taille. Pour un jeu optimisé on ne recommend pas d'avoir une taille du tableau plus grand que 25. Redonner la taille: ", "Combien de bateaux souhaitez-vous inclure dans le jeu ? "};
    char *txt_3[4] = {"It must be at least 1 for an optimized game. Try again : ", "For a game that respects the rules, you can place up to 6 ships. Try again : ", "Il doit être au minimum 1 pour un jeu optimisé. Réessayez : ", "Pour un jeu qui respecte les règles, vous pouvez placer jusqu'à 6 navires. Essayez à nouveau : "};

    init_nb_aleatoire();
    clearScreen();
    int language = choose_language();
    clearScreen();
     printf("\n\e[0;103m%s V2.1\e[0m\n--------------------------------\n%s\n", welcome_message[language], welcome_message_parameters[language]);
    int taille_plateau = get_user_input(txt_1[language], txt_1[language+1], txt_2[language], 4, 25, language);
    clearScreenWait(0.8);
    printf("\n\e[0;103m%s V2.0\e[0m\n--------------------------------\n%s\n", welcome_message[language], welcome_message[language + 1]);
    int number_of_navires = get_user_input(txt_2[language+1], txt_3[language], txt_3[language+1], 1, 6, language);
    clearScreenWait(0.8);

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

    // Allocation et initialisation des navires
    Liste_Navire liste;
    liste = initialisation_plateau(prop, taille_plateau, number_of_navires, language);

    // game loop
    int coulle = 0;
    int *NbNav = &coulle;
    bool repeat = true; // be used when positioning ships in the begining and to repeat the game procedure

    int round = 1; // used to show the number of the round
    int *NbJoue = &round;
    rules_interface(ROUNDS, taille_plateau, language);
    msleep(100);
    waitForKeypress();
    waitForKeypress();
    clearScreen();
    rules_reminder(ROUNDS, taille_plateau, language);

    while (repeat)
    {
        // loop's logic
        printf("\n%s No %d\n\n",round_txt[language], *NbJoue);
        printing_the_grille_v2(prop, taille_plateau);
        if (waitForMenuKeypress(language))
        {
            if (midle_game_menu(ROUNDS, taille_plateau, 1, 1, language) == 1) // 1 is internal code foe saving the progress and continuing another time, second 1 is for game version and third one indicates the game mode
            {
                api_save_game(number_of_navires, taille_plateau, coulle, round, prop, liste, language);
                error_graphics(5, language);
            }
        }
        else
        {
            if (proposition_joueur(prop, NbJoue, liste, taille_plateau, NbNav, language)) // NbNav and NbJoue are updated on the function's core via pointers
            {
                clearScreen();
                printf("\033[0;36m\n=====================  Congratsulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n\n", *NbNav, number_of_navires);
                printing_the_grille_v2(prop, taille_plateau);
                msleep(300);
            }
            clearScreen();

            // decision making if the user wins or loses the game
            if (round == ROUNDS && *NbNav < number_of_navires)
            {
                repeat = false;
                lost_graphics(2, language);
                return 1; // returns 1 if the user ran out of rounds - it also works as the while(repeat) stopper
            }
            if (*NbNav == number_of_navires)
            {
                win_graphics(taille_plateau, prop, *NbJoue - 1, 1, "", language);
                return 0; // returns 0 if the user found all the ships - it also works as the while(repeat) stopper
            }
        }
    }

    return 10;
}