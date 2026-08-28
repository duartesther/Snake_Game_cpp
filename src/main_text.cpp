/**
 * @file main_text.cpp
 * @brief Versão otimizada em modo texto do jogo Snake para terminal/CMD
 * @author Esther e Lucas
 * @date 2025
 * 
 * Snake modo texto (CMD) – Windows/MSYS2
 * Controles: setas (↑ ↓ ← →) ou WASD. ESC sai.
 * Objetivo: comer (*) e crescer; bateu nas bordas ou em si mesmo = game over.
 * 
 * Esta versão inclui otimizações para reduzir flickering:
 * - Buffer único para toda a tela
 * - Cursor escondido
 * - Limpeza otimizada da tela
 * - Prevenção de movimento inverso
 */

#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <thread>
#include <conio.h>      // _kbhit, _getch
#include <windows.h>    // Sleep, SetConsoleOutputCP

/**
 * @brief Configurações específicas para versão em texto
 */
namespace txtcfg {
    constexpr int W = 40;        ///< Largura do tabuleiro em caracteres
    constexpr int H = 20;        ///< Altura do tabuleiro em caracteres  
    constexpr int TICK_MS = 120; ///< Velocidade (ms por passo)
}

/**
 * @brief Representa uma posição 2D no grid
 */
struct Vec { 
    int x;  ///< Coordenada X
    int y;  ///< Coordenada Y
};

/**
 * @brief Direções possíveis de movimento
 */
enum class Dir { 
    Up,     ///< Movimento para cima
    Down,   ///< Movimento para baixo  
    Left,   ///< Movimento para esquerda
    Right   ///< Movimento para direita
};

/**
 * @brief Limpa a tela do terminal de forma otimizada (sem flickering)
 * 
 * Usa API do Windows para limpar a tela de forma mais eficiente
 * que system("cls"), reduzindo flickering.
 */
static void clear_screen() {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    // Obtém informações do buffer da tela
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Preenche toda a tela com espaços
    if (!FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count)) return;
    
    // Mantém atributos de cor atuais
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;
    
    // Reposiciona cursor no início
    SetConsoleCursorPosition(hConsole, homeCoords);
}

/**
 * @brief Esconde o cursor do terminal para reduzir flickering
 */
static void hide_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // Torna cursor invisível
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

/**
 * @brief Desenha o estado atual do jogo usando buffer único
 * @param snake Deque contendo os segmentos da cobra
 * @param food Posição atual da comida
 * @param score Pontuação atual do jogador
 * 
 * Usa buffer de string para construir toda a tela de uma vez,
 * reduzindo flickering comparado com múltiplas operações de cout.
 */
static void draw(const std::deque<Vec>& snake, const Vec& food, int score) {
    // Buffer para construir a tela toda de uma vez (reduz flickering)
    std::string screenBuffer;
    
    // HUD - informações do jogo
    screenBuffer += "SNAKE GAME (modo texto) | Score: " + std::to_string(score);
    screenBuffer += "  | Controles: ← → ou A/D (gira), ↑/↓ (opcional), ESC sai\n";
    
    // Cria a grid em memória
    std::vector<std::string> grid(txtcfg::H, std::string(txtcfg::W, ' '));

    // Desenha bordas horizontais
    for (int x = 0; x < txtcfg::W; ++x) {
        grid[0][x] = '#';           // Borda superior
        grid[txtcfg::H-1][x] = '#'; // Borda inferior
    }
    
    // Desenha bordas verticais
    for (int y = 0; y < txtcfg::H; ++y) {
        grid[y][0] = '#';           // Borda esquerda
        grid[y][txtcfg::W-1] = '#'; // Borda direita
    }

    // Desenha comida (se estiver dentro dos limites)
    if (food.y >= 0 && food.y < txtcfg::H && food.x >= 0 && food.x < txtcfg::W)
        grid[food.y][food.x] = '*';

    // Desenha cobra
    for (size_t i = 0; i < snake.size(); ++i) {
        char c = (i == 0 ? '@' : 'o');  // Cabeça é '@', corpo é 'o'
        const auto& v = snake[i];
        
        // Verifica se está dentro dos limites (evita acesso inválido)
        if (v.y > 0 && v.y < txtcfg::H-1 && v.x > 0 && v.x < txtcfg::W-1)
            grid[v.y][v.x] = c;
    }

    // Adiciona a grid ao buffer
    for (int y = 0; y < txtcfg::H; ++y) {
        screenBuffer += grid[y] + '\n';
    }
    
    // Imprime tudo de uma vez (reduz flickering significativamente)
    std::cout << screenBuffer;
}

/**
 * @brief Gera posição aleatória livre no grid
 * @param rng Gerador de números aleatórios
 * @param snake Cobra atual para evitar sobreposição
 * @return Vec com posição livre
 * 
 * Garante que a comida nunca apareça sobre a cobra ou nas bordas.
 */
static Vec random_free_cell(std::mt19937& rng, const std::deque<Vec>& snake) {
    std::uniform_int_distribution<int> rx(1, txtcfg::W-2);  // Evita bordas
    std::uniform_int_distribution<int> ry(1, txtcfg::H-2);  // Evita bordas
    
    while (true) {
        Vec v{ rx(rng), ry(rng) };
        bool clash = false;
        
        // Verifica se a posição não conflita com a cobra
        for (auto& s : snake) {
            if (s.x == v.x && s.y == v.y) { 
                clash = true; 
                break; 
            }
        }
        
        if (!clash) return v;  // Posição livre encontrada
    }
}

/**
 * @brief Ponto de entrada da versão em texto otimizada
 * @return 0 em caso de sucesso
 * 
 * Implementa loop principal do jogo com controles otimizados
 * e prevenção de movimento inverso (impede que cobra vá contra si mesma).
 */
int main() {
    // Configura UTF-8 no Windows para acentuação
    SetConsoleOutputCP(CP_UTF8);
    hide_cursor(); // Esconde o cursor para reduzir flickering

    // Inicializa gerador de números aleatórios
    std::mt19937 rng{ std::random_device{}() };
    
    // Estado inicial da cobra
    std::deque<Vec> snake;
    snake.push_back({ txtcfg::W/2, txtcfg::H/2 });     // Cabeça (centro)
    snake.push_back({ txtcfg::W/2-1, txtcfg::H/2 });   // Primeiro segmento
    snake.push_back({ txtcfg::W/2-2, txtcfg::H/2 });   // Segundo segmento

    Dir dir = Dir::Right;    // Direção inicial (direita)
    bool running = true;     // Controle do loop principal
    bool grow = false;       // Flag para crescimento
    int score = 0;           // Pontuação inicial

    // Posiciona primeira comida
    Vec food = random_free_cell(rng, snake);

    // Tela de ajuda inicial
    clear_screen();
    std::cout <<
        "SNAKE GAME – modo texto\n"
        "Controles:\n"
        "  ←/→ ou A/D: girar esquerda/direita\n"
        "  ↑/↓        : (opcional) mudar para cima/baixo\n"
        "  ESC        : sair\n\n"
        "Pressione qualquer tecla para iniciar...";
    _getch();  // Aguarda entrada do usuário
    
    // Desenha tela inicial do jogo
    clear_screen();
    draw(snake, food, score);

    // ====== LOOP PRINCIPAL DO JOGO ======
    while (running) {
        // 1) INPUT (não-bloqueante)
        if (_kbhit()) {
            int ch = _getch();
            
            // Tecla ESC - sai do jogo
            if (ch == 27) { 
                break;
            }
            
            // Teclas de seta (código especial 224 + código da seta)
            if (ch == 224 && _kbhit()) {
                int a = _getch();
                
                // CORREÇÃO: Prevenção de movimento inverso
                // Impede que a cobra vá na direção oposta instantaneamente
                if (a == 72 && dir == Dir::Down) { // ↑ quando está indo para ↓
                    // Não faz nada - movimento inverso bloqueado
                } else if (a == 80 && dir == Dir::Up) { // ↓ quando está indo para ↑
                    // Não faz nada - movimento inverso bloqueado
                } else if (a == 75 && dir == Dir::Right) { // ← quando está indo para →
                    // Não faz nada - movimento inverso bloqueado
                } else if (a == 77 && dir == Dir::Left) { // → quando está indo para ←
                    // Não faz nada - movimento inverso bloqueado
                } 
                // Movimentos permitidos
                else if (a == 75) { // ←
                    // CORREÇÃO: Inverte os controles quando indo para baixo
                    // Para manter consistência visual com jogos clássicos
                    if (dir == Dir::Down) {
                        // Quando para baixo, ← vira direita (consistência visual)
                        dir = (dir == Dir::Up   ? Dir::Right
                             : dir == Dir::Right ? Dir::Down
                             : dir == Dir::Down ? Dir::Left
                                                : Dir::Up);
                    } else {
                        // Comportamento normal para outras direções
                        dir = (dir == Dir::Up   ? Dir::Left
                             : dir == Dir::Left ? Dir::Down
                             : dir == Dir::Down ? Dir::Right
                                                : Dir::Up);
                    }
                } else if (a == 77) { // →
                    // CORREÇÃO: Inverte os controles quando indo para baixo
                    if (dir == Dir::Down) {
                        // Quando para baixo, → vira esquerda (consistência visual)
                        dir = (dir == Dir::Up   ? Dir::Left
                             : dir == Dir::Left ? Dir::Down
                             : dir == Dir::Down ? Dir::Right
                                                : Dir::Up);
                    } else {
                        // Comportamento normal para outras direções
                        dir = (dir == Dir::Up   ? Dir::Right
                             : dir == Dir::Right ? Dir::Down
                             : dir == Dir::Down ? Dir::Left
                                                : Dir::Up);
                    }
                } else if (a == 72) { // ↑ - movimento direto para cima
                    dir = Dir::Up;
                } else if (a == 80) { // ↓ - movimento direto para baixo
                    dir = Dir::Down;
                }
            } else {
                // Controles WASD - CORREÇÃO: Prevenção de movimento inverso
                if ((ch == 'w' || ch == 'W') && dir == Dir::Down) { // W quando está indo para S
                    // Não faz nada - movimento inverso bloqueado
                } else if ((ch == 's' || ch == 'S') && dir == Dir::Up) { // S quando está indo para W
                    // Não faz nada - movimento inverso bloqueado
                } else if ((ch == 'a' || ch == 'A') && dir == Dir::Right) { // A quando está indo para D
                    // Não faz nada - movimento inverso bloqueado
                } else if ((ch == 'd' || ch == 'D') && dir == Dir::Left) { // D quando está indo para A
                    // Não faz nada - movimento inverso bloqueado
                } 
                // Movimentos permitidos
                else if (ch == 'a' || ch == 'A') {
                    // CORREÇÃO: Inverte os controles quando indo para baixo
                    if (dir == Dir::Down) {
                        // Quando para baixo, A vira direita
                        dir = (dir == Dir::Up   ? Dir::Right
                             : dir == Dir::Right ? Dir::Down
                             : dir == Dir::Down ? Dir::Left
                                                : Dir::Up);
                    } else {
                        // Comportamento normal para outras direções
                        dir = (dir == Dir::Up   ? Dir::Left
                             : dir == Dir::Left ? Dir::Down
                             : dir == Dir::Down ? Dir::Right
                                                : Dir::Up);
                    }
                } else if (ch == 'd' || ch == 'D') {
                    // CORREÇÃO: Inverte os controles quando indo para baixo
                    if (dir == Dir::Down) {
                        // Quando para baixo, D vira esquerda
                        dir = (dir == Dir::Up   ? Dir::Left
                             : dir == Dir::Left ? Dir::Down
                             : dir == Dir::Down ? Dir::Right
                                                : Dir::Up);
                    } else {
                        // Comportamento normal para outras direções
                        dir = (dir == Dir::Up   ? Dir::Right
                             : dir == Dir::Right ? Dir::Down
                             : dir == Dir::Down ? Dir::Left
                                                : Dir::Up);
                    }
                } else if (ch == 'w' || ch == 'W') { // W - movimento direto para cima
                    dir = Dir::Up;
                } else if (ch == 's' || ch == 'S') { // S - movimento direto para baixo
                    dir = Dir::Down;
                }
            }
        }

        // 2) UPDATE - Lógica do jogo
        Vec head = snake.front();  // Posição atual da cabeça
        
        // Calcula nova posição baseada na direção
        switch (dir) {
            case Dir::Up:    head.y -= 1; break;    // Move para cima
            case Dir::Down:  head.y += 1; break;    // Move para baixo
            case Dir::Left:  head.x -= 1; break;    // Move para esquerda
            case Dir::Right: head.x += 1; break;    // Move para direita
        }

        // Verifica colisão com bordas
        if (head.x <= 0 || head.x >= txtcfg::W-1 ||
            head.y <= 0 || head.y >= txtcfg::H-1) {
            running = false;  // Game over por colisão com borda
        }

        // Verifica colisão com próprio corpo
        for (auto& s : snake) {
            if (s.x == head.x && s.y == head.y) { 
                running = false;  // Game over por colisão consigo mesma
                break; 
            }
        }

        // Sai do loop se game over
        if (!running) break;

        // Move cobra: adiciona nova cabeça
        snake.push_front(head);

        // Verifica se comeu comida
        if (head.x == food.x && head.y == food.y) {
            score += 1;                           // Incrementa score
            food = random_free_cell(rng, snake);  // Nova comida
            grow = true;                          // Ativa crescimento
        }
        
        // Remove cauda apenas se não cresceu
        if (!grow) {
            snake.pop_back();
        }
        grow = false;  // Reseta flag de crescimento

        // 3) DRAW - Renderização otimizada
        clear_screen();
        draw(snake, food, score);

        // 4) TICK - Controle de velocidade
        std::this_thread::sleep_for(std::chrono::milliseconds(txtcfg::TICK_MS));
    }

    // ====== TELA DE GAME OVER ======
    clear_screen();
    std::cout << "GAME OVER! Score final: " << score << "\n";
    std::cout << "Pressione qualquer tecla para sair...";
    _getch();  // Aguarda entrada final do usuário
    
    return 0;
}