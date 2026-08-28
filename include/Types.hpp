/**
 * @file Types.hpp
 * @brief Configurações globais, estruturas de dados e utilitários
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include "Vector2D.hpp"
#include <cstdint>

/**
 * @brief Namespace contendo todas as configurações globais do jogo
 */
namespace cfg {
    // Tamanho da janela
    inline constexpr int WindowW = 600;  ///< Largura da janela
    inline constexpr int WindowH = 800;  ///< Altura da janela

    // Tamanho do campo (grid)
    inline constexpr int FieldW  = 400;  ///< Largura do campo de jogo
    inline constexpr int FieldH  = 400;  ///< Altura do campo de jogo

    // Reservas de espaço para HUD
    inline constexpr int TopHUD    = 70;  ///< Espaço superior para HUD
    inline constexpr int BottomHUD = 90;  ///< Espaço inferior para HUD

    // Centralização do campo:
    inline constexpr int FieldOffsetX = (WindowW - FieldW) / 2;  ///< Offset X do campo
    inline constexpr int FieldOffsetY = TopHUD + ( (WindowH - TopHUD - BottomHUD - FieldH) / 2 );  ///< Offset Y do campo

    // Relógio do jogo
    inline constexpr int TimerFps = 60;  ///< FPS do timer principal

    // Passo de crescimento da cobra
    inline constexpr int SnakeGrowthStep = 8;  ///< Segmentos adicionados ao crescer
}

/**
 * @brief Limita um valor entre mínimo e máximo
 * @tparam T Tipo do valor
 * @param v Valor a ser limitado
 * @param lo Limite inferior
 * @param hi Limite superior
 * @return Valor limitado entre lo e hi
 */
template <class T>
inline T clampv(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

/**
 * @brief Presets de cores disponíveis para a cobra
 */
enum class SnakeColorPreset { 
    Red,    ///< Esquema de cores vermelho
    Green,  ///< Esquema de cores verde  
    Blue,   ///< Esquema de cores azul
    Purple, ///< Esquema de cores roxo
    Orange  ///< Esquema de cores laranja
};

/**
 * @brief Template para tripla de cores RGB
 * @tparam C Tipo dos componentes de cor (uint8_t, float, etc)
 */
template <typename C>
struct ColorTriplet { 
    C r{};  ///< Componente vermelho
    C g{};  ///< Componente verde  
    C b{};  ///< Componente azul
};

/**
 * @brief Define uma paleta completa de cores para a cobra
 */
struct SnakePalette {
    ColorTriplet<uint8_t> bodyDark;   ///< Cor escura do corpo
    ColorTriplet<uint8_t> bodyMid;    ///< Cor média do corpo
    ColorTriplet<uint8_t> bodyLight;  ///< Cor clara do corpo
    ColorTriplet<uint8_t> headLight;  ///< Cor clara da cabeça
    ColorTriplet<uint8_t> eyeWhite{255,255,255};  ///< Branco dos olhos
    ColorTriplet<uint8_t> eyeBlack{20,20,20};     ///< Preto das pupilas
};

/**
 * @brief Converte preset enum para estrutura de paleta de cores
 * @param p Preset de cor desejado
 * @return SnakePalette correspondente ao preset
 */
inline SnakePalette presetToPalette(SnakeColorPreset p) {
    switch (p) {
        case SnakeColorPreset::Green:  return {{  0,120,  0},{ 20,220, 20},{100,255,100},{120,255,120}};
        case SnakeColorPreset::Blue:   return {{  0,  0,120},{ 20, 20,220},{100,100,255},{120,120,255}};
        case SnakeColorPreset::Purple: return {{120,  0,120},{200, 20,200},{255,100,255},{255,120,255}};
        case SnakeColorPreset::Orange: return {{140, 60,  0},{220,120, 20},{255,180,100},{255,200,120}};
        default: /*Red*/               return {{140,  0,  0},{220, 20, 20},{255,100,100},{255,120,120}};
    }
}