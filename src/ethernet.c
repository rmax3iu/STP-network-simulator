#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ethernet.h"
#include "adresse.h"
#include "utils.h"
#include "reseau.h"
#include "stp.h"

// On affiche tous les champs de la trame Ethernet
void afficher_trame(trame_t *t)
{
    // On fait un saut de ligne 
    printf("\n");

    // On affiche le préambule octet par octet
    printf("Préambule : ");
    for(size_t i = 0; i < 7; i++)
    {
        printf("%02X", t->preambule[i]);

        // On ajoute un espace entre chaque octet sauf le dernier
        if(i < 6)
        {
            printf(" ");
        }
    }
    printf("\n");

    // On affiche le Start Frame Delimiter
    printf("SFD : %02X\n", t->sfd);

    // On affiche l'adresse MAC destination
    printf("MAC destination : ");
    afficher_mac(t->dest);
    printf("\n");

    // On affiche l'adresse MAC source
    printf("MAC source : ");
    afficher_mac(t->src);

    // On affiche le type de trame en hexadécimal
    printf("\nType : 0x%04x\n", t->type);

    // On affiche les données sous forme de caractères
    printf("Les données : ");
    for(size_t i = 0; i < t->taille_data; i++)
    {
        printf("%c", t->data[i]);
    }
    printf("\n");

    // On calcule le nombre d'octets de bourrage si les données font moins de 46 octets
    size_t bourrage = 0;
    if (t->taille_data < 46)
    {
        bourrage = 46 - t->taille_data;
    }

    // On affiche la taille réelle et le bourrage éventuel
    printf("Taille : %zu octets", t->taille_data);
    
    // Si un bourrage est nécessaire, on l'affiche
    if (bourrage > 0)
    {
        printf(" (+ %zu octets de bourrage)", bourrage);
    }
    printf("\n");

    // On affiche le Frame Check Sequence
    printf("FCS : %08x\n", t->fcs);
    printf("\n");
}

// On affiche la trame brute directement en hexadécimal
void afficher_trame_hex(trame_t *t)
{
    // On affiche les 7 octets du préambule
    afficher_hex(t->preambule, 7);
    
    // On affiche l'unique octet du SFD en passant son adresse mémoire
    afficher_hex(&(t->sfd), 1);
    
    // On affiche les 6 octets de la MAC destination
    afficher_hex((uint8_t*)&(t->dest), 6);
    
    // On affiche les 6 octets de la MAC source
    afficher_hex((uint8_t*)&(t->src), 6);
    
    // On affiche les 2 octets du type de trame
    afficher_hex((uint8_t*)&(t->type), 2);
    
    // On affiche les octets des données selon la taille réelle stockée
    afficher_hex(t->data, t->taille_data);
    
    // On affiche les 4 octets du FCS
    afficher_hex((uint8_t*)&(t->fcs), 4);
}
