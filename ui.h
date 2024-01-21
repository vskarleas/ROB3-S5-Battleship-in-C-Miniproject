// #############################################################################
// # File ui.h
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#ifndef _USER_INPUT_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to choose the language
int choose_language();

// Asking a user for a number that corresponds to message and error_message and returning that value
int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max, int language);

// HUMOUR Function: If it's the name of one of the profesors the game is completed with the profesor to win every single time :)
bool checkSpecialName(const char *name);

// Some humour here :) - Connection with the previosu function
void displaySpecialWinMessage(int language);

//Assking user's name
char *get_user_name(char message[1024], int language);

//Printing the rules of the game
void rules_interface(int points, int taille_plateau, int language);

//Printing the rules of the game in SOlo mode with option Time
void rules_interface_temps(int rounds, int taille_plateau, int language);

//Printing the rule sof the game once it's asked from the user during a pause
void rules_reminder(int rounds, int taille_plateau, int language);

//Printing the rule sof the game once it's asked from the user during a pause on a Solo game with option Time selected
void rules_reminder_temps(int rounds, int taille_plateau, int language);

//Lost graphics appear on screen
void lost_graphics(int mode, int language);

//Win graphics appear on screen
void win_graphics(int taille_plateau, int **prop, int round_nb, int mode, char name[30], int language);

//Printing error message for allocating memory with corresponding ID
void allocation_error_print_with_id(char reference[512], int i);

//Printing error message for allocating memory with corresponding general
void allocation_error_print_general(char reference[512]);

//Menu to choose the game mode 
int game_mode_menu(int language);

//Choose the game mode once Solo game is selected
int game_mode_solo(int language);

//Choose the game mode once Multiplayer game is selected
int game_mode_multi(int language);

//Choose the game mode once AI game is selected
int game_mode_ai(int language);

//Menu that is printed when the game is paused (it provides different options)
int midle_game_menu(int rounds, int taille_plateau, int version, int mode, int language);

//Menu that is printed when the game is paused (it provides different options) on a Solo game with option Time selected
int midle_game_menu_saving_unavailable_temps(int rounds, int taille_plateau, int mode, int *time_paused, int language);

//Menu that is printed when the game is paused (it provides different options) where saving the game isn't available
int midle_game_menu_saving_unavailable(int rounds, int taille_plateau, int mode, int language);

//Accesing either the Menu (paused mode) or playing this round
bool waitForMenuKeypress(int language);

//Logic to choose total time for a Solo game with option Time selected
void ajuster_temps(int taille_plateau, int *temps_limite);

//Logic to choose total number of rounds (limiting the number of rounds when playing against teh computer [Solo mode option Rounds])
void ajuster_tours(int taille_plateau, int *max_tours, int nb_navires, int mode);

//Printing different messages on different parts of the game via identification 
void custom_graphics_on_proposition(int i, int **plateau, int taille_plateau, int colour, int mode, int id, int language);

//Congratulations message printing function
void game_mode_graphics_congratulations(int **prop, int taille_plateau, int nb_navires, int nb_navires_found, int id, char *buffer, int language);

//Printing any errors function
void error_graphics(int error_code, int language);

//Printing a message indicating that the game has been loaded from the server
void game_loaded_graphics(int max_rounds_load, int taille_plateau_load, int language);

//Indicating on which round we are for different game modes
void new_round_graphics(int round, int taille_plateau, int **prop, int id, char *buffer, int temps, int language);

#endif