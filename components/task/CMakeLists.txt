idf_component_register(SRCS "task.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_gpio network nvs_flash)
