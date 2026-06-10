#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>
#include "adresse.h"

// On déclare la fonction qui servira à comparer deux adresses MAC
int comparer_mac(mac_t mac1, mac_t mac2);    

// On déclare la fonction qui servira à comparer deux adresses IPv4
int comparer_ipv4(ipv4_t ip1, ipv4_t ip2);      

// On déclare la fonction qui servira à afficher un tableau en hexadécimal
void afficher_hex(uint8_t* buffer, size_t taille);       

#endif