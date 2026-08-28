/**
 * @file AllegroLoaders.hpp
 * @brief Loaders e unloaders para recursos Allegro (bitmaps e fonts)
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

/**
 * @brief Loader para bitmaps Allegro
 * 
 * Carrega um bitmap a partir de um caminho de arquivo
 */
struct BitmapLoader {
    /**
     * @brief Carrega bitmap do arquivo especificado
     * @param path Caminho para o arquivo de imagem
     * @return Ponteiro para ALLEGRO_BITMAP ou nullptr em caso de erro
     */
    ALLEGRO_BITMAP* operator()(const std::string& path) const {
        return al_load_bitmap(path.c_str());
    }
};

/**
 * @brief Unloader para bitmaps Allegro
 * 
 * Libera a memória de um bitmap carregado
 */
struct BitmapUnloader {
    /**
     * @brief Destroi um bitmap Allegro
     * @param bmp Ponteiro para o bitmap a ser destruído
     */
    void operator()(ALLEGRO_BITMAP* bmp) const {
        if (bmp) al_destroy_bitmap(bmp);
    }
};

/**
 * @brief Loader para fonts Allegro com especificação de tamanho
 * 
 * Suporta formato "caminho.ttf@TAMANHO" para carregar fonts TTF
 */
struct FontSpecLoader {
    /**
     * @brief Carrega font TTF com tamanho especificado
     * @param spec Especificação no formato "caminho.ttf@TAMANHO"
     * @return Ponteiro para ALLEGRO_FONT ou nullptr em caso de erro
     */
    ALLEGRO_FONT* operator()(const std::string& spec) const {
        auto at = spec.find('@');
        std::string path = spec;
        int sz = 16;
        if (at != std::string::npos) {
            path = spec.substr(0, at);
            sz = std::stoi(spec.substr(at + 1));
        }
        return al_load_ttf_font(path.c_str(), sz, 0);
    }
};

/**
 * @brief Unloader para fonts Allegro
 * 
 * Libera a memória de uma font carregada
 */
struct FontUnloader {
    /**
     * @brief Destroi uma font Allegro
     * @param f Ponteiro para a font a ser destruída
     */
    void operator()(ALLEGRO_FONT* f) const {
        if (f) al_destroy_font(f);
    }
};