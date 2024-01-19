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
#include <ctype.h>

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

#define HORIZONTAL 1
#define VERTICAL 2

#define OK 1 // navire added on the boats list (parallel to boats checklist)

#define VIDE 0
#define NAVIRE 1			   // navire placed on position
#define AUCUN_NAVIRE -1		   // deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2		   // one point was found
#define NAVIRE_TROUVE_PLUS_1 3 // one point of a navire was refound (it was choosen before)

#define CUSTOM_NAVIRE 8

#define COULE 10 // congratsulations you find the whole navire

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

int random_color()
{
	int random_number;
	do
	{
		random_number = nb_random(32, 36);
	} while (random_number == 33);

	return random_number;
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

void printing_the_grille_v2(int **table, int taille_plateau)
{
	int state;
	char c = 'A';

	// Printing columns
	printf("x/y ");
	for (int k = 0; k < taille_plateau; k++)
	{
		if (k < 9)
		{
			printf("| %c ", c + k);
		}
		else
		{
			printf("|%c ", c + k);
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

				case NAVIRE_TROUVE_PLUS_1:
					printf(" \033[1;36m*\033[1;0m |");
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
				case CUSTOM_NAVIRE:
					printf(" \e[0;101m \033[1;0m |");
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
					printf(" \033[1;33m*\033[1;0m |"); // one point of navire found
					break;

				case NAVIRE_TROUVE_PLUS_1:
					printf(" \033[1;36m*\033[1;0m |"); // one point of a navire typed at least twice
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
				case CUSTOM_NAVIRE: // TO BE REMOVED. IT WAS PLACED TO VISUALIZE THE DIFFERENT BOATS. IT HAS TO BE REPLACED WITH A .
					printf(" \e[0;101m \033[1;0m |");
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
bool est_valide_pro(int **table_navire, Navire nav, int taille_plateau)
{
	int x = nav.premiere_case.x;
	int y = nav.premiere_case.y;
	int sens = nav.sens;
	int taille = nav.taille;
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
			if (table_navire[j][y] == NAVIRE || table_navire[j][y] == CUSTOM_NAVIRE) // custom navire identification is used for custom game plate creation
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
			if (table_navire[x][i] == NAVIRE || table_navire[x][i] == CUSTOM_NAVIRE)
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
			if (table_navire[j][y] == NAVIRE || table_navire[j][y] == CUSTOM_NAVIRE)
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
			if (table_navire[x][i] == NAVIRE || table_navire[x][i] == CUSTOM_NAVIRE)
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
				if (prop[j][el->data.premiere_case.y] == NAVIRE_TROUVE || prop[j][el->data.premiere_case.y] == NAVIRE_TROUVE_PLUS_1)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.x; j > el->data.premiere_case.x - el->data.taille; j--)
				{
					prop[j][el->data.premiere_case.y] = COULE;
				}
				return true;
			}

			break;

		case LEFT:
			for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
			{
				if (prop[el->data.premiere_case.x][j] == NAVIRE_TROUVE || prop[el->data.premiere_case.x][j] == NAVIRE_TROUVE_PLUS_1)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.y; j > el->data.premiere_case.y - el->data.taille; j--)
				{
					prop[el->data.premiere_case.x][j] = COULE;
				}
				return true;
			}

			break;

		case DOWN:
			for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
			{
				if (prop[j][el->data.premiere_case.y] == NAVIRE_TROUVE || prop[j][el->data.premiere_case.y] == NAVIRE_TROUVE_PLUS_1)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.x; j < el->data.premiere_case.x + el->data.taille; j++)
				{
					prop[j][el->data.premiere_case.y] = COULE;
				}
				return true;
			}

			break;

		case RIGHT:
			for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
			{
				if (prop[el->data.premiere_case.x][j] == NAVIRE_TROUVE || prop[el->data.premiere_case.x][j] == NAVIRE_TROUVE_PLUS_1)
				{
					counter++;
				}
			}

			if (counter == el->data.taille)
			{
				// suprimer_navire(el, &L);
				for (int j = el->data.premiere_case.y; j < el->data.premiere_case.y + el->data.taille; j++)
				{
					prop[el->data.premiere_case.x][j] = COULE;
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
	if (prop[x][y] == NAVIRE)
	{
		prop[x][y] = NAVIRE_TROUVE;
	}
	else
	{
		if (prop[x][y] == NAVIRE_TROUVE || prop[x][y] == NAVIRE_TROUVE_PLUS_1)
		{
			prop[x][y] = NAVIRE_TROUVE_PLUS_1;
		}
		else
		{
			prop[x][y] = AUCUN_NAVIRE;
		}
	}

	return;
}

int point_decryption(char *input, int *number_returned, int *letter_returned)
{
	int number;
	char letter;

	// Check if the input is in the correct format
	if (strlen(input) != 2)
	{
		return 7;
	}

	if (!isdigit(input[0]))
	{
		return 9;
	}

	// Extract the number
	number = atoi(&input[0]);

	// Check if the character is a letter
	if (!isalpha(input[1]))
	{
		return 8;
	}

	// Convert the letter to the corresponding number (0-25)
	letter = input[1];
	if (isupper(letter))
	{
		letter = tolower(letter);
	}
	letter -= 'a';
	letter += 1;

	// Print the separated number and letter
	*number_returned = number;
	(*letter_returned) = letter;

	return 0;
}

/* cette fonction demande `a l’utilisateur de saisir une case (x,y) `a jouer et selon la valeur contenue plateau[x][y] enregistre dans prop[x][y] la valeur */
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav)
{
	int x, y;
	bool coordinates = true; // used to ask the user new coordinates for a vessel if the previous ones are not inside the specified limits mentioned on the instructions
	bool navire_founded;
	char input[3];
	int status_code;
	bool repeater = true;

	while (coordinates)
	{
		custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 0, -1);
		scanf("%s", input);

		while (repeater)
		{
			status_code = point_decryption(input, &x, &y);
			if (status_code == 0)
			{
				repeater = false;
			}
			else if (status_code == 8)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 1, -1);
				scanf("%s", input);
			}
			else if (status_code == 9)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 2, -1);
				scanf("%s", input);
			}
			else if (status_code == 7)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 3, -1);
				scanf("%s", input);
			}
			else
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 4, 1);
			}
		}

		repeater = true;

		if (!(x < 1 || x > taille_plateau || y < 1 || y > taille_plateau)) // coordinates verification out of limit
		{

			coordinates = false;
		}
		else
		{
			custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 5, -1);
		}
	}

	coordinates = true;

	x--; // Adjustment for the indexes.
	y--;

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
	fflush(stdout); // TO BE COMMENTED
}

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau, int number_of_navires)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	bool verification;
	float percentage = 100 / number_of_navires;

	// creating the six boats and putting them on the plateau (invisible to the user)
	for (int i = 0; i < number_of_navires; i++)
	{
		custom_graphics_on_proposition(i, plateau, taille_plateau, -2, percentage, -1);

		/* Initialising navire randomly on the game's plate */
		msleep(300); // delay process for 1 second in order to provide real aleartory results

		nav.taille = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // logic for choosing the maximum length of a navire depending the size of the plate
		nav.sens = nb_random(0, 3);
		nav.premiere_case.x = nb_random(0, taille_plateau - 1);
		nav.premiere_case.y = nb_random(0, taille_plateau - 1);

		verification = true;

		while (verification)
		{
			if (est_valide_pro(plateau, nav, taille_plateau))
			{
				verification = false;
			}
			else
			{
				nav.sens = nb_random(0, 3);
				nav.taille = nb_random(2, (taille_plateau < 6) ? taille_plateau : 6); // required, oterwiese we may arrive to situations that a new navire is impossible to be created with the previous taille taken before for a size 4x4 of a grille
				nav.premiere_case.x = nb_random(0, taille_plateau - 1);
				nav.premiere_case.y = nb_random(0, taille_plateau - 1);
			}
		}

		nav.id = i;

		switch (nav.sens)
		{
		case UP:
			for (int j = nav.premiere_case.x; j > nav.premiere_case.x - nav.taille; j--)
			{
				plateau[j][nav.premiere_case.y] = NAVIRE;
			}

			break;

		case LEFT:
			for (int j = nav.premiere_case.y; j > nav.premiere_case.y - nav.taille; j--)
			{
				plateau[nav.premiere_case.x][j] = NAVIRE;
			}

			break;

		case DOWN:
			for (int j = nav.premiere_case.x; j < nav.premiere_case.x + nav.taille; j++)
			{
				plateau[j][nav.premiere_case.y] = NAVIRE;
			}

			break;

		case RIGHT:
			for (int j = nav.premiere_case.y; j < nav.premiere_case.y + nav.taille; j++)
			{
				plateau[nav.premiere_case.x][j] = NAVIRE;
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

void update_game_table_before_launch(int **prop, int taille_plateau)
{
	for (int i = 0; i < taille_plateau; i++)
	{
		for (int j = 0; j < taille_plateau; j++)
		{
			if (prop[i][j] == CUSTOM_NAVIRE)
			{
				prop[i][j] = NAVIRE;
			}
		}
	}
}

Liste_Navire initialisation_plateau_custom(int **plateau, int taille_plateau, int number_of_navires)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	int status_code;
	int x, y;
	char input[20]; // storing the input string
	bool repeater = true;
	int input_int;
	int colour;

	// creating the six boats and putting them on the plateau (invisible to the user)
	for (int i = 0; i < number_of_navires; i++)
	{
		colour = random_color();
		/* Initialising navire customly */
		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 0, -1);
		scanf("%s", input);
		clearScreen();

		while (repeater)
		{
			if (strlen(input) != 1 || (input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4' && input[0] != '5' && input[0] != '6'))
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 1, -1);
				scanf("%s", input);
				clearScreen();
			}
			else
			{
				repeater = false;
			}
		}
		input_int = atoi(input);
		nav.taille = input_int;
		repeater = true;

		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 4, -1);
		scanf("%s", input);
		clearScreen();

		while (repeater)
		{
			if (strlen(input) != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2' && input[0] != '3'))
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 3, -1);
				scanf("%s", input);
				clearScreen();
			}
			else
			{
				repeater = false;
			}
		}
		input_int = atoi(input);
		nav.sens = input_int;
		repeater = true;

		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 5, -1);
		scanf("%s", input);
		clearScreen();

		while (repeater)
		{
			status_code = point_decryption(input, &x, &y);
			if (status_code == 0)
			{
				repeater = false;
			}
			else if (status_code == 8)
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 6, -1);
				scanf("%s", input);
				clearScreen();
			}
			else if (status_code == 9)
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 7, -1);
				scanf("%s", input);
				clearScreen();
			}
			else if (status_code == 7)
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 2, -1);
				scanf("%s", input);
				clearScreen();
			}
			else
			{
				custom_graphics_on_proposition(-1, plateau, taille_plateau, -1, 4, -1);
			}
		}

		repeater = true;
		clearScreen();

		x--; // Adjustment for the indexes.
		y--;

		nav.premiere_case.x = x;
		nav.premiere_case.y = y;

		while (repeater)
		{
			if (est_valide_pro(plateau, nav, taille_plateau))
			{
				repeater = false;
			}
			else
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 0, 0);
				scanf("%s", input);
				clearScreen();

				while (repeater)
				{
					if (strlen(input) != 1 || (input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4' && input[0] != '5' && input[0] != '6'))
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 1, -1);
						scanf("%s", input);
						clearScreen();
					}
					else
					{
						repeater = false;
					}
				}
				input_int = atoi(input);
				nav.taille = input_int;
				repeater = true;

				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 4, -1);
				scanf("%s", input);
				clearScreen();

				while (repeater)
				{
					if (strlen(input) != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2' && input[0] != '3'))
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 3, -1);
						scanf("%s", input);
						clearScreen();
					}
					else
					{
						repeater = false;
					}
				}
				input_int = atoi(input);
				nav.sens = input_int;
				repeater = true;

				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 5, -1);
				scanf("%s", input);
				clearScreen();

				while (repeater)
				{
					status_code = point_decryption(input, &x, &y);
					if (status_code == 0)
					{
						repeater = false;
					}
					else if (status_code == 8)
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 6, -1);
						scanf("%s", input);
						clearScreen();
					}
					else if (status_code == 9)
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 7, -1);
						scanf("%s", input);
						clearScreen();
					}
					else if (status_code == 7)
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 2, -1);
						scanf("%s", input);
						clearScreen();
					}
					else
					{
						custom_graphics_on_proposition(-1, plateau, taille_plateau, -1, 4, -1);
					}
				}

				repeater = true;
				clearScreen();

				x--; // Adjustment for the indexes.
				y--;

				nav.premiere_case.x = x;
				nav.premiere_case.y = y;
			}
		}

		nav.id = i;

		switch (nav.sens)
		{
		case UP:
			for (int j = nav.premiere_case.x; j > nav.premiere_case.x - nav.taille; j--)
			{
				plateau[j][nav.premiere_case.y] = CUSTOM_NAVIRE;
			}

			break;

		case LEFT:
			for (int j = nav.premiere_case.y; j > nav.premiere_case.y - nav.taille; j--)
			{
				plateau[nav.premiere_case.x][j] = CUSTOM_NAVIRE;
			}

			break;

		case DOWN:
			for (int j = nav.premiere_case.x; j < nav.premiere_case.x + nav.taille; j++)
			{
				plateau[j][nav.premiere_case.y] = CUSTOM_NAVIRE;
			}

			break;

		case RIGHT:
			for (int j = nav.premiere_case.y; j < nav.premiere_case.y + nav.taille; j++)
			{
				plateau[nav.premiere_case.x][j] = CUSTOM_NAVIRE;
			}

			break;

		default:
			break;
		}

		ajouter_element_liste_Navire(&liste, nav);
		clearScreen();
	}

	update_game_table_before_launch(plateau, taille_plateau);

	return liste;
}

void tour_ia_random_v1(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue)
{
	int x, y;
	bool repeat = true;
	x = nb_random(0, taille_plateau - 1);
	y = nb_random(0, taille_plateau - 1);
	while (repeat)
	{
		if (prop[x][y] == AUCUN_NAVIRE || prop[x][y] == NAVIRE_TROUVE || prop[x][y] == NAVIRE_TROUVE_PLUS_1 || prop[x][y] == COULE)
		{
			x = nb_random(0, taille_plateau - 1);
			y = nb_random(0, taille_plateau - 1);
		}
		else
		{
			repeat = false;
		}
	}

	update_prop(prop, x, y);
	(*NbJoue)++; // next round added

	if (navire_found(prop, L))
	{
		(*NbNav)++;
		printf("L'IA a touché un navire en (%d, %d)!\n", x + 1, y + 1);
	}
	else
	{
		printf("L'IA a manqué en (%d, %d).\n", x + 1, y + 1);
	}
}

void proposition_ai(int **prop, int taille_plateau, Liste_Navire L, int *NbNav)
{
	if (navire_found(prop, L))
	{
		(*NbNav)++;
	}
	return;
}

bool random_point(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue, int x, int y)
{
	update_prop(prop, x, y);
	(*NbJoue)++; // next round added

	if (prop[x][y] == NAVIRE_TROUVE || prop[x][y] == NAVIRE_TROUVE_PLUS_1)
	{
		return false; // not_to_skip_action = FALSE
	}
	else
	{
		return true;
	}
}

void next_point(int **table, int taille_plateau, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens, int *state)
{
	int random_neighbor;
	bool repeat_generator = true;
	*state = 0;
	switch (*sens_mode)
	{
	case HORIZONTAL:
		switch (*deep_sens)
		{
		case LEFT:
			if (y_prev == 0)
			{
				*x_now = nb_random(0, taille_plateau - 1);
				*y_now = nb_random(0, taille_plateau - 1);
				while (repeat_generator)
				{
					if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
					{
						*x_now = nb_random(0, taille_plateau - 1);
						*y_now = nb_random(0, taille_plateau - 1);
					}
					else
					{
						repeat_generator = false;
					}
				}
				*sens_mode = HORIZONTAL;
				*deep_sens = LEFT;
			}
			else
			{
				*x_now = x_prev;
				*y_now = y_prev - 1;
			}
			break;
		case RIGHT:
			if (y_prev == taille_plateau - 1)
			{
				*x_now = nb_random(0, taille_plateau - 1);
				*y_now = nb_random(0, taille_plateau - 1);
				while (repeat_generator)
				{
					if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
					{
						*x_now = nb_random(0, taille_plateau - 1);
						*y_now = nb_random(0, taille_plateau - 1);
					}
					else
					{
						repeat_generator = false;
					}
				}
				*sens_mode = HORIZONTAL;
				*deep_sens = RIGHT;
			}
			else
			{
				*x_now = x_prev;
				*y_now = y_prev + 1;
			}
			break;
		default:
			random_neighbor = rand() % 2;
			if (random_neighbor == 1)
			{ // LEFT
				if (y_prev == 0)
				{
					*x_now = nb_random(0, taille_plateau - 1);
					*y_now = nb_random(0, taille_plateau - 1);
					while (repeat_generator)
					{
						if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
						{
							*x_now = nb_random(0, taille_plateau - 1);
							*y_now = nb_random(0, taille_plateau - 1);
						}
						else
						{
							repeat_generator = false;
						}
					}
				}
				else
				{
					*x_now = x_prev;
					*y_now = y_prev - 1;
				}
				*sens_mode = -1;
			}
			else // RIGHT
			{
				if (y_prev == taille_plateau - 1)
				{
					*x_now = nb_random(0, taille_plateau - 1);
					*y_now = nb_random(0, taille_plateau - 1);
					while (repeat_generator)
					{
						if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
						{
							*x_now = nb_random(0, taille_plateau - 1);
							*y_now = nb_random(0, taille_plateau - 1);
						}
						else
						{
							repeat_generator = false;
						}
					}
				}
				else
				{
					*x_now = x_prev;
					*y_now = y_prev + 1;
				}
				*sens_mode = -1;
			}
			break;
		}
		break;

	case VERTICAL:
		switch (*deep_sens)
		{
		case UP:
			if (x_prev == 0)
			{
				*x_now = nb_random(0, taille_plateau - 1);
				*y_now = nb_random(0, taille_plateau - 1);
				while (repeat_generator)
				{
					if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
					{
						*x_now = nb_random(0, taille_plateau - 1);
						*y_now = nb_random(0, taille_plateau - 1);
					}
					else
					{
						repeat_generator = false;
					}
				}
				*sens_mode = VERTICAL;
				*deep_sens = UP;
			}
			else
			{
				*x_now = x_prev - 1;
				*y_now = y_prev;
			}
			break;
		case DOWN:
			if (x_prev == taille_plateau - 1)
			{
				*x_now = nb_random(0, taille_plateau - 1);
				*y_now = nb_random(0, taille_plateau - 1);
				while (repeat_generator)
				{
					if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
					{
						*x_now = nb_random(0, taille_plateau - 1);
						*y_now = nb_random(0, taille_plateau - 1);
					}
					else
					{
						repeat_generator = false;
					}
				}
				*sens_mode = VERTICAL;
				*deep_sens = DOWN;
			}
			else
			{
				*x_now = x_prev + 1;
				*y_now = y_prev;
			}

			break;
		default:
			random_neighbor = rand() % 2;
			if (random_neighbor == 1) // UP
			{
				if (x_prev == 0)
				{
					*x_now = nb_random(0, taille_plateau - 1);
					*y_now = nb_random(0, taille_plateau - 1);
					while (repeat_generator)
					{
						if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
						{
							*x_now = nb_random(0, taille_plateau - 1);
							*y_now = nb_random(0, taille_plateau - 1);
						}
						else
						{
							repeat_generator = false;
						}
					}
				}
				else
				{
					*x_now = x_prev - 1;
					*y_now = y_prev;
				}
				*sens_mode = -1;
			}
			else // DOWN
			{
				if (x_prev == taille_plateau - 1)
				{
					*x_now = nb_random(0, taille_plateau - 1);
					*y_now = nb_random(0, taille_plateau - 1);
					while (repeat_generator)
					{
						if (table[*x_now][*y_now] == AUCUN_NAVIRE || table[*x_now][*y_now] == NAVIRE_TROUVE || table[*x_now][*y_now] == NAVIRE_TROUVE_PLUS_1 || table[*x_now][*y_now] == COULE)
						{
							*x_now = nb_random(0, taille_plateau - 1);
							*y_now = nb_random(0, taille_plateau - 1);
						}
						else
						{
							repeat_generator = false;
						}
					}
				}
				else
				{
					*x_now = x_prev + 1;
					*y_now = y_prev;
				}
				*sens_mode = -1;
			}
			break;
		}
		break;

	default:
		if (table[x_prev][y_prev + 1] == NAVIRE_TROUVE)
		{
			*sens_mode = HORIZONTAL;
			*x_now = x_prev;
			*y_now = y_prev - 1;
			*deep_sens = LEFT;
			break;
		}
		if (table[x_prev][y_prev - 1] == NAVIRE_TROUVE)
		{
			*sens_mode = HORIZONTAL;
			*x_now = x_prev;
			*y_now = y_prev + 1;
			*deep_sens = RIGHT;
			break;
		}
		if (table[x_prev + 1][y_prev] == NAVIRE_TROUVE)
		{
			*sens_mode = VERTICAL;
			*x_now = x_prev - 1;
			*y_now = y_prev;
			*deep_sens = UP;
			break;
		}
		if (table[x_prev - 1][y_prev] == NAVIRE_TROUVE)
		{
			*sens_mode = VERTICAL;
			*x_now = x_prev + 1;
			*y_now = y_prev;
			*deep_sens = DOWN;
			break;
		}

		random_neighbor = rand() % 4;
		switch (random_neighbor - 1)
		{
		case UP:
			*x_now = x_prev - 1;
			*y_now = y_prev;
			*previous_sens = VERTICAL;
			*state = 1;
			*deep_sens = -1;
			break;

		case DOWN:
			*x_now = x_prev + 1;
			*y_now = y_prev;
			*previous_sens = VERTICAL;
			*state = 1;
			*deep_sens = -1;
			break;

		case LEFT:
			*x_now = x_prev;
			*y_now = y_prev - 1;
			*previous_sens = HORIZONTAL;
			*state = 1;
			*deep_sens = -1;
			break;

		case RIGHT:
			*x_now = x_prev;
			*y_now = y_prev + 1;
			*previous_sens = HORIZONTAL;
			*state = 1;
			*deep_sens = -1;
			break;

		default:
			break;
		}
	}
}

/* cr�er une cellule de liste avec l'�l�ment v
   renvoie le pointeur sur la cellule de liste cr��e
   la fonction s'arrete si la cr�ation n'a pas pu se faire */
Cellule_Liste_Point *creer_element_liste_Point(int x, int y)
{
	Cellule_Liste_Point *el;
	el = (Cellule_Liste_Point *)malloc(sizeof(Cellule_Liste_Point));
	if (el == NULL)
	{
		fprintf(stderr, "creer_element_liste_Point : allocation impossible\n");
		exit(-1);
	}
	el->data.x = x;
	el->data.y = y;
	el->suiv = NULL;
	return el;
}

/* cr�er une liste vide */
Liste_Point creer_liste_Point_vide()
{
	Liste_Point L = {0, NULL, NULL};
	return L;
}

/* ajouter l'�l�ment e en fin de la liste L, renvoie la liste L modifi�e */
void ajouter_element_liste_Point(Liste_Point *L, int x, int y)
{
	Cellule_Liste_Point *el;

	el = creer_element_liste_Point(x, y);
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

// Liste_Point supprimer_liste_Navire(Liste_Navire L)

/* suppression de tous les �l�ments de la liste, renvoie la liste L vide */
Liste_Point supprimer_liste_Point(Liste_Point L)
{
	Cellule_Liste_Point *el = L.first;

	while (el)
	{
		Cellule_Liste_Point *suiv = el->suiv;
		free(el);
		el = suiv;
	}
	L.first = L.last = NULL;
	L.taille = 0;
	return L;
}

/* cr�er une s�quence de points sous forme d'un tableau de points
   � partir de la liste de points L */
Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L)
{
	Tableau_Point T;

	/* taille de T = taille de L */
	T.taille = L.taille;

	/* allocation dynamique du tableau de Point */
	T.tab = malloc(sizeof(Case) * T.taille);
	if (T.tab == NULL)
	{
		/* allocation impossible : arret du programme avec un message */
		fprintf(stderr, "sequence_points_liste_vers_tableau : ");
		fprintf(stderr, " allocation impossible\n");
		exit(-1);
	}

	/* remplir le tableau de points T en parcourant la liste L */
	int k = 0;						   /* indice de l'�l�ment dans T.tab */
	Cellule_Liste_Point *el = L.first; /* pointeur sur l'�l�ment dans L */
	while (el)
	{
		T.tab[k] = el->data;
		k++;		   /* incr�menter k */
		el = el->suiv; /* passer � l'�lement suivant dans la liste chainee */
	}

	return T;
}

void waitTime(int seconds, char *message, int colour1, int colour2, char *name1, char *name2)
{
	for (int i = seconds; i > 0; i--)
	{
		clearScreen();
		printf("\n\e[0;%dm%s\e[0m %s \e[0;%dm%s\e[0m (%d)\n", colour1, name1, message, colour2, name2, i);
		msleep(1000);
	}
	clearScreen();
}

void clearScreenWait(double seconds)
{
	msleep(1000 * seconds);
	clearScreen();
}