#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "adresse.h"
#include "stp.h"
#include "switch.h"

// On initialise un switch avec sa MAC, son nombre de ports et sa priorité
void init_switch(switch_t *mon_switch, mac_t mac, size_t nb_ports, uint16_t priorite)
{
    // On stocke le nombre de ports
    mon_switch->nb_ports = nb_ports;

    // On stocke la priorité STP
    mon_switch->priorite = priorite;

    // On copie l'adresse MAC dans le switch
    memcpy(mon_switch->adresse_mac, mac, 6);

    // On vide toute la table de commutation en mettant tous les octets à 0
    memset(mon_switch->switching_table, 0, MAX_CAM_SIZE * sizeof(cam_entry));

    memset(mon_switch->rx_bpdus, 0, MAX_RX_SIZE * sizeof(bpdu));
    mon_switch->rx_nb_bpdus = 0;

    memset(mon_switch->tx_bpdus, 0, MAX_TX_SIZE * sizeof(bpdu));
    mon_switch->tx_nb_bpdus = 0;
    
    // Au départ chaque switch se désigne comme racine
    memcpy(mon_switch->root_id, mon_switch->adresse_mac, 6);

    memset(mon_switch->rx_trames, 0, MAX_RX_SIZE * sizeof(trame_t));
    mon_switch->rx_nb_trames = 0;

    memset(mon_switch->tx_trames, 0, MAX_TX_SIZE * sizeof(trame_t));
    mon_switch->tx_nb_trames = 0;
}

// On affiche les informations générales du switch
void afficher_infos_switch(const switch_t *mon_switch)
{
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║         INFORMATIONS DU SWITCH         ║\n");
    printf("╠════════════════════════════════════════╣\n");

    // On affiche l'adresse MAC du switch
    printf("║ Adresse MAC     : ");
    afficher_mac(mon_switch->adresse_mac);
    printf(" ║\n");

    // On affiche le nombre de ports
    printf("║ Nombre de ports : %-20zu ║\n", mon_switch->nb_ports);

    // On affiche la priorité STP
    printf("║ Priorité STP    : %-20u ║\n", mon_switch->priorite);
    printf("╚════════════════════════════════════════╝\n\n");
}

// On affiche toutes les entrées actives de la table de commutation
void afficher_switching_table(const switch_t *mon_switch)
{
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║          TABLE DE COMMUTATION          ║\n");
    printf("╠═══════════╦════════════════════════════╣\n");
    printf("║   PORT    ║        ADRESSE MAC         ║\n");
    printf("╠═══════════╬════════════════════════════╣\n");

    // On suppose que la table est vide au départ
    bool table_vide = true;

    // On parcourt toute la table de commutation
    for (size_t i = 0; i < MAX_CAM_SIZE; i++)
    {
        // On affiche uniquement les entrées actives
        if (mon_switch->switching_table[i].est_active)
        {
            // On affiche le numéro de port et l'adresse MAC associée avec les bordures demandées
            printf("║  %4zu     ║     ", mon_switch->switching_table[i].numero_port);
            afficher_mac(mon_switch->switching_table[i].adresse_mac);
            printf("      ║\n");
            
            // On indique que la table n'est pas vide
            table_vide = false;
        }
    }
    
    // Si aucune entrée active n'a été trouvée, on affiche un message propre dans le tableau
    if (table_vide)
    {
        printf("║    --     ║    (La table est vide)     ║\n");
    }
    printf("╚═══════════╩════════════════════════════╝\n\n");
}

// On ajoute ou met à jour une adresse MAC dans la table de commutation
void apprendre_mac(switch_t *mon_switch, mac_t mac, size_t port)
{
    // On cherche si la MAC existe déjà dans la table
    for (size_t i = 0; i < MAX_CAM_SIZE; i++)
    {
        // On vérifie si l'entrée est active et si la MAC correspond
        if (mon_switch->switching_table[i].est_active && memcmp(mon_switch->switching_table[i].adresse_mac, mac, 6) == 0)
        {
            // On met à jour le port et on arrête
            mon_switch->switching_table[i].numero_port = port;
            return;
        }
    }
    // Si la MAC n'existe pas, on cherche une entrée libre
    for (size_t i = 0; i < MAX_CAM_SIZE; i++)
    {
        // On vérifie si l'entrée est libre
        if (!mon_switch->switching_table[i].est_active)
        {
            // On stocke la MAC, le port et on active l'entrée
            memcpy(mon_switch->switching_table[i].adresse_mac, mac, 6);
            mon_switch->switching_table[i].numero_port = port;
            mon_switch->switching_table[i].est_active = true;
            return;
        }
    }
}