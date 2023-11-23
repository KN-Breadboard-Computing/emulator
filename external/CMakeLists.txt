# TODO: Check whether this can be done by including the src directories
add_library(Unity STATIC
  Unity/src/unity.c
)

target_include_directories(Unity PUBLIC
  Unity/src
)

add_library(cJSON STATIC
    cJSON/cJSON.c
)

target_include_directories(cJSON PUBLIC
    cJSON/
)
