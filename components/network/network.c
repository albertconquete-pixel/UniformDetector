#include <stdio.h>
#include "network.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"

#include "esp_http_client.h"


EventGroupHandle_t wifiEventBits= NULL;
QueueHandle_t buffer = NULL;

static int retryNum =0;
static const char* TAG = "wifi sta";

static void wifi_event(void* arg, esp_event_base_t esp_event,int32_t event_id, void* event_data){
    if (esp_event== WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        
        case WIFI_EVENT_STA_DISCONNECTED:
            wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*)event_data;
            if (retryNum < ESP_WIFI_RETRY)
            {
                esp_wifi_connect();
                retryNum++;
                ESP_LOGI(TAG,"retry the connection %d",event->reason);
            }else
                xEventGroupSetBits(wifiEventBits,WIFI_FAILS_CONNECTED_BITS);
        }
    }
    if(esp_event ==IP_EVENT && event_id ==IP_EVENT_STA_GOT_IP)
        xEventGroupSetBits(wifiEventBits,WIFI_CONNECTED_BITS);
}

EventBits_t initWifi(){
    wifiEventBits= xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfw =WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfw));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&wifi_event,NULL,&instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&wifi_event,NULL,&instance_got_ip));

    wifi_config_t wifi_config ={
        .sta ={
            .password= ESP_WIFI_PASSWORD,
            .ssid = ESP_WIFI_SSDI,
            .threshold.authmode= WIFI_AUTH_OPEN,
            .threshold.rssi = -127,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    /**
     * @warning fonction potentiellement bloquante, a corriger pour eviter certains
     * freeze du programme
     */
    return xEventGroupWaitBits(wifiEventBits,WIFI_CONNECTED_BITS|WIFI_FAILS_CONNECTED_BITS,pdFALSE,pdFALSE,portMAX_DELAY);
}

esp_err_t httpHandleEvent(esp_http_client_event_t* espEvent){
    switch (espEvent->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE("https", "%.*s",espEvent->data_len,(char*)espEvent->data);
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI("https","esp connecté avec succes");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI("https","esp a envoyer le header");
        break;   
    default:
        break;
    }
    return ESP_OK;
}

static esp_http_client_config_t httpConfig={
    .url = "http://10.200.253.227:8080/endpoint",
    .event_handler = httpHandleEvent,
    .buffer_size= 2048,
    .buffer_size_tx = 16384,
    .timeout_ms= 1000,
};

void httpsTransfer(uint8_t* fb, int len){

    esp_http_client_handle_t client= esp_http_client_init(&httpConfig);
      esp_http_client_set_method(client, HTTP_METHOD_POST); 
    ESP_ERROR_CHECK(esp_http_client_set_header(client,"Content-Type","image/jpeg"));

    esp_http_client_set_post_field(client,(char*)fb,len);
    esp_err_t err= esp_http_client_perform(client);
    printf("%d\n",err);
    if(!err){ 
        int status =esp_http_client_get_status_code(client);
        ESP_LOGI("https","%d",status);
    }
    else 
        ESP_LOGE("http","echecs de la connexion au serveur");    
    
    esp_http_client_cleanup(client);    
      
}