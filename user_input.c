//#############################################################################
//# File user_input.c 
//# UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
//# Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
//#############################################################################

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int get_user_input()
{
    bool repeater = true;

    int taille_plateau;
    printf("Donner la taille du tableau: ");
    scanf("%d", &taille_plateau);
    if (taille_plateau < 4)
    {
        while (repeater)
        {
            printf("\033[0;33mATTENTION!\033[1;0m La taille doit etre au minimum 4. Redonner la taille: ");
            scanf("%d", &taille_plateau);
            printf("\n");
            if (taille_plateau >= 4)
            {
                repeater = false;
            }
        }
    }
    printf("C'est parti!\n\n");

    return taille_plateau;
}