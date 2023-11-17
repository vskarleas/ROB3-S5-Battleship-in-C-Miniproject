//#############################################################################
//# File programmes.c 
//# UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
//# Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
//#############################################################################

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "programmes.h"

/* Basic notions */
// x : columns (sometimes i refers to columns as well)
// y : lines (sometimes j refers to lines as well)

#define UP 0	// y--
#define DOWN 2	// y++
#define LEFT 1	// x--
#define RIGHT 3 // x++

#define OK 1 //navire added on the boats list (parallel to boats checklist)

#define VIDE 0
#define NAVIRE 1 //navire placed on position
#define DEJA_CHOISI 2 // cell was played previously
#define AUCUN_NAVIRE -1
#define NAVIRE_TROUVE 3

#define COULE 10 //congratsulations

int colour = 31;

void init_nb_aleatoire()
{
	srand(time(0));
}

/*Generates random numbers min and max given as arguments*/
int nb_random(int min, int max)
{
	return min + rand() % (max + 1 - min);
}

/* It prints the plate - Treats table of any size and makes sure that the printing is done correctly */
void printing_the_grille(int **table, int taille_plateau)
{
	int state;

	// Printing columns
	printf("   ");
	for (int k = 0; k < taille_plateau; k++)
	{
		if (k <= 9)
		{
			printf(" %d ", k);
		}
		else
		{
			printf("%d ", k);
		}
	}

	// Printing lines
	printf("\n\n");
	int i;
	int h;
	for (i = 0; i < taille_plateau; i++)
	{
		if(i <= 9)
		{
			printf("%d  ", i);
		}
		else
		{
			printf("%d ", i);
		}
		for (h = 0; h < taille_plateau; h++)
		{
			if (h <= 9)
			{
				state = table[i][h];
				switch (state)
				{
				case VIDE:
					printf(" . ");
					break;

				case NAVIRE_TROUVE:
					printf(" x ");
					break;

				case AUCUN_NAVIRE:
					printf(" o ");
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m ");
					break;

				case NAVIRE: //TO BE REMOVED!!!! from the official and final game code
					printf(" \033[1;%dmx\033[1;0m ", colour);
					break;
				}
			}
			else
			{
				state = table[i][h];
				switch (state)
				{
				case VIDE:
					printf(" . ");
					break;

				case NAVIRE_TROUVE:
					printf(" x ");
					break;

				case AUCUN_NAVIRE:
					printf(" o ");
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m ");
					break;

				case NAVIRE: // TO BE REMOVED. IT WAS PLACED TO VISUALIZE THE DIFFERENT BOATS
					printf(" \033[1;%dmx\033[1;0m ", colour);
					break;
				}
			}
		}
		printf("\n");
	}
	colour++;
}

/* Just put a 0 on every cell for initialization */
void initialize_plate(int taille_plateau, int **table)
{
	for (int i = 0; i < taille_plateau; i++)
	{
		for (int j = 0; j < taille_plateau; j++)
		{
			table[i][j] = VIDE;
		}
	}
}

bool est_valide_pro(int **table_navire, int x, int y, int sens, int taille, int taille_plateau)
{
	int max;
	switch (sens)
	{
	case UP: // vers le haut
		max = y - taille;
		if (max < -1)
		{
			return false;
			break;
		}
		for (int j = y; j < max; j--)
		{
			if (table_navire[x][j] == 1)
			{
				return false;
				break;
			}
		}
		return true;
		break;

	case RIGHT: // droit
		max = x + taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int i = x; i < max; i++)
		{
			if (table_navire[i][y] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case DOWN:
		max = y + taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int j = y; j < max; j++)
		{
			if (table_navire[x][j] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case LEFT: 
		max = x - taille;

		if (max < -1)
		{
			return false;
			break;
		}
		for (int i = x; i < max; i--)
		{
			if (table_navire[i][y] == 1)
			{
				return false;
				break;
			}
		}
		return true;
		break;

	default:
		return false;
		break;
	}
}

Navire *creer_navire(int taille, int taille_plateau, int **table_navire, int id)
{
	Navire *nav;
	nav = (Navire *)malloc(sizeof(Navire)); // allocating memory for this specific navire

	if (nav == NULL)
	{
		fprintf(stderr, "failed to allocate memory for navire.\n");
		exit(-1);
	}

	int randing = nb_random(0, 3);
	int x = nb_random(0, taille_plateau - 1);
	int y = nb_random(0, taille_plateau - 1);
	bool verification = true;

	while (verification)
	{
		if (est_valide_pro(table_navire, x, y, randing, taille, taille_plateau))
		{
			verification = false;
		}
		else
		{
			x = nb_random(0, taille_plateau - 1);
			y = nb_random(0, taille_plateau - 1);
		}
	}

	nav->taille = taille;
	nav->premiere_case.x = x;
	nav->premiere_case.y = y;
	nav->id = id; //setting up the unique id of the navire

	switch (randing)
	{
	case UP: 
		nav->sens = UP;
		return nav;
		break;

	case LEFT:
		nav->sens = LEFT;

		return nav;
		break;

	case DOWN:
		nav->sens = DOWN;
		return nav;
		break;

	case RIGHT:
		nav->sens = RIGHT;
		return nav;
		break;

	default:
		return nav;
		break;
	}
}

void copier_grille_de_reference_vers_la_grille_de_jeu(int **table_navire, int **table_jeu, int taille_tableau) //TO BE FIXED!!!!
{
	for (int i = 0; i < taille_tableau; i++)
	{
		for (int j = 0; j < taille_tableau; j++)
		{
			table_jeu[i][j] = table_navire[i][j];
		}
	}
}

void initialisation_plateau(int **table_navire, int taille_plateau, Navire **liste_of_navires, int **boats_checklist)
{
	int randing;
	for (int i = 0; i < 6; i++)
	{
		randing = (taille_plateau < 6) ? nb_random(2, taille_plateau) : nb_random(2, 6); // logic for choosing the maximum length of a navire depending the size of the plate
		Navire *nav = creer_navire(randing, taille_plateau, table_navire, i);

		// Enregistrement du navire dans le tableau
		liste_of_navires[i] = nav;
		boats_checklist[i][0] = OK;

		int x = nav->premiere_case.x;
		int y = nav->premiere_case.y;

		printf("\n================================================\n"); //TO BE REMOVED!!!!
		switch (nav->sens)
		{
		case UP:
			for (int j = y; j > y - nav->taille; j--)
			{
				table_navire[x][j] = 1;
			}

			//TO BE REMOVED!!!!
			// TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation
			printf("Navire No %d created succesfuly. Here are its data:\n", i + 1);
			printf("Grille %dx%d\nTaille de navire: %d\nSens: %d\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, nav->sens, x, y);
			// END OF TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON nav

			break;
		case DOWN:
			for (int j = y; j < y + nav->taille; j++)
			{
				table_navire[x][j] = 1;
			}
			
			//TO BE REMOVED!!!!
			// TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation
			printf("Navire No %d created succesfuly. Here are its data:\n", i + 1);
			printf("Grille %dx%d\nTaille de navire: %d\nSens: %d\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, nav->sens, x, y);
			// END OF TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON nav
			break;
		case RIGHT:
			for (int j = x; j < x + nav->taille; j++)
			{
				table_navire[j][y] = 1;
			}

			//TO BE REMOVED!!!!
			// TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation
			printf("Navire No %d created succesfuly. Here are its data:\n", i + 1);
			printf("Grille %dx%d\nTaille de navire: %d\nSens: %d\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, nav->sens, x, y);
			// END OF TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON nav
			break;
		case LEFT:
			for (int j = x; j > x - nav->taille; j--)
			{
				table_navire[j][y] = 1;
			}

			//TO BE REMOVED!!!!
			// TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation
			printf("Navire No %d created succesfuly. Here are its data:\n", i + 1);
			printf("Grille %dx%d\nTaille de navire: %d\nSens: %d\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, nav->sens, x, y);
			// END OF TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON nav
			break;
		}

		printf("\n"); //TO BE REMOVED!!!!
		printing_the_grille(table_navire, taille_plateau); //TO BE REMOVED!!!!
	}
}

void proposition_joueur(int **plateau, int **prop, int *NbTouche, int *NbJoue, int *NbToucheNav, int taille_plateau)
{
	int x, y;

	printf("Entrez les coordonnées (x, y) pour tirer: ");
	if (scanf("%d %d", &x, &y) != 2)
	{
		printf("Entrée invalide. Veuillez entrer des nombres.\n");
		while (getchar() != '\n')
			; // cleaning the buffer entrance
		return;
	}

	x--;
	y--; // Ajustement pour l'indexation

	if (x < 0 || x >= taille_plateau || y < 0 || y >= taille_plateau)
	{
		printf("Coordonnées hors du plateau. Veuillez réessayer.\n");
		return;
	}

	(*NbJoue)++;

	if (prop[x][y] != 0)
	{
		printf("Deja joué.\n");
	}
	else if (plateau[x][y] > 0)
	{
		printf("Touché!\n");
		prop[x][y] = 1;
		(*NbTouche)++;
		int idNavire = plateau[x][y];
		NbToucheNav[idNavire - 1]++; // Considering that every navire has a unique ID (1, 2, 3, ...)
	}
	else
	{
		printf("À l'eau.\n");
		prop[x][y] = 0;
	}
}