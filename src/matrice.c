#include <stdio.h>
#include <string.h>
#include "matrice.h"

// On affiche la matrice d'adjacence du réseau sous forme graphique et colorée
void afficher_matrice_adjacence(reseau *r)
{
    // On récupère les dimensions du réseau
    size_t nbSwitch = get_nb_switch(r);
    size_t nbStations = get_nb_station(r);
    size_t nbLiens = get_nb_liens(r);

    // On calcule la taille totale de la matrice
    size_t tailleTotale = nbSwitch + nbStations;

    // On crée la matrice d'adjacence en mémoire
    int matriceAdjacence[tailleTotale][tailleTotale];

    // On initialise toutes les cases de la matrice à -1
    for(size_t i = 0; i < tailleTotale; i++)
    {
        for(size_t j = 0; j < tailleTotale; j++)
        {
            matriceAdjacence[i][j] = -1;
        }
    }

    // On remplit la matrice avec le coût de chaque lien du réseau
    for(size_t k = 0; k < nbLiens; k++)
    {
        lien lienActuel = get_lien(r, k);
        size_t equipementA_actuel = lienActuel.equipement_a;
        size_t equipementB_actuel = lienActuel.equipement_b;
        int coutActuel = lienActuel.cout;

        matriceAdjacence[equipementA_actuel][equipementB_actuel] = coutActuel;
        matriceAdjacence[equipementB_actuel][equipementA_actuel] = coutActuel;
    }
    
    // On définit les codes de couleur pour le terminal
    #define RESET   "\033[0m"
    #define VERT    "\033[32m"
    #define ROUGE   "\033[31m"

    // On fait un saut de ligne
    printf("\n");

    char titre[] = "Matrice d'adjacence du réseau";
    int len_titre = (int)strlen(titre);

    // On ajuste les largeurs pour l'alignement du tableau graphique
    int espace_gauche = 8; 
    int larg_donnees = (int)tailleTotale * 4;
    int marge_droite = 2;

    int larg_interne = espace_gauche + larg_donnees + marge_droite;

    // Si la largeur calculée est plus petite que le titre, on l'agrandit
    if (larg_interne < len_titre + 4) 
    {
        larg_interne = len_titre + 4;
    }

    int reste_droite = larg_interne - (espace_gauche + larg_donnees);

    // On dessine le haut du cadre extérieur
    printf("╔");
    for(int i = 0; i < larg_interne - 1; i++) 
    {
        printf("═");
    }
    printf("╗\n");

    int espaces_blancs = larg_interne - len_titre;
    int espaces_gauche = espaces_blancs / 2;
    int espaces_droite = espaces_blancs - espaces_gauche;

    // On affiche la ligne du titre centrée
    printf("║");
    for(int i = 0; i < espaces_gauche; i++) 
    {
        printf(" ");
    }
    printf("%s", titre);
    for(int i = 0; i < espaces_droite; i++) 
    {
        printf(" ");
    }
    printf("║\n");

    // On dessine la ligne de séparation sous le titre
    printf("╠");
    for(int i = 0; i < larg_interne - 1; i++) 
    {
        printf("═");
    }
    printf("╣\n");

    // On affiche les numéros de colonnes du tableau
    printf("║       "); 
    for(size_t j = 0; j < tailleTotale; j++)
    {
        printf(" %2zu ", j);
    }
    for(int i = 0; i < reste_droite; i++) 
    {
        printf(" ");
    }
    printf("║\n");

    // On dessine le haut du sous-cadre interne
    printf("║     ╔═");
    for(size_t j = 0; j < tailleTotale; j++)
    {
        printf("════");
    }
    printf("═");
    for(int i = 0; i < reste_droite - 1; i++) 
    {
        printf("═");
    }
    printf("╣\n");

    // On affiche les lignes de la matrice avec les valeurs ou les points
    for(size_t i = 0; i < tailleTotale; i++)
    {
        printf("║ %2zu  ║ ", i);

        for(size_t j = 0; j < tailleTotale; j++)
        {
            // Si les équipements ne sont pas connectés, on affiche un point rouge
            if(matriceAdjacence[i][j] == -1)
            {
                printf("  " ROUGE "." RESET " ");
            }
            // Si une liaison existe, on affiche son poids en vert
            else
            {
                printf("  " VERT "%d" RESET " ", matriceAdjacence[i][j]);
            }
        }
        
        for(int k = 0; k < reste_droite; k++) 
        {
            printf(" ");
        }
        printf("║\n");
    }

    // On dessine le bas du grand cadre avec les raccords
    printf("╚");
    for(int i = 0; i < larg_interne - 1; i++) 
    {
        // Si on est à la position du raccord vertical, on place le symbole adapté
        if(i == 5)
        {
            printf("╩");
        }
        else
        {
            printf("═");
        }
    }
    printf("╝\n\n");

    // On affiche la légende explicative
    printf("Légende :\n");
    printf("  " VERT "[chiffre]" RESET " : Poids de la liaison entre les équipements\n");
    printf("  " ROUGE "." RESET " : Équipements non connectés\n\n");
}