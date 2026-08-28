/**
 * @file Field.cpp
 * @brief Implementação da classe Field
 * @author Esther e Lucas
 * @date 2025
 */

#include "Field.hpp"
#include <allegro5/allegro_primitives.h>

/**
 * @brief Constrói o campo com bitmap específico
 * @param bmp Bitmap do campo (pode ser nullptr)
 */
Field::Field(ALLEGRO_BITMAP* bmp)
    : bmp_(bmp) {}

/**
 * @brief Desenha o campo na tela com moldura e textura
 * 
 * Desenha uma moldura cinza escuro ao redor do campo e o preenche
 * com o bitmap fornecido ou com cor sólida verde como fallback.
 */
void Field::draw() const {
    const int border = 15;

    float x0 = cfg::FieldOffsetX - border;
    float y0 = cfg::FieldOffsetY - border;
    float x1 = cfg::FieldOffsetX + cfg::FieldW + border;
    float y1 = cfg::FieldOffsetY + cfg::FieldH + border;

    // Moldura cinza escuro
    al_draw_filled_rectangle(
        x0, y0, x1, y1,
        al_map_rgb(40, 40, 40)
    );

    // Campo verde listrado ou fallback
    if (bmp_) {
        al_draw_bitmap(
            bmp_,
            cfg::FieldOffsetX,
            cfg::FieldOffsetY,
            0
        );
    } else {
        // Fallback se não carregar a imagem
        al_draw_filled_rectangle(
            cfg::FieldOffsetX, cfg::FieldOffsetY,
            cfg::FieldOffsetX + cfg::FieldW,
            cfg::FieldOffsetY + cfg::FieldH,
            al_map_rgb(80, 182, 31)
        );
    }
}