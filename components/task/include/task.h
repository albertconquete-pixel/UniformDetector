/**
 * @file task.h
 * @author albertconquete-pixel/devmael
 * @brief interface du module de taches
 * @details contient les macros et fonctions pour 
 * la gestion des taches dans le systeme embrqué 
 * de detection d'uniforme
 */

#ifndef INTERRUPTEUR_H_
#define INTERRUPTEUR_H_

#define GPIO_SENSOR_PIR GPIO_NUM_3
#define GPIO_BUZZER  GPIO_NUM_2
#define GPIO_RED_LED GPIO_NUM_14
#define GPIO_BLUE_LED GPIO_NUM_15
#define GPIO_GREEN_LED GPIO_NUM_13

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/**
 *@brief ce jetons sert d'authorisation pour que le micro controlleur
 * execute la tache du capteur
 */
extern SemaphoreHandle_t binaryTaskSensor;

/**
 *@brief ce jetons sert d'authorisation pour que le micro controlleur
 * execute la tache de la prise de photos
 */
extern SemaphoreHandle_t binaryTaskPicture;

/**
 * @brief fonction d'initialisation des broches des differents composants
 * @details les composants initialisés sont :
 * 
 *   Les capteurs:
 * 
 *      capteurs infrarouges PIR pour detection de mouvement
 * 
 *   Les actionneurs:
 * 
 *     -buzzer pour effet sonores
 * 
 *     -leds
 * 
 *   contient la configuration des broches qui gere ces composants ainsi que
 * les etats au demarrage
 * @warning Ne retourne aucune valeur, ne l'appelez pas pour une variable
 */
extern void InitComponents();


/**
 * @brief Fonction de demarrage du wifi et lancement de la connexion au point d'acces @ref initWifi()
 * @details Contient les etapes suivantes: 
 * 
 *          -initialise la memoire flash avec nvs_flash_init qui sera indispensable pour notre pile wifi
 * 
 *          -initialise et demarre le wifi via @ref initWifi()
 * 
 *          demande du status de connexion wifi au module network
 * @warning Ne retourne aucune valeur, ne l'appelez pas pour une variable
*/
extern void taskInitAndStartWifi();


/**
 * @brief interruption de la tache en cours lorsque le capteur pir capte un mouvement
 * @details fonction verifians si une tache prioritaire est libre apres avoir
 * liberer le jeton (ou semaphone binaire)
 * Si l'entier long n'a pour valeur false,(et ça sera toujour true dans ce cas precis) alors,
 * on force l'ordonnanceur a ce dirigé vers cette tache par le biais de notre entier
 * @param arg pointeur vers l'id de la tache
 */
extern void InterrupSensorPIR(void* arg);

/**
 * @brief tache declanché par le capteur capteur
 * @details Le capteur declanche une interruption,
 * comme nous venons de le voir avec @ref InterrupSensorPIR().
 * Ensuite l'interruption coupera la tache en cour pour executer celle ci(etant la PH)
 * utilise driver/gpio.h pour gerer la logique metier.
 * @param arg pointeur vers l'id de la tache
 */
extern void taskSensorPIR(void* arg);

void taskPicture(void* arg);

#endif