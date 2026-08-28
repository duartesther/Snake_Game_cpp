# 🐍 Snake Game - Instruções de Execução

## Requisitos
- Docker instalado
- (Opcional) Docker Compose

## Execução Rápida

### Método 1: Script Automático (Recomendado)
```bash
chmod +x run.sh
./run.sh
```

### Método 2: Comandos Manuais
Versão Gráfica:
```bash
docker compose build --no-cache
docker compose up snake-game-no-audio
```
### Método 3: Comandos Manuais
```bash
docker compose build --no-cache  
docker compose up snake-text
```

### Método 3: Docker Direto

# Build da imagem
```bash
docker build -t snake-game .
```
# Versão gráfica
```bash
docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  snake-game ./build/bin/SnakeGame
```
# Versão texto
```bash
docker run -it --rm snake-game ./build/bin/SnakeText
```

## 🎮 Controles
### SnakeGame (Gráfico)
← →: Virar esquerda/direita

P: Pausar

ESC: Sair

C: Mudar cor da cobra

M/N: Mute música/efeitos

### SnakeText (Terminal)
← → ou A/D: Virar esquerda/direita

↑ ↓ ou W/S: Mover para cima/baixo

ESC: Sair

## Desenvolvimento
Sistema: Desenvolvido em C++ com Allegro 5

Plataforma: Container Docker universal

Compatibilidade: Windows, Mac, Linux

## Estrutura do Projeto
SnakeGame/
├── Dockerfile
├── docker-compose.yml
├── run.sh
├── README_DOCKER.md
├── CMakeLists.txt
├── src/
├── include/
└── assets/



# Em resumo, siga os seguintes comandos:

```bash
# Torne o script executável
chmod +x run.sh

# Teste o build
docker compose build --no-cache

# Teste a versão texto (sempre funciona)
docker compose up snake-text

# Teste a versão gráfica (precisa de X11 no Linux/Mac)
./run.sh
```

### De forma manual:

#### - Versão Gráfica Completa (Docker não aceita áudio, então pode não funcionar)
```docker compose up snake-game```

#### - Versão Gráfica Sem Áudio (garantida no Docker)
```docker compose up snake-game-no-audio```

#### - Versão Texto (Terminal)
```docker compose up snake-text```