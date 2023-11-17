# Calendrier

* 15/11/2023: Creation de la structure et initialisation de tous les fnctionnes principales du programme
* 16/11/2023: Creation du fichier compte-rendu
* 17/11/2023: Correction du problem de Debuger - Maintenant c'est possible de debauger le code

# Notes - Problems

1. copier_grille_de_reference_vers_la_grille_de_jeu il y a un **erreur**!!! (PROBLEM)
2. À tester proposition_joueur (TO_DO)
3. est_valide_pro est une fonction qui repond au cahier de charge de la fonction demandé est_valide sauf que celui fait traite chaque sous ca seperament tout en en faisant la verification du placement d'une navire à une position à partir du premier moment que les coordonees x, y aleartoires sont cree. (NOTE)

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

# Ameliorations

* Ajouter plus de fonctionalite sur l'affichafe de la grille du jeu (if you want more than that)

# Versions

* V1.1 Code de base
* V1.2 Code corrigé selon indications de Manu
* V1.3 Code optimized from the previous source code
