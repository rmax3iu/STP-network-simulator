#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "reseau.h"
#include "station.h"
#include "switch.h"
#include "adresse.h"

// On lit le fichier de configuration pour charger le réseau en mémoire
reseau lire_config(const char *nom_fichier)
{
    // On initialise un réseau vide
    reseau r = init_reseau();

    // On ouvre le fichier en lecture
    FILE *fichier = fopen(nom_fichier, "r");

    // On vérifie que le fichier s'est bien ouvert
    if(fichier == NULL)
    {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    // On déclare deux variables pour stocker le nombre d'équipement et le nombre de lien
    size_t nb_equipements;
    size_t nb_liens;

    // On utilise la fonction fscanf pour lire le nombre d'équipements et de liens
    fscanf(fichier, "%zu %zu", &nb_equipements, &nb_liens);

    // On parcourt tous les équipements
    for(size_t i = 0; i < nb_equipements; i++)
    {
        // On lit le type de l'équipement
        int type;
        fscanf(fichier, "%d;", &type);

        // Si c'est une station
        if(type == 1)
        {
            // On lit l'adresse MAC octet par octet en hexadécimal
            mac_t mac;
            fscanf(fichier, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
           
            // On lit l'adresse IPv4 octet par octet en décimal
            ipv4_t ip;
            fscanf(fichier, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
            
            // On crée une station vide puis on lui assigne la MAC et l'IP
            station s = init_station();
            setMac(&s, mac);
            setIPv4(&s, ip);

            // On ajoute la station au réseau
            add_station(&r, s);
        }

        // Si c'est un switch 
        if(type == 2)
        {
            // On lit l'adresse MAC octet par octet en hexadécimal
            mac_t mac;
            fscanf(fichier, " %hhx:%hhx:%hhx:%hhx:%hhx:%hhx;", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
            
            // On lit le nombre de ports et la priorité
            size_t nb_ports;
            uint16_t priorite;
            fscanf(fichier, " %zu;%hu", &nb_ports, &priorite);

            // On crée un switch avec la MAC, le nombre de ports et la priorité
            switch_t s;
            init_switch(&s, mac, nb_ports, priorite);

            // On ajoute le switch au réseau
            add_switch(&r, s);
        }
    }

    // On parcourt tous les liens
    for(size_t i = 0; i < nb_liens; i++)
    {
        // On lit les indices des deux équipements et le coût du lien
        size_t indice_a;
        size_t indice_b;
        int cout;
        fscanf(fichier, " %zu;%zu;%d", &indice_a, &indice_b, &cout);
        
        // On crée un lien et on remplit ses champs
        lien l;
        l.equipement_a = indice_a;
        l.equipement_b = indice_b;
        l.cout = cout;

        // On ajoute le lien au réseau
        add_lien(&r, l);
    }

    // On ferme le fichier
    fclose(fichier);

    // On retourne le réseau rempli
    return r;
}