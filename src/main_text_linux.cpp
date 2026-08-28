/**
 * @file main_text_linux.cpp
 * @brief Versão Linux otimizada do jogo Snake para terminal
 * @author Esther e Lucas
 * @date 2025
 */

#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <csignal>  
#include <cstdlib> 

namespace txtcfg {
    constexpr int W = 40;
    constexpr int H = 20;
    constexpr int TICK_MS = 120;
}

struct Vec { int x; int y; };
enum class Dir { Up, Down, Left, Right };

static struct termios old_tio;

/**
 * @brief Configura terminal para modo não-bloqueante
 */
static void set_nonblocking_mode() {
    struct termios new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    new_tio.c_cc[VMIN] = 0;
    new_tio.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

/**
 * @brief Restaura configurações originais do terminal
 */
static void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    std::cout << "\033[?25h";  // Mostra cursor
}

/**
 * @brief Limpa a tela do terminal
 */
static void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

/**
 * @brief Esconde o cursor do terminal
 */
static void hide_cursor() {
    std::cout << "\033[?25l";
}

/**
 * @brief Verifica se há tecla pressionada
 */
static bool kbhit() {
    int byteswaiting;
    ioctl(STDIN_FILENO, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

/**
 * @brief Lê um caractere do input
 */
static char getch() {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) == 1) return ch;
    return 0;
}

/**
 * @brief Desenha o estado atual do jogo
 */
static void draw(const std::deque<Vec>& snake, const Vec& food, int score) {
    clear_screen();
    
    // HUD
    std::cout << "SNAKE GAME (modo texto) | Score: " << score;
    std::cout << " | Controles: ← → ou A/D, ESC sai\n";
    
    // Desenha grid com bordas
    for (int y = 0; y < txtcfg::H; ++y) {
        for (int x = 0; x < txtcfg::W; ++x) {
            // Borda
            if (x == 0 || x == txtcfg::W-1 || y == 0 || y == txtcfg::H-1) {
                std::cout << '#';
                continue;
            }
            
            // Comida
            if (x == food.x && y == food.y) {
                std::cout << '*';
                continue;
            }
            
            // Cobra
            bool isSnake = false;
            for (size_t i = 0; i < snake.size(); ++i) {
                if (snake[i].x == x && snake[i].y == y) {
                    std::cout << (i == 0 ? '@' : 'o');
                    isSnake = true;
                    break;
                }
            }
            
            // Espaço vazio
            if (!isSnake) std::cout << ' ';
        }
        std::cout << '\n';
    }
    
    std::cout << std::flush;
}

/**
 * @brief Gera posição aleatória livre no grid
 */
static Vec random_free_cell(std::mt19937& rng, const std::deque<Vec>& snake) {
    std::uniform_int_distribution<int> rx(1, txtcfg::W-2);
    std::uniform_int_distribution<int> ry(1, txtcfg::H-2);
    
    while (true) {
        Vec v{ rx(rng), ry(rng) };
        bool clash = false;
        
        for (auto& s : snake) {
            if (s.x == v.x && s.y == v.y) { 
                clash = true; 
                break; 
            }
        }
        
        if (!clash) return v;
    }
}

/**
 * @brief Ponto de entrada
 */
int main() {
    // Configura terminal
    set_nonblocking_mode();
    hide_cursor();
    
    // Garante limpeza ao sair
    auto cleanup = [&]() {
        restore_terminal();
        clear_screen();
        std::cout << "Jogo finalizado!\n";
    };
    
    // Configura sinal para limpeza no Ctrl+C
    std::signal(SIGINT, [](int) { std::exit(0); });

    std::mt19937 rng{ std::random_device{}() };
    
    // Estado inicial
    std::deque<Vec> snake;
    snake.push_back({ txtcfg::W/2, txtcfg::H/2 });
    snake.push_back({ txtcfg::W/2-1, txtcfg::H/2 });
    snake.push_back({ txtcfg::W/2-2, txtcfg::H/2 });

    Dir dir = Dir::Right;
    bool running = true;
    bool grow = false;
    int score = 0;

    Vec food = random_free_cell(rng, snake);

    // Tela inicial
    clear_screen();
    std::cout <<
        "SNAKE GAME – modo texto\n"
        "Controles:\n"
        "  ←/→ ou A/D: girar esquerda/direita\n" 
        "  ↑/↓        : mudar direção\n"
        "  ESC        : sair\n\n"
        "Pressione qualquer tecla para iniciar...";
    std::cout.flush();
    
    // Aguarda entrada
    while (!kbhit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    getch();

    // Loop principal
    while (running) {
        // INPUT
        if (kbhit()) {
            char ch = getch();
            
            if (ch == 27) { // ESC
                // Verifica se é sequência de escape (setas)
                if (kbhit()) {
                    getch(); // Descarta '['
                    char arrow = getch();
                    
                    if (arrow == 'A' && dir != Dir::Down) dir = Dir::Up;
                    else if (arrow == 'B' && dir != Dir::Up) dir = Dir::Down;
                    else if (arrow == 'D' && dir != Dir::Right) dir = Dir::Left;
                    else if (arrow == 'C' && dir != Dir::Left) dir = Dir::Right;
                } else {
                    break; // ESC sozinho - sai
                }
            } else {
                // Teclas normais
                if ((ch == 'w' || ch == 'W') && dir != Dir::Down) dir = Dir::Up;
                else if ((ch == 's' || ch == 'S') && dir != Dir::Up) dir = Dir::Down;
                else if ((ch == 'a' || ch == 'A') && dir != Dir::Right) dir = Dir::Left;
                else if ((ch == 'd' || ch == 'D') && dir != Dir::Left) dir = Dir::Right;
                else if (ch == 'q' || ch == 'Q') break;
            }
        }

        // UPDATE
        Vec head = snake.front();
        
        switch (dir) {
            case Dir::Up:    head.y -= 1; break;
            case Dir::Down:  head.y += 1; break;
            case Dir::Left:  head.x -= 1; break;
            case Dir::Right: head.x += 1; break;
        }

        // Colisão com bordas
        if (head.x <= 0 || head.x >= txtcfg::W-1 ||
            head.y <= 0 || head.y >= txtcfg::H-1) {
            running = false;
        }

        // Colisão com corpo
        for (auto& s : snake) {
            if (s.x == head.x && s.y == head.y) {
                running = false;
                break;
            }
        }

        if (!running) break;

        // Move cobra
        snake.push_front(head);

        // Comeu comida?
        if (head.x == food.x && head.y == food.y) {
            score += 1;
            food = random_free_cell(rng, snake);
            grow = true;
        }
        
        if (!grow) snake.pop_back();
        grow = false;

        // DRAW
        draw(snake, food, score);

        // WAIT
        std::this_thread::sleep_for(std::chrono::milliseconds(txtcfg::TICK_MS));
    }

    // Game over
    cleanup();
    std::cout << "GAME OVER! Score final: " << score << "\n";
    
    return 0;
}