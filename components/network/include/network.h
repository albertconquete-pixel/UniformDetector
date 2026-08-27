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
#define ESP_WIFI_SSDI "TECNO SPARK 40"
#define ESP_WIFI_PASSWORD "yxrgfi3rk9nr3mt"

#define WIFI_FAILS_CONNECTED_BITS 1 << 1
#define WIFI_CONNECTED_BITS 1 << 0

#define SERVEUR_IP "10.0.2.2"
#define PORT 3333
#define INVALID_SOCKET -1

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

/**
 * @brief fonction s'occupant du transfer de l'image vers l'application
 * @details cette fonction depend du protocol de communication HTTP.
 * elle se deroule de la façon suivante :
 * 
 * -Initiation du client ,
 * -configuration du type de post (header)
 * -requete d'envoie de la donné
 * -execution de la requete
 * -verification du message du serveur
 * -netoyage du client.
 * @warning fonction a sécurisé avec https. 
 */
void httpsTransfer(uint8_t* fb, int len);

#endif