#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reseau.h"
#include "station.h"
#include "switch.h"
#include "utils.h"

// On initialise un réseau avec une capacité initiale de 10 stations, 10 switchs et 10 liens
reseau init_reseau()
{
    reseau r;

    // On alloue la mémoire pour 10 stations
    r.stations = malloc(10 * sizeof(station));

    // On alloue la mémoire pour 10 switchs
    r.switchs = malloc(10 * sizeof(switch_t));

    // On alloue la mémoire pour 10 liens
    r.liens = malloc(10 * sizeof(lien));

    // On initialise les compteurs à 0
    r.nb_station = 0;
    r.nb_switch = 0;
    r.nb_liens = 0;

    // On initialise les capacités à 10
    r.stations_capacite = 10;
    r.switchs_capacite = 10;
    r.liens_capacite = 10;

    // On retourne le réseau initialisé
    return r;
}

// On libère la mémoire allouée pour le réseau
void deinit_reseau(reseau *r)
{
    // On libère la mémoire des stations si elle a été allouée
    if(r->stations != NULL)
    {
        free(r->stations);
        r->stations = NULL;
    }

    // On libère la mémoire des switchs si elle a été allouée
    if(r->switchs != NULL)
    {
        free(r->switchs);
        r->switchs = NULL;
    }

    // On libère la mémoire des liens si elle a été allouée
    if(r->liens != NULL)
    {
        free(r->liens);
        r->liens = NULL;
    }

    // On remet tous les compteurs et capacités à 0
    r->nb_station = 0;
    r->nb_switch = 0;
    r->nb_liens = 0;
    r->stations_capacite = 0;
    r->switchs_capacite = 0;
    r->liens_capacite = 0;
}

// On ajoute une station au réseau
void add_station(reseau *r, station s)
{
    // On vérifie si le tableau est plein
    if(r->nb_station == r->stations_capacite)
    {
        // On double la capacité avec realloc en passant par un pointeur temporaire
        station *tmp = realloc(r->stations, r->stations_capacite * 2 * sizeof(station));

        // On vérifie que le realloc a réussi
        if(tmp == NULL)
        {
            printf("Erreur : échec de realloc dans add_station.\n");
            exit(EXIT_FAILURE);
        }

        r->stations = tmp;
        r->stations_capacite = r->stations_capacite * 2;
    }

    // On ajoute la station à la fin du tableau
    r->stations[r->nb_station] = s;

    // On incrémente le compteur
    r->nb_station += 1;
}

// On ajoute un switch au réseau
void add_switch(reseau *r, switch_t s)
{
    // On vérifie si le tableau est plein
    if(r->nb_switch == r->switchs_capacite)
    {
        // On double la capacité avec realloc en passant par un pointeur temporaire
        switch_t *tmp = realloc(r->switchs, r->switchs_capacite * 2 * sizeof(switch_t));

        // On vérifie que le realloc a réussi
        if(tmp == NULL)
        {
            printf("Erreur : échec de realloc dans add_switch.\n");
            exit(EXIT_FAILURE);
        }

        r->switchs = tmp;
        r->switchs_capacite = r->switchs_capacite * 2;
    }

    // On ajoute le switch à la fin du tableau
    r->switchs[r->nb_switch] = s;

    // On incrémente le compteur
    r->nb_switch += 1;
}

// On ajoute un lien au réseau
void add_lien(reseau *r, lien l)
{
    // On vérifie si le tableau est plein
    if(r->nb_liens == r->liens_capacite)
    {
        // On double la capacité avec realloc en passant par un pointeur temporaire
        lien *tmp = realloc(r->liens, r->liens_capacite * 2 * sizeof(lien));

        // On vérifie que le realloc a réussi
        if(tmp == NULL)
        {
            printf("Erreur : échec de realloc dans add_lien.\n");
            exit(EXIT_FAILURE);
        }

        r->liens = tmp;
        r->liens_capacite = r->liens_capacite * 2;
    }

    // On ajoute le lien à la fin du tableau
    r->liens[r->nb_liens] = l;

    // On incrémente le compteur
    r->nb_liens += 1;
}

// On retourne la station à l'indice i
station get_station(reseau *r, size_t i)
{
    return r->stations[i];
}

// On retourne le switch à l'indice i
switch_t get_switch(reseau *r, size_t i)
{
    return r->switchs[i];
}

// On retourne le lien à l'indice i
lien get_lien(reseau *r, size_t i)
{
    return r->liens[i];
}

// On retourne le nombre de stations dans le réseau
size_t get_nb_station(reseau *r)
{
    return r->nb_station;
}

// On retourne le nombre de switchs dans le réseau
size_t get_nb_switch(reseau *r)
{
    return r->nb_switch;
}

// On retourne le nombre de liens dans le réseau
size_t get_nb_liens(reseau *r)
{
    return r->nb_liens;
}

// On incrémente et retourne le nombre de stations
size_t add_nb_station(reseau *r)
{
    r->nb_station += 1;
    return r->nb_station;
}

// On incrémente et retourne le nombre de switchs
size_t add_nb_switch(reseau *r)
{
    r->nb_switch += 1;
    return r->nb_switch;
}

// On retourne la capacité maximale actuelle du tableau de stations
size_t get_stations_capacite(reseau *r)
{
    return r->stations_capacite;
}

// On retourne la capacité maximale actuelle du tableau de switchs
size_t get_switch_capacite(reseau *r)
{
    return r->switchs_capacite;
}

// On retourne la capacité maximale actuelle du tableau de liens
size_t get_liens_capacite(reseau *r)
{
    return r->liens_capacite;
}

// On cherche et retourne le coût d'un lien entre deux switchs
int get_cout_lien(reseau *r, size_t switch1, size_t switch2)
{
    // On parcourt tous les liens du réseau
    for(size_t i = 0; i < r->nb_liens; i++)
    {
        lien l = r->liens[i];

        // Si le lien correspond aux deux switchs dans un sens ou dans l'autre
        if((l.equipement_a == switch1 && l.equipement_b == switch2) || (l.equipement_a == switch2 && l.equipement_b == switch1))
        {
            return l.cout;
        }
    }
    // On retourne -1 si aucun lien n'a été trouvé
    return -1; 
}

// On trouve l'index d'un switch dans le réseau grâce à son adresse MAC
size_t get_switch_index_by_mac(reseau *r, mac_t mac)
{
    // Si le pointeur du réseau est nul, on retourne la sentinelle d'erreur
    if (r == NULL) return (size_t)-1;

    // On parcourt tous les switchs du réseau
    for(size_t i = 0; i < r->nb_switch; i++)
    {
        // Si l'adresse MAC correspond au switch actuel
        if(comparer_mac(r->switchs[i].adresse_mac, mac) == 0)
        {
            return i;
        }
    }
    // On retourne la sentinelle d'erreur si le switch n'est pas trouvé
    return (size_t)-1; 
}

// On trouve le numéro de port sur un switch connecté à un voisin donné
size_t get_port_index_to_neighbor(etat_switch *etats, size_t switch_i, size_t neighbor_id)
{
    // On parcourt tous les ports du switch actuel
    for(size_t p = 0; p < etats[switch_i].nb_ports; p++)
    {
        // Si le voisin sur ce port correspond à l'identifiant recherché
        if(etats[switch_i].ports[p].voisin == neighbor_id)
        {
            return etats[switch_i].ports[p].numero_port;
        }
    }
    // On retourne la sentinelle d'erreur si aucun port ne mène à ce voisin
    return (size_t)-1;
}