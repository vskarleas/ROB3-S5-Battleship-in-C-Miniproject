

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

typedef struct Cellule_Liste_Navire_
{
	Navire data;    /* donn�e de l'�l�ment de liste */
	struct Cellule_Liste_Navire_* suiv; /* pointeur sur l'�l�ment suivant */
} Cellule_Liste_Navire;

typedef struct Liste_Navire_
{
	unsigned int taille;        /* nombre d'�l�ments dans la liste */
	Cellule_Liste_Navire *first; /* pointeur sur le premier �l�ment de la liste */
	Cellule_Liste_Navire *last;  /* pointeur sur le dernier �l�ment de la liste */
	                       /* first = last = NULL et taille = 0 <=> liste vide */
} Liste_Navire;