/**
 * @file Apple.hpp
 * @brief Representação da maçã coletável no jogo
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <allegro5/allegro.h>
#include "Vector2D.hpp"
#include "Types.hpp"

/**
 * @brief Representa uma maçã coletável no jogo Snake
 * 
 * A maçã pode ser posicionada aleatoriamente no campo e é desenhada
 * usando um sprite. Quando a cobra colide com a maçã, ela cresce.
 */
class Apple {
public:
    /**
     * @brief Constrói uma maçã com sprite específico
     * @param bmp Bitmap do sprite da maçã
     */
    explicit Apple(ALLEGRO_BITMAP* bmp);

    /**
     * @brief Posiciona a maçã em local aleatório dentro do campo
     */
    void spawnRandom();
    
    /**
     * @brief Desenha a maçã na tela
     */
    void draw() const;

    // Métodos de acesso
    /**
     * @brief Obtém a posição atual da maçã
     * @return Vector2D com coordenadas da maçã
     */
    Vector2D position() const { return pos_; }
    
    /**
     * @brief Obtém coordenada X da maçã
     * @return Coordenada X
     */
    float x() const { return pos_.x; }
    
    /**
     * @brief Obtém coordenada Y da maçã
     * @return Coordenada Y
     */
    float y() const { return pos_.y; }
    
    /**
     * @brief Obtém raio de colisão da maçã
     * @return Raio para detecção de colisão
     */
    float radius() const { return radius_; }

private:
    ALLEGRO_BITMAP* bmp_{nullptr};  ///< Sprite da maçã
    Vector2D        pos_{};         ///< Posição atual no campo
    float           radius_{12.0f}; ///< Raio para colisão
};