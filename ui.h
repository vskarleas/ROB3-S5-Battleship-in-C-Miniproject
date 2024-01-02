#ifndef _USER_INPUT_

int get_user_input(char message[1024], char error_message[1024], char error_message_2[1024], int min, int max);

char *get_user_name(char message[1024]);

void rules_interface(int points, int taille_plateau);

void rules_interface_temps(int rounds, int taille_plateau);

void rules_reminder(int rounds, int taille_plateau);

void rules_reminder_temps(int rounds, int taille_plateau);

void lost_graphics(int mode);

void win_graphics(int taille_plateau, int **prop, int round_nb, int mode, char name[30]);

void allocation_error_print_with_id(char reference[512], int i);

void allocation_error_print_general(char reference[512]);

int game_mode_menu();

int game_mode_solo();

int midle_game_menu(int rounds, int taille_plateau, int version, int mode);

int midle_game_menu_saving_unavailable(int rounds, int taille_plateau, int mode);

bool waitForMenuKeypress();

void ajuster_temps(int taille_plateau, int *temps_limite);

void ajuster_tours(int taille_plateau, int *max_tours, int nb_navires, int mode);

#endif