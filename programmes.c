// #############################################################################
// # File programmes.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#include "programmes.h"

/* Basic notions */
// ATTENTION: x, y doesn't follow the reperes in real world. Here x is the indicator of line and y is the indicator of column
//  x : lines
//  y : columns

#define UP 0	// y--
#define DOWN 2	// y++
#define LEFT 1	// x--
#define RIGHT 3 // x++

#define OK 1 // navire added on the boats list (parallel to boats checklist)

#define VIDE 0
#define NAVIRE 1	  // navire placed on position
#define DEJA_CHOISI 2 // cell was played previously
#define AUCUN_NAVIRE -1
#define NAVIRE_TROUVE 3

#define COULE 10 // congratsulations

int colour = 31;

int msleep(long tms)
{
    struct timespec ts;
    int ret;

    if (tms < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);

    return ret;
}

/* Starting the random engine */
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
		if (i <= 9)
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

				case NAVIRE: // TO BE REMOVED!!!! from the official and final game code
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

void printing_the_grille_v3(int **table, int taille_plateau)
{
	int state;

	// Printing columns
	printf("x/y ");
	for (int k = 0; k < taille_plateau; k++)
	{
		if (k <= 9)
		{
			printf("| %d ", k);
		}
		else
		{
			printf("|%d ", k);
		}
	}
	printf("|");

	// Printing lines
	printf("\n----");
	for (int g = 0; g < taille_plateau; g++)
	{
		printf("----");
	}
	printf("-\n");
	printf("----");
	for (int g = 0; g < taille_plateau; g++)
	{
		printf("----");
	}
	printf("-\n");
	int i;
	int h;
	for (i = 0; i < taille_plateau; i++)
	{
		if (i <= 9)
		{
			printf("%d  ||", i);
		}
		else
		{
			printf("%d ||", i);
		}
		for (h = 0; h < taille_plateau; h++)
		{
			if (h <= 9)
			{
				state = table[i][h];
				switch (state)
				{
				case VIDE:
					printf(" . |");
					break;

				case NAVIRE_TROUVE:
					printf(" x |");
					break;

				case AUCUN_NAVIRE:
					printf(" o |");
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m |");
					break;

				case NAVIRE: // TO BE REMOVED!!!! from the official and final game code
					printf(" \033[1;%dmx\033[1;0m |", colour);
					break;
				}
			}
			else
			{
				state = table[i][h];
				switch (state)
				{
				case VIDE:
					printf(" . |");
					break;

				case NAVIRE_TROUVE:
					printf(" x |");
					break;

				case AUCUN_NAVIRE:
					printf(" o |");
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m |");
					break;

				case NAVIRE: // TO BE REMOVED. IT WAS PLACED TO VISUALIZE THE DIFFERENT BOATS
					printf(" \033[1;%dmx\033[1;0m |", colour);
					break;
				}
			}
		}
		printf("\n----");
		for (int i = 0; i <taille_plateau; i++)
		{
			printf("----");
		}
		printf("-\n");
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

/*Verifies if a navire can be placed on a specific location with a specific size and orientation */
bool est_valide_pro(int **table_navire, int x, int y, int sens, int taille, int taille_plateau)
{
	int max;
	switch (sens)
	{
	case UP: // vers le haut
		max = x - taille;

		if (max < -1)
		{
			return false;
			break;
		}
		for (int j = x; j > max; j--)
		{
			if (table_navire[j][y] == 1)
			{
				return false;
				break;
			}
		}
		return true;
		break;

	case RIGHT: // droit
		max = y + taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int i = y; i < max; i++)
		{
			if (table_navire[x][i] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case DOWN:
		max = x + taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int j = x; j < max; j++)
		{
			if (table_navire[j][y] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case LEFT:
		max = y - taille;

		if (max < -1)
		{
			return false;
			break;
		}
		for (int i = y; i > max; i--)
		{
			if (table_navire[x][i] == 1)
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

bool est_valide_pro_v2(int **table_navire, int taille_plateau, Tmp_navire *navire)
{
	int max;
	switch (navire->sens)
	{
	case UP: // vers le haut
		max = navire->premiere_case.x - navire->taille;
		if (max < -1)
		{
			return false;
			break;
		}
		for (int j = navire->premiere_case.x; j > max; j--)
		{
			if (table_navire[j][navire->premiere_case.y] == 1)
			{
				return false;
				break;
			}
		}
		return true;
		break;

	case RIGHT: // droit
		max = navire->premiere_case.y + navire->taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int i = navire->premiere_case.y; i < max; i++)
		{
			if (table_navire[navire->premiere_case.x][i] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case DOWN:
		max = navire->premiere_case.x + navire->taille;

		if (max > taille_plateau)
		{
			return false;
			break;
		}
		for (int j = navire->premiere_case.x; j < max; j++)
		{
			if (table_navire[j][navire->premiere_case.y] == 1)
			{
				return false;
				break;
			}
		}

		return true;
		break;

	case LEFT:
		max = navire->premiere_case.y - navire->taille;

		if (max < -1)
		{
			return false;
			break;
		}
		for (int i = navire->premiere_case.y; i > max; i--)
		{
			if (table_navire[navire->premiere_case.x][i] == 1)
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

/* Creation of a navire/boat */
Navire *creer_navire(int taille, int taille_plateau, int **table_navire, int id)
{
	Navire *nav;
	nav = (Navire *)malloc(sizeof(Navire)); // allocating memory for this specific navire

	// Tmp_navire *tmp;
	// tmp = (Tmp_navire *)malloc(sizeof(Tmp_navire));
	/*
	if (nav == NULL || tmp == NULL)
	{
		fprintf(stderr, "failed to allocate memory for navire or tmp_navire.\n");
		exit(-1);
	}
	*/

	// V1 with est_valide_pro
	int randing = nb_random(0, 3);
	int x = nb_random(0, taille_plateau - 1);
	int y = nb_random(0, taille_plateau - 1);

	// V2 with est_valide_pro
	/*
	tmp->taille = taille;
	tmp->sens = nb_random(0, 3);
	tmp->premiere_case.x = nb_random(0, taille_plateau - 1);
	tmp->premiere_case.y = nb_random(0, taille_plateau - 1);
	*/

	bool verification = true;

	while (verification)
	{
		// V1 with est_valide_pro
		if (est_valide_pro(table_navire, x, y, randing, taille, taille_plateau))
		{
			verification = false;
		}
		else
		{
			randing = nb_random(0, 3);
			taille = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // required, oterwiese we may arrive to situations that a new navire is impossible to be created with the previous taille taken before
			x = nb_random(0, taille_plateau - 1);
			y = nb_random(0, taille_plateau - 1);
		}

		// V2 with est_valide_pro
		/*
		if (est_valide_pro_v2(table_navire, taille_plateau, tmp))
		{
			verification = false;
		}
		else
		{
			tmp->premiere_case.x = nb_random(0, taille_plateau - 1);
			tmp->premiere_case.y = nb_random(0, taille_plateau - 1);
		}
		*/
	}

	// Approach is here is to verify everything before save them into nav
	nav->taille = taille;

	// V1 with est_valide_pro
	nav->premiere_case.x = x;
	nav->premiere_case.y = y;

	// V2 with est_valide_pro
	/*
	nav->premiere_case.x = tmp->premiere_case.x;
	nav->premiere_case.y = tmp->premiere_case.x;
	nav->id = id; //setting up the unique id of the navire
	*/

	// V1 with est_valide_pro
	switch (randing)

	// V2 with est_valide_pro
	/*
	switch (tmp->sens)
	*/
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

/* Adding navires randomly on the game's plate */
void initialisation_plateau(int **table_navire, int taille_plateau, Navire **liste_of_navires)
{
	int randing, x, y;
	for (int i = 0; i < 6; i++)
	{
		msleep(1000);//delay process for 2 seconds in order to provide real aleartory results
		randing = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // logic for choosing the maximum length of a navire depending the size of the plate
		Navire *nav = creer_navire(randing, taille_plateau, table_navire, i);

		// Enregistrement du navire dans le tableau
		liste_of_navires[i] = nav;

		x = nav->premiere_case.x;
		y = nav->premiere_case.y;

		printf("\n================================================\n"); // TO BE REMOVED!!!!
		switch (nav->sens)
		{
		case UP:
			for (int j = x; j > x - nav->taille; j--)
			{
				table_navire[j][y] = 1;
			}

			break;
		case DOWN:
			for (int j = x; j < x + nav->taille; j++)
			{
				table_navire[j][y] = 1;
			}

			break;
		case RIGHT:
			for (int j = y; j < y + nav->taille; j++)
			{
				table_navire[x][j] = 1;
			}

			break;
		case LEFT:
			for (int j = y; j > y - nav->taille; j--)
			{
				table_navire[x][j] = 1;
			}

			break;
		}

		// TO BE REMOVED!!!!
		//  TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation
		printf("Navire No %d created succesfuly. Here are its data:\n", i + 1);
		if (nav->sens == 0)
		{
			printf("Grille %dx%d\nTaille de navire: %d\nSens: UP\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, x, y);
		}
		if (nav->sens == 2)
		{
			printf("Grille %dx%d\nTaille de navire: %d\nSens: DOWN\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, x, y);
		}
		if (nav->sens == 1)
		{
			printf("Grille %dx%d\nTaille de navire: %d\nSens: LEFT\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, x, y);
		}
		if (nav->sens == 3)
		{
			printf("Grille %dx%d\nTaille de navire: %d\nSens: RIGHT\n(%d,%d)\n\n", taille_plateau, taille_plateau, nav->taille, x, y);
		}

		// END OF TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON nav

		printf("\n");										  // TO BE REMOVED!!!!
		printing_the_grille_v3(table_navire, taille_plateau); // TO BE REMOVED!!!!
	}
}

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
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

void copier_navires(int **prop, int **plateau, int taille_plateau)
{

    for (int i=0; i<taille_plateau; i++)
    {
        for (int j=0; j<taille_plateau; j++)
        {
            prop[i][j] = plateau[i][j];
        }
    }
    return;
}