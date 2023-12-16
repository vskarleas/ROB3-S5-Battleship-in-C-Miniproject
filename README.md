# Calendrier

* 15/11/2023: Creation de la structure et initialisation de tous les fnctionnes principales du programme
* 16/11/2023: Creation du fichier compte-rendu
* 17/11/2023: Correction du problem de Debuger - Maintenant c'est possible de debauger le code + Cleaned the source code + Added
* 16/12/2023: The main game has been completed

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
6. Report the problem with taille = 4 and how wee resolved => created a new if on th elogic since teh tabke couldn't be completed with 6 navales.
7. We can use only one table thansk to the liste chainee qui s'appelle liste sur le programme
8. That way on our code we are using the listes chainees principles that allows to add whatever number kf navires we want in case that we didnt want to be exclusivky 6 (even the percentage affichage function is ufoated dynamicly accoridng to the number of navires) 

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

* [X] Game's whole logic on the main to be completed
* [X] A bool function that verifies if every boat has been found or not
* [X] A logic/algorythm that detects a boat according to the points placed from the user
* [ ] Sauvegarder le jeu et continuer un autre moment - functions to be created and complete the logic for jeu_v2 (COMPUTER mode call and LOAD case for continuing the game)
* [ ] Complete the multiplayer mode (2 individual players). It needs a function to manually insert the selected amount of navires on the game and then we have to complete the logic of game for jeu_v2
* *** Don't think that we will include the AI version due to lack of time and inutility as functionality***
* [X] Multiple rounds with points and a maximum number of tentatives for the user trying to find the boats that were placed randomly (it would be great if the number of tentatives for a round is based on the size of the plate [more cells => smaller chances to find directly even one boat => more tentatives should be provided]) IF WE WANT

![1702659377345](image/README/1702659377345.png)

![1702659344622](image/README/1702659344622.png)

# Ameliorations

* Create a menu game
  * Add a functionality to choose if he wants to play against an AI on the computer or againt another player
  * Add a personalisation functionality for the user to save his name
    * Add a multiplayer (2 players maximum) where each one is placing navires on his own plateau and the other one tries to find them respectfully
* Add a functionality to save the game

# Versions

* V1.1 Code de base
* V1.2 Code corrigé selon indications de Manu
* V1.3 Code optimized from the previous source code
* V1.4 Comments added and cleaned source code
* V2.1 Bug fixes regarding est_valid_pro
* V2.2 Makefile updated
* V3.1 Explained different things and added the function copier_navires . The part 'To be created' was updated. Check the proto on that section of what needs to be done.
* V3.2 Fixes copier_navire error and added the main game loop including the functions discused on the board. NOTE: Code is not optimised and it can not be runned until teh game loop logic is completed
* v4.5 Game logic has been completed and improvements were made on the source code
* V5.0 All the UI has been completed. Introduction to different game modes was included on this version. Needs to be done: create the saving functionality (check to be done section) and comeplete the multiplayer mode.
