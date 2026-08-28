/**
 * @file Entity.hpp
 * @brief Interface base para todas as entidades do jogo
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once

/**
 * @brief Classe base abstrata para objetos desenháveis/atualizáveis
 * 
 * Fornece a interface comum que todas as entidades do jogo devem implementar
 * seguindo o padrão de game loop update/draw.
 */
class Entity {
public:
    /**
     * @brief Destrutor virtual padrão
     */
    virtual ~Entity() = default;
    
    /**
     * @brief Atualiza o estado da entidade
     * @param dt Tempo delta desde a última atualização (em segundos)
     */
    virtual void update(double dt) {}
    
    /**
     * @brief Desenha a entidade na tela
     */
    virtual void draw() const = 0;
};