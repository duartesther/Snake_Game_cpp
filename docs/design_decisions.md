
## 📁 **docs/design_decisions.md**

# Decisões de Design

## 1. Escolha do Allegro 5

**Problema**: Necessidade de biblioteca gráfica multiplataforma

**Solução**: Allegro 5 por ser:
- Madura e bem documentada
- Boa performance para 2D
- Suporte nativo a áudio e input
- Comunidade ativa

**Alternativas Consideradas**:
- SDL2: Mais baixo nível, mais configuração
- SFML: Menos madura, documentação inconsistente

## 2. Sistema de Movimento

**Problema**: Movimento em grid vs. movimento contínuo

**Solução**: Movimento angular contínuo porque:
- Mais suave e fluido
- Permite curvas progressivas
- Experiência mais moderna

**Trade-off**: Colisão mais complexa que Snake tradicional

## 3. Gerenciamento de Recursos

**Problema**: Vazamento de memória com recursos Allegro

**Solução**: ResourceManager com:
- RAII (destruição automática)
- Cache para evitar recarregamento
- Policy-based design para flexibilidade

## 4. Arquitetura de Pastas

**Problema**: Organização de headers e implementações

**Solução**: Separar include/ e src/ porque:
- Interface pública clara
- Facilita distribuição de biblioteca
- Padrão da indústria para projetos C++

## 5. Sistema de Áudio

**Problema**: Música vs. efeitos sonoros

**Solução**: SoundBank unificado com:
- Controles de mute independentes
- Stream para música (longa)
- Samples para efeitos (curtos)
- Volume configurável