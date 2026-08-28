/**
 * @file Game.cpp
 * @brief Implementação da classe Game
 * @author Esther e Lucas
 * @date 2025
 */

#include "Game.hpp"
#include <stdexcept>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <fstream>  

/**
 * @brief Verifica se está executando em container Docker
 * @return true se está em container Docker
 */
bool isDockerEnvironment() {
    std::ifstream dockercheck("/.dockerenv");
    bool isDocker = dockercheck.good();
    dockercheck.close();
    return isDocker;
}

/**
 * @brief Constrói e inicializa todos os sistemas do jogo
 * @throws std::runtime_error se qualquer inicialização falhar
 */
Game::Game() {
    // Inicializa Allegro e addons
    if (!al_init())                   throw std::runtime_error("al_init fail");
    if (!al_install_keyboard())       throw std::runtime_error("keyboard fail");
    if (!al_init_image_addon())       throw std::runtime_error("image fail");
    if (!al_init_font_addon())        throw std::runtime_error("font fail");
    if (!al_init_ttf_addon())         throw std::runtime_error("ttf fail");
    if (!al_init_primitives_addon())  throw std::runtime_error("primitives fail");

    // Cria display
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display_ = al_create_display(cfg::WindowW, cfg::WindowH);
    if (!display_) throw std::runtime_error("create display fail");

    // Cria timer e fila de eventos
    queue_ = al_create_event_queue();
    timer_ = al_create_timer(1.0 / cfg::TimerFps);
    if (!queue_ || !timer_) throw std::runtime_error("core fail");

    // Registra fontes de eventos
    al_register_event_source(queue_, al_get_display_event_source(display_));
    al_register_event_source(queue_, al_get_timer_event_source(timer_));
    al_register_event_source(queue_, al_get_keyboard_event_source());

    // Carrega bitmaps
    ALLEGRO_BITMAP* fieldBmp   = bmpManager_.get("assets/Images/field.png");
    ALLEGRO_BITMAP* appleBmp   = bmpManager_.get("assets/Images/apple.png");
    ALLEGRO_BITMAP* headBmp    = bmpManager_.get("assets/Images/snake_head.png");
    ALLEGRO_BITMAP* bodyBmp    = bmpManager_.get("assets/Images/snake_body.png");
    ALLEGRO_BITMAP* tailBmp    = bmpManager_.get("assets/Images/snake_tail.png");
    ALLEGRO_BITMAP* consoleBmp = bmpManager_.get("assets/Images/console.png");
    ALLEGRO_BITMAP* kbNone     = bmpManager_.get("assets/Images/console_keyboard.png");
    ALLEGRO_BITMAP* kbLeft     = bmpManager_.get("assets/Images/console_keyboard_left.png");
    ALLEGRO_BITMAP* kbRight    = bmpManager_.get("assets/Images/console_keyboard_right.png");

    // Carrega fontes
    fontSmall_ = fontManager_.get("assets/Fonts/arial.ttf@20");
    fontBig_   = fontManager_.get("assets/Fonts/arial.ttf@50");

    // Cria entidades do mundo
    field_ = std::make_unique<Field>(fieldBmp);
    apple_ = std::make_unique<Apple>(appleBmp);
    snake_ = std::make_unique<Snake>(headBmp, bodyBmp, tailBmp);
    hud_   = std::make_unique<ConsoleHUD>(consoleBmp, kbNone, kbLeft, kbRight, fontSmall_, fontBig_);
    snake_->setColorPreset(colorPreset_);
    
    // Inicializa áudio
    sound_ = new SoundBank(
        "assets/Sounds/eat.wav",
        "assets/Sounds/game_over.wav",
        "assets/Sounds/music.wav"
    );
    
    // Só inicia música se não estiver em Docker e se o áudio estiver ok
    bool dockerEnv = isDockerEnvironment();
    if (sound_ && sound_->ok() && !dockerEnv) {
        sound_->startMusic(0.35f);
    }
    
    if (dockerEnv) {
        // Modo Docker - desativa mute automaticamente para evitar tentativas de áudio
        if (sound_) {
            sound_->setMusicMuted(true);
            sound_->setFxMuted(true);
        }
    }

    al_start_timer(timer_);
}

/**
 * @brief Destrutor que libera todos os recursos
 */
Game::~Game() {
    delete sound_; sound_ = nullptr;
    if (timer_)   al_destroy_timer(timer_);
    if (queue_)   al_destroy_event_queue(queue_);
    if (display_) al_destroy_display(display_);
}

/**
 * @brief Reinicia o jogo para estado inicial
 */
void Game::resetGame() {
    paused_    = false;
    leftHeld_  = false;
    rightHeld_ = false;
    state_     = GameState::Playing;

    // Recria a cobra
    ALLEGRO_BITMAP* headBmp = bmpManager_.get("assets/Images/snake_head.png");
    ALLEGRO_BITMAP* bodyBmp = bmpManager_.get("assets/Images/snake_body.png");
    ALLEGRO_BITMAP* tailBmp = bmpManager_.get("assets/Images/snake_tail.png");
    snake_ = std::make_unique<Snake>(headBmp, bodyBmp, tailBmp);
    snake_->setColorPreset(colorPreset_);

    // Reposiciona maçã e reseta HUD
    apple_->spawnRandom();
    hud_->setScore(0);
    hud_->setHighScore(highScore_);
}

/**
 * @brief Cicla entre os esquemas de cores da cobra
 */
void Game::cycleSnakeColor() {
    switch (colorPreset_) {
        case SnakeColorPreset::Red:    colorPreset_ = SnakeColorPreset::Green;  break;
        case SnakeColorPreset::Green:  colorPreset_ = SnakeColorPreset::Blue;   break;
        case SnakeColorPreset::Blue:   colorPreset_ = SnakeColorPreset::Purple; break;
        case SnakeColorPreset::Purple: colorPreset_ = SnakeColorPreset::Orange; break;
        case SnakeColorPreset::Orange: colorPreset_ = SnakeColorPreset::Red;    break;
    }
    if (snake_) snake_->setColorPreset(colorPreset_);
}

/**
 * @brief Executa o loop principal do jogo
 */
void Game::run() {
    while (running_) {
        ALLEGRO_EVENT ev{};
        al_wait_for_event(queue_, &ev);
        handleEvent(ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            updateLogic();
            render();
        }
    }
}

/**
 * @brief Processa eventos do Allegro
 * @param ev Evento a ser processado
 */
void Game::handleEvent(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 
        running_ = false; 
        return; 
    }

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        const int k = ev.keyboard.keycode;

        if (k == ALLEGRO_KEY_ESCAPE) { 
            running_ = false; 
            return; 
        }

        // Estado Menu
        if (state_ == GameState::Menu) {
            if (k == ALLEGRO_KEY_ENTER) {
                bool dockerEnv = isDockerEnvironment();
                if (sound_ && sound_->ok() && !sound_->musicMuted() && !dockerEnv) {
                    sound_->restartMusic(0.35f);
                }
                resetGame();
            }
            else if (k == ALLEGRO_KEY_C) cycleSnakeColor();
            else if (k == ALLEGRO_KEY_M && sound_) sound_->setMusicMuted(!sound_->musicMuted());
            else if (k == ALLEGRO_KEY_N && sound_) sound_->setFxMuted(!sound_->fxMuted());
            return;
        }

        // Estado GameOver
        if (state_ == GameState::GameOver) {
            if (k == ALLEGRO_KEY_ENTER) { 
                state_ = GameState::Menu;
            }
            return;
        }

        // Estado Playing
        switch (k) {
            case ALLEGRO_KEY_LEFT:  
                leftHeld_  = true; 
                hud_->setKeyState(KeyState::Left);  
                break;
            case ALLEGRO_KEY_RIGHT: 
                rightHeld_ = true; 
                hud_->setKeyState(KeyState::Right); 
                break;
            case ALLEGRO_KEY_P:
                paused_ = !paused_;
                if (sound_ && sound_->ok() && sound_->musicLoaded()) {
                    bool dockerEnv = isDockerEnvironment();
                    if (paused_) {
                        sound_->stopMusic();
                    } else {
                        if (!sound_->musicMuted() && !dockerEnv) {
                            sound_->startMusic(0.35f);
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_C: cycleSnakeColor(); break;
            case ALLEGRO_KEY_M: if (sound_) sound_->setMusicMuted(!sound_->musicMuted()); break;
            case ALLEGRO_KEY_N: if (sound_) sound_->setFxMuted(!sound_->fxMuted()); break;
            default: break;
        }
    } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)  leftHeld_  = false;
        if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) rightHeld_ = false;

        // Atualiza estado visual das teclas
        if (leftHeld_ && !rightHeld_)      hud_->setKeyState(KeyState::Left);
        else if (rightHeld_ && !leftHeld_) hud_->setKeyState(KeyState::Right);
        else                                hud_->setKeyState(KeyState::None);
    }
}

/**
 * @brief Atualiza a lógica do jogo
 */
void Game::updateLogic() {
    if (state_ != GameState::Playing || paused_) return;

    // Controles da cobra
    if (leftHeld_  && !rightHeld_) snake_->turnRight();
    if (rightHeld_ && !leftHeld_)  snake_->turnLeft();

    snake_->update(0.0);

    // Verifica colisão consigo mesma
    if (snake_->checkSelfCollision()) {
        state_ = GameState::GameOver;
        hud_->setKeyState(KeyState::None);
        if (sound_ && sound_->ok()) { 
            sound_->playGameOver(1.0f); 
            sound_->stopMusic();
        }

        // Atualiza recorde
        highScore_ = std::max(highScore_, snake_->score()/10);
        return;
    }

    // Verifica se comeu maçã
    Vector2D h = snake_->headPosition();
    Vector2D a = apple_->position();
    const double dx = h.x - a.x, dy = h.y - a.y;
    const double eatR = apple_->radius() + 7.8;

    if (dx*dx + dy*dy < eatR*eatR) {
        snake_->grow();
        apple_->spawnRandom();
        if (sound_ && sound_->ok()) sound_->playEat();
    }
    
    // Atualiza HUD
    hud_->setScore(snake_->score()/10);
    hud_->setHighScore(highScore_);
}

/**
 * @brief Renderiza o frame atual do jogo
 */
void Game::render() {
    al_clear_to_color(al_map_rgb(255,0,0));
    field_->draw();

    // Aplica clipping no campo de jogo
    al_set_clipping_rectangle(cfg::FieldOffsetX, cfg::FieldOffsetY, cfg::FieldW, cfg::FieldH);

    if (state_ == GameState::Playing) {
        apple_->draw();
        snake_->draw();
    }

    al_reset_clipping_rectangle();
    hud_->draw();

    // Overlay de Menu
    if (state_ == GameState::Menu && fontSmall_) {
        const char* line1 = "PRESS ENTER TO START";
        const char* line2 = "PRESS ESC TO EXIT";
        const char* line3 = "C: change color | M: mute music | N: mute FX";

        const float cx = cfg::WindowW * 0.5f;
        const float cy = cfg::WindowH * 0.5f;

        const int textW1 = al_get_text_width(fontSmall_, line1);
        const int textW2 = al_get_text_width(fontSmall_, line2);
        const int textW3 = al_get_text_width(fontSmall_, line3);
        const int maxTextW = std::max(std::max(textW1, textW2), textW3);
        
        const float rw = maxTextW + 60.0f;
        const float rh = 140.0f;
        const float rx = cx - rw * 0.5f;
        const float ry = cy - rh * 0.5f;

        // Fundo preto translúcido
        al_draw_filled_rounded_rectangle(
            rx, ry, rx + rw, ry + rh,
            15.0f, 15.0f, al_map_rgba(0, 0, 0, 230)
        );
        // Borda vermelha
        al_draw_rounded_rectangle(
            rx, ry, rx + rw, ry + rh,
            15.0f, 15.0f, al_map_rgb(255, 50, 50), 4.0f
        );

        const float totalTextHeight = 60.0f;
        const float startY = cy - totalTextHeight * 0.5f;

        al_draw_text(fontSmall_, al_map_rgb(255, 255, 255),
                     cx, startY, ALLEGRO_ALIGN_CENTER, line1);
        al_draw_text(fontSmall_, al_map_rgb(255, 255, 255),
                     cx, startY + 20.0f, ALLEGRO_ALIGN_CENTER, line2);
        al_draw_text(fontSmall_, al_map_rgb(255, 255, 255),
                     cx, startY + 40.0f, ALLEGRO_ALIGN_CENTER, line3);
    }

    // Overlay de Game Over
    if (state_ == GameState::GameOver && fontSmall_) {
        const char* msg = "GAME OVER - PRESS ENTER TO RESTART";

        const float cx = cfg::WindowW * 0.5f;
        const float cy = cfg::WindowH * 0.5f;

        const int textW = al_get_text_width(fontSmall_, msg);
        const float rw = textW + 40.0f;
        const float rh = 60.0f;
        const float rx = cx - rw * 0.5f;
        const float ry = cy - rh * 0.5f;

        al_draw_filled_rounded_rectangle(
            rx, ry, rx + rw, ry + rh,
            15.0f, 15.0f, al_map_rgba(0, 0, 0, 230)
        );
        al_draw_rounded_rectangle(
            rx, ry, rx + rw, ry + rh,
            15.0f, 15.0f, al_map_rgb(255, 50, 50), 4.0f
        );
        al_draw_text(fontSmall_, al_map_rgb(255, 255, 255),
                     cx, cy - 8.0f, ALLEGRO_ALIGN_CENTER, msg);
    }

    al_flip_display();
}