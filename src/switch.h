#ifndef SWITCH_H
#define SWITCH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "adresse.h"
#include "stp.h"
#include "ethernet.h"

// On définit la structure d'un message BPDU échangé entre les switchs
typedef struct bpdu
{
    // On stocke l'identifiant de la racine actuelle (son adresse MAC)
    mac_t root_id;

    // On stocke le coût du chemin entre l'émetteur et la racine
    int cout;

    // On stocke l'identifiant du switch qui émet ce BPDU
    mac_t transmitter_id;
} 
bpdu;

// On définit la taille maximale de la table de commutation
#define MAX_CAM_SIZE 100
#define MAX_RX_SIZE 10
#define MAX_TX_SIZE 10

// On définit une entrée dans la table de commutation
typedef struct {
    // On stocke l'adresse MAC de l'équipement
    mac_t adresse_mac;
    // On stocke le numéro du port derrière lequel se trouve l'équipement
    size_t numero_port;
    // On indique si cette entrée est active ou non
    bool est_active;
} 
cam_entry;


// On définit la structure d'un switch
typedef struct {
    // On stocke l'adresse MAC du switch
    mac_t adresse_mac;
    // On stocke le nombre de ports du switch
    size_t nb_ports;
    // On stocke la priorité du switch (utilisée par STP)
    uint16_t priorite;
    // On stocke la table de commutation du switch
    cam_entry switching_table[MAX_CAM_SIZE];

    bpdu rx_bpdus[MAX_RX_SIZE];

    size_t rx_nb_bpdus;

    bpdu tx_bpdus[MAX_TX_SIZE];

    size_t tx_nb_bpdus;

    mac_t root_id;

    trame_t rx_trames[MAX_RX_SIZE];
    
    size_t rx_nb_trames;

    trame_t tx_trames[MAX_TX_SIZE];
    
    size_t tx_nb_trames;
} 
switch_t;

// On déclare la fonction qui initialise un switch
void init_switch(switch_t *mon_switch, mac_t mac, size_t nb_ports, uint16_t priorite);

// On déclare la fonction qui affiche les infos générales du switch
void afficher_infos_switch(const switch_t *mon_switch);

// On déclare la fonction qui affiche la table de commutation du switch
void afficher_switching_table(const switch_t *mon_switch);

// On déclare la fonction qui ajoute une adresse MAC dans la table de commutation
void apprendre_mac(switch_t *mon_switch, mac_t mac, size_t port);

#endif