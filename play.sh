#!/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;36m'
PURPLE='\033[1;35m'
# Print a message to the user
echo -e "${BLUE}Welcome to the Battle Ship game from Vasileios Filippos Skarleas and Yanis Sadoun. Once you are ready to start playing simply press ENTER"
echo -e "${GREEN}Bienvenue dans le jeu Bataille Navale de Vasileios Filippos Skarleas et Yanis Sadoun. Une fois que vous êtes prêt à commencer à jouer, appuyez simplement sur ENTER"

# Read a line of input from the user
read input

# If the user presses Enter, run the specified commands
if [[ -n "$input" ]]; then
  echo -e "${PURPLE}You did not press ENTER and the program will terminate"
else
  # Run the commands
  echo -e "${PURPLE}Running commands..."
  make
  clear
  ./jeu_v2
  make clean
fi
