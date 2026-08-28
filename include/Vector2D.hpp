/**
 * @file Vector2D.hpp
 * @brief Representação matemática de vetor 2D
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <cmath>

/**
 * @brief Representa um vetor bidimensional com operações matemáticas
 * 
 * Fornece operações vetoriais básicas como soma, subtração, produto escalar,
 * normalização, etc. Usado para posicionamento e movimentação no jogo.
 */
struct Vector2D {
    double x{0.0};  ///< Coordenada X
    double y{0.0};  ///< Coordenada Y

    Vector2D() = default;
    
    /**
     * @brief Construtor com coordenadas específicas
     * @param x_ Coordenada X
     * @param y_ Coordenada Y
     */
    Vector2D(double x_, double y_) : x(x_), y(y_) {}

    // Operadores aritméticos
    Vector2D operator+(const Vector2D& o) const { return {x + o.x, y + o.y}; }
    Vector2D operator-(const Vector2D& o) const { return {x - o.x, y - o.y}; }
    Vector2D operator*(double s) const { return {x * s, y * s}; }
    Vector2D operator/(double s) const { return {x / s, y / s}; }

    Vector2D& operator+=(const Vector2D& o){ x += o.x; y += o.y; return *this; }
    Vector2D& operator-=(const Vector2D& o){ x -= o.x; y -= o.y; return *this; }
    Vector2D& operator*=(double s){ x *= s; y *= s; return *this; }
    Vector2D& operator/=(double s){ x /= s; y /= s; return *this; }

    bool operator==(const Vector2D& o) const { return x==o.x && y==o.y; }
    bool operator!=(const Vector2D& o) const { return !(*this==o); }

    /**
     * @brief Calcula produto escalar com outro vetor
     * @param o Outro vetor
     * @return Resultado do produto escalar
     */
    double dot(const Vector2D& o) const { return x*o.x + y*o.y; }
    
    /**
     * @brief Calcula norma ao quadrado (mais eficiente que norm())
     * @return Norma ao quadrado do vetor
     */
    double norm2() const { return x*x + y*y; }
    
    /**
     * @brief Calcula norma (magnitude) do vetor
     * @return Norma do vetor
     */
    double norm() const { return std::sqrt(norm2()); }
};

/**
 * @brief Multiplicação escalar por vetor (comutativa)
 * @param s Escalar
 * @param v Vetor
 * @return Vetor escalado
 */
inline Vector2D operator*(double s, const Vector2D& v){ return {v.x*s, v.y*s}; }