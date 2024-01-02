// #############################################################################
// # File user_input.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "programmes.h"

int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max)
{
    int var;

    printf("\n%s", message);
    scanf("%d", &var);

    while (var < min || var > max)
    {
        if (var < min)
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m%s", error_message);
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m%s", error_message_2);
        }

        scanf("%d", &var);
        printf("\n");
    }

    printf("Merci beaucoup!\n\n");

    return var;
}

char *get_user_name(char message[1024])
{
    char *name = (char *)malloc(30 * sizeof(char));

    if (name == NULL) {
        fprintf(stderr, "Memory allocation failed for name\n");
        exit(EXIT_FAILURE);
    }

    printf("\n%s", message);
    scanf("%29s", name);  // Use a width specifier to prevent buffer overflow

    printf("Merci beaucoup!\n\n");

    return name;
}

void rules_interface(int rounds, int taille_plateau)
{
    printf("\nRULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. \nThe goal is to find the hiden vessels coordinates. There are %d rounds.\n\n", taille_plateau, taille_plateau, rounds);
    printf("The coordinates x & y can take any value between 1 and %d\n", taille_plateau);
    printf("\n\033[1;36mWhen you are ready to start, press ENTER on the keyboard\033[0m\n");
    printf("================================================================\n");
}

void rules_interface_temps(int rounds, int taille_plateau)
{
    printf("\nRULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. \nThe goal is to find the hiden vessels coordinates. You have %d seconds to find the navires.\n\n", taille_plateau, taille_plateau, rounds);
    printf("The coordinates x & y can take any value between 1 and %d\n", taille_plateau);
    printf("\n\033[1;36mWhen you are ready to start, press ENTER on the keyboard\033[0m\n");
    printf("================================================================\n");
}

void rules_reminder(int rounds, int taille_plateau)
{
    printf("\nRULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. \nThe goal is to find the hiden vessels coordinates. There are %d rounds.\n\n", taille_plateau, taille_plateau, rounds);
    printf("The coordinates x & y can take any value between 1 and %d\n", taille_plateau);
    printf("================================================================\n");
}

void rules_reminder_temps(int rounds, int taille_plateau)
{
    printf("\nRULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. \nThe goal is to find the hiden vessels coordinates. You have %d seconds to find the navires.\n\n", taille_plateau, taille_plateau, rounds);
    printf("The coordinates x & y can take any value between 1 and %d\n", taille_plateau);
    printf("================================================================\n");
}

void lost_graphics(int mode)
{
    printf("\n=================================\n");
    printf("=========== Game over ===========\n");
    printf("=================================\n");
    if (mode == 1)
    {
        printf("     YOU RAN OUT OF ROUNDS       \n");
    }
    else if (mode == 2) //case solo option temps
    {
        printf("      YOU RAN OUT OF TIME        \n");
    }
}

void win_graphics(int taille_plateau, int **prop, int round_nb, int mode, char name[30])
{
    clearScreen();
    printf("\nTotal numbers of rounds that were played: %d\n\n", round_nb);
    if (mode == 1)
    {
        printing_the_grille_v3(prop, taille_plateau);
        printf("\n=====================================\n");
        printf("=========== Game finished ===========\n");
        printf("=====================================\n");
        printf("              YOU WIN                \n");
    }
    else if (mode == 2) // case multiplayer
    {
        printf("\n=====================================\n");
        printf("=========== Game finished ===========\n");
        printf("=====================================\n");
        printf("\033[0;33m%s\033[1;0m wins since he/she found more navires\n", name);
    }
}

void allocation_error_print_with_id(char reference[512], int i)
{
    fprintf(stderr, "Failed to allocate memory for %s %d.\n", reference, i);
    exit(-1);
}

void allocation_error_print_general(char reference[512])
{
    fprintf(stderr, "Failed to allocate memory for %s.\n", reference);
    exit(-1);
}

int game_mode_menu()
{
    char userInput[20];

    while (true)
    {
        printf("Choose an option (AI, Computer, Multiplayer or Load). Load alloes to continue a game from a previous session [suitable for computer mode only]. You can close the game by taping Cancel: ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "multiplayer") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "computer") == 0)
        {
            return 1;
        }
        else if (strcmp(userInput, "ai") == 0)
        {
            return 3;
        }
        else if (strcmp(userInput, "cancel") == 0)
        {
            return -1;
        }
        else if (strcmp(userInput, "load") == 0)
        {
            return 5;
        }
        else
        {
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from AI, Computer, Load, or Multiplayer.\n");
        }
    }
    return 0;
}

int game_mode_solo()
{
    char userInput[20];

    while (true)
    {
        printf("\nChoose now between the modes Temps or Rounds: ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "temps") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "rounds") == 0)
        {
            return 1;
        }
        else
        {
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Temps and Rounds.\n");
        }
    }
    return 0;
}

int midle_game_menu(int rounds, int taille_plateau, int version, int mode)
{
    char userInput[20];
    clearScreen();
    printf("\nPAUSED\n");

    while (true)
    {
        printf("What do you want to do ? Choose an option (Save, Cancel, Rules or Exit): ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "save") == 0)
        {
            if (version == 2 && mode == 2) // multiplayer mode
            {
                clearScreen();
                return 1;
            }
            else
            {
                float progress = 0.0;
                for (int i = 0; i < 100; i++)
                {
                    progress = 0.01 + progress;
                    msleep(73);
                    clearScreen();
                    printf("\nGAME IS GETTING SAVED...\n");
                    printProgress(progress);
                }
                return 1;
            }
        }
        else if (strcmp(userInput, "cancel") == 0)
        {
            clearScreen();
            printf("\nGAME CONTINUED\n");
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0)
        {
            clearScreen();
            rules_reminder(rounds, taille_plateau);
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0)
        {
            clearScreen();
            printf("\n\033[1;36mThe game has been terminated and it's not saved on the server. See you next time!\033[0m\n");
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Save, Cancel, or Exit. Try again!\n");
        }
    }
}

int midle_game_menu_saving_unavailable(int rounds, int taille_plateau, int mode)
{
    char userInput[20];
    clearScreen();
    printf("\nPAUSED\n");

    while (true)
    {
        printf("What do you want to do ? Choose an option (Save, Cancel, Rules or Exit): ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "save") == 0)
        {

            msleep(73);
            clearScreen();
            printf("\nThe game can not be saved on mode solo time.\n");
            msleep(1000);
            clearScreen();
            printf("\nGAME CONTINUED\n");
            return 1;
        }
        else if (strcmp(userInput, "cancel") == 0)
        {
            clearScreen();
            printf("\nGAME CONTINUED\n");
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0)
        {
            clearScreen();
            if (mode == 2) //case solo option temps
            {
            rules_reminder_temps(rounds, taille_plateau);
            }
            else if (mode == 1) //case multiplayer 
            {
                rules_reminder(rounds, taille_plateau);
            }
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0)
        {
            clearScreen();
            printf("\n\033[1;36mThe game has been terminated and it's not saved on the server. See you next time!\033[0m\n");
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Save, Cancel, or Exit. Try again!\n");
        }
    }
}

bool waitForMenuKeypress()
{
    char userInput[20];
    printf("Press any key and enter to play this round or type Menu to open it: ");
    scanf("%s", userInput);

    // Convert input to lowercase for case-insensitive comparison
    for (int i = 0; i < strlen(userInput); i++)
    {
        userInput[i] = tolower(userInput[i]);
    }

    return (strcmp(userInput, "menu") == 0);
}

void ajuster_temps(int taille_plateau, int *temps_limite)
{
    if (taille_plateau <= 4)
    {
        *temps_limite = 120; // 2 minutes pour un plateau 4x4
    }
    else if (taille_plateau <= 10)
    {
        *temps_limite = 300; // 5 minutes pour un plateau jusqu'à 10x10
    }
    else
    {
        *temps_limite = 600; // 10 minutes pour un plateau plus grand
    }
}

void ajuster_tours(int taille_plateau, int *max_tours, int nb_navires, int mode)
{
    if (mode == 1)
    {
        if (taille_plateau <= 4)
        {
            switch (nb_navires)
            {
            case 1:
                *max_tours = 10;
            case 2:
                *max_tours = 12;
            case 3:
                *max_tours = 13;
            case 4:
                *max_tours = 14;
            case 5:
                *max_tours = 15;
            case 6:
                *max_tours = 20;
            default:
                *max_tours = 20;
            }
        }
        else if (taille_plateau <= 10)
        {
            switch (nb_navires)
            {
            case 1:
                *max_tours = 20;
            case 2:
                *max_tours = 22;
            case 3:
                *max_tours = 23;
            case 4:
                *max_tours = 24;
            case 5:
                *max_tours = 35;
            case 6:
                *max_tours = 40;
            default:
                *max_tours = 40;
            }
        }
        else
        {
            *max_tours = 60;
        }
    }
    else if (mode == 2) // case multiplayer
    {
        *max_tours = 40;
    }
}
