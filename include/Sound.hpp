/**
 * @file Sound.hpp
 * @brief Sistema de áudio para efeitos sonoros e música de fundo
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/**
 * @brief Banco de sons que gerencia todos os recursos de áudio do jogo
 * 
 * Responsável por carregar, reproduzir e gerenciar efeitos sonoros e música.
 * Implementa sistema de mute individual para música e efeitos.
 */
class SoundBank {
public:
    /**
     * @brief Constrói o banco de sons com arquivos específicos
     * @param eatWav Caminho para o arquivo de som "comer maçã"
     * @param gameOverWav Caminho para o arquivo de som "game over"
     * @param musicFile Caminho para o arquivo de música de fundo
     */
    SoundBank(const std::string& eatWav,
              const std::string& gameOverWav,
              const std::string& musicFile);
    
    /**
     * @brief Destrutor que libera todos os recursos de áudio
     */
    ~SoundBank();

    /**
     * @brief Verifica se o banco de sons foi inicializado com sucesso
     * @return true se os efeitos essenciais foram carregados
     */
    bool ok() const { return ok_; }

    // ====== EFEITOS SONOROS ======
    
    /**
     * @brief Reproduz som de "comer maçã"
     * @param vol Volume do som (0.0 a 1.0, padrão: 0.35)
     */
    void playEat(float vol = 0.35f);
    
    /**
     * @brief Reproduz som de "game over"
     * @param vol Volume do som (0.0 a 1.0, padrão: 1.0)
     */
    void playGameOver(float vol = 1.0f);

    // ====== MÚSICA ======
    
    /**
     * @brief Verifica se a música foi carregada com sucesso
     * @return true se o arquivo de música está disponível
     */
    bool musicLoaded() const { return music_ != nullptr; }
    
    /**
     * @brief Inicia a reprodução da música de fundo
     * @param gain Ganho/volume da música (0.0 a 1.0, padrão: 0.35)
     */
    void startMusic(float gain = 0.35f);
    
    /**
     * @brief Para a reprodução da música de fundo
     */
    void stopMusic();
    
    /**
     * @brief Reinicia a música do início
     * @param gain Ganho/volume da música (0.0 a 1.0, padrão: 0.35)
     */
    void restartMusic(float gain = 0.35f);

    // ====== CONTROLES DE MUTE ======
    
    /**
     * @brief Ativa/desativa mute da música
     * @param b true para mutar, false para desmutar
     * 
     * @note Quando desmutado, a música é reiniciada automaticamente
     */
    void setMusicMuted(bool b);
    
    /**
     * @brief Ativa/desativa mute dos efeitos sonoros
     * @param b true para mutar, false para desmutar
     */
    void setFxMuted(bool b);
    
    /**
     * @brief Verifica se a música está mutada
     * @return true se música está mutada
     */
    bool musicMuted() const;
    
    /**
     * @brief Verifica se os efeitos sonoros estão mutados
     * @return true se efeitos estão mutados
     */
    bool fxMuted() const;

private:
    bool ok_{false};                    ///< Indica se inicialização foi bem-sucedida
    bool audioInstalled_{false};        ///< Flag de instalação do sistema de áudio

    // Recursos de áudio
    ALLEGRO_SAMPLE* eat_{nullptr};      ///< Som de comer maçã
    ALLEGRO_SAMPLE* gameOver_{nullptr}; ///< Som de game over
    ALLEGRO_AUDIO_STREAM* music_{nullptr}; ///< Stream de música de fundo

    // Estados de mute
    bool musicMuted_{false};            ///< Mute da música
    bool fxMuted_{false};               ///< Mute dos efeitos sonoros
};