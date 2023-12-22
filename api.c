#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "api.h"

Liste_Navire api_load_game(const char *filename, int *ptr1, int *ptr2, int *ptr3, int *ptr4, int **matrix)
{
	Liste_Navire liste;
	liste = creer_liste_Navire_vide();
	Navire nav;

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
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


int api_table_size(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	char buffer[256];
	fscanf(file, "%s", buffer);
	fscanf(file, "%s", buffer); //it's placed second on the header of the file!!!
	return (atoi(buffer));
}


void api_clearFile(const char *filename) 
{
    // Open the file in write mode, which truncates the file if it exists
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Close the file
    fclose(file);

    printf("Content removed from %s.\n", filename);
}