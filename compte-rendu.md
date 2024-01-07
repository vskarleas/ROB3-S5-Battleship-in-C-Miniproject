# Bataille Navale S5 2023-2024

###### *Yanis SADOUN, Vasileios Filippos SKARLEAS | All rights reserved*

## Sommaire

1. Introduction (faire un inclus avec les differents modes de jeu qu'on est en train de programmer, ainsi que l'idee general du jeu)
2. Regles du jeu
3. Indications (presentation des fonctions demandé par le prof [indication des changements qui etaient effectues sur quelque'unes parmis eux])
4. Procedure (comment on a travaillé et communiqué)
5. Programmation / Algorithmes [d ́ecrit les algorithmes que vous avez utilis ́es pour faire le mini-projet. Ces algorithmes seront  ́ecrits de pr ́ef ́erence en langage naturel ; en particulier, vous pourrez y souligner vos contributions personnelles par rapport au sujet initial]
   1. UI
   2. Structures
   3. Saving API
   4. Filing codec
   5. Mode Solo (ajouter aussi des remarques pour d'autres fonctions construit)
   6. Mode Load
   7. Mode Multiplayer
   8. Mode AI
6. Notes / Problemes / leur resolution
7. Exit codes
8. Versioning [following the pull requests on Github]
9. Conclusion

# Introduction

Pendant le cours d'informatique du premier semestre en Anee 3 de la specialite Robotique à Polytech Sorbonne, l'idee principal est d'apprendre les bases de programmation en c, ainsi que de mettre de bases d'analyses ou encore imaginer des algorithmes qui servirons de repondre à un cahier de charges precis.

Dans ce cadre là, on est demandé de developer un mini prjet qui nous permeta d'exploiter nous competences dans le domaine de la programmation et la collaboration dans un environment complex en ce qui concerne l'organisation des fichiers et leur liason. Le projet demande de recreer le jeu de la bataille navale en informatique. À partir des informations essentiels tels que la taille du tableau de jeu et ses regles, il faut placer aleartoirement six navires de taille variant
de 2 à 6 cases sur ce plateau de jeu (version prinicpal).

Dans notre jeu de bataille navale vous allez trouver d'autres modes de jeu qu'on viens d'exploiter:

* Multiplayer
* Sauvegrader/Continuer
* IA

Avant commencer il faudrait de se mettre en commun en ce qui concerne les regles du jeu. Comment on peut decider si quelqu'un a gagne ou pas quand il joue en solo ou contre un autre utilisateur ? Est-qu'il y a d'autres manieres de voir si quelq'un a gagné ou pas ?

# Regles du jeu
Inspiré par la version simple du jeu qu'on avait programmé pour le TP2, l'agorithme qui decide si le joueur a gagne ou pas, faudrait etre en relation avec un numero des essais global. Sauf qu'il faut prendre en compte que la taille du tableu de jeu n'est pas toujours la meme taille. Ainsi l'algorithme proposé decide selon la taille du plateau. 
Algo general (language d'algo)

De plus, on etait inspire par les arcades games qui sont surtout associé avec un trnps limité. C'etait une tres bonne occasion d'exploiter la librerie de time.h tout en integrant un systeme qui arrete le jeu si il y a plus de temps. Qu'est qu'il faut faire en cas ou on est en mode pause ? Ci-dessous vous allez trouver l'algorithme qui repond tous ces questions. 
Algo temps

# Indications
Pour la creation du jeu il y avait quelques fonctions de base à creer et à respecter. Plus precisement:
Ajouter les fonctions du jeu

# Procedure
Chaque projet infornatique est exigeant et durtout il demande la contribution de chaque membre de l'equipe pour eviter des erreurs sur la logique du prinet et sur le source code final. En ce qui concerne la communication, les messages direct sont la maniere le plus simple pour un equipe composé de deux membres. mais comment on fait pour orendre en conite tous les choses wu'on discuté en direct ou en messages ?

L'utilisation de Git est essentiel qui nous permettra de reprendre le projet, tout en contribuant chaqu'un independement et avoir un archive de chaque chagrment qui était effectué. Git aussi permet d'analyser le code avant compiler et faire un backup sur une service destiné de maintenir le code et ses changements toujours accesibles par n'importe quel appareil. Comme attendue, Git etait associe pour un repertoire Github rt un Copilot gcc pour l'analyse du code envoyé vers Github. 

Effectivement, on a toujours besoin d'avoir un planning qui permetra de rester actif tout en deposant le projet complet sans aucun retard. C'est pourquoi une charte GANTT etait crée avec des dates fixes et des taches glovales differents. 