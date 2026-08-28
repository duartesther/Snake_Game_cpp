/**
 * @file ConsoleHUD.cpp
 * @brief Implementação da classe ConsoleHUD
 * @author Esther e Lucas
 * @date 2025
 */

#include "ConsoleHUD.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

/**
 * @brief Constrói o HUD com todos os recursos
 * @param consoleBmp Bitmap do console
 * @param kbNone Bitmap teclado neutro
 * @param kbLeft Bitmap teclado esquerda
 * @param kbRight Bitmap teclado direita
 * @param fontSmall Fonte pequena
 * @param fontBig Fonte grande
 */
ConsoleHUD::ConsoleHUD(ALLEGRO_BITMAP* consoleBmp,
                       ALLEGRO_BITMAP* kbNone,
                       ALLEGRO_BITMAP* kbLeft,
                       ALLEGRO_BITMAP* kbRight,
                       ALLEGRO_FONT*   fontSmall,
                       ALLEGRO_FONT*   fontBig)
    : console_(consoleBmp)
    , kbNone_(kbNone)
    , kbLeft_(kbLeft)
    , kbRight_(kbRight)
    , fontSmall_(fontSmall)
    , fontBig_(fontBig)
{}

/**
 * @brief Desenha todo o HUD na tela
 * 
 * Inclui título, instruções, console inferior, teclado visual
 * e painel de score/recorde.
 */
void ConsoleHUD::draw() const {
    // ====== TÍTULO E SUBTÍTULO (topo) ======
    if (fontBig_) {
        const char* title   = "SNAKE GAME";
        const char* authors = "By: Esther e Lucas";

        // Posições (preservando estilo: bem alto, dentro do "quadro negro")
        const float titleY   = cfg::FieldOffsetY - 180.0f;
        const float authorsY = titleY + 60.0f;

        // Título branco
        al_draw_text(fontBig_, al_map_rgb(255, 255, 255),
                     cfg::WindowW * 0.5f, titleY,
                     ALLEGRO_ALIGN_CENTER, title);

        // Autores em cinza escuro
        if (fontSmall_) {
            al_draw_text(fontSmall_, al_map_rgb(0, 0, 0),
                         cfg::WindowW * 0.5f, authorsY,
                         ALLEGRO_ALIGN_CENTER, authors);
        }
    }

    // ====== INSTRUÇÕES ======
    if (fontSmall_) {
        const char* line1 = "PRESS P TO PAUSE";
        const char* line2 = "PRESS ESC TO EXIT";
        const char* line3 = "Use <- and -> to turn | C: color  |  M: music mute  |  N: FX mute";

        float baseY = cfg::FieldOffsetY + cfg::FieldH - 490.0f;

        al_draw_text(fontSmall_, al_map_rgb(180, 180, 180),
                     cfg::WindowW * 0.5f, baseY,
                     ALLEGRO_ALIGN_CENTER, line1);

        al_draw_text(fontSmall_, al_map_rgb(180, 180, 180),
                     cfg::WindowW * 0.5f, baseY + 20.0f,
                     ALLEGRO_ALIGN_CENTER, line2);

        al_draw_text(fontSmall_, al_map_rgb(200, 200, 200),
                     cfg::WindowW * 0.5f, baseY + 40.0f,
                     ALLEGRO_ALIGN_CENTER, line3);
    }

    // ====== HUD INFERIOR: CONSOLE + TECLADO CINZA ======
    float cx = 0.0f;
    const float cy = cfg::FieldOffsetY + cfg::FieldH + 80.0f;

    if (console_) {
        cx = (cfg::WindowW - al_get_bitmap_width(console_)) * 0.5f;
        al_draw_bitmap(console_, cx, cy, 0);
    }

    // Teclado (seta cinza clicável)
    ALLEGRO_BITMAP* keyBmp =
        (key_ == KeyState::Left)  ? kbLeft_  :
        (key_ == KeyState::Right) ? kbRight_ :
                                    kbNone_;
    if (keyBmp) {
        const float kx = (cfg::WindowW - al_get_bitmap_width(keyBmp)) * 0.5f;
        const float ky = cy + 15.0f;
        al_draw_bitmap(keyBmp, kx, ky, 0);
    }

    // ====== BLOCO SCORE | RECORD (central) ======
    if (fontSmall_) {
        const float centerX = cfg::WindowW * 0.5f;
        const float centerY = cfg::FieldOffsetY + cfg::FieldH + 80.0f;

        const float rectW = 200.0f;
        const float rectH = 80.0f;
        const float rx = centerX - rectW * 0.5f;
        const float ry = centerY - rectH * 0.5f;

        // Fundo preto translúcido
        al_draw_filled_rounded_rectangle(
            rx, ry, rx + rectW, ry + rectH,
            15.0f, 15.0f, al_map_rgba(0, 0, 0, 220)
        );
        // Borda branca
        al_draw_rounded_rectangle(
            rx, ry, rx + rectW, ry + rectH,
            15.0f, 15.0f, al_map_rgb(255, 255, 255), 3.0f
        );
        // Divisória vertical
        al_draw_line(centerX, ry + 15.0f, centerX, ry + rectH - 15.0f,
                     al_map_rgb(100, 100, 100), 2.0f);

        // SCORE (esquerda)
        al_draw_text(fontSmall_, al_map_rgb(200, 200, 200),
                     centerX - 50.0f, ry + 15.0f,
                     ALLEGRO_ALIGN_CENTER, "SCORE");
        al_draw_textf(fontSmall_, al_map_rgb(255, 255, 255),
                      centerX - 50.0f, ry + 40.0f,
                      ALLEGRO_ALIGN_CENTER, "%d", score_);

        // RECORD (direita)
        al_draw_text(fontSmall_, al_map_rgb(200, 200, 200),
                     centerX + 50.0f, ry + 15.0f,
                     ALLEGRO_ALIGN_CENTER, "RECORD");
        al_draw_textf(fontSmall_, al_map_rgb(255, 215, 0),
                      centerX + 50.0f, ry + 40.0f,
                      ALLEGRO_ALIGN_CENTER, "%d", record_);
    }
}