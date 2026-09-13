idf_component_register(SRCS "face_ai.cpp"
                    INCLUDE_DIRS "include"
                    EMBED_FILES "musk2.jpg"
                    REQUIRES esp-dl human_face_detect human_face_recognition vfs  fatfs)
