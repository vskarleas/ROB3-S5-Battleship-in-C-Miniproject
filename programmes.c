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
#define AUCUN_NAVIRE -1 //deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2 //one point was found

#define COULE 10 // congratsulations

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

	do
	{
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
					printf(" ^ ");
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
					printf(" ^ ");
					break;
				}
			}
		}
		printf("\n");
	}
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
					printf(" \033[1;34m.\033[1;0m |");
					break;

				case NAVIRE_TROUVE:
					printf(" \033[1;33m*\033[1;0m |");
					break;

				case AUCUN_NAVIRE:
					printf(" \033[1;35mo\033[1;0m |");
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m |");
					break;

				case NAVIRE: // TO BE REMOVED. IT WAS PLACED TO VISUALIZE THE DIFFERENT BOATS. IT HAS TO BE REPLACED WITH A .
					printf(" \033[1;34m&\033[1;0m |");
					break;
				}
			}
			else
			{
				state = table[i][h];
				switch (state)
				{
				case VIDE:
					printf(" \033[1;34m.\033[1;0m |"); //no points of navire here
					break;

				case NAVIRE_TROUVE:
					printf(" \033[1;33m*\033[1;0m |");
					break;

				case AUCUN_NAVIRE:
					printf(" \033[1;35mo\033[1;0m |"); //already played this point and no point of a navire was found
					break;

				case COULE:
					printf(" \033[1;36mx\033[1;0m |"); //all the points of a navire were found
					break;

				case NAVIRE: // TO BE REMOVED. IT WAS PLACED TO VISUALIZE THE DIFFERENT BOATS. IT HAS TO BE REPLACED WITH A .
					printf(" \033[1;34m&\033[1;0m |");
					break;
				}
			}
		}
		printf("\n----");
		for (int i = 0; i < taille_plateau; i++)
		{
			printf("----");
		}
		printf("-\n");
	}
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

Cellule_Liste_Navire *creer_element_liste_Navire(Navire v)
{
	Cellule_Liste_Navire *el;
	el = (Cellule_Liste_Navire *)malloc(sizeof(Cellule_Liste_Navire));
	if (el == NULL)
	{
		fprintf(stderr, "creer_element_liste_Navire : allocation impossible\n");
		exit(-1);
	}
	el->data = v;
	el->suiv = NULL;
	return el;
}

Liste_Navire creer_liste_Navire_vide()
{
	Liste_Navire L = {0, NULL, NULL};
	return L;
}

void ajouter_element_liste_Navire(Liste_Navire *L, Navire e)
{
	Cellule_Liste_Navire *el;

	el = creer_element_liste_Navire(e);
	if (L->taille == 0)
	{
		/* premier �l�ment de la liste */
		L->first = L->last = el;
	}
	else
	{
		L->last->suiv = el;
		L->last = el;
	}
	L->taille++;
	return;
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

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	int randing, random_orientation, x_tmp, y_tmp;
	bool verification;

	// creating the six boats and putting them on the plateau (invisible to the user)
	printf("Initializing the game\n\n");
	for (int i = 0; i < 6; i++)
	{
		/* Initialising navire randomly on the game's plate */
		msleep(1000); // delay process for 2 seconds in order to provide real aleartory results

		randing = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // logic for choosing the maximum length of a navire depending the size of the plate
		random_orientation = nb_random(0, 3);
		x_tmp = nb_random(0, taille_plateau - 1);
		y_tmp = nb_random(0, taille_plateau - 1);

		verification = true;

		while (verification)
		{
			if (est_valide_pro(plateau, x_tmp, y_tmp, random_orientation, randing, taille_plateau))
			{
				verification = false;
			}
			else
			{
				random_orientation = nb_random(0, 3);
				randing = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // required, oterwiese we may arrive to situations that a new navire is impossible to be created with the previous taille taken before for a size 4x4 of a grille
				x_tmp = nb_random(0, taille_plateau - 1);
				y_tmp = nb_random(0, taille_plateau - 1);
			}
		}

		nav.taille = randing;

		nav.premiere_case.x = x_tmp;
		nav.premiere_case.y = y_tmp;
		nav.id = i;

		switch (random_orientation)
		{
		case UP:
			nav.sens = UP;
			for (int j = nav.premiere_case.x; j > nav.premiere_case.x - nav.taille; j--)
			{
				plateau[j][nav.premiere_case.y] = 1;
			}

			break;

		case LEFT:
			nav.sens = LEFT;
			for (int j = nav.premiere_case.y; j > nav.premiere_case.y - nav.taille; j--)
			{
				plateau[nav.premiere_case.x][j] = 1;
			}

			break;

		case DOWN:
			nav.sens = DOWN;
			for (int j = nav.premiere_case.x; j < nav.premiere_case.x + nav.taille; j++)
			{
				plateau[j][nav.premiere_case.y] = 1;
			}

			break;

		case RIGHT:
			nav.sens = RIGHT;
			for (int j = nav.premiere_case.y; j < nav.premiere_case.y + nav.taille; j++)
			{
				plateau[nav.premiere_case.x][j] = 1;
			}

			break;

		default:
			break;
		}

		ajouter_element_liste_Navire(&liste, nav);
		//TO BE REMOVED ONCE TESTS ARE COMPLETED
		printf("\n");
		printing_the_grille_v3(plateau, taille_plateau);
		printf("\n================================================================\n");
	}

	return liste;
}

void copier_navires(int **prop, int **plateau, int taille_plateau)
{

	for (int i = 0; i < taille_plateau; i++)
	{
		for (int j = 0; j < taille_plateau; j++)
		{
			prop[i][j] = plateau[i][j];
		}
	}
	return;
}

bool navire_found(int **plateau, int **prop, Liste_Navire L)
{
	Cellule_Liste_Navire *el = L.first;

	int counter = 0;
	while (el != NULL)
	{
		switch (el->data.sens)
		{
		case UP:
			for (int j = el->data.premiere_case.x; j > el->data.premiere_case.x - el->data.taille; j--)
			{
				if (plateau[j][el->data.premiere_case.y] == 2)
				{
					counter++;
				}
			}

			break;

		case LEFT:
			for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
			{
				if (plateau[el->data.premiere_case.x][j] == 2)
				{
					counter++;
				}
			}

			break;

		case DOWN:
			for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
			{
				if (plateau[j][el->data.premiere_case.y] == 2)
				{
					counter++;
				}
			}

			break;

		case RIGHT:
			for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
			{
				if (plateau[el->data.premiere_case.x][j] == 2)
				{
					counter++;
				}
			}

			break;

		default:
			break;
		}

		if (counter == el->data.taille)
		{
			switch (el->data.sens)
			{
			case UP:
				for (int j = el->data.premiere_case.x; j > el->data.premiere_case.x - el->data.taille; j--)
				{
					prop[j][el->data.premiere_case.y] = 10;
				}

				break;

			case LEFT:
				for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
				{
					prop[el->data.premiere_case.x][j] = 10;
				}

				break;

			case DOWN:
				for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
				{
					prop[j][el->data.premiere_case.y] = 10;
				}

				break;

			case RIGHT:
				for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
				{
					prop[el->data.premiere_case.x][j] = 10;
				}

				break;

			default:
				break;
			}

			return true;
		}

		el = el->suiv;
	}

	return false;
}

void update_prop(int **prop, int x, int y)
{
	
}