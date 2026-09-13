#include <stdio.h>
#include "esp_camera.h"
#include "nvs_flash.h"
#include "picture_cam.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"



QueueHandle_t queueImageBuffer =NULL;
void (*freeBuffer)(camera_fb_t* )= esp_camera_fb_return;

esp_err_t initCamera(){
    camera_config_t camera_config ={
        .fb_count = 1,
        .fb_location = CAMERA_FB_IN_DRAM,
        .frame_size = FRAMESIZE_QVGA,
        .ledc_channel = LEDC_CHANNEL_0,
        .ledc_timer = LEDC_TIMER_0,
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
        .pin_d0 = CAM_PIN_D0,
        .pin_d1 = CAM_PIN_D1,
        .pin_d2 = CAM_PIN_D2,
        .pin_d3 = CAM_PIN_D3,
        .pin_d4 = CAM_PIN_D4,
        .pin_d5 = CAM_PIN_D5,
        .pin_d6 = CAM_PIN_D6,
        .pin_d7 = CAM_PIN_D7,                                                   
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,
        .pin_pwdn = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_sccb_scl = CAM_PIN_SIOC,
        .pin_sccb_sda = CAM_PIN_SIOD,
        .pin_vsync = CAM_PIN_VSYNC,
        .pixel_format = PIXFORMAT_JPEG,
        .jpeg_quality=12,
        .xclk_freq_hz= CONFIG_XCLK_FREQ,
        .pin_xclk = CAM_PIN_XCLK,
    };

    esp_err_t err = esp_camera_init(&camera_config);
    return err;
}

camera_fb_t* takePicture(){
    static camera_fb_t* picture= NULL;
    ESP_LOGI("camera","nombre de frame occupé: %d",uxQueueMessagesWaiting(queueImageBuffer)) ;    
    picture =esp_camera_fb_get();
    if (!picture){
        ESP_LOGE("camera","erreur de prise de la photos");
        return NULL;
    }
    
    ESP_LOGI("camera", "taille %d",picture->len);
    return picture;
}

