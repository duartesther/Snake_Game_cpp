/**
 * @file AllegroGfx.hpp
 * @brief Inicialização e finalização do Allegro
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/**
 * @brief Gerencia inicialização e finalização do Allegro e seus addons
 * 
 * Esta classe RAII garante que o Allegro seja inicializado corretamente
 * no construtor e limpo adequadamente no destrutor.
 */
struct AllegroInit {
    /**
     * @brief Inicializa Allegro e todos os addons necessários
     * @throws std::runtime_error se qualquer inicialização falhar
     */
    AllegroInit() {
        if (!al_init()) throw std::runtime_error("Falha al_init");
        if (!al_install_keyboard()) throw std::runtime_error("Falha al_install_keyboard");
        al_init_font_addon();
        if (!al_init_ttf_addon()) throw std::runtime_error("Falha TTF");
        if (!al_init_image_addon()) throw std::runtime_error("Falha image");
        if (!al_init_primitives_addon()) throw std::runtime_error("Falha primitives");
    }
    
    /**
     * @brief Finaliza Allegro e seus addons
     */
    ~AllegroInit() {
        al_shutdown_primitives_addon();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_uninstall_keyboard();
    }
};