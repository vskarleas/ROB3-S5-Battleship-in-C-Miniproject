#ifndef _USER_INPUT_

int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max);

void rules_interface(int points, int taille_plateau);

void rules_reminder(int rounds, int taille_plateau);

void lost_graphics();

void win_graphics(int taille_plateau, int **prop, int round_nb);

void allocation_error_print_with_id(char reference[512], int i);

void allocation_error_print_general(char reference[512]);

int game_mode_menu();

int midle_game_menu(int rounds, int taille_plateau, int version, int mode);

bool waitForMenuKeypress();

#endif