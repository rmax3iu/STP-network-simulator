#ifndef RESEAU_H
#define RESEAU_H

#include <stddef.h>
#include "switch.h"
#include "station.h"

// On fait une déclaration anticipée pour éviter les erreurs de type inconnu
typedef struct etat_switch etat_switch;

// On définit la structure d'un lien entre deux équipements avec un coût
typedef struct lien
{
    // On stocke l'indice de l'équipement A
    size_t equipement_a;

    // On stocke l'indice de l'équipement B
    size_t equipement_b;

    // On stocke le coût du lien
    int cout;
} 
lien;

// On définit la structure d'un réseau contenant des stations et des switchs
typedef struct reseau
{
    // On stocke le tableau dynamique de stations
    station *stations;

    // On stocke le tableau dynamique de switchs
    switch_t *switchs;

    // On stocke le tableau dynamique de liens
    lien *liens;

    // On stocke le nombre de stations actuellement dans le réseau
    size_t nb_station;

    // On stocke le nombre de switchs actuellement dans le réseau
    size_t nb_switch;

    // On stocke le nombre de liens actuellement dans le réseau
    size_t nb_liens;

    // On stocke la capacité maximale actuelle du tableau de stations
    size_t stations_capacite;

    // On stocke la capacité maximale actuelle du tableau de switchs
    size_t switchs_capacite;

    // On stocke la capacité maximale actuelle du tableau de liens
    size_t liens_capacite;
} 
reseau;

// On déclare la fonction qui initialise la structure du réseau
reseau init_reseau();

// On déclare la fonction qui libère la mémoire du réseau
void deinit_reseau(reseau *r);

// On déclare la fonction qui ajoute une station au réseau
void add_station(reseau *r, station s);

// On déclare la fonction qui ajoute un switch au réseau
void add_switch(reseau *r, switch_t s);

// On déclare la fonction qui ajoute un lien au réseau
void add_lien(reseau *r, lien l);

// On déclare la fonction qui retourne une station selon son index
station get_station(reseau *r, size_t i);

// On déclare la fonction qui retourne un switch selon son index
switch_t get_switch(reseau *r, size_t i);

// On déclare la fonction qui retourne un lien selon son index
lien get_lien(reseau *r, size_t i);

// On déclare la fonction qui retourne le nombre de stations du réseau
size_t get_nb_station(reseau *r);

// On déclare la fonction qui retourne le nombre de switchs du réseau
size_t get_nb_switch(reseau *r);

// On déclare la fonction qui retourne le nombre de liens du réseau
size_t get_nb_liens(reseau *r);

// On déclare la fonction qui incrémente et retourne le nombre de stations
size_t add_nb_station(reseau *r);

// On déclare la fonction qui incrémente et retourne le nombre de switchs
size_t add_nb_switch(reseau *r);

// On déclare la fonction qui retourne la capacité actuelle du tableau de stations
size_t get_stations_capacite(reseau *r);

// On déclare la fonction qui retourne la capacité actuelle du tableau de switchs
size_t get_switch_capacite(reseau *r);

// On déclare la fonction qui retourne la capacité actuelle du tableau de liens
size_t get_liens_capacite(reseau *r);

// On déclare la fonction qui cherche le coût d'un lien entre deux switchs
int get_cout_lien(reseau *r, size_t switch1, size_t switch2);

// On déclare la fonction qui cherche l'index d'un switch via sa MAC
size_t get_switch_index_by_mac(reseau *r, mac_t mac);

// On déclare la fonction qui cherche le numéro de port connecté à un voisin
size_t get_port_index_to_neighbor(etat_switch *etats, size_t switch_i, size_t neighbor_id);

#endif