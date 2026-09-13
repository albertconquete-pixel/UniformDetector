
/**
 * @file picture_cam.h
 * @author albertconquete-pixel/devmael
 * @brief interface du module de capture photos
 * @details contient les macros et fonctions pour 
 * la capture photos de notre esp_cam
 * 
 * Il contient tout nos différents configuration au niveau de
 * la camera et des broches, facilitant la modifications ou l'ajout pour 
 * des raisons de compatibilités
 */

#include "esp_camera.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#ifndef PICTURE_CAM_
#define PICTURE_CAM_


#define CAM_PIN_PWDN -1 /** Active/desactive l'alimentation de la camera */
#define CAM_PIN_RESET -1  /** broche de reintiation materielle, aucune broche n'est dédié */
#define CAM_PIN_SIOD 40  /** ligne de données*/
#define CAM_PIN_SIOC 39 /** ligne d'horloge */
#define CAM_PIN_XCLK  10

#define CAM_PIN_D7      48 /** bus de données de l'image (le plus fort)*/
#define CAM_PIN_D6      11
#define CAM_PIN_D5      12
#define CAM_PIN_D4      14
#define CAM_PIN_D3      16
#define CAM_PIN_D2      18
#define CAM_PIN_D1      17
#define CAM_PIN_D0      15 /** bus de données de l'image (le plus faible)*/
#define CAM_PIN_VSYNC   38
#define CAM_PIN_HREF    47
#define CAM_PIN_PCLK    13

#define CONFIG_XCLK_FREQ 10000000 
#define CONFIG_OV2640_SUPPORT 1
#define CONFIG_OV7725_SUPPORT 1
#define CONFIG_OV3660_SUPPORT 1
#define CONFIG_OV5640_SUPPORT 1

/**
 * Servant de tampon securisé des different donné, sans conflit de tache
 */
extern QueueHandle_t queueImageBuffer;

/**
 * @brief initiation de la caméra 
 * @details contient le code d'initiation de la camera, c'est a dire les 
 * la structure d'initiatrion ainsi que la configuration.
 * @return retourne ESP_OK si tout va bien
 */
extern esp_err_t initCamera();

/**
 * @brief implementation de la fonction de capture photos
 * @details contient la fonction de capture d'image, ça gestion d'erreur et
 * ainsi que le retour du buffer d'image
 * @return retourne un buffer d'image: un tableau d'entier 8bits
 */
extern camera_fb_t* takePicture();

/**
 * fonction permettant de liberer la framme d'image de la camera
 */
extern void (*freeBuffer)(camera_fb_t*);


#endif
