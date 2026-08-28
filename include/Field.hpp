/**
 * @file Field.hpp
 * @brief Representação visual do campo de jogo
 * @author Esther e Lucas  
 * @date 2025
 */

#pragma once
#include "Entity.hpp"
#include "Types.hpp"
#include <allegro5/allegro.h>

/**
 * @brief Representa o campo de jogo com moldura e fundo
 * 
 * Herda de Entity e desenha o campo com bordas e textura de fundo.
 * O campo define a área jogável onde a cobra se move.
 */
class Field : public Entity {
public:
    /**
     * @brief Constrói o campo com textura específica
     * @param bmp Bitmap do campo (pode ser nullptr para fallback)
     */
    explicit Field(ALLEGRO_BITMAP* bmp);

    /**
     * @brief Atualização do campo (não faz nada, apenas interface)
     * @param dt Tempo delta (não utilizado)
     */
    void update(double dt) override {}
    
    /**
     * @brief Desenha o campo na tela com bordas e textura
     */
    void draw() const override;

private:
    ALLEGRO_BITMAP* bmp_{nullptr};  ///< Textura do campo (opcional)
};