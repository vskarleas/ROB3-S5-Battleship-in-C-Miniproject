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
    bool repeater = true;

    int var;
    printf("%s", message);
    scanf("%d", &var);
    while (repeater)
    {
        if (var < min || var > max)
        {

            if (var < min)
            {
                printf("\033[0;33mATTENTION!\033[1;0m%s", error_message);
                scanf("%d", &var);
                printf("\n");
                if (var >= min && var <= max)
                {
                    repeater = false;
                }
            }
            else
            {
                printf("\033[0;33mATTENTION!\033[1;0m%s", error_message_2);
                scanf("%d", &var);
                printf("\n");
                if (var >= min && var <= max)
                {
                    repeater = false;
                }
            }
        }
    }
    printf("Merci beaucoup!\n\n");

    return var;
}

void rules_interface(int rounds, int taille_plateau)
{
    printf("\nRULES\n");
    printf("We are playing in a field of %dx%d. On every round you will be asked to provide the coordinates x & y for a vessel. \nThe goal is to find the hiden vessels coordinates. There are %d rounds.\n\n", taille_plateau, taille_plateau, rounds);
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

void lost_graphics()
{
    printf("=================================\n");
    printf("=========== Game over ===========\n");
    printf("=================================\n");
    printf("           YOU LOSE              \n");
}

void win_graphics(int taille_plateau, int **prop, int round_nb)
{
    clearScreen();
    printf("Total numbers of rounds that were played: %d\n\n", round_nb);
    printing_the_grille_v3(prop, taille_plateau);
    printf("\n=====================================\n");
    printf("=========== Game finished ===========\n");
    printf("=====================================\n");
    printf("              YOU WIN                \n");
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