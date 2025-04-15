#include<stdio.h>

void date_actuel(int *jour,int * mois,int *annee){
    time_t tempsactuel = time(NULL);
    struct tm*dateactuelle =
    localtime(&tempsactuel);
    *jour= dateactuelle->tm_mday;
    *mois=dateactuelle->tm_mon+1;
    *annee= dateactuelle->tm_year+1900;

}
void situation(char nom ,char pernom){
// trouvons les tontine auquelle appartient l'utilisateur
float a,b,c,d,e,f;
char tab[100];
 char tontine[15];
 char ID[];
 char nom[];
 char prenom[];
 int jouractu,moisactu,anneeactu;
 date_actuel(jouractu,moisactu,anneeactu);

FILE *f=fopen("tontine.csv","r");
if(f==NULL) {
    perror("erreur d'ouverture du fichier\n");
    return -1;
}
    while(fgets(tab,sizeof(tab),f)){
    // fichier contenant la liste des tontine contient la dates de cotisation
        sscanf(tab,"%*d;%[^;];%d;%d;%d",tontine,&a,&b,&c);
        sprintf(IDtontine,"%s.csv",tontine);
//on ouvre les fivhiers des identifiants de chaque tontines
    FILE *T=fopen("IDtontine","r");
  while (fgets(ID,sizeof(ID),T)){
    sscanf(ID;"%*d;%[^;];%[^;];%*[^;];%*f;%d,%d,%d"nomuser,prenomuser,&d,&e,&f);
// IDutilisateur correspond a l'identifiant de l'utilisateur
    if(strcmp(nom,nomuser)==0 && strcmp(prenom,prenomuser)==0){
        printf("%s\n",tontine);
    //comparons la date cotiser par l'utlisitateur a la date de la cotisation, sachant que la date quand il cotise s'efface apres un certaiin nombre de temps

    if(f>0 && (f < c  || (f==c && e<=b ) || (f==c && e==b && d<=a)) )
    printf("bravo, vous avez cotisez a tant, vous renforcez ainsi  votre profil\n");
    else if(f==0 && (anneeactu == c && moisactu == b && jouractu + 5 > a))
    printf("oh oh il vous reste  %d jours pour cotisez a tant  si non vous perdrez en credibilite \n",a-jouractu);
    else if(f==c && e== b && d>a && d<=a+10) // 10 peut etre remplacer par le nombre de jours limites 
    printf("vous avez cotiser en retard votre credibilité est touché\n");
    else if(f==0 && (anneeactu == c && moisactu == b && jouractu > a && jouractu <= a+10 ))
    printf("vous n'avez pas encore cotiser vous risquer une plainte la penalite s'eleve %d \n " (jouractu-a)*1000);// 1000 represente la penalite par jour
// afficher l'ordre de bouffe
²






    }
        }
        fclose(T);
            }
            fclose(f);
}


    }


  }
// SITUATION ORDRE BOUFE DANS CHAQUE TONTINE
