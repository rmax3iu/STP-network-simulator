#ifndef STATION_H
#define STATION_H

#include <stdio.h>
#include "adresse.h"

// On définit la structure d'une station avec une adresse MAC et une adresse IPv4
typedef struct station
{
    // On stocke l'adresse MAC de la station
    mac_t adrMac;

    // On stocke l'adresse IPv4 de la station
    ipv4_t adrIPv4;
}
station;

// On déclare la fonction qui crée et initialise une station à zéro
station init_station();

// On déclare la fonction qui remet à zéro une station existante
void deinit_station(station *s);

// On déclare la fonction qui récupère l'adresse MAC de la station
void getMac(station *s, mac_t mac);

// On déclare la fonction qui récupère l'adresse IPv4 de la station
void getIPv4(station *s, ipv4_t ipv4);

// On déclare la fonction qui modifie l'adresse MAC de la station
void setMac(station *s, mac_t newMac);

// On déclare la fonction qui modifie l'adresse IPv4 de la station
void setIPv4(station *s, ipv4_t newIPv4);

// Déclaration anticipée de la structure réseau pour éviter le bug d'inclusion croisée
struct reseau;

// On déclare la fonction qui affiche le tableau de toutes les stations disponibles
void afficher_stations_disponibles(struct reseau *r);

#endif