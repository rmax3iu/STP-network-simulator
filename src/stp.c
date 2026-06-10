#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "stp.h"
#include "reseau.h"
#include "switch.h"
#include "adresse.h"
#include "utils.h"
#include "ethernet.h"

etat_switch *executer_stp(reseau *r)
{
    // On alloue un tableau d'états STP, un par switch du réseau
    etat_switch *etats = malloc(r->nb_switch * sizeof(etat_switch));

    // On initialise l'état STP de chaque switch
    for(size_t i = 0; i < get_nb_switch(r); i++)
    {
        // On suppose que chaque switch est la racine au départ ((size_t)-1 signifie aucun)
        etats[i].root_port = (size_t)-1;

        // On initialise le coût vers la racine à 0
        etats[i].cout_racine = 0;

        // On récupère le switch correspondant dans le réseau
        switch_t s = get_switch(r, i);

        // On stocke temporairement le nombre de ports physiques pour l'allocation
        etats[i].nb_ports = s.nb_ports;

        // On alloue la mémoire pour les ports STP du switch
        etats[i].ports = malloc(s.nb_ports * sizeof(port_stp));

        // On initialise tous les ports à zéro
        memset(etats[i].ports, 0, s.nb_ports * sizeof(port_stp));
    }

    // On initialise les ports STP de chaque switch à partir des liens
    for(size_t i = 0; i < r->nb_switch; i++)
    {
        // On compte le numéro de port courant
        size_t numero_port = 0;

        // On parcourt tous les liens du réseau
        for(size_t j = 0; j < r->nb_liens; j++)
        {
            // On récupère le lien j
            lien l = get_lien(r, j);

            // On vérifie si ce lien concerne le switch i
            if(l.equipement_a == i || l.equipement_b == i)
            {
                // On stocke le numéro de port
                etats[i].ports[numero_port].numero_port = numero_port;

                // On stocke l'indice du voisin de l'autre côté du lien
                if(l.equipement_a == i)
                {
                    etats[i].ports[numero_port].voisin = l.equipement_b;
                }
                else
                {
                    etats[i].ports[numero_port].voisin = l.equipement_a;
                }

                // MODIFICATION PROF : On met le port DESIGNE par défaut au départ
                etats[i].ports[numero_port].etat = DESIGNE;

                // On passe au port suivant
                numero_port++;
            }
        }

        // On met à jour le nombre de ports réellement connectés
        etats[i].nb_ports = numero_port;
    }

    // On retourne le tableau d'états STP
    return etats;
}

// On libère les ports de chaque switch puis le tableau principal
void liberer_stp(reseau *r, etat_switch *etats)
{
    for(size_t i = 0; i < r->nb_switch; i++)
    {
        free(etats[i].ports);
        etats[i].ports = NULL;
    }
    free(etats);
}

void afficher_etat_stp(reseau *r, etat_switch *etats)
{
    // On définit les codes couleurs
    #define RESET   "\033[0m"
    #define JAUNE   "\033[33m"
    #define CYAN    "\033[36m"
    #define VERT    "\033[32m"
    #define ROUGE   "\033[31m"
    #define VIOLET  "\033[35m"
    #define BLEU    "\033[34m"
    #define BLANC   "\033[37m"

    // On parcourt tous les switchs du réseau
    for(size_t i = 0; i < r->nb_switch; i++)
    {
        // On affiche le titre du switch avec le style tableau
        printf(BLANC "╔══════════════════════════════════╗\n" RESET);
        printf(BLANC "║          " VIOLET "Switch n°%-2zu" BLANC "             ║\n" RESET, i);
        printf(BLANC "╠═══════════╦═══════════╦══════════╣\n" RESET);

        // On affiche les en-têtes des colonnes
        printf(BLANC "║   " BLEU "Port" BLANC "    ║  " BLEU "Voisin" BLANC "   ║   " BLEU "État" BLANC "   ║\n" RESET);
        printf(BLANC "╠═══════════╬═══════════╬══════════╣\n" RESET);

        // On parcourt tous les ports du switch
        for(size_t p = 0; p < etats[i].nb_ports; p++)
        {
            // On affiche le numéro de port et le voisin avec le bon format %zu
            printf(BLANC "║" RESET "  %4zu     " BLANC "║" RESET "  %4zu     " BLANC "║ " RESET, etats[i].ports[p].numero_port, etats[i].ports[p].voisin);

            // MODIFICATION PROF : On affiche "Racine" à la place d'"Actif"
            if(etats[i].ports[p].etat == RACINE)
            {
                printf(VERT "  Racine " RESET BLANC "║\n" RESET);
            }
            else if(etats[i].ports[p].etat == DESIGNE)
            {
                printf(JAUNE " Désigné" RESET BLANC " ║\n" RESET);
            }
            else
            {
                printf(ROUGE " Bloqué  " RESET BLANC "║\n" RESET);
            }
        }

        // On ferme le tableau
        printf(BLANC "╚═══════════╩═══════════╩══════════╝\n" RESET);
    }
}

void ordonnanceur_global(reseau *mon_reseau, etat_switch *etats, int stp_actif)
{
    int tick = 0;
    int silence_counter = 0;

    // On affiche si STP est actif ou non au démarrage
    if(stp_actif)
    {
        printf("\n\033[1;32mDémarrage du moteur de simulation (STP activé)...\033[0m\n\n");
    }
    else
    {
        printf("\n\033[1;31mDémarrage du moteur de simulation (STP désactivé)...\033[0m\n\n");
    }

    // On boucle tant que le réseau n'est pas convergé
    while(true)
    {
        bool a_bouge = false;

        // On parcourt tous les switchs pour générer les BPDU du root bridge
        // Si STP est désactivé on saute cette phase
        if(stp_actif)
        {
            for(size_t i = 0; i < mon_reseau->nb_switch; i++)
            {
                // Si ce switch est le root bridge, il envoie un BPDU
                if(comparer_mac(mon_reseau->switchs[i].adresse_mac, mon_reseau->switchs[i].root_id) == 0)
                {
                    bpdu mon_bpdu;
                    memcpy(mon_bpdu.root_id, mon_reseau->switchs[i].root_id, 6);
                    memcpy(mon_bpdu.transmitter_id, mon_reseau->switchs[i].adresse_mac, 6);
                    mon_bpdu.cout = etats[i].cout_racine;

                    if(mon_reseau->switchs[i].tx_nb_bpdus < MAX_TX_SIZE)
                    {
                        mon_reseau->switchs[i].tx_bpdus[mon_reseau->switchs[i].tx_nb_bpdus++] = mon_bpdu;
                    }
                }
            }
        }

        // On parcourt tous les switchs pour transmettre les messages
        for(size_t i = 0; i < mon_reseau->nb_switch; i++)
        {
            // On envoie les BPDU en attente vers les voisins
            for(size_t j = 0; j < mon_reseau->switchs[i].tx_nb_bpdus; j++)
            {
                bpdu msg = mon_reseau->switchs[i].tx_bpdus[j];

                for(size_t p = 0; p < etats[i].nb_ports; p++)
                {
                    if(etats[i].ports[p].etat == RACINE || etats[i].ports[p].etat == DESIGNE)
                    {
                        size_t voisin = etats[i].ports[p].voisin;

                        if(voisin < mon_reseau->nb_switch)
                        {
                            size_t nb = mon_reseau->switchs[voisin].rx_nb_bpdus;

                            if(nb < MAX_RX_SIZE)
                            {
                                mon_reseau->switchs[voisin].rx_bpdus[nb] = msg;
                                mon_reseau->switchs[voisin].rx_nb_bpdus++;
                            }
                        }
                    }
                }
            }
            mon_reseau->switchs[i].tx_nb_bpdus = 0;

            // On envoie les trames Ethernet en attente
            for(size_t j = 0; j < mon_reseau->switchs[i].tx_nb_trames; j++)
            {
                trame_t trame = mon_reseau->switchs[i].tx_trames[j];
                size_t port_cible = (size_t)-1;
                size_t port_source = (size_t)-1;

                // On recherche dans la table de commutation le port vers la destination
                for(size_t k = 0; k < MAX_CAM_SIZE; k++)
                {
                    if(mon_reseau->switchs[i].switching_table[k].est_active)
                    {
                        if(comparer_mac(mon_reseau->switchs[i].switching_table[k].adresse_mac, trame.dest) == 0)
                        {
                            port_cible = mon_reseau->switchs[i].switching_table[k].numero_port;
                        }

                        if(comparer_mac(mon_reseau->switchs[i].switching_table[k].adresse_mac, trame.src) == 0)
                        {
                            port_source = mon_reseau->switchs[i].switching_table[k].numero_port;
                        }
                    }
                }

                // Si on connaît le port de destination, on fait du transfert unicast
                if(port_cible != (size_t)-1)
                {
                    size_t voisin = etats[i].ports[port_cible].voisin;

                    if(voisin < mon_reseau->nb_switch)
                    {
                        // On vérifie que le port d'entrée chez le voisin n'est pas bloqué par STP
                        size_t p_entree = get_port_index_to_neighbor(etats, voisin, i);

                        if(p_entree != (size_t)-1 && etats[voisin].ports[p_entree].etat == BLOQUE)
                        {
                            printf("  \033[31m[STP]\033[0m Le Switch %zu détruit la trame (Port Bloqué)\n", voisin);
                        }
                        else
                        {
                            size_t nb = mon_reseau->switchs[voisin].rx_nb_trames;

                            if(nb < MAX_RX_SIZE)
                            {
                                mon_reseau->switchs[voisin].rx_trames[nb] = trame;
                                mon_reseau->switchs[voisin].rx_nb_trames++;
                                apprendre_mac(&mon_reseau->switchs[voisin], trame.src, p_entree);
                                printf("  \033[36m[Trame]\033[0m Switch %zu transfère au Switch %zu (Unicast)\n", i, voisin);
                                a_bouge = true;
                            }
                        }
                    }
                    else if(voisin >= mon_reseau->nb_switch)
                    {
                        printf("  \033[32m[Succès]\033[0m Trame arrivée à la station %zu !\n", voisin - mon_reseau->nb_switch);
                        a_bouge = true;
                    }
                }
                else
                {
                    // Si la destination est inconnue, on inonde tous les ports ouverts
                    printf("  \033[33m[Trame]\033[0m Switch %zu inonde (Broadcast) la trame\n", i);
                    a_bouge = true;

                    for(size_t p = 0; p < etats[i].nb_ports; p++)
                    {
                        // On ne renvoie pas la trame sur le port source
                        if(etats[i].ports[p].numero_port == port_source)
                        {
                            continue;
                        }

                        if(etats[i].ports[p].etat == RACINE || etats[i].ports[p].etat == DESIGNE)
                        {
                            size_t voisin = etats[i].ports[p].voisin;

                            if(voisin < mon_reseau->nb_switch)
                            {
                                // On vérifie que le port d'entrée chez le voisin n'est pas bloqué
                                size_t p_entree = get_port_index_to_neighbor(etats, voisin, i);

                                if(p_entree != (size_t)-1 && etats[voisin].ports[p_entree].etat == BLOQUE)
                                {
                                    // On ignore les ports bloqués par STP
                                    continue;
                                }

                                size_t nb = mon_reseau->switchs[voisin].rx_nb_trames;

                                if(nb < MAX_RX_SIZE)
                                {
                                    mon_reseau->switchs[voisin].rx_trames[nb] = trame;
                                    mon_reseau->switchs[voisin].rx_nb_trames++;
                                    apprendre_mac(&mon_reseau->switchs[voisin], trame.src, p_entree);
                                }
                            }
                            else if(voisin >= mon_reseau->nb_switch)
                            {
                                station st = get_station(mon_reseau, voisin - mon_reseau->nb_switch);

                                // Si c'est la bonne station destinataire, on l'indique
                                if(comparer_mac(st.adrMac, trame.dest) == 0)
                                {
                                    printf("  \033[32m[Succès]\033[0m Trame arrivée à la station %zu !\n", voisin - mon_reseau->nb_switch);
                                }
                            }
                        }
                    }
                }
            }
            mon_reseau->switchs[i].tx_nb_trames = 0;
        }

        // On traite les messages reçus et on met à jour les états
        for(size_t i = 0; i < mon_reseau->nb_switch; i++)
        {
            // On traite les BPDU reçus
            for(size_t j = 0; j < mon_reseau->switchs[i].rx_nb_bpdus; j++)
            {
                bpdu bpdu_recu = mon_reseau->switchs[i].rx_bpdus[j];
                size_t voisin_idx = get_switch_index_by_mac(mon_reseau, bpdu_recu.transmitter_id);

                if(voisin_idx != (size_t)-1)
                {
                    int cout_lien = get_cout_lien(mon_reseau, i, voisin_idx);

                    // On vérifie que le lien existe bien pour éviter le coût -1 fantôme
                    if(cout_lien != -1)
                    {
                        // On ne fait le calcul que si le lien existe vraiment
                        int cout_potentiel = bpdu_recu.cout + cout_lien;
                        int cmp_racine = comparer_mac(bpdu_recu.root_id, mon_reseau->switchs[i].root_id);

                        // Si le BPDU annonce une meilleure racine ou un meilleur chemin, on met à jour
                        if(cmp_racine < 0 || (cmp_racine == 0 && cout_potentiel < etats[i].cout_racine))
                        {
                            memcpy(mon_reseau->switchs[i].root_id, bpdu_recu.root_id, 6);
                            etats[i].cout_racine = cout_potentiel;
                            etats[i].root_port = get_port_index_to_neighbor(etats, i, voisin_idx);

                            if(mon_reseau->switchs[i].tx_nb_bpdus < MAX_TX_SIZE)
                            {
                                memcpy(bpdu_recu.transmitter_id, mon_reseau->switchs[i].adresse_mac, 6);
                                bpdu_recu.cout = etats[i].cout_racine;
                                mon_reseau->switchs[i].tx_bpdus[mon_reseau->switchs[i].tx_nb_bpdus++] = bpdu_recu;
                            }

                            printf("  \033[35m[STP]\033[0m Switch %zu met à jour sa racine (via voisin %zu)\n", i, voisin_idx);
                            a_bouge = true;
                        }
                    }
                }
            }
            mon_reseau->switchs[i].rx_nb_bpdus = 0;

            // On traite les trames reçues
            for(size_t j = 0; j < mon_reseau->switchs[i].rx_nb_trames; j++)
            {
                // Une trame reçue au tick T est mise dans le TX pour être routée au tick T+1
                if(mon_reseau->switchs[i].tx_nb_trames < MAX_TX_SIZE)
                {
                    mon_reseau->switchs[i].tx_trames[mon_reseau->switchs[i].tx_nb_trames++] = mon_reseau->switchs[i].rx_trames[j];
                }
            }
            mon_reseau->switchs[i].rx_nb_trames = 0;
        }

        // On applique les nouveaux états des ports en tenant compte de stp_actif
        appliquer_etats_ports(mon_reseau, etats, stp_actif);

        // On affiche la fin du tick si quelque chose s'est passé
        if(a_bouge)
        {
            printf("\n--- Fin du Tick %d ---\n\n", tick);
            silence_counter = 0;
        }
        else
        {
            // On affiche un point pour montrer que le temps passe
            printf(".");
            fflush(stdout);
            silence_counter++;
        }

        sleep(1);
        tick++;

        // Si 3 ticks sans activité, le réseau est convergé
        if(silence_counter >= 3)
        {
            printf("\n\n\033[1;32m✅ Le réseau est stabilisé (Plus aucun message). Retour au menu.\033[0m\n");
            break;
        }

        // On arrête après 50 ticks quoi qu'il arrive
        if(tick >= 50)
        {
            printf("\n\nFin de la simulation (50 ticks atteints).\n");
            break;
        }
    }
}

void appliquer_etats_ports(reseau *r, etat_switch *etats, int stp_actif)
{
    for(size_t i = 0; i < r->nb_switch; i++)
    {
        for(size_t p = 0; p < etats[i].nb_ports; p++)
        {
            // Si STP est désactivé, tous les ports sont désignés
            if(!stp_actif)
            {
                etats[i].ports[p].etat = DESIGNE;
                continue;
            }

            size_t voisin = etats[i].ports[p].voisin;

            // MODIFICATION PROF : Le port racine prend l'état RACINE
            if(etats[i].ports[p].numero_port == etats[i].root_port)
            {
                etats[i].ports[p].etat = RACINE;

                // On passe au port suivant
                continue;
            }

            // Si le voisin est une station, le port est désigné
            if(voisin >= r->nb_switch)
            {
                etats[i].ports[p].etat = DESIGNE;
                continue;
            }

            // On cherche qui est le port désigné sur ce lien
            if(voisin < r->nb_switch)
            {
                if(etats[i].cout_racine < etats[voisin].cout_racine)
                {
                    // J'ai un meilleur coût que mon voisin, je suis le port désigné
                    etats[i].ports[p].etat = DESIGNE;
                }
                else if(etats[i].cout_racine > etats[voisin].cout_racine)
                {
                    // Mon voisin est meilleur, je bloque mon port pour éviter la boucle
                    etats[i].ports[p].etat = BLOQUE;
                }
                else
                {
                    // En cas d'égalité des coûts, c'est le switch avec la plus petite adresse MAC qui gagne
                    if(comparer_mac(r->switchs[i].adresse_mac, r->switchs[voisin].adresse_mac) < 0)
                    {
                        etats[i].ports[p].etat = DESIGNE;
                    }
                    else
                    {
                        etats[i].ports[p].etat = BLOQUE;
                    }
                }
            }
        }
    }
}