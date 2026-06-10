#ifndef CONFIG_H
#define CONFIG_H

#include "reseau.h"

// On déclare la fonction de configuration pour charger le réseau en mémoire
reseau lire_config(const char *nom_fichier);

#endif