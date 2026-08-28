/**
 * @file ConsoleHUD.hpp
 * @brief Heads-Up Display (HUD) do jogo com informações e controles
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include "Types.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

/**
 * @brief Estado das teclas de direção
 */
enum class KeyState { 
    None,   ///< Nenhuma tecla pressionada
    Left,   ///< Tecla esquerda pressionada  
    Right   ///< Tecla direita pressionada
};

/**
 * @brief HUD que exibe informações do jogo e controles
 * 
 * Mostra score, recorde, estado do jogo e representação visual
 * das teclas pressionadas.
 */
class ConsoleHUD {
public:
    /**
     * @brief Constrói o HUD com todos os recursos necessários
     * @param consoleBmp Bitmap do console/base do HUD
     * @param kbNone Bitmap do teclado sem teclas pressionadas
     * @param kbLeft Bitmap do teclado com seta esquerda pressionada
     * @param kbRight Bitmap do teclado com seta direita pressionada
     * @param fontSmall Fonte para texto pequeno
     * @param fontBig Fonte para texto grande/título
     */
    ConsoleHUD(ALLEGRO_BITMAP* consoleBmp,
               ALLEGRO_BITMAP* kbNone,
               ALLEGRO_BITMAP* kbLeft,
               ALLEGRO_BITMAP* kbRight,
               ALLEGRO_FONT*   fontSmall,
               ALLEGRO_FONT*   fontBig);

    // Métodos para atualizar estado
    /**
     * @brief Define o score atual
     * @param s Score a ser exibido
     */
    void setScore(int s)           { score_ = s; }
    
    /**
     * @brief Define o recorde atual
     * @param r Recorde a ser exibido
     */
    void setRecord(int r)          { record_ = r; }
    
    /**
     * @brief Alias para setRecord para compatibilidade
     * @param r Recorde a ser exibido
     */
    void setHighScore(int r)       { setRecord(r); }
    
    /**
     * @brief Define se o jogo está em estado de game over
     * @param v true para game over, false caso contrário
     */
    void setGameOver(bool v)       { gameOver_ = v; }
    
    /**
     * @brief Define o estado das teclas de direção
     * @param k Estado atual das teclas
     */
    void setKeyState(KeyState k)   { key_ = k; }

    /**
     * @brief Desenha todo o HUD na tela
     */
    void draw() const;

    /**
     * @brief Alias para draw() para compatibilidade
     * @param title Título (ignorado, mantido para compatibilidade)
     */
    void drawTitleAndHelp(const char* title) const { draw(); }

private:
    // Bitmaps da HUD inferior (console + teclado)
    ALLEGRO_BITMAP* console_{nullptr};  ///< Base do console
    ALLEGRO_BITMAP* kbNone_{nullptr};   ///< Teclado neutro
    ALLEGRO_BITMAP* kbLeft_{nullptr};   ///< Teclado esquerda
    ALLEGRO_BITMAP* kbRight_{nullptr};  ///< Teclado direita

    // Fontes
    ALLEGRO_FONT* fontSmall_{nullptr};  ///< Fonte para texto normal
    ALLEGRO_FONT* fontBig_{nullptr};    ///< Fonte para títulos

    // Estado
    int       score_{0};          ///< Score atual
    int       record_{0};         ///< Recorde atual
    bool      gameOver_{false};   ///< Estado de game over
    KeyState  key_{KeyState::None}; ///< Estado das teclas
};