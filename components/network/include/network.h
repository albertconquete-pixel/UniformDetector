#ifndef NETWORK_H_
#define NETWORK_H_

#include "esp_event.h"
#define ESP_WIFI_RETRY ESP_WIFI_MAX_CONN_NUM
#define ESP_WIFI_SSDI "Wokwi-GUEST"
#define ESP_WIFI_PASSWORD ""

#define WIFI_FAILS_CONNECTED_BITS 0x00
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