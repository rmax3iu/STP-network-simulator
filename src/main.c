#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "reseau.h"
#include "stp.h"
#include "matrice.h"
#include "ethernet.h"

int main()
{
    // On affiche le titre du programme
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                             Simulateur de réseau avec STP                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════════════╝\n");

    // On demande le nom du fichier de configuration à l'utilisateur
    char nom_fichier[256];
    printf("Veuillez saisir le nom du fichier : ");
    scanf("%255s", nom_fichier);
    printf("\n");

    // On charge le réseau depuis le fichier de configuration
    reseau r = lire_config(nom_fichier);

    // On exécute l'algorithme STP sur le réseau chargé
    etat_switch *etats = executer_stp(&r);

    // On active le STP par défaut
    int stp_actif = 1;

    int choix;
    do
    {
        // On affiche le menu principal avec l'état actuel du STP
        printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                                          Menu                                         ║\n");
        printf("╚═══════════════════════════════════════════════════════════════════════════════════════╝\n");
        printf("\n");
        printf("   1. Lancer la simulation d'une trame\n");
        printf("   2. Afficher l'état des ports STP\n");
        printf("   3. Afficher la matrice d'adjacence\n");
        printf("   4. Afficher les tables MAC\n");
        printf("   5. Quitter\n");

        // On affiche l'option 6 avec l'état actuel du STP
        if(stp_actif)
        {
            printf("   6. Désactiver le STP (actuellement : \033[1;32mON\033[0m)\n");
        }
        else
        {
            printf("   6. Activer le STP (actuellement : \033[1;31mOFF\033[0m)\n");
        }

        printf("\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch(choix)
        {
            case 1:
                // On fait un saut de ligne
                printf("\n");

                // On affiche le titre de la simulation
                printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
                printf("║                            Simulation d'une trame Ethernet                            ║\n");
                printf("╚═══════════════════════════════════════════════════════════════════════════════════════╝\n");
                printf("\n");

                // On affiche les stations disponibles pour que l'utilisateur choisisse
                afficher_stations_disponibles(&r);
                printf("\n");

                // On demande les indices de la station source et destination
                int idx_src;
                int idx_dest;

                printf("Veuillez saisir l'index de la station source : ");
                scanf("%d", &idx_src);

                printf("Veuillez saisir l'index de la station destination : ");
                scanf("%d", &idx_dest);

                char message[1500];
                printf("Veuillez saisir le message à envoyer : ");
                scanf(" %[^\n]", message);

                // On récupère les stations source et destination depuis le réseau
                station src = get_station(&r, idx_src);
                station dst = get_station(&r, idx_dest);

                // On crée la trame Ethernet avec les MACs source et destination
                trame_t trame;
                memcpy(trame.src, src.adrMac, 6);
                memcpy(trame.dest, dst.adrMac, 6);

                // On définit le type IPv4
                trame.type = 0x0800;

                // On calcule la taille réelle du message saisi
                trame.taille_data = strlen(message);

                // On copie le contenu du message dans les données de la trame
                memcpy(trame.data, message, trame.taille_data);

                // On initialise le fcs à zéro
                trame.fcs = 0;

                // On initialise le préambule et le SFD
                memset(trame.preambule, 0xAA, 7);
                trame.sfd = 0xAB;

                // On affiche la trame créée
                afficher_trame(&trame);

                // 1. On cherche à quel switch la station source est branchée
                size_t switch_actuel = (size_t)-1;
                size_t indice_src_global = r.nb_switch + idx_src;

                for(size_t i = 0; i < r.nb_liens; i++)
                {
                    lien l = get_lien(&r, i);
                    if(l.equipement_a == indice_src_global && l.equipement_b < r.nb_switch)
                    {
                        switch_actuel = l.equipement_b;
                    }
                    else if(l.equipement_b == indice_src_global && l.equipement_a < r.nb_switch)
                    {
                        switch_actuel = l.equipement_a;
                    }
                }

                // 2. On met la trame dans la file d'envoi de ce switch
                if(switch_actuel != (size_t)-1)
                {
                    printf("Trame injectée dans le Switch %zu. Lancement du temps...\n", switch_actuel);
                    r.switchs[switch_actuel].tx_trames[r.switchs[switch_actuel].tx_nb_trames++] = trame;

                    // On apprend direct la MAC de la station source sur le port d'entrée
                    size_t p_entree = get_port_index_to_neighbor(etats, switch_actuel, indice_src_global);
                    apprendre_mac(&r.switchs[switch_actuel], trame.src, p_entree);
                }

                // 3. ON LANCE LA MACHINE (Boucle infinie)
                ordonnanceur_global(&r, etats, stp_actif);

                printf("\n");
                break;

            case 2:
                // On affiche l'état STP de chaque switch
                printf("\n");
                afficher_etat_stp(&r, etats);
                printf("\n");
                break;

            case 3:
                afficher_matrice_adjacence(&r);
                printf("\n");
                break;

            case 4:
                printf("\n");
                // On parcourt tous les switchs du réseau pour afficher leur table mise au propre
                for(size_t i = 0; i < r.nb_switch; i++)
                {
                    printf("\n🔹 Switch n°%zu : ", i);
                    afficher_switching_table(&r.switchs[i]);
                }
                break;

            case 5:
                printf("Au revoir !\n\n");
                break;

            case 6:
                // On bascule l'état du STP
                stp_actif = !stp_actif;

                if(stp_actif)
                {
                    printf("\nSTP \033[1;32mactivé\033[0m.\n\n");
                }
                else
                {
                    printf("\nSTP \033[1;31mdésactivé\033[0m.\n\n");
                }
                break;

            default:
                printf("Choix invalide !\n\n");
                break;
        }

    }
    while(choix != 5);

    // On libère le tableau d'états STP
    liberer_stp(&r, etats);

    // On libère la mémoire du réseau
    deinit_reseau(&r);

    return 0;
}