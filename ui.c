// #############################################################################
// # File ui.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "ui.h"
#include "api.h"
#include "programmes.h"

#define LANG_ENGLISH 0
#define LANG_FRENCH 2

// Function to choose the language
int choose_language()
{
    char language_str[124];
    printf("\nChoose your language English or French\n\e[0;36mChoisissez votre langue Anglais ou Francais: \e[0m");
    while (true)
    {
        scanf("%s", language_str);
        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(language_str); i++)
        {
            language_str[i] = tolower(language_str[i]);
        }

        if (strcmp(language_str, "english") == 0 || strcmp(language_str, "anglais") == 0)
        {
            return LANG_ENGLISH; // for english
        }
        else if (strcmp(language_str, "francais") == 0 || strcmp(language_str, "french") == 0)
        {
            return LANG_FRENCH; // for french
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: English, French, Francais, or/ou Anglais: ");
        }
    }
}

// Asking a user for a number that corresponds to message and error_message and returning that value
int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max, int language)
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
                char *message_str[4] = {"Make sure you type a number between", "and", "Assurez-vous de saisir un nombre entre", "et"};
                printf("\n\033[0;33mATTENTION!\033[1;0m %s %d %s %d\n", message_str[language], min, message_str[language + 1], max);
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

    char *message_str_2[4] = {"Thank you", "", "Merci beaucoup", ""};
    printf("%s!\n\n", message_str_2[language]);

    return var;
}

// HUMOUR Function: If it's the name of one of the profesors the game is completed with the profesor to win every single time :)
bool checkSpecialName(const char *name)
{
    return (strcmp(name, "plumet") == 0 || strcmp(name, "carillet") == 0 || strcmp(name, "maurice") == 0 || strcmp(name, "nathan") == 0 || strcmp(name, "frederic") == 0 || strcmp(name, "lilian") == 0 || strcmp(name, "denis") == 0 || strcmp(name, "christophe") == 0);
}

// Some humour here :) - Connection with the previosu function
void displaySpecialWinMessage(int language)
{
    char *message_str[4] = {"You won because you are the prof and you have all the power!", "", "Vous avez gagné car vous êtes le prof et vous avez tous les pouvoirs!", ""};
    printf("%s\n", message_str[language]);
}

// Assking user's name
char *get_user_name(char message[1024], int language)
{
    char *name = (char *)malloc(30 * sizeof(char));
    char temp[100];

    if (name == NULL)
    {
        fprintf(stderr, "Memory allocation failed for name\n");
#ifdef __APPLE__
        system("killall afplay");
#endif
        exit(EXIT_FAILURE);
    }

    printf("\n%s", message);
    scanf("%29s", name); // Use a width specifier to prevent buffer overflow

    strcpy(temp, name);

    // transforming the name to lowercase for comparing
    for (int i = 0; name[i]; i++)
    {
        name[i] = tolower(name[i]);
    }

    if (checkSpecialName(name))
    {
        displaySpecialWinMessage(language);
        free(name);
        exit(7); // Game completed with the prof that wins :)
    }

    strcpy(name, temp); // going to the previous state of the name since no case of Humour was found :|

    char *message_str[4] = {"Thank you", "", "Merci beaucoup", ""};
    printf("%s!\n\n", message_str[language]);

    return name;
}

// Printing the rules of the game
void rules_interface(int rounds, int taille_plateau, int language)
{
    char *rules[4] = {"RULES", "", "REGLES", ""};
    printf("\n%s\n", rules[language]);
    char *message[4] = {"We are playing in a field of", "On every round you will be asked to provide the coordinates x & y for a vessel.", "Nous jouons sur un terrain de", ""};
    char *message_explanation[4] = {"The goal is to find the hiden vessels coordinates. There are", "rounds.", "Le but est de trouver les coordonnées des vaisseaux cachés. Il y a", "tours."};
    printf("%s %dx%d. %s \n%s %d %s\n\n", message[language], taille_plateau, taille_plateau, message[language + 1], message_explanation[language], rounds, message_explanation[language + 1]);
    char *message_str[4] = {"The coordinates x & y can take any value between 1 and", "(ex: A, B, C, ...)", "Les coordonnées x et y peuvent prendre n'importe quelle valeur comprise entre 1 et", ""};
    printf("%s %d %s\n", message_str[language], taille_plateau, message_str[1]);
    char *message_str_play[4] = {"When you are ready to start, press ENTER on the keyboard", "", "Lorsque vous êtes prêt à commencer, appuyez sur ENTER sur le clavier", ""};
    printf("\n\033[1;36m%s\033[0m\n", message_str_play[language]);
    printf("================================================================\n");
}

// Printing the rules of the game in SOlo mode with option Time
void rules_interface_temps(int seconds, int taille_plateau, int language)
{
    char *rules[4] = {"RULES", "", "REGLES", ""};
    printf("\n%s\n", rules[language]);
    char *message[4] = {"We are playing in a field of", "On every round you will be asked to provide the coordinates x & y for a vessel.", "Nous jouons sur un terrain de", ""};
    char *message_explanation[4] = {"The goal is to find the hiden vessels coordinates. You have", "seconds to find the navires.", "Le but est de trouver les coordonnées des navires cachés. Vous avez", "secondes pour trouver les navires."};
    printf("%s %dx%d. %s \n%s %d %s\n\n", message[language], taille_plateau, taille_plateau, message[language + 1], message_explanation[language], seconds, message_explanation[language + 1]);
    char *message_str[4] = {"The coordinates x & y can take any value between 1 and", "(ex: A, B, C, ...)", "Les coordonnées x et y peuvent prendre n'importe quelle valeur comprise entre 1 et", ""};
    printf("%s %d %s\n", message_str[language], taille_plateau, message_str[1]);
    char *message_str_play[4] = {"When you are ready to start, press ENTER on the keyboard", "", "Lorsque vous êtes prêt à commencer, appuyez sur ENTER sur le clavier", ""};
    printf("\n\033[1;36m%s\033[0m\n", message_str_play[language]);
    printf("================================================================\n");
}

// Printing the rule sof the game once it's asked from the user during a pause
void rules_reminder(int rounds, int taille_plateau, int language)
{
    char *rules[4] = {"RULES", "", "REGLES", ""};
    printf("\n%s\n", rules[language]);
    char *message[4] = {"We are playing in a field of", "On every round you will be asked to provide the coordinates x & y for a vessel.", "Nous jouons sur un terrain de", ""};
    char *message_explanation[4] = {"The goal is to find the hiden vessels coordinates. There are", "rounds.", "Le but est de trouver les coordonnées des vaisseaux cachés. Il y a", "tours."};
    printf("%s %dx%d. %s \n%s %d %s\n\n", message[language], taille_plateau, taille_plateau, message[language + 1], message_explanation[language], rounds, message_explanation[language + 1]);
    char *message_str[4] = {"The coordinates x & y can take any value between 1 and", "(ex: A, B, C, ...)", "Les coordonnées x et y peuvent prendre n'importe quelle valeur comprise entre 1 et", ""};
    printf("%s %d %s\n", message_str[language], taille_plateau, message_str[1]);
    printf("================================================================\n");
}

// Printing the rule sof the game once it's asked from the user during a pause on a Solo game with option Time selected
void rules_reminder_temps(int seconds, int taille_plateau, int language)
{
    char *rules[4] = {"RULES", "", "REGLES", ""};
    printf("\n%s\n", rules[language]);
    char *message[4] = {"We are playing in a field of", "On every round you will be asked to provide the coordinates x & y for a vessel.", "Nous jouons sur un terrain de", ""};
    char *message_explanation[4] = {"The goal is to find the hiden vessels coordinates. You have", "seconds to find the navires.", "Le but est de trouver les coordonnées des navires cachés. Vous avez", "secondes pour trouver les navires."};
    printf("%s %dx%d. %s \n%s %d %s\n\n", message[language], taille_plateau, taille_plateau, message[language + 1], message_explanation[language], seconds, message_explanation[language + 1]);
    char *message_str[4] = {"The coordinates x & y can take any value between 1 and", "(ex: A, B, C, ...)", "Les coordonnées x et y peuvent prendre n'importe quelle valeur comprise entre 1 et", ""};
    printf("%s %d %s\n", message_str[language], taille_plateau, message_str[1]);
    printf("================================================================\n");
}

// Lost graphics appear on screen
void lost_graphics(int mode, int language)
{
    char *lines[4] = {"=================================", "", "=================================", ""};
    char *extra[4] = {"===========", "", "==========", ""};
    printf("\n%s\n", lines[language]);
    char *over[4] = {"Game over", "", "Jeu terminé", ""};
    printf("%s %s %s\n", extra[language], over[language], extra[language]);
    printf("%s\n", lines[language]);
    char *out_of[4] = {"YOU RAN OUT OF ROUNDS", "YOU RAN OUT OF TIME", "PLUS DE TOURS", "PLUS DE TEMPS"};
    if (mode == 1)
    {
        printf("     %s       \n", out_of[language]);
    }
    else if (mode == 2) // case solo option temps
    {
        printf("      %s        \n", out_of[language + 1]);
    }
}

// Win graphics appear on screen
void win_graphics(int taille_plateau, int **prop, int round_nb, int mode, char name[30], int language)
{
    clearScreen();
    char *total_number[4] = {"Total numbers of rounds that were played:", "", "Nombre total de tours joués :", ""};
    printf("\n%s %d\n\n", total_number[language], round_nb);

    char *finished[4] = {"Game finished", "  YOU WIN  ", " Jeu terminé ", "VOUS GAGNEZ"};
    char *won[4] = {"wins since he/she found more navires on time (before running out of rounds)", "", "gagne puisqu'il a trouvé plus de navires à temps (avant de manquer de tours)", ""};
    printing_the_grille_v2(prop, taille_plateau);
    printf("\n=====================================\n");
    printf("=========== %s ===========\n", finished[language]);
    printf("=====================================\n");
    if (mode == 1)
    {
        printf("            %s              \n", finished[language + 1]);
    }
    else if (mode == 2)
    {
        printf("\033[0;33m%s\033[1;0m %s\n", name, won[language]);
    }
}

// Printing error message for allocating memory with corresponding ID
void allocation_error_print_with_id(char reference[512], int i)
{
    fprintf(stderr, "Failed to allocate memory for %s %d.\n", reference, i);
    exit(-1);
}

// Printing error message for allocating memory with corresponding general
void allocation_error_print_general(char reference[512])
{
    fprintf(stderr, "Failed to allocate memory for %s.\n", reference);
    exit(-1);
}

// Menu to choose the game mode
int game_mode_menu(int language)
{
    char userInput[20];
    char *msg[4] = {"Choose an option (AI, Solo, Multiplayer or Load). Load allows to continue a game from a previous", "session [suitable for Solo mode only]. You can close the game by taping Cancel. Your choice: ", "Choisissez une option (IA, Solo, Multijoueur ou Chargement). Charger permet de continuer une partie d'une précédente", "session [convient uniquement au mode Solo]. Vous pouvez fermer le jeu en appuyant sur Annuler. Votre choix: "};
    char *msg_attention[4] = {"You can only choose from AI, Solo, Load, or Multiplayer.", "", "Vous ne pouvez choisir qu'entre IA, Solo, Chargement ou Multijoueur.", ""};

    while (true)
    {
        printf("\n%s \n%s", msg[language], msg[language + 1]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "multiplayer") == 0 || strcmp(userInput, "multijoueur") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "solo") == 0)
        {
            return 1;
        }
        else if (strcmp(userInput, "ai") == 0 || strcmp(userInput, "ia") == 0)
        {
            return 3;
        }
        else if (strcmp(userInput, "cancel") == 0 || strcmp(userInput, "annuler") == 0)
        {
            return -1;
        }
        else if (strcmp(userInput, "load") == 0 || strcmp(userInput, "chargement") == 0)
        {
            return 5;
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
    return 0;
}

// Choose the game mode once Solo game is selected
int game_mode_solo(int language)
{
    char userInput[20];
    char *msg[4] = {"Choose now between the modes Time or Rounds: ", "", "Choisissez maintenant entre les modes Temps ou Tours : ", ""};
    char *msg_attention[4] = {"You can only choose from Time and Rounds.", "", "Vous ne pouvez choisir que parmi les Temps et les Tours.", ""};

    while (true)
    {
        printf("\n%s", msg[language]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "temps") == 0 || strcmp(userInput, "time") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "rounds") == 0 || strcmp(userInput, "tours") == 0)
        {
            return 1;
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
    return 0;
}

// Choose the game mode once Multiplayer game is selected
int game_mode_multi(int language)
{
    char userInput[20];
    char *msg[4] = {"Choose now between the modes Automatic or Custom. Automatic creates different plates based on algorithm. Custom allow to every user to create his own plate.", "Your choice : ", "Choisissez maintenant entre les modes Automatique ou Personnalisé. Automatique crée différentes plaques basées sur un algorithme. Personnalisé permet à chaque utilisateur de créer sa propre plateau de jeu.", "Votre choix : "};
    char *msg_attention[4] = {"You can only choose from Automatic and Custom.", "", "Vous ne pouvez choisir que parmi les Automatique et Personnalise.", ""};

    while (true)
    {
        printf("\n%s\n%s", msg[language], msg[language + 1]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "automatic") == 0 || strcmp(userInput, "automatique") == 0)
        {
            return 2;
        }
        else if (strcmp(userInput, "custom") == 0 || strcmp(userInput, "personnalise") == 0)
        {
            return 1;
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
    return 0;
}

// Choose the game mode once AI game is selected
int game_mode_ai(int language)
{
    char userInput[20];
    char *msg[4] = {"Choose now between the available AI versions Spark or Fireball. Fireball has a more advanced logic than Spark.", "Your choice : ", "Choisissez maintenant entre les versions AI disponibles Spark ou Fireball. Fireball a une logique plus avancée que Spark.", "Votre choix : "};
    char *msg_ext[4] = {"You can only choose between Spark and Fireball", "", "Vous ne pouvez choisir qu'entre Spark et Fireball", ""};

    while (true)
    {
        printf("\n%s\n%s", msg[language], msg[language + 1]);
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
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s.\n", msg_ext[language]);
        }
    }
    return 0;
}

// Menu that is printed when the game is paused (it provides different options)
int midle_game_menu(int rounds, int taille_plateau, int version, int mode, int language)
{
    char userInput[20];
    char *msg[4] = {"PAUSED", "", "EN PAUSE", ""};
    char *choice[4] = {"What do you want to do ? Choose an option (Save, Close, Rules or Exit): ", "", "Qu'est-ce que vous voulez faire ? Choisissez une option (Enregistrer, Fermer, Règles ou Quitter) : ", ""};
    char *str[4] = {"GAME IS GETTING SAVED...", "GAME CONTINUED", "LE JEU EST EN COURS DE SAUVEGARDE...", "JEU CONTINUÉ"};
    char *msg_attention[4] = {"You can only choose from Save, Close, Rules or Exit. Try again!", "The game has been terminated and it's not saved on the server. See you next time!", "Vous pouvez uniquement choisir entre Enregistrer, Fermer, Regles ou Quitter. Essayer à nouveau!", "Le jeu a été terminé et il n'est pas enregistré sur le serveur. À la prochaine!"};

    clearScreen();
    printf("\n%s\n", msg[language]);

    while (true)
    {
        printf("%s", choice[language]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "save") == 0 || strcmp(userInput, "enregistrer") == 0)
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
                    printf("\n%s\n", str[language]);
                    printProgress(progress);
                }
                return 1;
            }
        }
        else if (strcmp(userInput, "close") == 0 || strcmp(userInput, "fermer") == 0)
        {
            clearScreen();
            printf("\n%s\n", str[language + 1]);
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0 || strcmp(userInput, "regles") == 0)
        {
            clearScreen();
            rules_reminder(rounds, taille_plateau, language);
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0 || strcmp(userInput, "quitter") == 0)
        {
            clearScreen();
            printf("\n\033[1;36m%s\033[0m\n", msg_attention[language + 1]);
            api_delete_game_file(language);

            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
}

// Menu that is printed when the game is paused (it provides different options) on a Solo game with option Time selected
int midle_game_menu_saving_unavailable_temps(int rounds, int taille_plateau, int mode, int *time_paused, int language)
{
    time_t pause_start = time(NULL);
    time_t pause;
    char *msg[4] = {"PAUSED", "", "EN PAUSE", ""};
    char *choice[4] = {"What do you want to do ? Choose an option (Save, Close, Rules or Exit): ", "", "Qu'est-ce que vous voulez faire ? Choisissez une option (Enregistrer, Fermer, Règles ou Quitter) : ", ""};
    char *str[4] = {"The game can not be saved on mode solo time.", "GAME CONTINUED", "Le jeu ne peut pas être sauvegardé en mode solo.", "JEU CONTINUÉ"};
    char *msg_attention[4] = {"You can only choose from Save, Close, Rules or Exit. Try again!", "The game has been terminated and it's not saved on the server. See you next time!", "Vous pouvez uniquement choisir entre Enregistrer, Fermer, Regles ou Quitter. Essayer à nouveau!", "Le jeu a été terminé et il n'est pas enregistré sur le serveur. À la prochaine!"};

    char userInput[20];
    clearScreen();
    printf("\n%s\n", msg[language]);

    while (true)
    {
        pause = time(NULL);
        printf("%s", choice[language]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "save") == 0 || strcmp(userInput, "enregistrer") == 0)
        {

            msleep(73);
            clearScreen();
            printf("\n%s\n", str[language]);
            msleep(1000);
            clearScreen();
            printf("\n%s\n", str[language + 1]);
            *time_paused = (int)difftime(pause, pause_start);
            return 1;
        }
        else if (strcmp(userInput, "close") == 0 || strcmp(userInput, "fermer") == 0)
        {
            clearScreen();
            printf("\n%s\n", str[language + 1]);
            *time_paused = (int)difftime(pause, pause_start);
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0 || strcmp(userInput, "regles") == 0)
        {
            clearScreen();
            if (mode == 2) // case solo option temps
            {
                rules_reminder_temps(rounds, taille_plateau, language);
            }
            else if (mode == 1) // case multiplayer
            {
                rules_reminder(rounds, taille_plateau, language);
            }
            *time_paused = (int)difftime(pause, pause_start);
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0 || strcmp(userInput, "quitter") == 0)
        {
            clearScreen();
            printf("\n\033[1;36m%s\033[0m\n", msg_attention[language + 1]);
            api_delete_game_file(language);
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
}

// Menu that is printed when the game is paused (it provides different options) where saving the game isn't available
int midle_game_menu_saving_unavailable(int rounds, int taille_plateau, int mode, int language)
{
    char userInput[20];
    clearScreen();
    char *msg[4] = {"PAUSED", "", "EN PAUSE", ""};
    printf("\n%s\n", msg[language]);

    char *choice[4] = {"What do you want to do ? Choose an option (Save, Close, Rules or Exit): ", "", "Qu'est-ce que vous voulez faire ? Choisissez une option (Enregistrer, Fermer, Règles ou Quitter) : ", ""};
    char *str[4] = {"The game can not be saved on mode solo time.", "GAME CONTINUED", "Le jeu ne peut pas être sauvegardé en mode solo.", "JEU CONTINUÉ"};
    char *msg_attention[4] = {"You can only choose from Save, Close, Rules or Exit. Try again!", "The game has been terminated and it's not saved on the server. See you next time!", "Vous pouvez uniquement choisir entre Enregistrer, Fermer, Regles ou Quitter. Essayer à nouveau!", "Le jeu a été terminé et il n'est pas enregistré sur le serveur. À la prochaine!"};

    while (true)
    {
        printf("%s", choice[language]);
        scanf("%s", userInput);

        // Convert input to lowercase for case-insensitive comparison and returns
        for (int i = 0; i < strlen(userInput); i++)
        {
            userInput[i] = tolower(userInput[i]);
        }

        if (strcmp(userInput, "save") == 0 || strcmp(userInput, "enregistrer") == 0)
        {

            msleep(73);
            clearScreen();
            printf("\n%s\n", str[language]);
            msleep(1000);
            clearScreen();
            printf("\n%s\n", str[language + 1]);
            return 1;
        }
        else if (strcmp(userInput, "close") == 0 || strcmp(userInput, "fermer") == 0)
        {
            clearScreen();
            printf("\n%s\n", str[language + 1]);
            return 2;
        }
        else if (strcmp(userInput, "rules") == 0 || strcmp(userInput, "regles") == 0)
        {
            clearScreen();
            if (mode == 2) // case solo option temps
            {
                rules_reminder_temps(rounds, taille_plateau, language);
            }
            else if (mode == 1) // case multiplayer
            {
                rules_reminder(rounds, taille_plateau, language);
            }
            return 3;
        }
        else if (strcmp(userInput, "exit") == 0 || strcmp(userInput, "quitter") == 0)
        {
            clearScreen();
            printf("\n\033[1;36m%s\033[0m\n", msg_attention[language + 1]);
            api_delete_game_file(language);
            exit(4); // code on the log that determines that the game was exited without any saving action taking place
        }
        else
        {
            clearScreen();
            printf("\n\033[0;33mATTENTION!\033[1;0m: %s\n", msg_attention[language]);
        }
    }
}

// Accesing either the Menu (paused mode) or playing this round
bool waitForMenuKeypress(int language)
{
    char userInput[20];
    char *msg[4] = {"Press any key and enter to play this round or type Menu to open it: ", "", "Appuyez sur n'importe quelle touche et entrez pour jouer à ce tour ou tapez Menu pour l'ouvrir : ", ""};
    printf("%s", msg[language]);
    scanf("%s", userInput);

    // Convert input to lowercase for case-insensitive comparison
    for (int i = 0; i < strlen(userInput); i++)
    {
        userInput[i] = tolower(userInput[i]);
    }

    return (strcmp(userInput, "menu") == 0);
}

// Logic to choose total time for a Solo game with option Time selected
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

// Logic to choose total number of rounds (limiting the number of rounds when playing against teh computer [Solo mode option Rounds])
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
                *max_tours = 32;
            case 3:
                *max_tours = 43;
            case 4:
                *max_tours = 54;
            case 5:
                *max_tours = 65;
            case 6:
                *max_tours = 70;
            default:
                *max_tours = 100;
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
                *max_tours = 33;
            case 4:
                *max_tours = 44;
            case 5:
                *max_tours = 55;
            case 6:
                *max_tours = 60;
            default:
                *max_tours = 70;
            }
        }
        else
        {
            *max_tours = 110;
        }
    }
    else if (mode == 2) // case multiplayer
    {
        *max_tours = 40;
    }
}

// Printing different messages on different parts of the game via identification
void custom_graphics_on_proposition(int i, int **plateau, int taille_plateau, int colour, int mode, int id, int language)
{
    char *msg[4] = {"Initializing the game...", "Enter the coordinates of the chosen point. The accepted format is number+letter (ex: 2B).", "Initialisation du jeu...", "Entrez les coordonnées du point choisi. Le format accepté est number+letter (ex: 2B)."};
    char *msg_ext[4] = {"Your choice : ", "Vessel Size: ", "Votre choix : ", "Taille du navire: "};
    char *err_msg[4] = {"Invalid input format. The second character must be a letter. Try again!", "The first character must be a number. Try again!", "Format d'entrée invalide. Le deuxième caractère doit être une lettre. Essayer à nouveau!", "Le premier caractère doit être un nombre. Essayer à nouveau!"};
    char *err_cor[4] = {"The selected coordinates are out of the game's plate. TRY AGAIN!", "OOOPPPPSSSS... We couldn't generate your previous navire because either there was conflict with another navire or because it would be out of the game table \e[4;35mLet's try again Navire No", "Les coordonnées sélectionnées sont hors de la plaque du jeu. ESSAYER À NOUVEAU!", "OOOPPPSSS... Nous n'avons pas pu générer votre navire précédent car il y a eu un conflit avec un autre navire ou parce qu'il serait hors de la table de jeu \e[4;35 Réessayons Navire Non"};
    char *dir[4] = {"Choose direction: UP(0), DOWN(2), LEFT(1), RIGHT(3)", "Invalid input format. We can accept only the values 0, 1, 2, 3. Try again!", "Choisissez la direction : HAUT(0), BAS(2), GAUCHE(1), DROITE(3)", "Format d'entrée invalide. Nous ne pouvons accepter que les valeurs 0, 1, 2, 3. Réessayez !"};
    char *dir_tai[4] = {"Size should be between 1 and 6.", "", "La taille doit etre entre 1 et 6.", ""};
    if (colour == -2) // initilaising game mode
    {
        printf("\n%s\n\n", msg[language]);
        printProgress(0.01 * mode * i);
    }
    else
    {
        if (i == -1) // proposition jour graphics
        {
            switch (mode)
            {
            case 0:
                printf("%s\nVotre choix: ", msg[language + 1]);
                break;
            case 1:
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language], msg_ext[language]);
                break;
            case 2:
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language + 1], msg_ext[language]);
                break;
            case 3:
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language], msg_ext[language]);
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
                printf("\033[0;33m%s\033[0m\n\n", err_cor[language]);
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
                printf("\n%s %d\e[0m\n\n", err_cor[language + 1], i + 1);
                break;
            default:
                printf("\n\e[4;%dmNavire No %d\e[0m\n\n", colour, i + 1);
                break;
            }
            printing_the_grille_v2(plateau, taille_plateau);
            switch (mode)
            {
            case 0:
                printf("\n%s", msg_ext[language + 1]);
                break;
            case 1:
                printf("\n%s %s", dir_tai[language], msg_ext[language + 1]);
                break;
            case 2:
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language], msg_ext[language]);
                break;
            case 3:
                printf("\n\033[0;33mATTENTION!\033[1;0m %s\n%s\nDirection: ", dir[language + 1], dir[language]);
                break;
            case 4:
                printf("\n%s\nDirection: ", dir[language]);
                break;
            case 5:
                printf("\n%s\n%s", msg[language + 1], msg_ext[language]);
                break;
            case 6:
                printf("\n\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language], msg_ext[language]);
                break;
            case 7:
                printf("\n\n\033[0;33mATTENTION!\033[1;0m %s\n%s", err_msg[language + 1], msg_ext[language]);
                break;
            default:
                break;
            }
        }
    }
}

// Congratulations message printing function
void game_mode_graphics_congratulations(int **prop, int taille_plateau, int nb_navires, int nb_navires_found, int id, char *buffer, int language)
{
    char *msg[4] = {"Congratsulations, you found a navire", "so far out of", "Félicitations, vous avez trouvé jusqu'à présent un navire", "sur"};
    clearScreen();
    switch (id)
    {
    case 1:
        printf("\033[0;36m\n=====================  %s %s. %d %s %d!!!  =====================\033[0m\n\n", msg[language], buffer, nb_navires_found, msg[language + 1], nb_navires);
        break;
    default:
        printf("\033[0;36m\n=====================  %s. %d %s %d!!!  =====================\033[0m\n\n", msg[language], nb_navires_found, msg[language + 1], nb_navires);
        break;
    }
    printing_the_grille_v2(prop, taille_plateau);
    msleep(3000);
}

// Printing any errors function
void error_graphics(int error_code, int language)
{
    clearScreen();
    char *msg[4] = {"The game has been saved succesfully on server!", "", "Le jeu a été sauvegardé avec succès sur le serveur !", ""};
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
        printf("\n\e[0;32m%s\e[0m\n", msg[language]);
        exit(5);
        break;
    default:
        break;
    }
}

// Printing a message indicating that the game has been loaded from the server
void game_loaded_graphics(int max_rounds_load, int taille_plateau_load, int language)
{
    char *msg[4] = {"The previous game has been loaded from the server succesfully! For your reference, here are the rules:", "Round", "Le jeu précédent a été chargé depuis le serveur avec succès ! Pour information, voici les règles :", "Tour"};
    printf("\n\033[1;36m%s\033[0m\n", msg[language]);
    rules_interface(max_rounds_load, taille_plateau_load, language);
    msleep(100);
}

// Indicating on which round we are for different game modes
void new_round_graphics(int round, int taille_plateau, int **prop, int id, char *buffer, int temps, int language)
{
    char *msg[4] = {"Remaining time", "Round", "Temps restant", "Tour"};
    char *msg_ext[4] = {"is playing", "", "joue", ""};
    switch (id)
    {
    case 0:
        printf("\n\e[4;32m%s No %d\e[0m\n\n", msg[language + 1], round);
        break;
    case 1:
        printf("\n\e[4;32m%s No %d | %s: %d sec.\e[0m\n\n", msg[language + 1], round, msg[language], temps);
        break;
    case 2:
        printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;32m%s\e[0m %s\n\n", msg[language + 1], (round) / 2, buffer, msg_ext[language]); // the function proposition_jouer changes the NbJoue_global on every call and there are two calls (two players) before we change the number of round, that's why we divide by 2
        break;
    case 3:
        printf("\n\e[4;32m%s No %d\e[0m\n\n\e[1;32m%s\e[0m %s\n\n", msg[language + 1], ((round) / 2) - 1, buffer, msg_ext[language]); // the function proposition_jouer changes the Nb
        break;
    default:
        break;
    }
    printing_the_grille_v2(prop, taille_plateau);
}