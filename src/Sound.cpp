/**
 * @file Sound.cpp
 * @brief Implementação do sistema de áudio do jogo
 * @author Esther e Lucas
 * @date 2025
 */

#include "Sound.hpp"
#include <stdexcept>
#include <fstream>  // ← ADICIONE ESTA LINHA

/**
 * @brief Constrói o banco de sons e carrega todos os recursos de áudio
 * @param eatWav Caminho para o arquivo WAV do som "comer maçã"
 * @param gameOverWav Caminho para o arquivo WAV do som "game over" 
 * @param musicFile Caminho para o arquivo de música de fundo
 * 
 * @note A música é considerada opcional - ok_ não depende dela
 * @throws Nenhuma exceção é lançada, falhas são silenciosas (ok_ = false)
 */
SoundBank::SoundBank(const std::string& eatWav,
                     const std::string& gameOverWav,
                     const std::string& musicFile) {
    // Verifica se está em container Docker
    std::ifstream dockercheck("/.dockerenv");
    bool isDocker = dockercheck.good();
    dockercheck.close();

    if (isDocker) {
        // Modo Docker - desativa áudio silenciosamente
        ok_ = true;  // Marca como "ok" mas não carrega nada
        return;
    }

    // Inicializa sistema de áudio do Allegro (se ainda não foi)
    audioInstalled_ = al_install_audio();
    al_init_acodec_addon();
    
    // Configura mixer padrão se áudio foi instalado
    if (audioInstalled_) {
        al_reserve_samples(8);  // Reserva 8 canais de sample
    }

    // Carrega efeitos sonoros (essenciais para o jogo)
    eat_      = al_load_sample(eatWav.c_str());
    gameOver_ = al_load_sample(gameOverWav.c_str());

    // Carrega música (opcional - o jogo funciona sem música)
    music_ = al_load_audio_stream(musicFile.c_str(), 4, 2048);

    // Banco é considerado ok se pelo menos os efeitos foram carregados
    ok_ = (eat_ != nullptr) && (gameOver_ != nullptr);
    // música é opcional — ok_ não depende dela
}

/**
 * @brief Destrutor que libera todos os recursos de áudio
 * 
 * Garante que todos os samples e streams sejam destruídos
 * adequadamente para evitar vazamentos de memória.
 */
SoundBank::~SoundBank() {
    // Libera stream de música
    if (music_) { 
        al_destroy_audio_stream(music_); 
        music_ = nullptr; 
    }
    
    // Libera samples de efeitos
    if (eat_) { 
        al_destroy_sample(eat_); 
        eat_ = nullptr; 
    }
    
    if (gameOver_) { 
        al_destroy_sample(gameOver_); 
        gameOver_ = nullptr; 
    }
    
    // Não desinstala áudio global pois outros sistemas podem estar usando
    if (audioInstalled_) {
        // al_uninstall_audio() não é chamado por segurança
    }
}

/**
 * @brief Reproduz som de "comer maçã"
 * @param vol Volume do som (0.0 a 1.0)
 * 
 * @note Não reproduz se efeitos estão mutados ou sample não carregado
 */
void SoundBank::playEat(float vol) {
    if (!ok_ || fxMuted_ || !eat_) return;  // ← Corrigido com !ok_
    
    // Reproduz sample uma vez
    al_play_sample(eat_, vol, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
}

/**
 * @brief Reproduz som de "game over" 
 * @param vol Volume do som (0.0 a 1.0)
 * 
 * @note Não reproduz se efeitos estão mutados ou sample não carregado
 */
void SoundBank::playGameOver(float vol) {
    if (!ok_ || fxMuted_ || !gameOver_) return;  // ← Corrigido com !ok_
    
    // Reproduz sample uma vez
    al_play_sample(gameOver_, vol, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
}

/**
 * @brief Inicia a reprodução da música de fundo
 * @param gain Ganho/volume da música (0.0 a 1.0)
 * 
 * Configura música para loop infinito e a conecta ao mixer.
 * Não inicia se música está mutada ou não foi carregada.
 */
void SoundBank::startMusic(float gain) {
    if (!ok_ || musicMuted_ || !music_) return;  // ← Corrigido com !ok_
    
    // Configura música para loop
    al_set_audio_stream_playmode(music_, ALLEGRO_PLAYMODE_LOOP);
    
    // Ajusta volume
    al_set_audio_stream_gain(music_, gain);
    
    // Conecta ao mixer padrão para reprodução
    al_attach_audio_stream_to_mixer(music_, al_get_default_mixer());
}

/**
 * @brief Para a reprodução da música de fundo
 * 
 * Desconecta a música do mixer, efetivamente pausando-a.
 * A posição de reprodução é mantida.
 */
void SoundBank::stopMusic() {
    if (!music_) return;  // Verifica se música foi carregada
    
    // Desconecta do mixer (pausa)
    al_detach_audio_stream(music_);
}

/**
 * @brief Reinicia a música do início
 * @param gain Ganho/volume da música (0.0 a 1.0)
 * 
 * Para a música atual e reinicia do começo com novo volume.
 * Útil para reiniciar o jogo ou quando música é desmutada.
 */
void SoundBank::restartMusic(float gain) {
    if (!music_ || musicMuted_) return;  // Verifica carregamento e mute
    
    // Para música atual
    al_detach_audio_stream(music_);
    
    // Reinicia configurações
    al_set_audio_stream_playmode(music_, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(music_, gain);
    
    // Reconecta ao mixer (reinicia do começo)
    al_attach_audio_stream_to_mixer(music_, al_get_default_mixer());
}

// ====== IMPLEMENTAÇÕES DOS CONTROLES DE MUTE ======

/**
 * @brief Ativa/desativa mute da música com comportamento inteligente
 * @param b true para mutar, false para desmutar
 * 
 * Quando mutado: para a música imediatamente
 * Quando desmutado: reinicia a música do início
 */
void SoundBank::setMusicMuted(bool b) { 
    musicMuted_ = b; 
    
    if (musicMuted_) {
        // Muta: para a música
        stopMusic();
    } else {
        // Desmuta: reinicia música
        if (music_) {
            restartMusic(0.35f);  // Volume padrão
        }
    }
}

/**
 * @brief Ativa/desativa mute dos efeitos sonoros
 * @param b true para mutar, false para desmutar
 * 
 * Afeta apenas os efeitos sonoros (eat e gameOver).
 * A música não é afetada por este controle.
 */
void SoundBank::setFxMuted(bool b) { 
    fxMuted_ = b; 
}

/**
 * @brief Verifica estado do mute da música
 * @return true se música está mutada
 */
bool SoundBank::musicMuted() const { 
    return musicMuted_; 
}

/**
 * @brief Verifica estado do mute dos efeitos sonoros  
 * @return true se efeitos estão mutados
 */
bool SoundBank::fxMuted() const { 
    return fxMuted_; 
}