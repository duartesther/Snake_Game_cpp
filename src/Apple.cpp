/**
 * @file Apple.cpp
 * @brief Implementação da classe Apple
 * @author Esther e Lucas
 * @date 2025
 */

#include "Apple.hpp"
#include <random>
#include <allegro5/allegro.h>

namespace {
    /**
     * @brief Gera e retorna uma referência para o RNG do módulo
     * @return Referência para gerador de números aleatórios
     */
    std::mt19937& rng() {
        static std::mt19937 gen{ std::random_device{}() };
        return gen;
    }
}

/**
 * @brief Constrói a maçã e posiciona aleatoriamente
 * @param bmp Bitmap do sprite da maçã
 */
Apple::Apple(ALLEGRO_BITMAP* bmp)
    : bmp_(bmp)
{
    spawnRandom();
}

/**
 * @brief Posiciona a maçã em local aleatório dentro do campo
 * 
 * A posição é gerada respeitando os limites do campo e o raio da maçã
 * para evitar posicionamento nas bordas.
 */
void Apple::spawnRandom() {
    // Sorteia dentro da área útil do campo (em pixels), respeitando o raio
    std::uniform_real_distribution<float> dx(
        cfg::FieldOffsetX + radius_,
        cfg::FieldOffsetX + cfg::FieldW - radius_
    );
    std::uniform_real_distribution<float> dy(
        cfg::FieldOffsetY + radius_,
        cfg::FieldOffsetY + cfg::FieldH - radius_
    );

    pos_.x = dx(rng());
    pos_.y = dy(rng());
}

/**
 * @brief Desenha a maçã na tela
 * 
 * O sprite é desenhado centralizado na posição da maçã.
 * Se o bitmap não estiver carregado, nada é desenhado.
 */
void Apple::draw() const {
    if (!bmp_) return;

    const float w = static_cast<float>(al_get_bitmap_width(bmp_));
    const float h = static_cast<float>(al_get_bitmap_height(bmp_));

    // Desenha centralizando a maçã na posição
    al_draw_bitmap(bmp_, pos_.x - w * 0.5f, pos_.y - h * 0.5f, 0);
}