#ifndef BIZARRE_H
#define BIZARRE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

// Définition des structures
typedef struct {
    int jour;
    int mois;
} Date;

typedef struct {
    int numero;
    char nom[15];
    char membre[20][15];
    char Admin[20][15];
    float caisse;
    float interet;
    float cotisation;
    char Mdp[20];
    int nb_membre;
    int nb_admin;
} Tontine;

typedef struct {
    char nom[15];
    char prenom[10];
    char mdp[10];
    int id;
    float solde;
} User;

// Variables globales
extern char nom_actuel[15];
extern char prenom_actuel[15];
extern char mdp_actuel[15];
extern Tontine les_tontines[50];
extern User les_utilisateurs[50];
extern int nb_utilisateurs;
extern int nb_tontine;
extern int utilisateur_actuel;
//extern const char *chemin_dossier_tontines;
extern char menu_global_texte[300];
extern char menu[100];
extern char texte_admin[300];
extern char interet[10];
// Déclarations des fonctions
int trouver_user(char *nom_pers, char *prenom_pers);
int trouver_reunion(char *nom);
int derniere_ligne(char *tontine);
void Ajouter_utilisateur();
void ajouter_membre(char *nom_tontine, char *nom_per, char *prenom);
void supprimer_membre(char *tontine, char *nom_pers, char *prenom_pers);
void supprimer_reunion(char *tontine);
//int statut(char *pers, char *prenom, char *reunion);
void Creer_tontine(char *nom, double montant, double interet_des_dettes, char *mdp);
void cotiser(char *nom_reunion);  
//char** liste_tontine(char *nom, char *prenom);
void membre_reunion(char *reunion);
int authentifier(char *nom_pers, char *prenom_pers, char *mdp_ton);
void Promotion(char *nom_pers, char *prenom_pers, char *tontine);
void menu_Admin(char *nom, char *prenom, char *cible);
void menu_membre(char *nom, char *prenom, char *cible);
void sous_menu(char *nom, char *prenom);
void menu_global();

#endif // BIZARRE_H