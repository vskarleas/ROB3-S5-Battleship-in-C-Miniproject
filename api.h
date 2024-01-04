#ifndef _API_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "programmes.h"

Liste_Navire api_load_game(const char *filename, int *ptr1, int *ptr2, int *ptr3, int *ptr4, int **matrix);

int api_table_size(const char *filename);

void api_clearFile(const char *filename) ;

void api_save_game(int number_of_navires, int taille_plateau, int coulle, int round, int **matrix, Liste_Navire liste);

void api_delete_game_file();

#endif