#include <stdio.h>
#include "task.h"
#include "network.h"
#include "picture_cam.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_camera.h"
#include "face_ai.h"


SemaphoreHandle_t binaryTaskSensor= NULL;
SemaphoreHandle_t binaryTaskPicture = NULL;
static bool iaWork=false;

void InitComponents(){
    /*init camera*/
    initCamera();

    gpio_reset_pin(GPIO_BUZZER);
    gpio_reset_pin(GPIO_SENSOR_PIR);
    queueImageBuffer = xQueueCreate(2,sizeof( camera_fb_t*));
    /*configuration of the PIR sensor*/
    gpio_config_t ioConfigSensorPIR= {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << GPIO_SENSOR_PIR,
        .pull_down_en= GPIO_PULLDOWN_ENABLE,
        .pull_up_en= GPIO_PULLUP_DISABLE
    };
    gpio_config(&ioConfigSensorPIR);

    /*configuration du buzzer*/
    gpio_config_t ioBuzzer= {
        .intr_type =GPIO_INTR_DISABLE,
        .mode= GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << GPIO_BUZZER,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&ioBuzzer);
  
    /*configuration des leds*/
    gpio_config_t ioLeds = {
        .intr_type= GPIO_INTR_DISABLE,
        .mode= GPIO_MODE_OUTPUT,
        .pin_bit_mask = ((1ULL << GPIO_GREEN_LED)|(1ULL << GPIO_RED_LED)|(1ULL << GPIO_BLUE_LED)),
        .pull_down_en= GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&ioLeds);
   
}

void taskInitAndStartWifi(){
    /*initialisation de la memoire flash*/
    esp_err_t rec= nvs_flash_init();
    if (rec==ESP_ERR_NVS_NO_FREE_PAGES || rec==ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        rec= nvs_flash_init();
    }
    ESP_ERROR_CHECK(rec);
    
    ESP_ERROR_CHECK(rec);  
    /*initiation et demarrage du wifi*/
     EventBits_t bits=initWifi();
    if(bits & WIFI_CONNECTED_BITS)
       ESP_LOGI("wifi_tag","connected to wifi");
    else if (bits & WIFI_FAILS_CONNECTED_BITS)
       ESP_LOGI("wifi_tag","connection failed");
    else 
       ESP_LOGI("wifi_tag", "bisarre");
}

void IRAM_ATTR InterrupSensorPIR(void* arg){

    BaseType_t priorityTask = pdFALSE;
    xSemaphoreGiveFromISR(binaryTaskSensor,&priorityTask);
    portYIELD_FROM_ISR(priorityTask);
}


void taskSensorPIR(void* arg){
    
    while (1)
    {
        if (xSemaphoreTake(binaryTaskSensor,portMAX_DELAY)==pdTRUE){

            printf("ok \n");
            xSemaphoreGive(binaryTaskPicture);
            iaWork=true;
            /* clignotement de la led bleue */
            while (iaWork)
            {
                gpio_set_level(GPIO_BLUE_LED,0);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(GPIO_BLUE_LED,1);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }   
}

void taskPicture(void* arg){
    while (1)
    {
        if(xSemaphoreTake(binaryTaskPicture,portMAX_DELAY)){

            printf("processus de prise de photos\n");
            camera_fb_t* img= NULL;
            img= takePicture();
           
            if (img)
            {
                bool result= verifieImage(img->buf,img->len);
                if (result)
                {
                    ESP_LOGI("result","cette personne est celle qu'on cherche");
                    gpio_set_level(GPIO_BUZZER,1);
                    gpio_set_level(GPIO_GREEN_LED,1);
                    vTaskDelay(pdMS_TO_TICKS(500));
                    gpio_set_level(GPIO_BUZZER,0);
                    gpio_set_level(GPIO_GREEN_LED,0);
                }
                else
                {
                    ESP_LOGI("result","cette personne n'est pas celle qu'on cherche");
                    gpio_set_level(GPIO_BUZZER,1);
                    gpio_set_level(GPIO_RED_LED,1);
                    vTaskDelay(pdMS_TO_TICKS(700));
                    gpio_set_level(GPIO_BUZZER,0);
                    gpio_set_level(GPIO_RED_LED,0);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    gpio_set_level(GPIO_BUZZER,1);
                    gpio_set_level(GPIO_RED_LED,1);
                    vTaskDelay(pdMS_TO_TICKS(700));
                    gpio_set_level(GPIO_BUZZER,0);
                    gpio_set_level(GPIO_RED_LED,0);

                }
            
            }
            else 
            {
                ESP_LOGE("camera", "capture invalide");
                gpio_set_level(GPIO_BUZZER,1);
                gpio_set_level(GPIO_RED_LED,1);
                vTaskDelay(pdMS_TO_TICKS(1000));
                gpio_set_level(GPIO_BUZZER,0);
                gpio_set_level(GPIO_RED_LED,0);
                
            } 
           
            
            iaWork= false;
           
            xQueueSend(queueImageBuffer,&img,portMAX_DELAY) ;
        }

    }
     
}

void sendImageHttp(void* arg){
    camera_fb_t* imageBuffer = NULL;
    while (1)
    {
        printf("preuve\n");
        if(xQueueReceive(queueImageBuffer,&imageBuffer,portMAX_DELAY)){
            if (imageBuffer)
            {
                freeBuffer(imageBuffer);
                printf("buffer libéré %d\n",imageBuffer->len); 
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        
    }

}
