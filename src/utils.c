#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "utils.h"
#include "adresse.h"

// On définit la fonction qui compare deux adresses MAC (standard type memcmp)
int comparer_mac(mac_t mac1, mac_t mac2)
{
    return memcmp(mac1, mac2, 6 * sizeof(uint8_t));
}

// On définit la fonction qui compare deux adresses IPv4 
// Attention : renvoie 1 si identiques, 0 si différentes
int comparer_ipv4(ipv4_t ip1, ipv4_t ip2)
{
    // On initialise un booléen à true pour supposer que les IP sont égales
    bool ipv4E = true;
    
    // On parcourt les 4 octets de l'adresse IPv4
    for(size_t i = 0; i < 4; i++)
    {
        // On vérifie si deux octets à la même position sont différents
        if(ip1[i] != ip2[i])
        {
            // On passe le booléen à faux car on a trouvé une différence
            ipv4E = false;
            
            // On interrompt la boucle pour gagner du temps
            break;
        }
    }

    // Si le booléen est resté vrai après le parcours
    if(ipv4E)
    {
        // On renvoie 1 pour dire que les adresses sont identiques
        return 1;
    }
    // Sinon
    else
    {
        // On renvoie 0 pour dire que les adresses sont différentes
        return 0;
    }
}

// On définit la fonction qui permet d'afficher un buffer en hexadécimal
void afficher_hex(uint8_t* buffer, size_t taille)
{
    // On parcourt chaque octet du buffer jusqu'à la taille indiquée
    for(size_t i = 0; i < taille; i++)
    {
        // On affiche l'octet actuel en hexadécimal
        printf("%02X", buffer[i]);

        // Si l'octet actuel n'est pas le tout dernier du buffer
        if(i < taille - 1)
        { 
            // On affiche un espace pour séparer les octets visibles
            printf(" ");
        }
    }
}