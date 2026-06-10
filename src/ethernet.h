#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stddef.h> 

#include "adresse.h"

// On définit la structure d'une trame Ethernet II
typedef struct trame_t
{
    uint8_t preambule[7];
    uint8_t sfd;
    mac_t dest;
    mac_t src;
    uint16_t type;
    uint8_t data[1500];
    size_t taille_data;
    uint32_t fcs;
} 
trame_t;

// On déclare la fonction qui affiche la trame de façon lisible
void afficher_trame(trame_t *t);

// On déclare la fonction qui affiche la trame brute en hexadécimal
void afficher_trame_hex(trame_t *t);


#endif