# Calendrier

* 15/11/2023: Creation de la structure et initialisation de tous les fnctionnes principales du programme
* 16/11/2023: Creation du fichier compte-rendu
* 17/11/2023: Correction du problem de Debuger - Maintenant c'est possible de debauger le code + Cleaned the source code + Added
* 16/12/2023: The main game has been completed

# Notes - Problems

1. est_valide_pro est une fonction qui repond au cahier de charge de la fonction demandé est_valide sauf que celui fait traite chaque sous ca seperament tout en en faisant la verification du placement d'une navire à une position à partir du premier moment que les coordonees x, y aleartoires sont cree. (NOTE)
2. strcmpr -> 0 si ok, 1 sinon (for two strings) [0 si code number for false, and any other number is considered as true in C in an if or a bool]
3. ```c
   (taille_plateau < 6) ? nb_random(2, taille_plateau) : nb_random(2, 6);
   ```

   was replaved with

   ```c
   nb_random(2, (taille_plateau < 6) ? taille_plateau : 6);
   ```

   for optimization reasons.
4. A new function was created called msleep that adds a controled delay in ms whenever it's called. It's useful when you want to ensure the uniqueness of aleartory values since the leartory principle is based on the ms inside the processor's clock.
5. Report the problem with taille = 4 and how wee resolved => created a new if on th elogic since teh tabke couldn't be completed with 6 navales.
6. We can use only one table thansk to the liste chainee qui s'appelle liste sur le programme
7. That way on our code we are using the listes chainees principles that allows to add whatever number kf navires we want in case that we didnt want to be exclusivky 6 (even the percentage affichage function is ufoated dynamicly accoridng to the number of navires)
8. On the jeu_v2 we hasd used a switch for the mode_game, however during compilation there were many misunderstadnign for the compiler to understand if the treated cases were correct and it was responding with incorrect errors for the variables declaration. To by pass this problem it needed to declare everything before the switch, however we didn't want to do it (allocate more ressources that we won't use). That's why we sticked with the if statements.
9. Our API calls are responsible to connect the game to the external environment (computer or server) - that's why the name that was chosen was API from Application Programming Interface.
10. **Create a graph with the association of the different files between them (check INF404 for references)**
11. For every play mode, we decided to create unique global variables for each section in order to eliminate warnongs from the Makefile that were saying that we maybe forgot that we already had declared a function.

# Crucial changes

* Created a new file called structures.h that includes all the custom created structures required for the project like Case or even Navire
* Part of the main's program code was transfered to the new file called user_input to minimize the main's code
* Structure of navire was updated to include a unique ID
* A liste chainne with type Liste_Navire is used in order to make all the required verifications. Ideally we would like to use the method that deletes a navire from that liste in irder to minimise the processing time. However we need to fix the function responsible for doing that.

  To be noted that this procedure allows us to have only one plateau where everything is parsed dynamicly. The liste looks like the following:

| ID | Data                      |
| -- | ------------------------- |
| 0  | Data of navire 0 -> 1     |
| 1  | Data of navire 1 -> 2    |
| 2  | Data of navire 2 -> 3    |
| 3  | Data of navire 3 -> 4    |
| 4  | Data of navire 4 -> 5    |
| 5  | Data of navire 5 -> NULL |

* A new verison of the printing function is available called printing_the_grille_v2 that is more UI friendly and it's the one that is used on the latest version of the code
* There are two versions of main functions. The version 2 is the more stable one that includes for more functionality than version 1.

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
* [X] Continuer un autre moment - functions to be created and complete the logic for jeu_v2 (COMPUTER mode call and LOAD case for continuing the game)
* [X] Save the game function needs to be created. Its signature should definetly include Liste_Navire, taille_plateau, coulle, round, number_of_navires and prop. Create the file according to the filing codec as described at **Filing codec** section. NOTE: If the file already exists, use the function api_clearFile to clean the file before write on it. In the end of that file we can include the text "COPYRIGHT 2023-2024 NAVALE FILING CODEC BY YANIS SADOUN & VASILEIOS FILIPPOS SKARLEAS | ALL RIGHTS RESERVED"
* [X] Complete the multiplayer mode (2 individual players). It needs a function to manually insert the selected amount of navires on the game and then we have to complete the logic of game for jeu_v2.
* [X] OPTIONAL Improve the code by fixing the function suprimer_navire and using an advanced logic of L.taille on the jeu and jeu_v2 game loops
* [X] Multiple rounds with points and a maximum number of tentatives for the user trying to find the boats that were placed randomly (it would be great if the number of tentatives for a round is based on the size of the plate [more cells => smaller chances to find directly even one boat => more tentatives should be provided]) IF WE WANT
* [X] fix issues with Spark AI model that looks like it doesn't choose any point of the created navires on the table
* [ ] Improve Fireball AI + finish the logic of the function tour_ia_random_v2 model and .

![1702659377345](image/README/1702659377345.png)

![1702659344622](image/README/1702659344622.png)

# Ameliorations

* Create a menu game
  * Add a functionality to choose if he wants to play against an AI on the computer or againt another player
  * Add a personalisation functionality for the user to save his name
    * Add a multiplayer (2 players maximum) where each one is placing navires on his own plateau and the other one tries to find them respectfully
* Add a functionality to save the game & load the game

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
* V6.1 load game functionality has been added and fixed some bugs regarding the UI as well as the repsonse of the program n several extreme scenarions
* V6.2 Loading and saving game functionality was completed, fixed some bugs on the get_user_input ui and we fixed some UI customisation issues as well.
* V6.3 Added the temps choice on Solo mode and there were some bug fixes as well
* v7.0 multiplayer added and more functionality was added on the ui with intelligent logic printing modes as well as there were some bug fixes on the source code programmes.c
* v7.1 Imporved the program to have the exact signatures that the prof asked for and added some functionality for the points that were selected at least twice. Moreover the logic for the end of LOAD mode new logic was added to prevent a user to play again with a game file that was played before.
* v7.2 Tried to fix the non paused time for solo mode option temps with no sucess
* v7.3 Added a functionality to accept the format 2B for points coordinates
* V7.4 AI added and the module to create navires customly
* V8.0 Fixed issue with initialisatio_plateau_custom which couldn't detect the already existed navires, updated the UI files and transfered the majority of texts to the UI file as well. In addition to that, the code was optimised and there were several small bug fixes. The different game modes were seperated in order to be able to test them without the whole's game menu.
* V8.1 Fixed some logique issues on the AI model called Spark and added some fixes.
* V8.2 optimizzed the main program and added code to Fireball AI version
* V8.3 Fireball ai has been updated and for now it works excpet from some extreme edge cases
* V8.4 Code compeletd. Needs to add the translatiosn and fix the ctes[pndacnces to the single programs
* V8.5 Many trans;ations were added. We need to add know the translations on the api.c and on the jeu_v2.c. Then we need to copy the main code for every mode of the game and add it to the several main programs.

# Filing codec

`number_of_navires taille_plateau coulle round sens_nav_1 x_nav_1 y_nav_1 taille_nav_1 id_nav_1 sens_nav_2 ...  $ prop_table_codec`

We can modify the codec in order to include a timestamp that will indicate to the user when it was the last time it was saved the game, as well as from which user if we want to use this functionality for the multiplayer.

# Game's exit codes / errors

| Codes     | Status | Explanation                                           | Solution                                                                                                                    |
| --------- | ------ | ----------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------- |
| exit(-1)  | ERROR  | There is an allocation memory error                   | Check output on terminal that indicates the row, column and table with the error                                            |
| exit(5)   | OUTPUT | The game has been saved succesfully on the server     | N/A                                                                                                                         |
| exit(-2)  | STATE  | No file of saved game found                           | Play a game at least once and save it, then try again in order to load it and continue from were you stoppe dthe last time. |
| 7         | STATE  | the coordinates for a point are not wth a length of 2 |                                                                                                                             |
| 8         | STATE  | Point choosen first carcater is not a letter          |                                                                                                                             |
| exit(-4); | ERROR  | Unidentfied error with the point declaration          | Check the logic on the function proposition_joueur                                                                          |
| 9         | STATE  | Point choosen first carcater is not a number          |                                                                                                                             |
| exit(-3)  | ERROR  | Multiplayer sub mode failed                           | Check the output of the function and try again                                                                              |
| exit(-4)  | ERROR  | Solo sub mode failed                                  | Check the output of the function and try again                                                                              |
