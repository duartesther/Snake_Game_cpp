/**
 * @file ResourceManager.hpp
 * @brief Gerenciador genérico de recursos com cache
 * @author Esther e Lucas
 * @date 2025
 */

#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <utility>

/**
 * @brief Gerenciador de recursos com cache e carregamento lazy
 * @tparam Key Tipo da chave de identificação
 * @tparam Resource Tipo do recurso gerenciado
 * @tparam Loader Policy para carregar recursos
 * @tparam Unloader Policy para liberar recursos
 * 
 * Implementa padrão RAII com cache para evitar carregamento
 * duplicado de recursos. Usa policy-based design para flexibilidade.
 */
template<typename Key, typename Resource, typename Loader, typename Unloader>
class ResourceManager {
public:
    /**
     * @brief Construtor padrão
     */
    ResourceManager() = default;
    
    /**
     * @brief Destrutor que limpa todos os recursos
     */
    ~ResourceManager() { clear(); }

    /**
     * @brief Obtém recurso (carrega se necessário)
     * @param key Chave identificadora do recurso
     * @return Recurso carregado
     * @throws std::runtime_error se falhar ao carregar
     */
    Resource get(const Key& key) {
        auto it = cache_.find(key);
        if (it != cache_.end()) return it->second;

        Loader loader;
        Resource res = loader(key);
        if (!res) throw std::runtime_error("ResourceManager: falha ao carregar: " + key);

        cache_.emplace(key, res);
        return res;
    }

    /**
     * @brief Verifica se recurso está em cache
     * @param key Chave do recurso
     * @return true se recurso está carregado
     */
    bool contains(const Key& key) const {
        return cache_.count(key) != 0;
    }

    /**
     * @brief Libera todos os recursos do cache
     */
    void clear() {
        Unloader un;
        for (auto& [k, res] : cache_) {
            if (res) un(res);
        }
        cache_.clear();
    }

    /**
     * @brief Insere recurso manualmente no cache
     * @param key Chave do recurso
     * @param res Recurso a ser inserido
     */
    void insert(const Key& key, Resource res) {
        cache_[key] = res;
    }

private:
    std::unordered_map<Key, Resource> cache_;  ///< Cache de recursos carregados
};