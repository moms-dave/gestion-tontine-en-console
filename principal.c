#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>  
#include<conio.h>
#include "serveur.c"
#include "client.c"


typedef struct
{
    int jour ;
    int mois ;
}Date;
  
typedef struct
{
    int numero ;
    char nom[15];
    char membre[20][15];
    char Admin[20][15];
    float caisse;
    float interet;
    float cotisation;
    char Mdp[20];
    int nb_membre ;
    int nb_admin ;

}Tontine;

typedef struct
{
    char nom[15] ;
    char prenom[10];
    char mdp[10];
    int id ;
    float solde ;


}User ;

char nom_actuel[15];
char prenom_actuel[15];
Tontine les_tontines[50];
User les_utilisateurs[50];
int nb_utilisateurs=0 ;
int nb_tontine=0 ;
int utilisateur_actuel ; 

int trouver_user (char *nom_pers , char *prenom_pers)
{
  int trouve = 0 ;
  FILE *f = fopen("utilisateurs.csv", "r");
  if (f == NULL) {
      perror("Erreur lors de l'ouverture du fichier utilisateurs.csv");
      return -1;
  }

  char ligne[100];
  while (fgets(ligne, sizeof(ligne), f)) {
      char nom[15];
      char prenom[15];
      sscanf(ligne, "%*d,%[^,],%[^,],%*[^,],%*f", nom , prenom);
      if (strcmp(nom_pers, nom) == 0 && strcmp(prenom_pers , prenom)==0) {
          trouve = 1 ;
          fclose(f); 
      }
  
  }
  fclose(f);
  return trouve;

}

int trouver_reunion ( char *nom)
{
  FILE *f = fopen("les tontines.csv", "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier les tontines.csv");
        return -1;
    }

    char line[100];
    while (fgets(line, sizeof(line), f)) 
    {
        char nom_fichier[15];
        sscanf(line, "%*d;%[^;];%*[^;],%*[^;];%*f", nom_fichier);
        if (strcmp(nom_fichier, nom) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return -1;
}

int derniere_ligne( char *tontine)
{
  int num=0;
  char fichier[25];
  sprintf(fichier , "%s.csv", tontine);
  FILE *f =fopen(fichier , "r");
  if (f == NULL) {
    perror("Erreur lors de l'ouverture du fichier");
    return -1;
}
  char ligne[100];
  while(fgets(ligne , sizeof(ligne) , f))
  {
     num ++ ;
  }
  fclose(f);
  return num+1;

}

void Ajouter_utilisateur() {
    char nom[15];
    char prenom[15];
    char MDP[10];
    int i=0;
    char carac ;
    printf("Entrer le nom :\n");
    scanf("%s", nom);
    printf("Entrer le prenom :\n");
    scanf("%s", prenom);
    printf("Quel sera votre mot de passe :\n");
    while(carac = getch() && i<12)
    {
      if(i == 13) break ;
      MDP[i++] = carac ;
      printf("*");
    }
    scanf("%s", MDP);

    FILE *f = fopen("utilisateurs.csv", "a+");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier utilisateurs.csv");
        return;
    }

    fprintf(f, "%d,%s,%s,%s,%.2f\n",derniere_ligne("utilisateurs") , nom, prenom, MDP, 50000.0);
    fclose(f);

    printf("Utilisateur ajouté avec succès !\n");
    nb_utilisateurs++;
}

void ajouter_membre(char* nom_tontine , char *nom_per, char *prenom)
{
      int numero ;
      char nomFichier[50];
      if(trouver_user(nom_per , prenom )==1)
      {
      sprintf(nomFichier , "%s%s", nom_tontine , ".csv"); 
      FILE *f = fopen(nomFichier ,"a+");
      if (f == NULL) {
          perror("Erreur lors de l'ouverture du fichier de la tontine");
          return;}
          printf("la derniere ligne est %d", derniere_ligne(nom_tontine));
      fprintf( f , "%d;%s;%s;%s;%s\n",derniere_ligne(nom_tontine),nom_per , prenom, "NON","membre") ; 
      fclose (f);  
       return ;
      }else{
        printf("L'utilisateur n'existe pas \n");
      }
} 

void supprimer_membre(char *tontine, char *nom_pers, char *prenom_pers)
{
    char reunion[50];
    sprintf(reunion, "%s.csv", tontine);

    FILE *f = fopen(reunion, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de la tontine");
        return;
    }

    FILE *temp = fopen("tempo.csv", "w");
    if (temp == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(f);
        return;
    }

    char ligne[100];
    int membre_supprime = 0;
    int id=1;

    while (fgets(ligne, sizeof(ligne), f)) {
        char nom[15];
        char prenom[15];
        char etat[10];
        char statut[10];
        

           sscanf(ligne, "%*d;%[^;];%[^;];%[^;];%[^;]",nom, prenom , etat , statut);
            if (strcmp(nom, nom_pers) == 0 && strcmp(prenom, prenom_pers) == 0) 
            {
                membre_supprime = 1;
                continue;
            }else
            {
              //id = derniere_ligne("tempo");
              fprintf(temp, "%d;%s;%s;%s;%s",id++, nom , prenom , etat , statut);
            }
    }
    fclose(f);
    fclose(temp);
    // Remplacer l'ancien fichier par le fichier temporaire
    if (membre_supprime) {
        if (remove(reunion) != 0) {
            perror("Erreur lors de la suppression de l'ancien fichier");
        } else if (rename("tempo.csv", reunion) != 0) {
            perror("Erreur lors du renommage du fichier temporaire");
        } else {
            printf("Le membre %s %s a ete supprime avec succes de la tontine %s.\n", nom_pers, prenom_pers, tontine);
        }
    } else {
        printf("Le membre %s %s n'a pas ete trouve dans la tontine %s.\n", nom_pers, prenom_pers, tontine);
        remove("temp.csv"); // Supprimer le fichier temporaire inutile
    }
}
void supprimer_reunion( char *tontine)
{
    char reunion[20];
    sprintf(reunion ,"%s.csv", tontine);
    FILE *f = fopen("tontines.csv", "r+");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de la tontine");
        return;
    }
    FILE *temp = fopen("temp.csv", "a+");
    if (temp == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(f);
        return;
    }

    char ligne[100];
    int reunion_supprime = 0;
     int num=1 ;
    while (fgets(ligne, sizeof(ligne), f)) {
        char nom[25];
        float coti;
        float interet;
        char mdp[10];

           sscanf(ligne, "%*d;%[^;];%f;%f;%[^;]", nom, &coti , &interet, mdp);
            if (strcmp(nom, tontine) == 0) 
            {
                reunion_supprime = 1;
                continue;
            }
              fprintf(temp, "%d;%s;%f;%f;%s",num++, nom , coti , interet , mdp);
            
    }
    fclose(f);
    fclose(temp);
    
    if (reunion_supprime==1) {
      if (remove("tontines.csv") != 0) {
          perror("Erreur lors de la suppression de l'ancien fichier");
      } else if (rename("temp.csv", "tontines.csv") != 0) {
          perror("Erreur lors du renommage du fichier temporaire");
      } else {
          remove(reunion);
          printf("La tontine %s a ete supprime avec succes.\n", tontine);
      }
  } else {
      printf("La tontine %s n'a pas ete trouve \n",tontine);
      remove("temp.csv"); // Supprimer le fichier temporaire inutile
  }
}
int statut(char *pers ,char *prenom , char *reunion)
 {
    char tontine[15];
    char nom_pers[15];
    char prenom_pers[15];
    sprintf(tontine , "%s.csv", reunion);
    FILE *f=fopen(tontine , " r");
    char ligne[100];
    while(fgets(ligne , sizeof(ligne), f))
    {
      char statut[15];
      sscanf(ligne , "%*d;%[^;];%[^;];%*[^;];%[^\n]", nom_pers,prenom_pers ,statut); 
        if(strcmp(statut , "admin")==0  && strcmp(prenom_pers , prenom)==0 && strcmp(nom_pers , pers)==0)
        {
          fclose(f);
          return 1 ;
        }
    }
    fclose(f);
    return 0;
 }

void Creer_tontine(char *nom , float montant, float interet_des_dettes, char *mdp)
{
  char nom_fichier[50];
  sprintf(nom_fichier, "%s.csv", nom);

  FILE *f = fopen(nom_fichier, "w");
  if (f == NULL) {
      perror("Erreur lors de l'ouverture du fichier de la tontine");
      return;
  }
  fprintf(f, "1;%s;%s;%s;%s\n", nom_actuel,prenom_actuel ,"paye","admin");
  fclose(f);

  FILE *ft = fopen("tontines.csv", "a+");
  if (ft == NULL) {
      perror("Erreur lors de l'ouverture du fichier tontines.csv");
      return;
  }

  int num = derniere_ligne("tontines");
  fprintf(ft, "%d;%s;%0.2f;%.2f;%s\n",num, nom, montant, interet_des_dettes, mdp);
  fclose(ft);

  printf("La tontine %s a ete cree avec succes !\n", nom);
  nb_tontine++;
}

void cotiser(char *nom_reunion)
 {
    char NomReunion[18];
    float coti_r;
    float solde;
    int i=0 ;
    sprintf(NomReunion, "%s.csv", nom_reunion);

    FILE *f = fopen("utilisateurs.csv", "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier utilisateurs.csv");
        return;
    }

    FILE *f1 = fopen("tontines.csv", "r");
    if (f1 == NULL) {
        perror("Erreur lors de l'ouverture du fichier tontines.csv");
        fclose(f);
        return;
    }

    // Trouver le montant à cotiser
    char ligne1[100];
    while (fgets(ligne1, sizeof(ligne1), f1)) {
        float test;
        char nom[18];
        sscanf(ligne1, "%*d;%[^;];%f;%*f;%*[^\n]", nom, &test);
        if (strcmp(nom, nom_reunion) == 0) {
            coti_r = test;
            break;
        }
    }
    fclose(f1);

    // Vérifier le solde de l'utilisateur actuel
    FILE *temp = fopen("tempo.csv", "w");
    if (temp == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(f);
        return;
    }

    char ligne[100];
    int id = 1;

    while (fgets(ligne, sizeof(ligne), f)) {
        char nom[15];
        char prenom[15];
        char mdp[10];
        float test2 ;


        sscanf(ligne, "%*d,%[^,],%[^,],%[^,],%f", nom, prenom, mdp, &test2);
        if (strcmp(nom, nom_actuel) == 0 && strcmp(prenom, prenom_actuel) == 0)
         {  
          solde = test2;
          if (test2 >= coti_r) 
            {
                i=1 ;
                solde -= coti_r;
                fprintf(temp, "%d,%s,%s,%s,%0.2f\n", id++, nom, prenom, mdp, solde);
            } else {
              i=0 ;
                fprintf(temp, "%d,%s,%s,%s,%0.2f\n", id++, nom, prenom, mdp, test2);
            }
         } else {
            fprintf(temp, "%d,%s,%s,%s,%0.2f\n", id++, nom, prenom, mdp, test2);
          }

    }
    fclose(f);
    fclose(temp);
    if (remove("utilisateurs.csv") != 0) {
        i=2 ;
        perror("Erreur lors de la suppression de l'ancien fichier");
    } else if (rename("tempo.csv", "utilisateurs.csv") != 0) {
      i=2;
        perror("Erreur lors du renommage du fichier temporaire");
    }
    else if (i==1 ){
      printf(" Vous avez cotise %0.1f dans la tontine %s, il vous reste %0.1f", coti_r , nom_reunion , solde);
      sleep(2);
    }
    else 
    {
      printf("Vous ne pouvez pas cotiser car votre solde est insuffisant : %0.2f cfa", solde);
      sleep(2);
    }
}

void liste_tontine(char *nom , char *prenom)
 {
  int compt = 0;
    FILE *f = fopen("tontines.csv", "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier tontines.csv");
        return ;
    }
    char ligne[100];
    printf(" \t\t liste des tontines\n");
    while (fgets(ligne, sizeof(ligne), f)) {
        char reunion[15];
        char vreunion[15];
        sscanf(ligne, "%*d;%[^;];%*[^;];%*f;%*f;%*[^;]", reunion);
        strcpy(vreunion , reunion);
        strcat(reunion , ".csv");
        FILE *f1 = fopen(reunion, "r");
        if (f1 == NULL) {
            perror("Erreur lors de l'ouverture du fichier reunion");
            return ;
        }
        char ligne2[100];
        while ( fgets(ligne2  ,sizeof(ligne2), f1))
        {
          char nom_pers[15];
          char prenom_pers[15];
          sscanf(ligne2 , "%*d;%[^;];%[^;];%*[^;];%*[^;]", nom_pers , prenom_pers );
          if(strcmp(nom_pers , nom)==0 && strcmp(prenom_pers , prenom)==0)
          {
            compt ++;
            printf("  --> %s \n", vreunion);
          }
         }
          fclose(f1);   
    }
    fclose(f);
    if(compt==0) printf("Vous n'etes encore dans aucune tontine \n");
 }
 
void membre_reunion(char *reunion)
{
  char reunionFichier[50];
  char nom[20];
  char prenom[20];
  int a =1;
  sprintf(reunionFichier, "%s.csv", reunion);
  FILE *f = fopen(reunionFichier, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier les tontines.csv");
        return ;
    }
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        char nom_fichier[15];
        sscanf(line, "%*d;%[^;];%[^;];%*[^;]", nom , prenom);
        printf ("%d -> %s  %s \n",a++, nom , prenom);  
    }

    fclose(f);
    return ;
}
 
int authentifier(char *nom_pers ,  char *prenom_pers ,char *mdp_ton )
 {
  FILE *f = fopen("utilisateurs.csv", "r");
  if (f == NULL) {
      perror("Erreur lors de l'ouverture du fichier utilisateurs.csv");
      return -1;
  }

  char ligne[100];
  while (fgets(ligne, sizeof(ligne), f)) {
      char nom[15];
      char mdp[15];
      char prenom[15];
      sscanf(ligne, "%*d,%[^,],%[^,],%[^,],%*f", nom ,prenom , mdp);
      if (strcmp(nom_pers, nom) == 0 && strcmp(prenom , prenom_pers)==0 && strcmp(mdp_ton , mdp)==0) {
        printf(" Bienvenu %s %s\n", nom_pers, prenom_pers);
          fclose(f);
          return 1 ;
      }
   }
      system("cls");
      printf("\t\t Nom ou mot de passe incorrect \n");
      sleep(2);
      system("cls");
   return 0;
 }
void Promotion(char *nom_pers , char *prenom_pers , char *tontine)
{
  char reunion[50];
  sprintf(reunion, "%s.csv", tontine);

  FILE *f = fopen(reunion, "r");
  if (f == NULL) {
      perror("Erreur lors de l'ouverture du fichier de la tontine");
      return;
  }

  FILE *temp = fopen("tempo.csv", "w");
  if (temp == NULL) {
      perror("Erreur lors de la création du fichier temporaire");
      fclose(f);
      return;
  }

  char ligne[100];
  int id=1;

  while (fgets(ligne, sizeof(ligne), f)) {
      char nom[15];
      char prenom[15];
      char etat[10];
      char statut[10];
      

         sscanf(ligne, "%*d;%[^;];%[^;];%[^;];%[^;]",nom, prenom , etat , statut);
          if (strcmp(nom, nom_pers) == 0 && strcmp(prenom, prenom_pers) == 0) 
          {
              fprintf(temp, "%d;%s;%s;%s;%s\n",id++, nom , prenom , etat , "admin");
          }else
          {
            //id = derniere_ligne("tempo");
            fprintf(temp, "%d;%s;%s;%s;%s",id++, nom , prenom , etat , statut);
          }
  }
  fclose(f);
  fclose(temp);

  if (remove(reunion) != 0) {
    perror("Erreur lors de la suppression de l'ancien fichier");
} else if (rename("tempo.csv", reunion) != 0) {
    perror("Erreur lors du renommage du fichier temporaire");
} else {
    printf("Le membre %s %s a ete promu avec succes dans la tontine %s.\n", nom_pers, prenom_pers, tontine);
}
} 

 void menu_Admin(char *nom , char *prenom , char *cible)
 { 
   int c= 1;
   while(c==1)
   {
   int choix ;
   printf("\n Selectionner une action\n");
    printf("1) Consulter la liste des membres \n");
    printf("2) Ajouter un membre \n");
    printf("3) Supprimer un membre \n");
    printf("4) supprimer la tontine \n");
    printf("5) promouvoir un membre \n");
    printf( "6) Cotiser \n");
    printf("7) Retour \n...");
 
    scanf("%d",&choix);
   
    switch(choix)
    {
       case 1 : 
       {
         membre_reunion(cible);
         printf("appuyer une touche pour continuer.....");
         getch();
         system("cls");
         break ;
       }
     
       case 2:
       char personne_prenom[15];
    char personne_nom[15];
     printf("Entrer le nom de la personne \n");
     scanf("%s", personne_nom);
     printf("Entrer le prenom de la personne \n");
     scanf(" %s", personne_prenom);
     ajouter_membre(cible , personne_nom ,personne_prenom);
     printf("\n appuyer une touche pour continuer....");
     getch();
     system("cls");
     break ; 
 
     case 3:
     char prenom_pers[15];
     char nom_pers[15];
     printf("entrer le nom du membre a supprimer : ");
     scanf("%s",nom_pers);
     printf("entrer le prenom du  membre:  ");
     scanf("%s", prenom_pers);
     supprimer_membre(cible , nom_pers , prenom_pers);
     printf("appuyer une touche pour continuer\n...");
      getch();
      system("cls");
      break ;
     
     case 4:
      supprimer_reunion(cible);
      c=0;
      printf("appuyer une touche pour continuer\n...");
      getch();
      system("cls");
      break;
     
     case 5 :
     char name[15];
     char surname[15];
      printf("entrer le nom du membre a promouvoir : ");
      scanf("%s",name); 
      printf("entrer le prenom du membre a promouvoir : ");
      scanf("%s",surname);
      Promotion(name , surname , cible); 
      printf("appuyer une touche pour continuer\n...");
      getch();
      system("cls");
      break ;

     case 6 :
      cotiser(cible);
      system("cls");
      break ;

     case 7:
     c=0;
     system("cls");
    }
 } 
 }
void menu_membre( char *nom , char *prenom, char *cible)
{
  int choix ;
 printf("Bienvenu %s %s \n", nom , prenom);
 printf("1) Cotiser \n");
 scanf("%d",&choix);
 if( choix == 1) cotiser(cible);
 if(choix != 1) printf("choix non valide");
 sleep(2);
}
 
void sous_menu(char *nom , char *prenom)
{

  int s=1;
  while(s==1)
  {
  liste_tontine(nom_actuel , prenom_actuel);
  char cible[15];
  int choix ;
   printf("1) Acceder a une tontine \n");
   printf("2) Creer une tontine \n");
   printf("3) Quitter \n....");
   scanf("%d",&choix);

   switch(choix)
   {
     case 1 :
     printf("A quelle tontine voulez vous acceder \n");
     scanf("%s", cible);
     if(statut(nom_actuel ,prenom_actuel, cible)==1)
     {
       menu_Admin(nom_actuel, prenom_actuel, cible);
     }
     else
     {
       menu_membre(nom_actuel, prenom_actuel, cible);
     }
     break ;
    case 2 :
    char nom_ton[15];
    char Mdp_r[15];
    system("cls");
    printf("entrer le nom de la tontine \n");
    scanf("%s", nom_ton);
     printf("entrer le mot de passe de la tontine \n");
     scanf("%s", Mdp_r);
     Creer_tontine(nom_ton ,15000 , 1500 , Mdp_r);
     printf("appuyer une touche pour continuer....");
     getch();
      system("cls");
     break ; 

     case 3 :
     s= 0;
     system("cls");
     break;
    }
   }

}

void menu_global()
{
int s = 1 ;
int choix ;
while(s==1)
{
  printf("-------------------------LES GRANDES TONTINES---------------------\n\n\n");
  printf(" 1) S'enregistrer comme utilisateur \n");
  printf(" 2) Se connecter\n");
  printf(" 3) A propos \n");
  printf(" 4) Quitter \n");
  printf("....");
  scanf("%d",&choix);

  switch(choix)
  {
  case 1:
  {
    Ajouter_utilisateur();
    system("cls");
    break ;
  } 
  case 2:
  {
    int i=0;
    char carac ;
    char mdp [12];
    printf("Entrer votre nom : \n");
    scanf("%s", nom_actuel);
    printf("Entrer votre prenom : \n");
    scanf("%s", prenom_actuel);
    printf("Entrer votre mot de passe : \n");
    i = 0;
    while (1) {
        carac = getch(); 
        if (carac == 13) // touche entree
        { 
            mdp[i] = '\0';
            break;
        } else if (carac == 8) { // Touche retour
            if (i > 0) {
                i--;
                printf("\b \b"); //Effacer 
            }
        } else if (i < 12) { 
            mdp[i++] = carac;
            printf("*");
        }
    }
    if( authentifier(nom_actuel,prenom_actuel, mdp)==1)
    {
        system("cls");      
        sous_menu(nom_actuel , prenom_actuel);
       
    }
    break ;
  }

    case 4:   
    s=0;
    break ;

  }
  
}
}


int main()
{
   menu_global();
}