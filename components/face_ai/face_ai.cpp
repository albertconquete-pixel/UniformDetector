#include <stdio.h>
#include "face_ai.h"
#include "dl_image.hpp"
#include "human_face_detect.hpp"
#include "human_face_recognition.hpp"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "filesystem"

#include "esp_log.h"
#include "esp_check.h"

static wl_handle_t handle;
static esp_err_t fatfs_sdcard_mont()
{
    esp_vfs_fat_mount_config_t fatFsConfig;
    memset(&fatFsConfig,0,sizeof(esp_vfs_fat_mount_config_t));
    fatFsConfig.format_if_mount_failed = true;
    fatFsConfig.max_files =5;
    ESP_ERROR_CHECK(esp_vfs_fat_spiflash_mount_rw_wl("/spiflash","storage",&fatFsConfig,&handle));
    return ESP_OK;
}

HumanFaceDetect *humanFaceDetect = new HumanFaceDetect();
HumanFaceRecognizer* humanFaceRecognition;
extern "C" void uploadImageSecu(){
    extern const uint8_t albUserStart[] asm("_binary_musk2_jpg_start");
    extern const uint8_t albUserEnd[] asm("_binary_musk2_jpg_end");
    ESP_ERROR_CHECK(fatfs_sdcard_mont());

    dl::image::jpeg_img_t imgUser = {
        .data = (void*)albUserStart,
        .data_len = (size_t)(albUserEnd-albUserStart),
    };

    auto imgSecu= dl::image::sw_decode_jpeg(imgUser,dl::image::DL_IMAGE_PIX_TYPE_RGB888); 
    
    auto dbPath = std::filesystem::path("/spiflash")/"faceSecu.db";
    humanFaceRecognition = new HumanFaceRecognizer(dbPath.string());
    humanFaceRecognition->enroll(imgSecu,humanFaceDetect->run(imgSecu));
    heap_caps_free(imgSecu.data);
}

extern "C" bool verifieImage(uint8_t* imgSize, size_t lenImage){
    dl::image::jpeg_img_t img={
        .data =imgSize,
        .data_len= lenImage,
    };
    auto imgSucpect= dl::image::sw_decode_jpeg(img,dl::image::DL_IMAGE_PIX_TYPE_RGB888);
    if(!imgSucpect.data){
        ESP_LOGE("jpeg_decode","erreur de conversion");
        return false;
    }
    auto result= humanFaceRecognition->recognize(imgSucpect,humanFaceDetect->run(imgSucpect));
    printf("done\n");
    for (const auto &k:result)
    {
        ESP_LOGI("result", "id: %d, sim: %f", k.id, k.similarity);
        if(k.similarity >=0.50){
            heap_caps_free(imgSucpect.data);
            return true;
        }
           
    }

    heap_caps_free(imgSucpect.data);
    return false;
    
}