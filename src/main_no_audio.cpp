/**
 * @file main_no_audio.cpp
 * @brief Versão sem áudio para Docker
 * @author Esther e Lucas
 * @date 2025
 */

#include "Game.hpp"
#include <cstdio>
#include <exception>
#include <fstream>

/**
 * @brief Função principal sem inicialização de áudio
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int main() {
    // Detecta se está no Docker e mostra mensagem
    std::ifstream dockercheck("/.dockerenv");
    bool isDocker = dockercheck.good();
    dockercheck.close();
    
    if (isDocker) {
        std::printf("🐍 Snake Game - Modo Docker (Áudio desativado)\n");
    }
    
    try {
        Game game;      ///< Instância principal do jogo
        game.run();     ///< Executa o loop principal
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Erro fatal: %s\n", e.what());
        return 1;
    } catch (...) {
        std::fprintf(stderr, "Erro fatal desconhecido.\n");
        return 1;
    }
    return 0;
}