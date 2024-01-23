// #############################################################################
// # File jeu_v2.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "structures.h"
#include "solo.h"
#include "ai.h"
#include "multiplayer.h"
#include "load.h"
#include "programmes.h"
#include "ui.h"

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
#ifdef __APPLE__
    // Create a child process to play the music file
    pid_t pid = fork();
    if (pid == 0)
    {
        // In the child process, play the music file
        system("afplay music.mp3");
        exit(0);
    }
#endif

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
    Liste_Navire liste1, liste2;
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
    int coulle1 = 0;
    int *NbNav1 = &coulle1;

    int round_global = 3; // used to show the number of the round (multiplayer & AI)
    int *NbJoue_global = &round_global;

    int round = 1; // used to show the number of the round (solo version & load)
    int *NbJoue = &round;

    int coulle2 = 0;
    int *NbNav2 = &coulle2;

    int mode = game_mode_menu(language);

    if (mode == COMPUTER)
    {
        solo(prop1, liste1, NbNav1, NbJoue, coulle1, round, taille_plateau_jeu, number_of_navires_jeu, language);
    }

    if (mode == MULTIPLAYER)
    {
        multiplayer(prop1, prop2, liste1, liste2, NbNav1, NbNav2, NbJoue_global, coulle1, coulle2, round_global, taille_plateau_jeu, number_of_navires_jeu, language);
    }
    if (mode == AI)
    {
        ai(prop1, prop2, liste1, liste2, NbNav1, NbNav2, NbJoue_global, coulle1, coulle2, round_global, taille_plateau_jeu, number_of_navires_jeu, language);
    }
    if (mode == LOAD)
    {
        load(language);
    }

#ifdef __APPLE__
    system("killall afplay");
#endif
    return 1;
}