/**
 * @file main.cpp
 * @brief Ponto de entrada principal do jogo Snake
 * @author Esther e Lucas
 * @date 2025
 */

#include "Game.hpp"
#include <cstdio>
#include <exception>

/**
 * @brief Função principal do programa
 * @return 0 em caso de sucesso, 1 em caso de erro
 * 
 * Inicializa e executa o jogo, capturando exceções para
 * garantir tratamento adequado de erros.
 */
int main() {
    try {
        Game game;      ///< Instância principal do jogo
        game.run();     ///< Executa o loop principal
    } catch (const std::exception& e) {
        // Captura e exibe exceções conhecidas
        std::fprintf(stderr, "Erro fatal: %s\n", e.what());
        return 1;
    } catch (...) {
        // Captura exceções desconhecidas
        std::fprintf(stderr, "Erro fatal desconhecido.\n");
        return 1;
    }
    return 0;
}