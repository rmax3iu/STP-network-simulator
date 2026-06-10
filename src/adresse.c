#include <stdio.h>
#include "adresse.h"

// On affiche une adresse MAC au format standard
void afficher_mac(const mac_t mac)
{
    // On parcourt les 6 octets de l'adresse MAC
    for(size_t i = 0; i < 6; i++)
    {
        printf("%02X", mac[i]);

        // Si on n'est pas sur le dernier octet, on ajoute le séparateur
        if(i < 5)
        {
            printf(":");
        }
    }
}

// On affiche une adresse IPv4 au format décimal
void afficher_ipv4(const ipv4_t ipv4)
{
    // On parcourt les 4 octets de l'adresse IPv4
    for(size_t i = 0; i < 4; i++)
    {
        printf("%d", ipv4[i]);
        
        // Si on n'est pas sur le dernier octet, on ajoute le point
        if(i < 3)
        {
            printf(".");
        }
    }
}