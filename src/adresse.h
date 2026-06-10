#ifndef ADRESSE_H
#define ADRESSE_H

#include <stdint.h>
 
// On définit le type pour les adresses MAC : 6 octets
typedef uint8_t mac_t[6];

// On définit le type pour les adresses IPv4 : 4 octets
typedef uint8_t ipv4_t[4];
 
// On déclare la fonction qui affiche une adresse MAC
void afficher_mac(const mac_t mac);

// On déclare la fonction qui affiche une adresse IPv4
void afficher_ipv4(const ipv4_t ip);

#endif