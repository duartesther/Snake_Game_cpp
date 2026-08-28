/**
 * @file Snake.cpp
 * @brief Implementação da classe Snake
 * @author Esther e Lucas
 * @date 2025
 */

#include "Snake.hpp"
#include "Types.hpp"
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <cmath>

/**
 * @brief Constrói a cobra com sprites e posição inicial
 * @param headBmp Sprite da cabeça
 * @param bodyBmp Sprite do corpo  
 * @param tailBmp Sprite da cauda
 */
Snake::Snake(ALLEGRO_BITMAP* headBmp, ALLEGRO_BITMAP* bodyBmp, ALLEGRO_BITMAP* tailBmp)
    : headBmp_(headBmp), bodyBmp_(bodyBmp), tailBmp_(tailBmp) {
    // Inicializa segmentos
    body_.assign(storedSize_, {});
    const double x0 = cfg::FieldOffsetX + cfg::FieldW * 0.5;
    const double y0 = cfg::FieldOffsetY + cfg::FieldH * 0.5;
    for (int i = 0; i < storedSize_; ++i) {
        body_[i].pos = { x0 - i * 1.0, y0 };
        body_[i].angleDeg = 0.0;
        body_[i].size = 8.0;
    }
    visibleSize_  = 12;
    headAngleDeg_ = 0.0;
}

/**
 * @brief Vira a cobra para esquerda
 */
void Snake::turnLeft()  { headAngleDeg_ += rotationSpeed_; }

/**
 * @brief Vira a cobra para direita  
 */
void Snake::turnRight() { headAngleDeg_ -= rotationSpeed_; }

/**
 * @brief Atualiza posição e ângulos da cobra
 * @param dt Tempo delta (não utilizado, mantido para interface)
 */
void Snake::update(double /*dt*/) {
    const double rad = degToRad(headAngleDeg_);
    const double vx = std::cos(rad) * speedModule_;
    const double vy = std::sin(rad) * speedModule_;

    // Move segmentos (shift array)
    for (int i = storedSize_-1; i > 0; --i) body_[i] = body_[i-1];
    
    // Atualiza posição da cabeça
    body_[0].pos.x = body_[1].pos.x + vx;
    body_[0].pos.y = body_[1].pos.y + vy;

    // Aplica wrapping nas bordas
    body_[0].pos.x = wrapCoord(body_[0].pos.x, cfg::FieldOffsetX, cfg::FieldW);
    body_[0].pos.y = wrapCoord(body_[0].pos.y, cfg::FieldOffsetY, cfg::FieldH);
    body_[0].angleDeg = headAngleDeg_;

    // Atualiza ângulos dos segmentos do corpo
    for (int i = 1; i < visibleSize_; ++i) {
        double dx = body_[i-1].pos.x - body_[i].pos.x;
        double dy = body_[i-1].pos.y - body_[i].pos.y;
        auto wrapDelta = [](double d, double span){
            if (d >  span/2.0) d -= span;
            if (d < -span/2.0) d += span;
            return d;
        };
        dx = wrapDelta(dx, cfg::FieldW);
        dy = wrapDelta(dy, cfg::FieldH);
        body_[i].angleDeg = std::atan2(dy, dx) * 180.0 / 3.14159265358979323846;
    }
}

/**
 * @brief Desenha a cobra com cores e detalhes visuais
 * 
 * Renderiza cada segmento com gradiente de cores, realces
 * e olhos na cabeça. Aplica wrapping para transições suaves.
 */
void Snake::draw() const {
    const SnakePalette pal = presetToPalette(colorPreset_);
    const ALLEGRO_COLOR bodyDark  = al_map_rgb(pal.bodyDark.r, pal.bodyDark.g, pal.bodyDark.b);
    const ALLEGRO_COLOR bodyMid   = al_map_rgb(pal.bodyMid.r,  pal.bodyMid.g,  pal.bodyMid.b);
    const ALLEGRO_COLOR bodyLight = al_map_rgb(pal.bodyLight.r,pal.bodyLight.g,pal.bodyLight.b);
    const ALLEGRO_COLOR headLight = al_map_rgb(pal.headLight.r,pal.headLight.g,pal.headLight.b);
    const ALLEGRO_COLOR eyeWhite  = al_map_rgb(pal.eyeWhite.r, pal.eyeWhite.g, pal.eyeWhite.b);
    const ALLEGRO_COLOR eyeBlack  = al_map_rgb(pal.eyeBlack.r, pal.eyeBlack.g, pal.eyeBlack.b);

    // Offsets para wrapping (9 cópias para transições suaves)
    const float offX[3] = { 0.0f, -static_cast<float>(cfg::FieldW),  static_cast<float>(cfg::FieldW)  };
    const float offY[3] = { 0.0f, -static_cast<float>(cfg::FieldH),  static_cast<float>(cfg::FieldH)  };

    // Função lambda para desenhar um segmento
    auto drawBodyDisk = [&](const Segment& s, bool isHead) {
        const float ang = static_cast<float>(degToRad(s.angleDeg));
        const float r   = static_cast<float>(s.size);
        for (float dx : offX) for (float dy : offY) {
            const float cx = static_cast<float>(s.pos.x) + dx;
            const float cy = static_cast<float>(s.pos.y) + dy;

            // Skip se estiver muito fora da tela (otimização)
            if (cx < cfg::FieldOffsetX - 2*r || cx > cfg::FieldOffsetX + cfg::FieldW + 2*r) continue;
            if (cy < cfg::FieldOffsetY - 2*r || cy > cfg::FieldOffsetY + cfg::FieldH + 2*r) continue;

            // Desenha segmento com gradiente
            al_draw_filled_circle(cx, cy, r + 1.5f, bodyDark);  // Sombra
            al_draw_filled_circle(cx, cy, r,       isHead ? headLight : bodyMid);  // Corpo

            // Realce
            const float hlR = r * 0.7f;
            const float hlY = cy - r * 0.35f;
            al_draw_filled_circle(cx, hlY, hlR, bodyLight);

            // Olhos (apenas na cabeça)
            if (isHead) {
                const float ux = std::cos(ang), uy = std::sin(ang);
                const float sx = -uy,           sy =  ux;
                const float eyeR = r * 0.32f, eyeInR = eyeR * 0.45f;
                const float ahead = r * 0.65f, side = r * 0.35f;

                const float ex1 = cx + ux * ahead + sx * side;
                const float ey1 = cy + uy * ahead + sy * side;
                const float ex2 = cx + ux * ahead - sx * side;
                const float ey2 = cy + uy * ahead - sy * side;

                al_draw_filled_circle(ex1, ey1, eyeR, eyeWhite);
                al_draw_filled_circle(ex2, ey2, eyeR, eyeWhite);

                // Pupilas
                const float pupilAhead = eyeR * 0.25f;
                const float px1 = ex1 + ux * pupilAhead, py1 = ey1 + uy * pupilAhead;
                const float px2 = ex2 + ux * pupilAhead, py2 = ey2 + uy * pupilAhead;

                al_draw_filled_circle(px1, py1, eyeInR, eyeBlack);
                al_draw_filled_circle(px2, py2, eyeInR, eyeBlack);
            }
        }
    };

    // Desenha corpo (com passo para otimização)
    const int step = bodyDrawStepIndices();
    const int lastVisible = std::max(1, visibleSize_ - 1);
    for (int i = 1; i < lastVisible; i += step) drawBodyDisk(body_[i], false);
    
    // Desenha cabeça por último (sobrepõe)
    drawBodyDisk(body_[0], true);
}

/**
 * @brief Aumenta tamanho da cobra e atualiza score
 */
void Snake::grow() {
    visibleSize_ = std::min(storedSize_, visibleSize_ + cfg::SnakeGrowthStep);
    score_ += 10;
}

/**
 * @brief Verifica colisão da cabeça com o corpo
 * @return true se cabeça colidiu com qualquer segmento do corpo
 */
bool Snake::checkSelfCollision() const {
    const int start = std::max(3 * cfg::SnakeGrowthStep, 1);  // Ignora segmentos próximos
    const double dc = (body_[0].size + body_[1].size) - 4.0;  // Distância de colisão

    for (int i = start; i < visibleSize_; ++i) {
        double dx = body_[0].pos.x - body_[i].pos.x;
        double dy = body_[0].pos.y - body_[i].pos.y;
        auto wrapDelta = [](double d, double span){
            if (d > span/2.0) d -= span;
            if (d < -span/2.0) d += span;
            return d;
        };
        dx = wrapDelta(dx, cfg::FieldW);
        dy = wrapDelta(dy, cfg::FieldH);
        if (dx*dx + dy*dy < dc*dc) return true;
    }
    return false;
}