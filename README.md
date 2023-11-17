# Calendrier

* 15/11/2023: Creation de la structure et initialisation de tous les fnctionnes principales du programme
* 16/11/2023: Creation du fichier compte-rendu
* 17/11/2023: Correction du problem de Debuger - Maintenant c'est possible de debauger le code


# Notes - Problems

1. Le programme ne s'arrete pas. Il faut detecter la source du probleme (15/11/2023) - Regarder ci-dessous pour les dernieres remarques sur ce probleme!
2. Detected an error while trying to acces the table_navire coordinates on line
3. validation_navire has a problem regarding the veririfcation of bypassing the limits of the table_navire or not. Check out lines 173 and 120 for a temporarily or not fix
   * SOUTH & EAST has to be OK
   * Detected an error on cases NORTH & WEST without being identified yet ("*//THERE IS AN ERROR HERE*")
   * Added a temporary section of code to initialization_plateau to help us identify the problem ("*//TEMPORARY FOR FIXING THE LOGIC PROBLEMS ON navire_validation*\"\)

# Crucial changes

* Created a new file called structures.h that includes all the custom created structures required for the project like Case or even Navire
* Part of the main's program code was transfered to the new file called user_input to minimize the main's code

# Discovered

1. Printing of grille marche
2. Had forgot to create the navire on the memory on line 171 at programmes.c, now this part is fixed!
3. According to the latest test the following functions work as expected:
   * creer_navire
   * logic for choosing the size of a navire: *randing= (taille_plateau<6) ?nb_random(2, taille_plateau) :nb_random(2, 6);
   * *initialize_plate
   * printing_the_grille
   * nb_random
   * init_nb_aleatoire


# Ameliorations

* Ajouter plus de fonctionalite sur l'affichafe de la grille du jeu
