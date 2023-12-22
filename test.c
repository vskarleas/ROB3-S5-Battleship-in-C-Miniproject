#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool processFile(const char *filename, int *ptr1, int *ptr2, int *ptr3, int *ptr4, int **matrix)
{
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
                    printf("Sens: %d\n", value);
                    break;
                case 1:
                    printf("X: %d\n", value);
                    break;
                case 2:
                    printf("Y: %d\n", value);
                    break;
                case 3:
                    printf("Taille: %d\n", value);
                    break;
                case 4:
                    printf("ID: %d\n", value);
                    break;
                default:
                    break;
                }
            }
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
            return false;
        }

    fclose(file);
    return true;
}

void clearFile(const char *filename) {
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

void printMatrix(int **matrix, int numRows)
{
    printf("Matrix:\n");
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < 4; j++)
        { // Assuming 4 values after '$'
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int num1;
    int num2;
    int num3;
    int num4;

    // Pointer to num1
    int *num1_point = &num1;
    int *num2_point = &num2;
    int *num3_point = &num3;
    int *num4_point = &num4;

    int **prop_load;
    prop_load = malloc(4 * sizeof(int *));
    if (prop_load == NULL)
    {
        printf("error");
    }

    for (int i = 0; i < 4; i++)
    {
        prop_load[i] = malloc(4 * sizeof(int));
        if (prop_load[i] == NULL)
        {
            printf("error");
        }
    }

    bool status = false;

    status = processFile("test.txt", num1_point, num2_point, num3_point, num4_point, prop_load);

    if (status)
    {

        // Output the results
        printf("****************************************************************\n");
        printf("number_of_navires: %d\n", num1);
        printf("taille_tableau: %d\n", num2);
        printf("coule: %d\n", num3);
        printf("round: %d\n", num4);
        printf("\n****************************************************************\n");

        printMatrix(prop_load, num2);
    }
    else
    {
        printf("\nFunction failed!!!\n");
    }

    clearFile("test2.txt");

    return 0;
}
