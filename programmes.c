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
#include <unistd.h>

#include "programmes.h"
#include "ui.h"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

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
#define NAVIRE 1		// navire placed on position
#define AUCUN_NAVIRE -1 // deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2 // one point was found

#define COULE 10 // congratsulations

void waitForKeypress()
{
	getchar(); // Waits for a key press
}

void clearScreen()
{
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

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
		if (k < 9)
		{
			printf("| %d ", k + 1);
		}
		else
		{
			printf("|%d ", k + 1);
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
		if (i < 9)
		{
			printf("%d  ||", i + 1);
		}
		else
		{
			printf("%d ||", i + 1);
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
					printf(" \033[0;31mx\033[1;0m |");
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
					printf(" \033[1;34m.\033[1;0m |"); // no points of navire here
					break;

				case NAVIRE_TROUVE:
					printf(" \033[1;33m*\033[1;0m |");
					break;

				case AUCUN_NAVIRE:
					printf(" \033[1;35mo\033[1;0m |"); // already played this point and no point of a navire was found
					break;

				case COULE:
					printf(" \033[0;31mx\033[1;0m |"); // all the points of a navire were found
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

void suprimer_navire(Cellule_Liste_Navire *principal, Liste_Navire *liste)
{
	// Si on est sur le premier maillon (cas particulier à la con)
	if (principal == liste->first)
	{
		// on modifie le premier élément
		liste->first = principal->suiv;
	}
	else
	{
		Cellule_Liste_Navire *prec;
		// il faut rechercher le maillon précédent
		for (prec = liste->first; prec->suiv != principal; prec = prec->suiv)
			;

		// on modifie le précédent
		prec->suiv = principal->suiv;
	}
	// Le lien est sauvegardé, on peut libérer le maillon
	free(principal);
	liste->taille--;
	/* Attention, si ton maillon contient des tableaux d'éléments alloués par malloc, faut d'abord libérer les
		éléments avant de nettoyer le maillon => d'où l'utilité de créer des fonctions de nettoyage associées
		 à chaque type => c'est le premier pas vers la programmation objet                                               */
}

Cellule_Liste_Navire *creer_element_liste_Navire(Navire v)
{
	Cellule_Liste_Navire *el;
	el = (Cellule_Liste_Navire *)malloc(sizeof(Cellule_Liste_Navire));
	if (el == NULL)
	{
		allocation_error_print_general("creer_element_liste_Navire");
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

bool navire_found(int **prop, Liste_Navire L) // this functionw as optimized to sscan the gaeme according to the list of navires created and not the plateau and the prop tables
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
				if (prop[j][el->data.premiere_case.y] == 2)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.x; j > el->data.premiere_case.x - el->data.taille; j--)
				{
					prop[j][el->data.premiere_case.y] = 10;
				}
				return true;
			}

			break;

		case LEFT:
			for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
			{
				if (prop[el->data.premiere_case.x][j] == 2)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
				{
					prop[el->data.premiere_case.x][j] = 10;
				}
				return true;
			}

			break;

		case DOWN:
			for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
			{
				if (prop[j][el->data.premiere_case.y] == 2)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
				{
					prop[j][el->data.premiere_case.y] = 10;
				}
				return true;
			}

			break;

		case RIGHT:
			for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
			{
				if (prop[el->data.premiere_case.x][j] == 2)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
				{
					prop[el->data.premiere_case.x][j] = 10;
				}
				return true;
			}

			break;

		default:
			break;
		}

		counter = 0; // reinitialize counter
		el = el->suiv;
	}

	return false;
}

void update_prop(int **prop, int x, int y)
{
	if (prop[x][y] == 1)
	{
		prop[x][y] = 2;
	}
	else
	{
		prop[x][y] = -1;
	}

	return;
}

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav)
{
	int x, y;
	bool coordinates = true; // used to ask the use new coordinates for a vessel if the previous ones are not inside the specified limits mentioned on the instructions
	bool navire_founded;
	while (coordinates)
	{
		printf("Entrez les coordonnées (x, y) pour tirer: ");
		if (scanf("%d %d", &x, &y) != 2)
		{
			printf("\033[0;33mEntrée invalide. Veuillez entrer des nombres\033[0m\n");
			while (getchar() != '\n')
				; // cleaning the buffer entrance
			return false;
		}

		if (!(x < 1 || x > taille_plateau || y < 1 || y > taille_plateau)) // coordinates verification out of limit
		{

			coordinates = false;
		}
		else
		{
			printf("\033[0;33mThe coordinates x & y can take any value between 1 and %d. TRY AGAIN!\033[0m\n\n", taille_plateau);
		}
	}

	coordinates = true;

	x--;
	y--; // Adjustment for the indexes

	update_prop(prop, x, y);
	(*NbJoue)++; // next round

	navire_founded = navire_found(prop, L);
	if (navire_founded == true)
	{
		(*NbNav)++;
		return true;
	}

	clearScreen();
	return false;
}

void printProgress(double percentage)
{
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
}

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau, int number_of_navires)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	int randing, random_orientation, x_tmp, y_tmp;
	bool verification;
	float percentage = 100 / number_of_navires;

	// creating the six boats and putting them on the plateau (invisible to the user)
	for (int i = 0; i < number_of_navires; i++)
	{
		printf("Initializing the game...\n\n");
		printProgress(0.01 * percentage * i);

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

		// TO BE REMOVED ONCE TESTS ARE COMPLETED
		// printf("Taille: %d\n (x ,y): %d,%d\nDirection: %d\n, ID: %d\n\n", nav.taille, nav.premiere_case.x+1, nav.premiere_case.y+1, random_orientation, nav.id);

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
		clearScreen();
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