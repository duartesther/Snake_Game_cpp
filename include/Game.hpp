/**
 * @file Game.hpp
 * @brief Classe principal do jogo que gerencia todo o estado e loop
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once

// Allegro primeiro (tipos ALLEGRO_*)
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// STL
#include <memory>
#include <string>

// Projeto
#include "Types.hpp"
#include "Field.hpp"
#include "Apple.hpp"
#include "Snake.hpp"
#include "ConsoleHUD.hpp"
#include "Sound.hpp"
#include "ResourceManager.hpp"
#include "AllegroLoaders.hpp"
#include "AllegroGfx.hpp"

/**
 * @brief Estados possíveis do jogo
 */
enum class GameState { 
    Menu,       ///< Tela de menu inicial
    Playing,    ///< Jogo em andamento  
    GameOver    ///< Tela de fim de jogo
};

/**
 * @brief Classe principal que gerencia todo o jogo
 * 
 * Responsável por inicializar sistemas, gerenciar o loop principal,
 * processar eventos e coordenar todas as entidades do jogo.
 */
class Game {
public:
    /**
     * @brief Constrói e inicializa o jogo
     * @throws std::runtime_error se inicialização falhar
     */
    Game();
    
    /**
     * @brief Destrutor que limpa recursos
     */
    ~Game();

    /**
     * @brief Executa o loop principal do jogo
     */
    void run();

private:
    /**
     * @brief Reinicia o jogo para estado inicial
     */
    void resetGame();
    
    /**
     * @brief Processa eventos do Allegro
     * @param ev Evento a ser processado
     */
    void handleEvent(const ALLEGRO_EVENT& ev);
    
    /**
     * @brief Atualiza a lógica do jogo
     */
    void updateLogic();
    
    /**
     * @brief Renderiza o frame atual
     */
    void render();
    
    /**
     * @brief Cicla entre os esquemas de cores da cobra
     */
    void cycleSnakeColor();

private:
    // --- Core Allegro ---
    ALLEGRO_DISPLAY*     display_{nullptr};    ///< Janela principal
    ALLEGRO_EVENT_QUEUE* queue_{nullptr};      ///< Fila de eventos
    ALLEGRO_TIMER*       timer_{nullptr};      ///< Timer para FPS fixo

    // --- Managers de recursos ---
    using BitmapRM = ResourceManager<std::string, ALLEGRO_BITMAP*, BitmapLoader,  BitmapUnloader>;
    using FontRM   = ResourceManager<std::string, ALLEGRO_FONT*,   FontSpecLoader, FontUnloader>;

    BitmapRM bmpManager_;  ///< Gerenciador de bitmaps
    FontRM   fontManager_; ///< Gerenciador de fonts

    // --- Mundo / HUD / Som ---
    std::unique_ptr<Field>      field_;  ///< Campo de jogo
    std::unique_ptr<Apple>      apple_;  ///< Maçã coletável
    std::unique_ptr<Snake>      snake_;  ///< Cobra do jogador
    std::unique_ptr<ConsoleHUD> hud_;    ///< Interface do usuário
    SoundBank*                  sound_{nullptr};  ///< Sistema de áudio

    // --- Fontes correntes ---
    ALLEGRO_FONT* fontSmall_{nullptr};  ///< Fonte para texto normal
    ALLEGRO_FONT* fontBig_{nullptr};    ///< Fonte para títulos

    // --- Estado de execução ---
    bool running_{true};    ///< Controle do loop principal
    bool paused_{false};    ///< Jogo pausado
    bool leftHeld_{false};  ///< Tecla esquerda pressionada
    bool rightHeld_{false}; ///< Tecla direita pressionada

    GameState state_{GameState::Menu};  ///< Estado atual do jogo

    // --- Customização / placar ---
    SnakeColorPreset colorPreset_{SnakeColorPreset::Red};  ///< Cor atual da cobra
    int highScore_{0};  ///< Recorde do jogador
};