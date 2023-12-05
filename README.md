# Calendrier

* 15/11/2023: Creation de la structure et initialisation de tous les fnctionnes principales du programme
* 16/11/2023: Creation du fichier compte-rendu
* 17/11/2023: Correction du problem de Debuger - Maintenant c'est possible de debauger le code + Cleaned the source code + Added

# Notes - Problems

1. Test the proposition_joueur  function and (TO_DO)
2. est_valide_pro est une fonction qui repond au cahier de charge de la fonction demandé est_valide sauf que celui fait traite chaque sous ca seperament tout en en faisant la verification du placement d'une navire à une position à partir du premier moment que les coordonees x, y aleartoires sont cree. (NOTE)
3. strcmpr -> 0 si ok, 1 sinon (for two strings) [0 si code number for false, and any other number is considered as true in C in an if or a bool]
4. ```c
   (taille_plateau < 6) ? nb_random(2, taille_plateau) : nb_random(2, 6);
   ```

   was replaved with

   ```c
   nb_random(2, (taille_plateau < 6) ? taille_plateau : 6);
   ```

   for optimization reasons.
5. A new function was created called msleep that adds a controled delay in ms whenever it's called. It's useful when you want to ensure the uniqueness of aleartory values since the leartory principle is based on the ms inside the processor's clock.

# Crucial changes

* Created a new file called structures.h that includes all the custom created structures required for the project like Case or even Navire
* Part of the main's program code was transfered to the new file called user_input to minimize the main's code
* Structure of navire was updated to include a unique ID
* A boats checlist was created as well that includes the id of every placed boat and 1 if it's placed (value to be updated every time a user finds a boat - it's going to be used in order to verify if the game is completed or not). The table looks like the following:

| ID | Placement |
| -- | --------- |
| 0  | 1         |
| 1  | 1         |
| 2  | 1         |
| 3  | 1         |
| 4  | 1         |
| 5  | 1         |

* A new verison of the printing function is available called printing_the_grille_v3 that is more UI friendly

# Discovered

1. Printing of grille marche
2. Had forgot to create the navire on the memory on line 171 at programmes.c, now this part is fixed!
3. According to the latest test the following functions work as expected:
   * creer_navire
   * logic for choosing the size of a navire: *randing= (taille_plateau<6) ?nb_random(2, taille_plateau) :nb_random(2, 6);
   * initialize_plate
   * printing_the_grille
   * nb_random
   * init_nb_aleatoire
   * est_valide_pro
   * initialisation_plateau

# To be created

* [ ] Game's whole logic on game_loop.c and call it on the main
* [ ] A bool function that verifies if every boat has been found or not
* [ ] A logic/algorythm that detects a boat according to the points placed from the user
* [ ] The user has to give the coordinates in format (1, taille_tableau) and not in format (0, taille-tableau-1) - TO TAKE INTO ACCOUNT
* [ ] Multiple rounds with points and a maximum number of tentatives for the user trying to find the boats that were placed randomly (it would be great if the number of tentatives for a round is based on the size of the plate [more cells => smaller chances to find directly even one boat => more tentatives should be provided])

# Ameliorations
* Try to use est_valide_pro_v2 instead of using est_valide_pro since it's written more cleverly while using pointers

# Versions

* V1.1 Code de base
* V1.2 Code corrigé selon indications de Manu
* V1.3 Code optimized from the previous source code
* V1.4 Comments added and cleaned source code
* V2.1 Bug fixes regarding est_valid_pro
