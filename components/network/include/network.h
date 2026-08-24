/**
 * @file network.h
 * @author albertconquete-pixel/devmael
 * @brief interface du module de taches
 * @details continent les differents macros et fonctions pour la 
 * gestion des la connexion a un point d'accés, la gestion des sockets et l'envoie des 
 * données.
 */


#ifndef NETWORK_H_
#define NETWORK_H_

#include "esp_event.h"
#define ESP_WIFI_RETRY 2
#define ESP_WIFI_SSDI "Wokfhthj"
#define ESP_WIFI_PASSWORD ""

#define WIFI_FAILS_CONNECTED_BITS 1 << 1
#define WIFI_CONNECTED_BITS 1 << 0

extern EventGroupHandle_t wifiEventBits;

/**
 * @brief initialise et demarre la connexion wifi
 * @details Contien les etapes suivantes :
 *  
 *      - Initiation de la pile TCP/ip
 * 
 *      - Initialise la pile wifi par defaut
 * 
 *      - Associe la boucle d'evenement a notre pile
 * 
 *      -configure notre esp en mode station
 * 
 *      -demmare le wifi
 * 
 *      -retourne le status de connexion
 * @warning retour de fonction blocquante
 */
extern EventBits_t initWifi();

#endif