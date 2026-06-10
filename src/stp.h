#ifndef STP_H
#define STP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "adresse.h"
#include "switch.h"

// On définit les états possibles d'un port STP
typedef enum 
{
    RACINE,
    BLOQUE,
    DESIGNE
} 
etat_port;

// On définit la structure d'un port STP
typedef struct port_stp
{
    // On met size_t pour le numéro du port
    size_t numero_port;

    // On met size_t pour l'indice du voisin connecté
    size_t voisin;

    // On stocke l'état du port
    etat_port etat;
} 
port_stp;

// On définit la structure qui représente l'état STP d'un switch
typedef struct etat_switch
{
    // On met size_t pour le port racine
    size_t root_port;

    // Le coût du chemin est un entier
    int cout_racine;

    // On stocke le tableau des ports STP
    port_stp *ports;

    // On met size_t pour le nombre de ports
    size_t nb_ports;
} 
etat_switch;

// On fait une déclaration anticipée pour que le compilateur connaisse reseau
typedef struct reseau reseau;

// On déclare la fonction qui exécute l'algorithme STP sur le réseau
etat_switch *executer_stp(reseau *r);

// On déclare la fonction qui libère la mémoire allouée par executer_stp
void liberer_stp(reseau *r, etat_switch *etats);

// On déclare la fonction qui affiche l'état STP de chaque switch
void afficher_etat_stp(reseau *r, etat_switch *etats);

// On déclare la fonction qui lance la simulation tick par tick
void ordonnanceur_global(reseau *mon_reseau, etat_switch *etats, int stp_actif);

// On déclare la fonction qui applique les états des ports
void appliquer_etats_ports(reseau *r, etat_switch *etats, int stp_actif);

#endif