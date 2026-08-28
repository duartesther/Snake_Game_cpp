/**
 * @file Snake.hpp
 * @brief Representação da cobra jogável
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include "Types.hpp"
#include <allegro5/allegro.h>
#include <vector>
#include <cmath>

/**
 * @brief Representa um segmento individual da cobra
 */
struct Segment {
    Vector2D pos{};         ///< Posição do segmento
    double   size{8.0};     ///< Tamanho/raio do segmento  
    double   angleDeg{0.0}; ///< Ângulo de rotação em graus
};

/**
 * @brief Classe principal da cobra jogável
 * 
 * Gerencia movimento, crescimento, colisões e renderização
 * da cobra com sistema de segmentos interconectados.
 */
class Snake {
public:
    /**
     * @brief Constrói a cobra com sprites específicos
     * @param headBmp Sprite da cabeça
     * @param bodyBmp Sprite do corpo
     * @param tailBmp Sprite da cauda
     */
    Snake(ALLEGRO_BITMAP* headBmp, ALLEGRO_BITMAP* bodyBmp, ALLEGRO_BITMAP* tailBmp);

    /**
     * @brief Vira a cobra para esquerda
     */
    void turnLeft();
    
    /**
     * @brief Vira a cobra para direita  
     */
    void turnRight();
    
    /**
     * @brief Atualiza posição e estado da cobra
     * @param dt Tempo delta desde última atualização
     */
    void update(double dt);
    
    /**
     * @brief Desenha a cobra na tela
     */
    void draw() const;

    /**
     * @brief Aumenta tamanho da cobra e score
     */
    void grow();
    
    /**
     * @brief Verifica colisão da cabeça com o corpo
     * @return true se houve colisão consigo mesma
     */
    bool checkSelfCollision() const;

    // Métodos de acesso
    /**
     * @brief Obtém posição da cabeça
     * @return Vector2D com posição da cabeça
     */
    Vector2D headPosition() const { return body_.empty() ? Vector2D{0,0} : body_[0].pos; }
    
    /**
     * @brief Obtém raio da cabeça
     * @return Raio do segmento da cabeça
     */
    double   radius() const       { return body_.empty() ? 0.0 : body_[0].size; }
    
    /**
     * @brief Obtém score atual
     * @return Score do jogador
     */
    int      score() const        { return score_; }

    /**
     * @brief Define esquema de cores da cobra
     * @param p Preset de cores a ser aplicado
     */
    void setColorPreset(SnakeColorPreset p) { colorPreset_ = p; }
    
    /**
     * @brief Alias para setColorPreset para compatibilidade
     * @param p Preset de cores a ser aplicado
     */
    void setPreset(SnakeColorPreset p)      { setColorPreset(p); }

private:
    /**
     * @brief Converte graus para radianos
     * @param d Ângulo em graus
     * @return Ângulo em radianos
     */
    static double degToRad(double d) { return d * 3.14159265358979323846 / 180.0; }
    
    /**
     * @brief Ajusta coordenada para wrapping no campo
     * @param v Valor da coordenada
     * @param offset Offset do campo
     * @param span Comprimento do campo
     * @return Coordenada ajustada com wrapping
     */
    static double wrapCoord(double v, double offset, double span) {
        const double rel = v - offset;
        double w = std::fmod(rel, span);
        if (w < 0) w += span;
        return offset + w;
    }

    /**
     * @brief Obtém passo de desenho do corpo
     * @return Número de índices para pular entre segmentos desenhados
     */
    int bodyDrawStepIndices() const { return 2; }

private:
    ALLEGRO_BITMAP* headBmp_{};        ///< Sprite da cabeça
    ALLEGRO_BITMAP* bodyBmp_{};        ///< Sprite do corpo  
    ALLEGRO_BITMAP* tailBmp_{};        ///< Sprite da cauda

    std::vector<Segment> body_;        ///< Segmentos da cobra
    int    storedSize_{256};           ///< Tamanho máximo armazenado
    int    visibleSize_{12};           ///< Tamanho visível atual
    double headAngleDeg_{0.0};         ///< Ângulo atual da cabeça
    double rotationSpeed_{4.0};        ///< Velocidade de rotação
    double speedModule_{3.0};          ///< Velocidade de movimento
    int    score_{0};                  ///< Score do jogador

    SnakeColorPreset colorPreset_{SnakeColorPreset::Red};  ///< Esquema de cores
};