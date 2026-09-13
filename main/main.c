
/**
 * @file main.c 
 * @author albertconquete-pixel/devmael
 * @brief point d'entrée du firware et initialisation generale
 * @details contient le app main dans lequelle ont retrouve:
 * 
 *             -La creations des differents taches de notre systeme embarqué pour les
 * differentes fonctionnalités;
 * 
 *             -La boucle de notre programme, representant l'etat au repos lorsque aucune tache n'est presente
 * 
 * ce fichier appelle @ref task.h pour l'initiation , 
 * la configuration et le demarrage des taches de notre systemes
 * @version v0.1.1
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "task.h"


void app_main(void)
{
   uploadImageSecu();
   printf("commencons \n");
   InitComponents();
   binaryTaskSensor = xSemaphoreCreateBinary();
   binaryTaskPicture = xSemaphoreCreateBinary();
   if (binaryTaskSensor || binaryTaskPicture) 
   {
      /*creer l'interruption*/ 
      gpio_install_isr_service(0);
      gpio_isr_handler_add(GPIO_SENSOR_PIR,InterrupSensorPIR,(void*)GPIO_SENSOR_PIR);

      /*creer la tache*/
      xTaskCreatePinnedToCore(taskSensorPIR,"taskSensorPIR",2048,NULL,16,NULL,0);
      xTaskCreatePinnedToCore(taskPicture,"taskPicture",8192,NULL,16,NULL,1);
      xTaskCreatePinnedToCore(sendImageHttp,"sendImageHttp",6144,NULL,10,NULL,0);
      printf("tache creé \n");

      /*initialiser le wifi*/
      taskInitAndStartWifi();
      printf("fin de configuration \n");
      /*demarage du programe*/
      gpio_set_level(GPIO_BLUE_LED,1);
   }

   while(1){
         vTaskDelay(pdMS_TO_TICKS(100)); 
         printf("etas du capteur %d \n",gpio_get_level(GPIO_SENSOR_PIR));
   }
     
   
}
