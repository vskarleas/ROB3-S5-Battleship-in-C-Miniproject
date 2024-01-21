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

#define HORIZONTAL 0
#define VERTICAL 1

#define OK 1 // navire added on the boats list (parallel to boats checklist)

#define VIDE 0
#define NAVIRE 1			   // navire placed on position
#define AUCUN_NAVIRE -1		   // deja joue et pas trouve un point de n'importe quelle navire
#define NAVIRE_TROUVE 2		   // one point was found
#define NAVIRE_TROUVE_PLUS_1 3 // one point of a navire was refound (it was choosen before)

#define CUSTOM_NAVIRE 8

#define COULE 10 // congratsulations you find the whole navire

/* Waiting for keypress before moving to the rest of the program */
void waitForKeypress()
{
	getchar(); // Waits for a key press
}

/* Clearing the terminal screen for more optimised visualisations */
void clearScreen()
{
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

/* Introducing a controled delay whenever we call this function */
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

/* Printing the game's table in a more UI friendly way */
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

/* Deletes a specific navire from a liste chainee of navires */
//THE FUNCTION IS NOT USED ON THE CURRENT VERSION OF THE PROGRAM
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

/* create a list cell with a navire v
    returns the pointer to the created list cell
    the function stops if the creation could not be done */
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

/* Creates an empty lise chainee of navires (allocation) */
Liste_Navire creer_liste_Navire_vide()
{
	Liste_Navire L = {0, NULL, NULL};
	return L;
}

/* add the navire e at the end of the list L, returns the modified list L */
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

/* Checks if a navire was found by scanning the game's plate with an advanced logic and an optimised algorithm
The idea was to remove any navire that was found from this liste chainne L in order to have more optimised checking times */
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

/* Updating the specific coordinates that the user chose (x,y) according to their state  */
void update_prop(int **prop, int x, int y, int mode)
{
	if (prop[x][y] == NAVIRE)
	{
		prop[x][y] = NAVIRE_TROUVE;
	}
	else if (prop[x][y] == COULE)
	{
		prop[x][y] = COULE;
	}
	else
	{
		if ((prop[x][y] == NAVIRE_TROUVE || prop[x][y] == NAVIRE_TROUVE_PLUS_1) && mode == 1)
		{
			prop[x][y] = NAVIRE_TROUVE;
		}
		else if ((prop[x][y] == NAVIRE_TROUVE || prop[x][y] == NAVIRE_TROUVE_PLUS_1) && mode != 1)
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

/* Decrypting the informations of a point of the type numer+letter (returns for instance for a point 2A the following (1,0))*/
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
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav, int language)
{
	int x, y;
	bool coordinates = true; // used to ask the user new coordinates for a vessel if the previous ones are not inside the specified limits mentioned on the instructions
	bool navire_founded;
	char input[3];
	int status_code;
	bool repeater = true;

	while (coordinates)
	{
		custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 0, -1, language);
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
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 1, -1, language);
				scanf("%s", input);
			}
			else if (status_code == 9)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 2, -1, language);
				scanf("%s", input);
			}
			else if (status_code == 7)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 3, -1, language);
				scanf("%s", input);
			}
			else
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 4, 1, language);
			}
		}

		repeater = true;

		if (!(x < 1 || x > taille_plateau || y < 1 || y > taille_plateau)) // coordinates verification out of limit
		{

			coordinates = false;
		}
		else
		{
			custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 5, -1, language);
		}
	}

	coordinates = true;

	x--; // Adjustment for the indexes.
	y--;

	update_prop(prop, x, y, 0);
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

/* Printing a loading indicator */
void printProgress(double percentage)
{
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout); // TO BE COMMENTED
}

// Allocation et initialisation des navires
Liste_Navire initialisation_plateau(int **plateau, int taille_plateau, int number_of_navires, int language)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	bool verification;
	float percentage = 100 / number_of_navires;

	// creating the six boats and putting them on the plateau (invisible to the user)
	for (int i = 0; i < number_of_navires; i++)
	{
		custom_graphics_on_proposition(i, plateau, taille_plateau, -2, percentage, -1, language);

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

/* Any navire that was placed customly CUSTOMNAVIRE is now updated to NAVIRE */
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

/* Allocating and initialising the navires customly */
Liste_Navire initialisation_plateau_custom(int **plateau, int taille_plateau, int number_of_navires, int language)
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
		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 0, -1, language);
		scanf("%s", input);
		clearScreen();

		while (repeater)
		{
			if (strlen(input) != 1 || (input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4' && input[0] != '5' && input[0] != '6'))
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 1, -1, language);
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

		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 4, -1, language);
		scanf("%s", input);
		clearScreen();

		while (repeater)
		{
			if (strlen(input) != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2' && input[0] != '3'))
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 3, -1, language);
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

		custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 5, -1, language);
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
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 6, -1, language);
				scanf("%s", input);
				clearScreen();
			}
			else if (status_code == 9)
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 7, -1, language);
				scanf("%s", input);
				clearScreen();
			}
			else if (status_code == 7)
			{
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 2, -1, language);
				scanf("%s", input);
				clearScreen();
			}
			else
			{
				custom_graphics_on_proposition(-1, plateau, taille_plateau, -1, 4, -1, language);
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
				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 0, 0, language);
				scanf("%s", input);
				clearScreen();

				while (repeater)
				{
					if (strlen(input) != 1 || (input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4' && input[0] != '5' && input[0] != '6'))
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 1, -1, language);
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

				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 4, -1, language);
				scanf("%s", input);
				clearScreen();

				while (repeater)
				{
					if (strlen(input) != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2' && input[0] != '3'))
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 3, -1, language);
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

				custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 5, -1, language);
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
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 6, -1, language);
						scanf("%s", input);
						clearScreen();
					}
					else if (status_code == 9)
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 7, -1, language);
						scanf("%s", input);
						clearScreen();
					}
					else if (status_code == 7)
					{
						custom_graphics_on_proposition(i, plateau, taille_plateau, colour, 2, -1, language);
						scanf("%s", input);
						clearScreen();
					}
					else
					{
						custom_graphics_on_proposition(-1, plateau, taille_plateau, -1, 4, -1, language);
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

/* AI logic for spark mode that simly chooses points aleartory that it hasn't visited before */
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

	update_prop(prop, x, y, 0);
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

/* Calls the function taht verfies if a navire was found from the AI. If this is the case then it adds a point to the AI's score */
void proposition_ai(int **prop, int taille_plateau, Liste_Navire L, int *NbNav)
{
	if (navire_found(prop, L))
	{
		(*NbNav)++;
	}
	return;
}

/* Decides if we need to find a new point or not for the next round */
bool play_point_and_decide(int **prop, int taille_plateau, Liste_Navire L, int *NbNav, int *NbJoue, int x, int y)
{
	update_prop(prop, x, y, 1);
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

/*
//Advanced AI logic V1
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

// Advanced AI logic V2
void next_point_v2(int **table, int taille_plateau, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens)
{
	// Check for points around the previous point that are already known to be part of a ship
	int found = 0;
	if (*sens_mode == HORIZONTAL)
	{
		if (*deep_sens == LEFT)
		{
			if (x_prev > 0 && table[x_prev - 1][y_prev] == 2)
			{
				*x_now = x_prev - 1;
				*y_now = y_prev;
				found = 1;
				*sens_mode = HORIZONTAL;
				*deep_sens = RIGHT;
			}
			else if (y_prev > 0 && table[x_prev][y_prev - 1] == 2)
			{
				*x_now = x_prev;
				*y_now = y_prev - 1;
				found = 1;
				*sens_mode = VERTICAL;
			}
		}
		else
		{
			if (x_prev < taille_plateau - 1 && table[x_prev + 1][y_prev] == 2)
			{
				*x_now = x_prev + 1;
				*y_now = y_prev;
				found = 1;
				*sens_mode = HORIZONTAL;
				*deep_sens = LEFT;
			}
			else if (y_prev < taille_plateau - 1 && table[x_prev][y_prev + 1] == 2)
			{
				*x_now = x_prev;
				*y_now = y_prev + 1;
				found = 1;
				*sens_mode = VERTICAL;
			}
		}
	}
	else
	{
		if (*deep_sens == UP)
		{
			if (y_prev > 0 && table[x_prev][y_prev - 1] == 2)
			{
				*x_now = x_prev;
				*y_now = y_prev - 1;
				found = 1;
				*sens_mode = VERTICAL;
				*deep_sens = DOWN;
			}
			else if (x_prev < taille_plateau - 1 && table[x_prev + 1][y_prev] == 2)
			{
				*x_now = x_prev + 1;
				*y_now = y_prev;
				found = 1;
				*sens_mode = HORIZONTAL;
			}
		}
		else
		{
			if (y_prev < taille_plateau - 1 && table[x_prev][y_prev + 1] == 2)
			{
				*x_now = x_prev;
				*y_now = y_prev + 1;
				found = 1;
				*sens_mode = VERTICAL;
				*deep_sens = UP;
			}
			else if (x_prev > 0 && table[x_prev - 1][y_prev] == 2)
			{
				*x_now = x_prev - 1;
				*y_now = y_prev;
				found = 1;
				*sens_mode = HORIZONTAL;
			}
		}
	}

	// If no new ship segments were found, randomly select a point in a direction that has not been explored yet
	if (!found)
	{
		int direction = rand() % 2;
		if (direction == 0)
		{
			*sens_mode = HORIZONTAL;
			*deep_sens = rand() % 2;
		}
		else
		{
			*sens_mode = VERTICAL;
			*deep_sens = rand() % 2;
		}

		// Randomize the starting point within the selected direction]
		bool repeat_generator = true;
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
}
*/

/* Checks if the chosen point can be accesed - Function to check if a point is within the limits of game table */
int is_valid_point(int x, int y, int taille_plateau)
{
	return (x >= 0 && x < taille_plateau && y >= 0 && y < taille_plateau);
}

// Function to update the search direction based on the current direction (inverses them)
void update_direction(int *sens, int *deep_sens)
{
	/* The idea here is that if we chose a point-au-voisinage in a direction and no point was found (HORIZONTAL),
	then we need to go on the opposite inversed (VERTICAL) and search for points on that direction according to
	the different propabilities */
	if (*sens == HORIZONTAL)
	{
		/* Switching from horizontal to vertical + choosing sub-direction. For instance if direction chosen
		is HORIZONTAL then sub-direction could be either left or ight and vice versa for VERTICAL */
		*sens = VERTICAL;
		*deep_sens = (*deep_sens == 0) ? 1 : -1;
	}
	else
	{
		*sens = HORIZONTAL;
	}
}

/* Advanced AI logic V3 */
void next_point_v3(int **table, int taille_plateau, int x_prev, int y_prev, int *x_now, int *y_now, int *previous_sens, int *sens_mode, int *deep_sens)
{
	// Check for ship segments around the previous point
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int new_x = x_prev + i;
			int new_y = y_prev + j;

			if (is_valid_point(new_x, new_y, taille_plateau) &&
				table[new_x][new_y] == NAVIRE)
			{
				// Found points-au-voisinage that should we visit and give at least a point
				*x_now = new_x;
				*y_now = new_y;
				table[new_x][new_y] = 2; // Mark as NAVIRE_TROUVE
				return;
			}
		}
	}

	// If no new points of a ship has been found, switch direction or choose random point
	if (*sens_mode == HORIZONTAL)
	{
		update_direction(previous_sens, deep_sens);
		*sens_mode = VERTICAL; // Switching to direction-based mode
	}
	else
	{
		// Randomly choose a point in a direction that has not been explored
		int direction = rand() % 2;
		*sens_mode = HORIZONTAL;;

		if (direction == 0)
		{
			*sens_mode = VERTICAL;
			*previous_sens = HORIZONTAL;
			*deep_sens = (rand() % 2 == 0) ? 1 : -1; // Following the same idea on update_direction function
		}
		else
		{
			*previous_sens = VERTICAL;
		}

		// Keep generating random points until an unexplored point is found. This is more optimise dversion of the following code
		/*
		x_now = nb_random(0, taille_plateau_jeu - 1);
		y_now = nb_random(0, taille_plateau_jeu - 1);
		while (repeat_generator)
		{
			if (prop1[x_now][y_now] == AUCUN_NAVIRE || prop1[x_now][y_now] == NAVIRE_TROUVE || prop1[x_now][y_now] == NAVIRE_TROUVE_PLUS_1 || prop1[x_now][y_now] == COULE)
			{
				x_now = nb_random(0, taille_plateau_jeu - 1);
				y_now = nb_random(0, taille_plateau_jeu - 1);
			}
			else
			{
				repeat_generator = false;
			}
		}
		*/

		while (true)
		{
			*x_now = nb_random(0, taille_plateau - 1);
			*y_now = nb_random(0, taille_plateau - 1);
			if (table[*x_now][*y_now] == 0)
			{
				break;
			}
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

/* Provides some waiting time with a message for the user */
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

/* Waiting some time and clearing the screen */
void clearScreenWait(double seconds)
{
	msleep(1000 * seconds);
	clearScreen();
}