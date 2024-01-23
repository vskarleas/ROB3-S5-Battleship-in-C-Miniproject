// #############################################################################
// # File multiplayer.h
// # UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
// # Authors: Yannis Sadoun, Vasileios Filippos Skarleas - All rights reserved.
// #############################################################################

#ifndef _MULTIPLAYER_H_
#define _MULTIPLAYER_H_

#include "structures.h"

int multiplayer(int **prop1, int **prop2, Liste_Navire liste1, Liste_Navire liste2, int *NbNav1, int *NbNav2, int *NbJoue_global, int coulle1, int coulle2, int round_global, int taille_plateau_jeu, int number_of_navires_jeu, int language);

#endif