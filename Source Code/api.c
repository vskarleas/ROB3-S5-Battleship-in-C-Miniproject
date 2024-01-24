// #############################################################################
// # File api.c
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "api.h"
#include "programmes.h"

Liste_Navire api_load_game(const char *filename, int *ptr1, int *ptr2, int *ptr3, int *ptr4, int **matrix)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");

#ifdef __APPLE__
		system("killall afplay");
#endif

		exit(EXIT_FAILURE);
	}

	char buffer[256];
	int currentIndex = 0;
	int value;

	while (currentIndex < 4)
	{
		fscanf(file, "%s", buffer);
		value = atoi(buffer);

		switch (currentIndex)
		{
		case 0:
			*ptr1 = value;
			break;
		case 1:
			*ptr2 = value;
			break;
		case 2:
			*ptr3 = value;
			break;
		case 3:
			*ptr4 = value;
			break;
		default:
			break;
		}

		currentIndex++;
	}
	// for all the other navires
	for (int i = 0; i < *ptr1; i++)
	{
		for (int i = 0; i < 5; i++)
		{
			fscanf(file, "%s", buffer);
			value = atoi(buffer);
			switch (i)
			{
			case 0:
				nav.sens = value;
				break;
			case 1:
				nav.premiere_case.x = value;
				break;
			case 2:
				nav.premiere_case.y = value;
				break;
			case 3:
				nav.taille = value;
				break;
			case 4:
				nav.id = value;
				break;
			default:
				break;
			}
		}
		ajouter_element_liste_Navire(&liste, nav);
	}

	fscanf(file, "%s", buffer);

	if (buffer[0] == '$')
	{
		for (int i = 0; i < *ptr2; i++)
		{
			for (int j = 0; j < *ptr2; j++)
			{
				fscanf(file, "%s", buffer);
				value = atoi(buffer);
				matrix[i][j] = value;
			}
		}
	}
	else
	{
		printf("\nERROR ON THE FUNCTION OR THE FILE\n");
		exit(-8); // error code for loading function fail
	}

	fclose(file);
	return liste;
}

int api_table_size(const char *filename, int language)
{
	char *msg[4] = {"We did not find any saved game. Start a new game, save it and try again :-", "", "Nous n'avons trouvé aucune partie sauvegardée. Démarrez un nouveau jeu, enregistrez-le et réessayez :-", ""};
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		clearScreen();
		printf("\n\n%s", msg[language]);

#ifdef __APPLE__
		system("killall afplay");
#endif
		exit(-2);
	}

	char buffer[256];
	fscanf(file, "%s", buffer);
	fscanf(file, "%s", buffer); // it's placed second on the header of the file!!!
	return (atoi(buffer));
}

void api_clearFile(const char *filename, int language)
{
	// Open the file in write mode, which truncates the file if it exists
	FILE *file = fopen(filename, "w");

	if (file == NULL)
	{
		perror("Error opening file");
#ifdef __APPLE__
		system("killall afplay");
#endif
		exit(EXIT_FAILURE);
	}

	// Close the file
	fclose(file);

	char *msg[4] = {"Content removed from", "", "Contenu supprimé de", ""};
	printf("%s %s.\n", msg[language], filename);
}

void api_save_game(int number_of_navires, int taille_plateau, int coulle, int round, int **matrix, Liste_Navire liste, int language)
{
	FILE *fptr;
	bool exists = false;

	fptr = fopen("filecodec239012V1.txt", "w");
	if (fptr != NULL)
	{
		exists = true;
	}

	if (exists)
	{
		// deleting any existing content on the file that we have verfied that it exists
		api_clearFile("filecodec239012V1.txt", language);

		// saving the game
		fprintf(fptr, "%d ", number_of_navires);
		fprintf(fptr, "%d ", taille_plateau);
		fprintf(fptr, "%d ", coulle);
		fprintf(fptr, "%d ", round);

		Cellule_Liste_Navire *el = liste.first;
		while (el != NULL)
		{
			fprintf(fptr, "%d ", el->data.sens);
			fprintf(fptr, "%d ", el->data.premiere_case.x);
			fprintf(fptr, "%d ", el->data.premiere_case.y);
			fprintf(fptr, "%d ", el->data.taille);
			fprintf(fptr, "%d ", el->data.id);

			el = el->suiv;
		}

		fprintf(fptr, "$ ");

		for (int i = 0; i < taille_plateau; i++)
		{
			for (int j = 0; j < taille_plateau; j++)
			{
				fprintf(fptr, "%d ", matrix[i][j]);
			}
		}
	}
	else
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	fclose(fptr);
}

void api_delete_game_file(int language)
{
	char *msg[4] = {"See you next time...", "", "A la prochaine fois...", ""};
	if (remove("filecodec239012V1.txt") == 0)
		printf("\n\n%s\n\n", msg[language]);
	else
		printf("\n\n \n\n");

	return;
}