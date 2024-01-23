#############################################################################
# Fichier Makefile 
# UE Infomatics for Robotics - Polytech Sorbonne - 2023/2024 - S5
# Author: Vasileios Filippos Skarleas - All rights reserved.
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond � toutes les d�pendances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commancant par @ n'est pas affichee a l'ecran)
#############################################################################

# Chaque exercise ies identifie par un programme principale nomé comme exercise_xxx.c 
# Les programmes peuvent etre base sur d'autres fonctionctions definies aux different fichiers

#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = clang

# chemin d'acces aux librairies (interfaces)
INCDIR = .

# chemin d'acces aux librairies (binaires)
LIBDIR = .

# options pour l'�dition des liens
LDOPTS = -L$(LIBDIR) -lm

# options pour la recherche des fichiers .o et .h
INCLUDEOPTS = -I$(INCDIR)

# options de compilation
COMPILOPTS = -g -Wall $(INCLUDEOPTS)

# liste des executables
EXECUTABLES = jeu_v2 jeu_v1 test

#############################################################################
# definition des regles
#############################################################################

########################################################
# la r�gle par d�faut
all : $(EXECUTABLES)

########################################################
# regle generique : 
#  remplace les regles de compilation separee de la forme
#	module.o : module.c module.h
#		$(CC) -c $(COMPILOPTS) module.c
%.o : %.c %.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$*
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

########################################################
# regles explicites de compilation separee de modules
# n'ayant pas de fichier .h ET/OU dependant d'autres modules
	
programmes.o : programmes.c structures.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

jeu_v1.o : jeu_v1.c api.h ui.h structures.h programmes.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

jeu_v2.o : jeu_v2.c solo.h ai.h structures.h load.h multiplayer.h programmes.h ui.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

ai.o : ai.c ai.h programmes.h structures.h ui.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

load.o : load.c load.h api.h structures.h ui.h programmes.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

multiplayer.o : multiplayer.c multiplayer.h programmes.h structures.h ui.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

solo.o : solo.c solo.h programmes.h structures.h ui.h api.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

test.o : test.c
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

api.o : api.c api.h programmes.h structures.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

ui.o : ui.c ui.h api.h programmes.h structures.h
	@echo ""
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

		
########################################################
# regles explicites de creation des executables

jeu_v1 : jeu_v1.o programmes.o ui.o api.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

jeu_v2 : jeu_v2.o programmes.o ui.o api.o solo.o multiplayer.o load.o ai.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

test : test.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@



# regle pour "nettoyer" le r�pertoire
clean:
	rm -fR $(EXECUTABLES) *.o 
