#include <stdio.h>
#include <string.h>

#include "station.h"
#include "reseau.h"
#include "adresse.h"

// On initialise une station avec des adresses MAC et IPv4 à zéro
station init_station()
{
    // On crée une station vide
    station s;

    // On crée une adresse MAC vide 
    mac_t newMac = {0};

    // On crée une adresse IPv4 vide 
    ipv4_t newIPv4 = {0};

    // On applique les adresses vides à la station
    setMac(&s, newMac);
    setIPv4(&s, newIPv4);

    // On retourne la station initialisée
    return s;
}

// On remet à zéro les adresses d'une station existante
void deinit_station(station *s)
{
    // On crée une adresse MAC vide 
    mac_t newMac = {0};

    // On crée une adresse IPv4 vide
    ipv4_t newIPv4 = {0};
    
    // On applique les adresses vides à la station
    setMac(s, newMac);
    setIPv4(s, newIPv4);
}

// On copie l'adresse MAC de la station dans le buffer mac
void getMac(station *s, mac_t mac)
{
    memcpy(mac, s->adrMac, 6);
}

// On copie l'adresse IPv4 de la station dans le buffer ipv4
void getIPv4(station *s, ipv4_t ipv4)
{
    memcpy(ipv4, s->adrIPv4, 4);
}

// On copie newMac dans l'adresse MAC de la station
void setMac(station *s, mac_t newMac)
{
    memcpy(s->adrMac, newMac, 6);
}

// On copie newIPv4 dans l'adresse IPv4 de la station
void setIPv4(station *s, ipv4_t newIPv4)
{
    memcpy(s->adrIPv4, newIPv4, 4);
}

// On affiche la liste de toutes les stations enregistrées dans le réseau
void afficher_stations_disponibles(struct reseau *r)
{
    // On définit les codes de couleur pour le terminal
    #define RESET  "\033[0m"
    #define BLANC  "\033[37m"
    #define ORANGE "\033[33m"
    #define VERT   "\033[32m"

    // On vérifie que le réseau existe
    if (r == NULL) 
    {
        printf("[Erreur] Le pointeur réseau est NULL\n");
        return;
    }

    // On vérifie que le tableau de stations existe
    if (r->stations == NULL) 
    {
        printf("[Erreur] Le tableau des stations n'est pas initialisé (NULL)\n");
        return;
    }

    // On affiche l'en-tête du tableau graphique
    printf(BLANC "╔══════════════════════════════════════════════════════════════════════════╗\n" RESET);
    printf(BLANC "║" RESET "                           "  "STATIONS DISPONIBLES"  "                           " BLANC "║\n" RESET);
    printf(BLANC "╠═══════════╦═══════════════════════════════════╦══════════════════════════╣\n" RESET);

    // On affiche les titres des colonnes
    printf(BLANC "║   " RESET ORANGE "Index" RESET BLANC "   ║            " RESET ORANGE "Adresse MAC" RESET BLANC "            ║        " RESET ORANGE "Adresse IP" RESET BLANC "        ║\n" RESET);
    printf(BLANC "╠═══════════╬═══════════════════════════════════╬══════════════════════════╣\n" RESET);

    // On parcourt toutes les stations du réseau
    for (size_t i = 0; i < r->nb_station; i++)
    {
        // On affiche l'index courant de la station
        printf(BLANC "║     " RESET "%-6zu" BLANC "║         " RESET, i);

        // On utilise la fonction commune d'affichage MAC
        printf(VERT);
        afficher_mac(r->stations[i].adrMac);
        printf(RESET BLANC "         ║        " RESET VERT);

        // On utilise un formatage fixe pour garder l'alignement parfait du tableau vertical
        char ip_str[16];
        sprintf(ip_str, "%d.%d.%d.%d", r->stations[i].adrIPv4[0], r->stations[i].adrIPv4[1], r->stations[i].adrIPv4[2], r->stations[i].adrIPv4[3]);
        printf("%-15s", ip_str);
        
        printf(RESET BLANC "   ║\n" RESET);
    }

    // On affiche la bordure inférieure du tableau
    printf(BLANC "╚═══════════╩═══════════════════════════════════╩══════════════════════════╝\n" RESET);
}