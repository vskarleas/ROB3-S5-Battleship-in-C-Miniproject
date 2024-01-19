// #############################################################################
// # File user_input.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "api.h"

int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max)
{
    int var;
    bool repeater = true;
    char buffer[20];

    while (repeater)
    {
        printf("\n%s", message);
        scanf("%s", buffer);

        while (repeater)
        {
            if (strlen(buffer) != 1 && strlen(buffer) != 2)
            {
                clearScreen();
                printf("\n\033[0;33mATTENTION!\033[1;0m Make sure you type a number between %d and %d\n", min, max);
                printf("\n%s", message);
                scanf("%s", buffer);
            }
            else
            {
                repeater = false;
            }
        }

        var = atoi(buffer);
        repeater = true;

        if (var < min || var > max)
        {
            if (var < min)
            {
                clearScreen();
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n", error_message);
            }
            else
            {
                clearScreen();
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n", error_message_2);
            }
        }
        else
        {
            repeater = false;
        }
    }

    printf("Merci beaucoup!\n\n");

    return var;
}

char *get_user_name(char message[1024])
{
    char *name = (char *)malloc(30 * sizeof(char));

    if (name == NULL)
    {
        fprintf(stderr, "Memory allocation failed for name\n");
        exit(EXIT_FAILURE);
    }

    printf("\n%s", message);
    scanf("%29s", name); // Use a width specifier to prevent buffer overflow

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
    else if (mode == 2) // case solo option temps
    {
        printf("      YOU RAN OUT OF TIME        \n");
    }
}

void win_graphics(int taille_plateau, int **prop, int round_nb, int mode, char name[30])
{
    clearScreen();
    printf("\nTotal numbers of rounds that were played: %d\n\n", round_nb);

    printing_the_grille_v2(prop, taille_plateau);
    printf("\n=====================================\n");
    printf("=========== Game finished ===========\n");
    printf("=====================================\n");
    if (mode == 1)
    {
        printf("              YOU WIN                \n");
    }
    else if (mode == 2)
    {
        printf("\033[0;33m%s\033[1;0m wins since he/she found more navires on time (before running out of rounds)\n", name);
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
        printf("\nChoose an option (AI, Solo, Multiplayer or Load). \nLoad allows to continue a game from a previous \nsession [suitable for Solo mode only]. \nYou can close the game by taping Cancel.\nYour choice: ");
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
        else if (strcmp(userInput, "solo") == 0)
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
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from AI, Solo, Load, or Multiplayer.\n");
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
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Temps and Rounds.\n");
        }
    }
    return 0;
}

int game_mode_multi()
{
    char userInput[20];

    while (true)
    {
        printf("\nChoose now between the modes Automatic or Custom. Automatic creates \ndifferent plates based on algorithm. Custom allow to every user to create his own plate.\n");
        printf("Your choice : ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "automatic") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "custom") == 0)
        {
            return 1;
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Automatic and Custom.\n");
        }
    }
    return 0;
}

int game_mode_ai()
{
    char userInput[20];

    while (true)
    {
        printf("\nChoose now between the available AI versions Spark or Fireball. Fireball has a more advanced logic than Spark.\n");
        printf("Your choice : ");
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "spark") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "fireball") == 0)
        {
            return 1;
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose between Spark and Fireball.\n");
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
                    msleep(18);
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
            api_delete_game_file();
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: You can only choose from Save, Cancel, or Exit. Try again!\n");
        }
    }
}

int midle_game_menu_saving_unavailable_temps(int rounds, int taille_plateau, int mode, int *time_paused)
{
    time_t pause_start = time(NULL);
    time_t pause;

    char userInput[20];
    clearScreen();
    printf("\nPAUSED\n");

    while (true)
    {
        pause = time(NULL);
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
            *time_paused = (int)difftime(pause, pause_start);
            return 1;
        }
        else if (strcmp(userInput, "cancel") == 0)
        {
            clearScreen();
            printf("\nGAME CONTINUED\n");
            *time_paused = (int)difftime(pause, pause_start);
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0)
        {
            clearScreen();
            if (mode == 2) // case solo option temps
            {
                rules_reminder_temps(rounds, taille_plateau);
            }
            else if (mode == 1) // case multiplayer
            {
                rules_reminder(rounds, taille_plateau);
            }
            *time_paused = (int)difftime(pause, pause_start);
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0)
        {
            clearScreen();
            printf("\n\033[1;36mThe game has been terminated and it's not saved on the server. See you next time!\033[0m\n");
            api_delete_game_file();
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
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
            if (mode == 2) // case solo option temps
            {
                rules_reminder_temps(rounds, taille_plateau);
            }
            else if (mode == 1) // case multiplayer
            {
                rules_reminder(rounds, taille_plateau);
            }
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0)
        {
            clearScreen();
            printf("\n\033[1;36mThe game has been terminated and it's not saved on the server. See you next time!\033[0m\n");
            api_delete_game_file();
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
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
                *max_tours = 20;
            case 2:
                *max_tours = 22;
            case 3:
                *max_tours = 23;
            case 4:
                *max_tours = 34;
            case 5:
                *max_tours = 45;
            case 6:
                *max_tours = 50;
            default:
                *max_tours = 60;
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

void custom_graphics_on_proposition(int i, int **plateau, int taille_plateau, int colour, int mode, int id)
{
    if (colour == -2) // initilaising game mode
    {
        printf("\nInitializing the game...\n\n");
        printProgress(0.01 * mode * i);
    }
    else
    {
        if (i == -1) // proposition jour graphics
        {
            switch (mode)
            {
            case 0:
                printf("Entrez les coordonnées du point choisi. Le format accepté est number+letter (ex: 2B).\nVotre choix: ");
                break;
            case 1:
                printf("\n\033[0;33mATTENTION!\033[1;0m Invalid input format. The second character must be a letter. Try again!\nVotre choix: ");
                break;
            case 2:
                printf("\n\033[0;33mATTENTION!\033[1;0m The first character must be a number. Try again!\nVotre choix: ");
                break;
            case 3:
                printf("\n\033[0;33mATTENTION!\033[1;0m Invalid input format. Please enter a number followed by a letter. Try again!\nVotre choix: ");
                break;
            case 4:
                if (id == -1)
                {
                    clearScreen();
                }
                printf("Unidentfied error with the point declaration\n");
                exit(-4);
                break;
            case 5:
                printf("\033[0;33mThe slected coordinates are out of the game's plate. TRY AGAIN!\033[0m\n\n");
                break;
            default:
                break;
            }
        }
        else
        {
            switch (id)
            {
            case 0:
                printf("\nOOOPPPPSSSS...\nWe couldn't generate your previous navire because either \nthere was conflict with another navire or\nbecause it would be out of the game table\n\n\e[4;35mLet's try again Navire No %d\e[0m\n\n", i + 1);
                break;
            default:
                printf("\n\e[4;%dmNavire No %d\e[0m\n\n", colour, i + 1);
                break;
            }
            printing_the_grille_v2(plateau, taille_plateau);
            switch (mode)
            {
            case 0:
                printf("\nTaille du navire: ");
                break;
            case 1:
                printf("\nLa taille doit etre entre 1 et 6. Taille du navire: ");
                break;
            case 2:
                printf("\n\033[0;33mATTENTION!\033[1;0m Invalid input format. Please enter a number followed by a letter. Try again!\nVotre choix: ");
                break;
            case 3:
                printf("\n\033[0;33mATTENTION!\033[1;0m Invalid input format. We can accept only the values 0, 1, 2, 3. Try again!\nChoisir la direction: UP(0), DOWN(2), LEFT(1), RIGHT(3)\nDirection: ");
                break;
            case 4:
                printf("\nChoisir la direction: UP(0), DOWN(2), LEFT(1), RIGHT(3)\nDirection: ");
                break;
            case 5:
                printf("\nEntrez les coordonnées du point de depart. Le format accepté est number+letter (ex: 2B).\nVotre choix: ");
                break;
            case 6:
                printf("\n\n\033[0;33mATTENTION!\033[1;0m Invalid input format. The second character must be a letter. Try again!\nVotre choix: ");
                break;
            case 7:
                printf("\n\n\033[0;33mATTENTION!\033[1;0m The first character must be a number. Try again!\nVotre choix: ");
                break;
            default:
                break;
            }
        }
    }
}

void game_mode_graphics_congratulations(int **prop, int taille_plateau, int nb_navires, int nb_navires_found, int id, char *buffer)
{
    clearScreen();
    switch (id)
    {
    case 1:
        printf("\033[0;36m\n=====================  Congratsulations, you found a navire %s. %d so far out of %d!!!  =====================\033[0m\n\n", buffer, nb_navires_found, nb_navires);
        break;
    default:
        printf("\033[0;36m\n=====================  Congratulations, you found a navire. %d so far out of %d!!!  =====================\033[0m\n\n", nb_navires_found, nb_navires);
        break;
    }
    printing_the_grille_v2(prop, taille_plateau);
    msleep(3000);
}

void error_graphics(int error_code)
{
    clearScreen();
    switch (error_code)
    {
    case 6:
        printf("Error entering a sub mode in AI\n");
        exit(-6);
        break;
    case 4:
        printf("Error entering a sub-mode on solo\n");
        exit(-4);
        break;
    case 3:
        printf("Error entering a sub-mode on multiplayer\n");
        exit(-3);
        break;
    case 5:
        printf("\n\e[0;32mThe game has been saved succesfully on server!\e[0m\n");
        exit(5);
        break;
    default:
        break;
    }
}

void game_loaded_graphics(int max_rounds_load, int taille_plateau_load)
{
    printf("\n\033[1;36mThe previous game has been loaded from the server succesfully! For your reference, here are the rules:\033[0m\n");
    rules_interface(max_rounds_load, taille_plateau_load);
    msleep(100);
}

void new_round_graphics(int round, int taille_plateau, int **prop, int id, char *buffer, int temps)
{
    switch (id)
    {
    case 0:
        printf("\n\e[4;32mRound No %d\e[0m\n\n", round);
        break;
    case 1:
        printf("\n\e[4;32mRound No %d | Temps restant: %d secondes\e[0m\n\n", round, temps);
        break;
    case 2:
        printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing\n\n", (round) / 2, buffer); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
        break;
    case 3:
        printf("\n\e[4;32mRound No %d\e[0m\n\n\e[1;32m%s\e[0m is playing\n\n", ((round) / 2) - 1, buffer);
        break;
    default:
        break;
    }
    printing_the_grille_v2(prop, taille_plateau);
}