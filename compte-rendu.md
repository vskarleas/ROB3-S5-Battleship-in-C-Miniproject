# Bataille Navale S5 2023-2024

## Introduction

Pendant le cours d'informatique du premier semestre en Anee 3 de la specialite Robotique à Polytech Sorbonne, l'idee principal est d'apprendre les bases de programmation en c, ainsi que de mettre de bases d'analyses ou encore imaginer des algorithmes qui servirons de repondre à un cahier de charges precis.

Dans ce cadre là, on est demandé de developer un mini prjet qui nous permeta d'exploiter nous competences dans le domaine de la programmation et la collaboration dans un environment complex en ce qui concerne l'organisation des fichiers et leur liason. Le projet demande de recreer le jeu de la bataille navale en informatique. À partir des informations essentiels tels que la taille du tableau de jeu et ses regles, il faut placer aleartoirement six navires de taille variant
de 2 à 6 cases sur ce plateau de jeu (version prinicpal).

Dans notre jeu de bataille navale vous allez trouver d'autres modes de jeu qu'on viens d'exploiter:

* Multiplayer
* Sauvegrader/Continuer
* IA

Avant commencer il faudrait de se mettre en commun en ce qui concerne les regles du jeu. Comment on peut decider si quelqu'un a gagne ou pas quand il joue en solo ou contre un autre utilisateur ? Est-qu'il y a d'autres manieres de voir si quelq'un a gagné ou pas ?

## Regles du jeu

Inspiré par la version simple du jeu qu'on avait programmé pour le TP2, l'agorithme qui decide si le joueur a gagne ou pas, faudrait etre en relation avec un numero des essais global. Sauf qu'il faut prendre en compte que la taille du tableu de jeu n'est pas toujours la meme taille. Ainsi l'algorithme proposé decide selon la taille du plateau.

```python
# Check if the maximum number of rounds has been reached and if the user hasn't found all the ships
if current_round == max_rounds and number_of_navires_found < number_of_navires:
    # User ran out of rounds
    repeat = False  # Stop the game loop
    lost_graphics()  # Display lost message

# Check if the user has found all the ships
if number_of_navires_found == number_of_navires:
    # User won the game
    win_graphics(taille_plateau, prop, *NbJoue - 1, 1, "")  # Display win message and end the game
    return 0  # Indicate game won

```

De plus, on etait inspire par les arcades games qui sont surtout associé avec un trnps limité. C'etait une tres bonne occasion d'exploiter la librerie de time.h tout en integrant un systeme qui arrete le jeu si il y a plus de temps. Qu'est qu'il faut faire en cas ou on est en mode pause ? Ci-dessous vous allez trouver l'algorithme qui repond tous ces questions.
Algo temps

```python
# Check if the remaining time has expired
if temps_restant <= 0:
    # User ran out of time
    repeat = False  # Stop the game loop
    lost_graphics()  # Display lost message

```

## Indications

Pour la creation du jeu il y avait quelques fonctions de base à creer et à respecter. Plus precisement:

* int est valide(int ** plateau,int taille plateau, struct navire * nav)
* Navire creer navire(int taille,int taille plateau)
* void init_nb_aleatoire()
* int nb_aleatoire(int max)
* void initialisation plateau(int ** plateau,int taille plateau)
* void proposition joueur(int ** plateau, int ** prop, int * NbTouche,int * NbJoue,int *
  NbToucheNav, int taille plateau)
* void affichage plateau(int ** plateau,int taille plateau)

### Fonction est_valide_pro

La fonction est_valide (est_valide_pro sur notre programme) est utilisé à l'interieur de la fonction proposition_joueur:

```c
bool proposition_joueur(int **prop, int *NbJoue, Liste_Navire L, int taille_plateau, int *NbNav)
{
	int x, y;
	bool coordinates = true; // used to ask the user new coordinates for a vessel if the previous ones are not inside the specified limits mentioned on the instructions
	bool navire_founded;
	char input[3];
	int status_code;
	bool repeater = true;

	while (coordinates)
	{
		custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 0, -1);
		scanf("%s", input);

		while (repeater)
		{
			status_code = point_decryption(input, &x, &y);
			if (status_code == 0)
			{
				repeater = false;
			}
			else if (status_code == 8)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 1, -1);
				scanf("%s", input);
			}
			else if (status_code == 9)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 2, -1);
				scanf("%s", input);
			}
			else if (status_code == 7)
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 3, -1);
				scanf("%s", input);
			}
			else
			{
				custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 4, 1);
			}
		}

		repeater = true;

		if (!(x < 1 || x > taille_plateau || y < 1 || y > taille_plateau)) // coordinates verification out of limit
		{

			coordinates = false;
		}
		else
		{
			custom_graphics_on_proposition(-1, prop, taille_plateau, -1, 5, -1);
		}
	}

	coordinates = true;

	x--; // Adjustment for the indexes.
	y--;

	update_prop(prop, x, y);
	(*NbJoue)++; // next round

	navire_founded = navire_found(prop, L);
	if (navire_founded == true)
	{
		(*NbNav)++;
		return true;
	}

	clearScreen();
	return false;
}
```

Le code fourni représente une partie d'une fonction `proposition_joueur` qui gère la saisie de l'utilisateur pour jouer un coup dans un jeu de bataille navale. Il permet au joueur de saisir un point de coordonnées (x, y) et met à jour le tableau `prop` en conséquence.

* La fonction `point_decryption` est appelée pour valider le format d'entrée et extraire les composants numériques et alphabétiques.
* Le code d'état `status_code` renvoyé par `point_decryption` est vérifié pour détecter les erreurs :
  * `7`: L'entrée n'est pas au bon format (c'est-à-dire qu'elle ne comporte pas deux caractères).
  * `9`: Le premier caractère n'est pas un chiffre.
  * `8`: Le deuxième caractère n'est pas une lettre minuscule.

Une fois que des coordonnées valides ont été saisies, la fonction vérifie l'élément correspondant du tableau `prop` et le met à jour en fonction de la valeur trouvée dans le tableau `plateau`.

* Si `prop[x][y] = NAVIRE`: mettre `prop[x][y] = NAVIRE_TROUVE` pour indiquer un navire touché.
* Si `prop[x][y] = NAVIRE`: mettez `prop[x][y] = NAVIRE_TROUVE_PLUS_1` pour indiquer un navire précédemment touché.
* Sinon (pas de navire): mettez `prop[x][y] = AUCUN_NAVIRE` pour indiquer une cellule vide.

**Pourquoi est_valide_pro et pas est_valide ?**

est_valide_pro est une fonction qui repond au cahier de charge de la fonction demandé est_valide sauf que celui fait traite chaque sous ca seperament tout en en faisant la verification du placement d'une navire à une position à partir du premier moment que les coordonees x, y aleartoires sont cree.

### Fonction navire_found

Derniere etape est de verifier si une navire etait coullé selon l'algorithme navire_found

```python
fonction navire_trouve (x, y)
    navire_coule = faux

    si prop[x][y] = AUCUN_NAVIRE alors
        navire_coule = vrai
    sinon si prop[x][y] = NAVIRE_TROUVE alors
        navire_coulé = vrai
        nombre_touches = nombre_touches + 1
    sinon
        navire_trouve = faux
    fin si

    return navire_coule
fin fonction

```

1. La fonction prend deux paramètres: `x` et `y`, qui représentent les coordonnées de la cellule qui vient d'être touchée.
2. La fonction initialise une variable `navire_coulé` à `faux`. Cette variable servira à stocker si le navire a coulé.
3. La fonction vérifie si la cellule aux coordonnées `x, y` est vide (`prop[x][y] = AUCUN_NAVIRE`). Si c'est le cas, le navire a coulé et `navire_coulé` est mis à `vrai`.
4. La fonction vérifie également si la cellule aux coordonnées `x, y` a déjà été touchée (`prop[x][y] = NAVIRE_TROUVE`). Si c'est le cas, le navire n'a pas coulé, mais le nombre de coups sur le navire est incrémenté (`nombre_touches = nombre_touches + 1`).
5. Si aucune des conditions précédentes n'est remplie, le navire n'a pas été trouvé et `navire_trouvé` est mis à `faux`.
6. La fonction renvoie la valeur de `navire_coulé`, ce qui indique si le navire a coulé.

### Fonction ajuster_tours

Comme prevu, sur notre jeu de battaille navale, l'utilisateur peut choisir la taille du plateau de jeu. De plus, sur le sujet etait marqué qu'il faut qu'il existe toujours 6 navires. Une fois que cette fonctionalité etait realisé, on a donné à l'utilisateur le droit de chosir aussi le numero des navires du jeu. En fait selon tous ceux parametres, tu peut avoir soir un jeu facile, soit un moyenne, soit un jeu difficile. Du coup, tu ne peut pas etre dependant des regles qui sont statiques - par exemples le maximum des tours pour chaque jeu d'etre 55 car il ne serait pas interessant d'avoir un plateau de jeu taille pettie (ex: 4x4) par exemple).

Alors, la fonction ajuster_tours decide le maximum de tours selon les parametres du jeu choisit par l'utilisateur. Son algorithme est:

1. Vérifier le mode de jeu (`mode`) :

   * Si `mode` est égal à 1 (mode solo), ajuster le nombre maximum de tours en fonction de la taille du plateau de jeu (`taille_plateau`) et du nombre de navires (`nb_navires`).
   * Sinon, si `mode` est égal à 2 (mode multiplayer), fixer le nombre maximum de tours à 40.
2. Gérer les cas du mode *solo* :

   * Si la taille du plateau (`taille_plateau`) est inférieur ou égal à 4 :
     * Basculer entre différentes valeurs de `max_tours` en fonction du nombre de navires (`nb_navires`).
   * Si la taille du plateau (`taille_plateau`) est supérieur à 4 et inférieur ou égal à 10 :
     * Basculer entre différentes valeurs de `max_tours` en fonction du nombre de navires (`nb_navires`).
   * Sinon (taille du plateau supérieure à 10) :
     * Fixer `max_tours` à 60.
3. Affecter la valeur calculée de `max_tours` à la variable globale

## Procedure

Chaque projet infornatique est exigeant et durtout il demande la contribution de chaque membre de l'equipe pour eviter des erreurs sur la logique du prinet et sur le source code final. En ce qui concerne la communication, les messages direct sont la maniere le plus simple pour un equipe composé de deux membres. mais comment on fait pour orendre en conite tous les choses wu'on discuté en direct ou en messages ?

L'utilisation de Git est essentiel qui nous permettra de reprendre le projet, tout en contribuant chaqu'un independement et avoir un archive de chaque chagrment qui était effectué. Git aussi permet d'analyser le code avant compiler et faire un backup sur une service destiné de maintenir le code et ses changements toujours accesibles par n'importe quel appareil. Comme attendue, Git etait associe pour un repertoire Github avec un Copilot gcc pour l'analyse du code envoyé vers le compte Github.

## Programmation

### UI

L'UI définit un ensemble de fonctions liées à l'interface graphique du jeu "Bataille Navale". Ces fonctions sont responsables de l'affichage d'informations à l'utilisateur, telles que le plateau de jeu, les instructions et les messages. Très pratique si on veut modfier un message ou addresser un probleme sur la partie de l'affichage par exemple.

Voici une brève description des fonctions declares et construits :

#### **custom_graphics_on_proposition:**

Cette fonction permet d'afficher différents messages et invites à l'utilisateur en fonction de l'état actuel du jeu. Elle prend les paramètres suivants :

* `i`: Un entier représentant l'itération actuelle du processus de placement
* `plateau`: Un tableau bidimensionnel représentant le plateau de jeu
* `taille_plateau`: La taille du plateau de jeu
* `colour`: La couleur du joueur
* `mode`: Le mode de jeu (solo ou multijoueur)
* `id`: Un identifiant pour des messages d'erreur spécifiques

La fonction vérifie la valeur de `i` et détermine le message approprié à afficher, comme demander à l'utilisateur de saisir des coordonnées ou d'afficher des messages d'erreur.

#### **game_mode_graphics_congratulations:**

Cette fonction permet d'afficher un message de félicitations à l'utilisateur lorsqu'il a réussi à toucher un navire. Elle prend les paramètres suivants :

* `prop`: Un tableau bidimensionnel représentant le plateau de jeu
* `taille_plateau`: La taille du plateau de jeu
* `nb_navires`: Le nombre total de navires
* `nb_navires_trouvés`: Le nombre de navires trouvés jusqu'à présent
* `id`: Un identifiant pour le type de navire trouvé (navire du joueur ou navire de l'adversaire)
* `buffer`: Une chaîne contenant le type de navire trouvé

La fonction affiche un message félicitant l'utilisateur et met à jour la barre de progression pour refléter le nombre de navires trouvés.

#### **error_graphics:**

Cette fonction permet d'afficher des messages d'erreur à l'utilisateur. Elle prend le paramètre suivant :

* `error_code`: Un entier représentant le code d'erreur

La fonction affiche un message correspondant au code d'erreur, comme une erreur lors de l'enregistrement du jeu ou lors de la saisie d'une entrée non valide.

#### **game_loaded_graphics:**

Cette fonction permet d'afficher un message informant l'utilisateur que le jeu a été chargé à partir du serveur. Elle affiche également les règles du jeu à titre de référence.

#### **new_round_graphics:**

Cette fonction permet d'afficher un message indiquant le début d'une nouvelle manche. Elle prend les paramètres suivants :

* `round`: Le numéro de la manche actuelle
* `taille_plateau`: La taille du plateau de jeu
* `prop`: Un tableau bidimensionnel représentant le plateau de jeu
* `id`: Un identifiant pour le mode de jeu (tour du joueur ou tour de l'adversaire)
* `buffer`: Le nom du joueur dont c'est le tour
* `temps`: Le temps restant dans la manche (pour le mode solo)

La fonction affiche un message indiquant le numéro de la manche et, si applicable, le temps restant. Elle affiche également le plateau de jeu.

### Structures

Selon le sujet, on etait demandé de declarer les structures suivantes:

```c
typedef struct une_case {
  int x; /* position de la case en x*/
  int y; /* position de la case en y*/
} Case;

// type: Structure
typedef struct navire
{
    int sens; /* 0 haut 1 droite 2 bas 3 gauche */
    Case premiere_case;
    int taille;
    int id;
} Navire;
```

De plus de ça, on a aussi declaré les structures qui nous avons permet de traiter des cas dynamique en differents modes de jeu tels que:

* Les structures `Cellule_Liste_Navire` et `Cellule_Liste_Point` sont utilisées pour créer des listes chaînées de navires et de points. Ces listes sont utilisées pour représenter les navires et les coups dans le jeu de bataille navale.
* La structure `Liste_Navire` est utilisée pour gérer une liste de navires. Elle permet d'ajouter, de supprimer et de rechercher des navires dans la liste.
* La structure `Liste_Point` est utilisée pour gérer une liste de points. Elle permet d'ajouter, de supprimer et de rechercher des points dans la liste.
* La structure `Tableau_Point` est utilisée pour représenter un tableau de points. Elle est plus efficace que les listes chaînées pour représenter des listes de points de petite taille.

```c
typedef struct Cellule_Liste_Navire_
{
	Navire data;  
	struct Cellule_Liste_Navire_* suiv; 
} Cellule_Liste_Navire;

typedef struct Liste_Navire_
{
	unsigned int taille;  
	Cellule_Liste_Navire *first; 
	Cellule_Liste_Navire *last;  
	                       /* first = last = NULL et taille = 0 <=> liste vide */
} Liste_Navire;


/*---- le type cellule de liste de point ----*/
typedef struct Cellule_Liste_Point_
{
	Case data;   
	struct Cellule_Liste_Point_* suiv; 
} Cellule_Liste_Point;

/*---- le type liste de point ----*/
typedef struct Liste_Point_
{
	unsigned int taille;  
	Cellule_Liste_Point *first; 
	Cellule_Liste_Point *last;  
	                       /* first = last = NULL et taille = 0 <=> liste vide */
} Liste_Point;

/*---- le type tableau de point ----*/
typedef struct Tableau_Point_
{
	unsigned int taille; 
	Case *tab;    
} Tableau_Point;
```

#### **La structure Cellule_Liste_Navire**

Représente un élément d'une liste de navires. Elle contient deux champs :

* `data`: Une structure `Navire` qui représente les données du navire.
* `suiv`: Un pointeur vers l'élément suivant de la liste.

#### **La structure Liste_Navire**

Représente une liste de navires. Elle contient trois champs :

* `taille`: Le nombre d'éléments dans la liste.
* `first`: Un pointeur vers le premier élément de la liste.
* `last`: Un pointeur vers le dernier élément de la liste.

#### **La structure Cellule_Liste_Point**

Représente un élément d'une liste de points. Elle contient deux champs :

* `data`: Une structure `Case` qui représente les données du point.
* `suiv`: Un pointeur vers l'élément suivant de la liste.

#### **La structure Liste_Point**

Représente une liste de points. Elle contient trois champs :

* `taille`: Le nombre d'éléments dans la liste.
* `first`: Un pointeur vers le premier élément de la liste.
* `last`: Un pointeur vers le dernier élément de la liste.

#### **La structure Tableau_Point**

Représente un tableau de points. Elle contient deux champs :

* `taille`: Le nombre d'éléments dans le tableau.
* `tab`: Un pointeur vers le tableau des éléments.

### Saving API

L'option de sauvegarde pour nous etais tres important qui est basé sur le "filing codec" inventé par nous meme. La procedure est tres simple. Si quelqu'un veut sauvegarder le jeu, il suffit d'acceder au menu et chosir l'option "Sauvegarder" ou "Save" en anglais. Notre fonction void `api_save_game(int number_of_navires, int taille_plateau, int coulle, int round, int **matrix, Liste_Navireliste)` va encoder le jeu selon le codec.

Si on veut continuer à partir du jeu sauvegardé, il suffit de choisir  l'ooption "Continuer" ou "Load" par le menu principal. Ce mode de jeu fait tous les initilisations selon les donnes lu par la fonction `Liste_Navire api_load_game(const char *filename, int *ptr1, int *ptr2, int *ptr3, int *ptr4, int **matrix)`. D'autres fonction ssuplemetaires etait construit pour ameliorer la logique des differents modes. Par exemple, si qulqu'un a continué par le je jeu sauvegardé, il faut etre sur qui si le jeu termine - le fichier de sauvegarde est suprimé, ou si l'utilisateur re-sauvegrade - il faut nettoyer le fichier de sauvegarde avant re-ecrire à l'interieur:

#### **api_load_game**

**Algorithme:**

1. Ouvrir le fichier `filename` en mode lecture (`"r"`)
2. Si le fichier ne peut pas être ouvert, afficher un message d'erreur et quitter le programme.
3. Initialiser une liste `liste` vide de navires.
4. Allouer une nouvelle structure `Navire` et initialisez ses champs avec les valeurs lues du fichier.
5. Ajoutez le navire à la liste `liste`.
6. Repeat steps 4 et 5 jusqu'à ce que le fichier soit lu.
7. Fermer le fichier.
8. Retourner la liste `liste` des navires.

#### **api_table_size**

**Algorithme:**

1. Ouvrir le fichier `filename` en mode lecture (`"r"`)
2. Si le fichier ne peut pas être ouvert, afficher un message d'erreur et quitter le programme.
3. Lire le nombre de navires du fichier
4. Fermer le fichier
5. Retourner le nombre de navires

#### **api_clearFile**

**Algorithme:**

1. Ouvrir le fichier `filename` en mode écriture `w"`, ce qui supprime le contenu du fichier s'il existe déjà
2. Si le fichier ne peut pas être ouvert, afficher un message d'erreur et quitter le programme
3. Fermer le fichier

#### **api_save_game**

**Algorithme:**

1. Ouvrir le fichier `"filecodec239012V1.txt"` en mode écriture `w"`
2. Si le fichier ne peut pas être ouvert, afficher un message d'erreur et quitter le programme
3. Supprimer tout contenu existant dans le fichier, si cela est nécessaire
4. Écrire le nombre de navires, la taille du plateau, le nombre de navires coulés, le numéro de la manche et les informations sur chaque navire dans le fichier
5. Écrire `$` pour indiquer la fin des informations sur les navires
6. Écrire le tableau du plateau de jeu dans le fichier
7. Fermer le fichier

#### **api_delete_game_file**

**Algorithme:**

1. Supprimer le fichier `"filecodec239012V1.txt"`
2. Afficher un message de réussite
3. Retourner

### Filing codec

`number_of_navires taille_plateau coulle round sens_nav_1 x_nav_1 y_nav_1 taille_nav_1 id_nav_1 sens_nav_2 ...  $ prop_table_codec`

Notre filling codec représente un format structuré pour encoder et décoder les données du jeu Battleship. Il adhère à une séquence spécifique d'éléments pour garantir l'intégrité et la lisibilité des fichiers de jeu enregistrés.

Voici une ventilation du codec :

1. **Nombre de navires:** Cet entier indique le nombre total de navires présents dans le jeu.
2. **Taille du tableau:** Cet entier représente la dimension du plateau de jeu, généralement représenté comme `x` par `y`.
3. **Navires coulés:** Cet entier signifie le nombre actuel de navires coulés.
4. **Numéro de manche:** Cet entier indique la manche actuelle du jeu.
5. **Informations sur le navire:** Pour chaque navire, les attributs suivants sont codés:

   a. **Orientation (sens_nav):** Représente l'orientation du navire (horizontale ou verticale).

   b. **Coordonnée X (x_nav):** Indique la coordonnée X de départ de la tête du navire.

   c. **Coordonnée Y (y_nav):** Spécifie la coordonnée Y de départ de la tête du navire.

   d. **Taille du navire (taille_nav):** Représente la longueur du navire.

   e. **ID du navire (id_nav):** Identifie le navire de manière unique parmi tous les navires du jeu.
6. **Marqueur de fin des données de navire (\$):** Ce caractère marque la fin de la section des informations sur le navire.
7. **Données du tableau de jeu:** Cette section encode l'état réel du plateau de jeu, représenté comme une matrice d'entiers. Chaque élément de la matrice correspond à une cellule sur le plateau, avec des valeurs indiquant si la cellule est vide, touchée ou coulée.

L'ajout proposé d'un horodatage améliorerait le codec en fournissant un contexte supplémentaire sur le jeu enregistré. Le horodatage indiquerait le moment exact où le jeu a été enregistré, permettant à l'utilisateur de suivre l'évolution du jeu au fil du temps. De plus, si le codec est étendu pour la fonctionnalité multijoueur, le horodatage pourrait également indiquer le joueur qui a enregistré le jeu, ce qui renforcerait encore la transparence et la responsabilité.

### Mode Solo

Dans le mode solo du jeu de bataille navale, le joueur joue contre l'ordinateur. Le joueur commence par placer ses navires sur le plateau de jeu, puis l'ordinateur place ses navires. Ensuite, le joueur joue à tour de rôle en essayant de couler les navires de l'ordinateur. Le joueur gagne la partie en coulant tous les navires de l'ordinateur.

Il existe deux variantes du mode solo :

* **Mode classique :** Le joueur dispose d'un nombre illimité de tours pour couler les navires de l'ordinateur.
* **Mode chronométré :** Le joueur dispose d'un temps limite pour couler les navires de l'ordinateur.

Dans le mode classique, le joueur peut prendre son temps pour jouer et tenter de couler tous les navires de l'ordinateur. Dans le mode chronométré, le joueur doit jouer plus rapidement et faire attention à ne pas manquer de temps. Les deux variants du mode solo sont organisé comme ci-dessous:

1. **Initialisation:** Le joueur et l'ordinateur placent leurs navires sur le plateau de jeu.
2. **Tour du joueur:** Le joueur joue un coup en sélectionnant une case sur le plateau de jeu.
3. **Tour de l'ordinateur:** L'ordinateur joue un coup en sélectionnant une case sur le plateau de jeu.
4. **Fin de la partie:** La partie se termine lorsque tous les navires d'un joueur sont coulés.

### Mode Load

Le mode sauvegarder sur notre jeu permet à l'utilisateur de reprendre une partie précédemment enregistrée. Le fichier de sauvegarde contient les données suivantes (reference sur la partie Filing codec)):

* La taille du plateau de jeu
* Le nombre de navires
* L'emplacement des navires
* Le nombre de navires coulés
* Le nombre de tours joués

Une fois le fichier de sauvegarde lu, le jeu reprend à l'état où il était lorsque la partie a été sauvegardée. Le joueur peut ensuite continuer à jouer la partie normalement.

Le mode sauvegarder est une fonctionnalité utile qui permet aux joueurs de reprendre une partie qu'ils ont interrompue ou de continuer une partie qu'ils ont commencée sur un autre appareil si ils le souhaute (demande d'avoir le meme source code de jeu sur tous les appareils). La question est, comment on pourrait ameliorer ce more de jeu ?

* Le code pourrait être amélioré en fournissant des instructions plus claires et concises pour l'initialisation du jeu en mode sauvegarde.
* La progression du jeu pourrait être améliorée en introduisant une barre de progression ou un autre indicateur visuel pour représenter les manches restantes et le nombre de navires restants à couler.
* Le mécanisme de sauvegarde pourrait être amélioré en implémentant un mécanisme automatique de sauvegarde qui sauvegarde l'état du jeu à intervalles réguliers ou lorsqu'un événement significatif se produit, tel que la fin d'une manche ou le coulage d'un navire.
* L'interface utilisateur pourrait être améliorée avec des graphiques plus attrayants, tels que des tuiles animées ou des couleurs différentes pour les cases touchées, coulées et vides. Le jeu pourrait également incorporer des effets sonores pour encore plus immerger le joueur dans l'expérience.
* Le mode sauvegarder pourrait être étendu pour inclure une fonction de replay qui permet au joueur de revoir ses mouvements précédents et la progression du jeu. Cela pourrait être un outil d'apprentissage précieux et aider le joueur à identifier les domaines d'amélioration.

### Mode Multiplayer

Le mode multiplayer pour le jeu de bataille navale permet à deux joueurs humains de s'affronter en temps réel. Chaque joueur dispose d'un plateau de jeu individuel et d'un nombre de navires à placer sur ce plateau. Les joueurs prennent ensuite tour à tour pour essayer de couler les navires de leur adversaire. Le premier joueur à couler tous les navires de son adversaire gagne la partie.

L'agorithme qui decide quel utilisateur a gagné prend en compte qui a coulé le plus grand nombre des navires sur un nombre des tours specifique, ainsi que le nombre des navires coulés par rapport le nombre des navires totales sur le jeu. Plus precisement:

**Algorithme :**

1. Initialiser un compteur de navires coulés pour chaque joueur.
2. Pour chaque joueur, itérer sur les navires de son plateau de jeu et vérifier si le navire est coulé.
   * Si un navire est coulé, incrémenter le compteur de navires coulés correspondant.
3. Comparer les compteurs de navires coulés des deux joueurs.
   * Si le compteur de navires coulés d'un joueur est égal ou supérieur au nombre total de navires de son adversaire, ce joueur est déclaré vainqueur.
   * Sinon, le jeu continue jusqu'à ce qu'un joueur coule tous les navires de son adversaire.

**Pseudo-code :**

```python
compteurCoulesJoueur1 = 0
compteurCoulesJoueur2 = 0

pour chaque navire du plateau du joueur 1 :
    if navire coulé :
        compteurCoulesJoueur1++

pour chaque navire du plateau du joueur 2 :
    if navire coulé :
        compteurCoulesJoueur2++

if compteurCoulesJoueur1 >= nombreNaviresJoueur2 :
    joueur1 gagne
elseif compteurCoulesJoueur2 >= nombreNaviresJoueur1 :
    joueur2 gagne
else
    match continue
```

### Mode AI

## Problemes - Solutions

1. We had an issue when the chosen taille of the plateau was smaller than 6. It wasn't guarantee from the program if the creation of six navires would be succesful, especially for tailles petites tel que 4x4. In order to address that issue, we tried to understand if the problem was on the direction of the navire en train d'etre cree, or on the aleartory choosen taille of the navire.
   * Eventually, we found that the appropriate bool variable responsible for incrementing the counter of created_navires wasn't re-initialised which was provoking a logic issue when the program had to take new taille, sens, or coordinates for the navire en question.
2. Another issue that we had was with the detection of existign navires that were placed from the user manually (on multiplayer or AI mode). It was like the function est_valide_pro wasn't returning a response.
   * Eventually, we discovered that we used another identification code for the navires placed manually from the user - allowing us simply to distinhue the different operations - that were retransformed to real navire points once the lacement was completed.
   * The solution was to include the logical 'or' on the `est_valide_pro` function to detect those points as well.

## Exit codes

Below you will find the definition an dthe explanation of ou rpogram exit codes:

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

## Versioning

Le versioning est une pratique importante en programmation informatique. Il permet de garantir la cohérence des modifications apportées à un programme et de faciliter la collaboration entre nous, ainsi que de récupérer des données perdues ou corrompues. Voici les differents versions pendant l'evolution du code:

* **V1.1** Code de base
* **V1.2** Code corrigé selon indications de Manu
* **V1.3** Code optimized from the previous source code
* **V1.4** Comments added and cleaned source code
* **V2.1** Bug fixes regarding est_valid_pro
* **V2.2** Makefile updated
* **V3.1** Explained different things and added the function copier_navires . The part 'To be created' was updated. Check the proto on that section of what needs to be done.
* **V3.2** Fixes copier_navire error and added the main game loop including the functions discused on the board. NOTE: Code is not optimised and it can not be runned until teh game loop logic is completed
* **v4.5** Game logic has been completed and improvements were made on the source code
* **V5.0** All the UI has been completed. Introduction to different game modes was included on this version. Needs to be done: create the saving functionality (check to be done section) and comeplete the multiplayer mode.
* **V6.1** load game functionality has been added and fixed some bugs regarding the UI as well as the repsonse of the program n several extreme scenarions
* **V6.2** Loading and saving game functionality was completed, fixed some bugs on the get_user_input ui and we fixed some UI customisation issues as well.
* **V6.3** Added the temps choice on Solo mode and there were some bug fixes as well
* **v7.0** multiplayer added and more functionality was added on the ui with intelligent logic printing modes as well as there were some bug fixes on the source code programmes.c
* **v7.1** Imporved the program to have the exact signatures that the prof asked for and added some functionality for the points that were selected at least twice. Moreover the logic for the end of LOAD mode new logic was added to prevent a user to play again with a game file that was played before.
* **v7.2** Tried to fix the non paused time for solo mode option temps with no sucess
* **v7.3** Added a functionality to accept the format 2B for points coordinates
* **V7.4** AI added and the module to create navires customly
* **V8.0** Fixed issue with initialisatio_plateau_custom which couldn't detect the already existed navires, updated the UI files and transfered the majority of texts to the UI file as well. In addition to that, the code was optimised and there were several small bug fixes. The different game modes were seperated in order to be able to test them without the whole's game menu.

## Conclusion

###### Yanis SADOUN, Vasileios Filippos SKARLEAS | All rights reserved
