cmake_minimum_required(VERSION 3.20)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Allegro (busca direta - funciona sempre)
find_library(ALLEGRO_LIB allegro)
find_library(ALLEGRO_PRIMITIVES_LIB allegro_primitives)
find_library(ALLEGRO_IMAGE_LIB allegro_image)
find_library(ALLEGRO_FONT_LIB allegro_font)
find_library(ALLEGRO_TTF_LIB allegro_ttf)
find_library(ALLEGRO_AUDIO_LIB allegro_audio)
find_library(ALLEGRO_ACODEC_LIB allegro_acodec)

set(ALLEGRO_LIBRARIES
    ${ALLEGRO_LIB}
    ${ALLEGRO_PRIMITIVES_LIB}
    ${ALLEGRO_IMAGE_LIB}
    ${ALLEGRO_FONT_LIB}
    ${ALLEGRO_TTF_LIB}
    ${ALLEGRO_AUDIO_LIB}
    ${ALLEGRO_ACODEC_LIB}
)

set(ALLEGRO_INCLUDE_DIRS "/usr/include/allegro5")

if(NOT ALLEGRO_LIB)
    message(FATAL_ERROR "Allegro library not found! Install with: sudo apt install liballegro5-dev")
endif()

# --- Includes do projeto
set(PROJ_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PRIVATE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/private)

# Coleta fontes comuns (APENAS para SnakeGame)
file(GLOB SRC_COMMON CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/private/*.cpp"
)

# REMOVE ambos os mains da lista comum
list(REMOVE_ITEM SRC_COMMON
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/main_text.cpp"
    "${CMAKE_SOURCE_DIR}/src/main_text_linux.cpp" 
    "${CMAKE_SOURCE_DIR}/src/main_no_audio.cpp"
)

# =========================
# Executável GUI (Allegro)
# =========================
add_executable(SnakeGame ${SRC_COMMON} src/main.cpp)  # ← APENAS main.cpp

target_include_directories(SnakeGame PRIVATE
    ${PROJ_INCLUDE_DIR}
    ${PRIVATE_INCLUDE_DIR}
    ${ALLEGRO_INCLUDE_DIRS}
)

target_link_libraries(SnakeGame PRIVATE
    ${ALLEGRO_LIBRARIES}
)

# =========================
# Executável TEXTO (terminal) - Linux
# =========================
add_executable(SnakeText src/main_text_linux.cpp)  # ← APENAS main_text_linux.cpp

target_include_directories(SnakeText PRIVATE
    ${PROJ_INCLUDE_DIR}
)

target_link_libraries(SnakeText PRIVATE)

# =========================
# Executável GRÁFICO sem áudio (Docker)
# =========================
add_executable(SnakeGameNoAudio ${SRC_COMMON} src/main_no_audio.cpp)

target_include_directories(SnakeGameNoAudio PRIVATE
    ${PROJ_INCLUDE_DIR}
    ${PRIVATE_INCLUDE_DIR}
    ${ALLEGRO_INCLUDE_DIRS}
)

target_link_libraries(SnakeGameNoAudio PRIVATE
    ${ALLEGRO_LIBRARIES}
)