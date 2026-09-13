idf_component_register(SRCS "picture_cam.c"
                    INCLUDE_DIRS "include"
                    EMBED_FILES "camera_test/musk1.jpg"
                    REQUIRES nvs_flash esp32-camera)
