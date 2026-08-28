
## 📁 **docs/architecture.md**

# Arquitetura do Sistema

## Visão Geral

O Snake Game segue uma arquitetura baseada em componentes com separação clara de responsabilidades.

## Diagrama de Componentes

```mermaid
graph TB
    MAIN[main.cpp] --> GAME[Game]
    GAME --> SNAKE[Snake]
    GAME --> APPLE[Apple] 
    GAME --> FIELD[Field]
    GAME --> HUD[ConsoleHUD]
    GAME --> SOUND[SoundBank]
    
    SNAKE --> ENTITY[Entity]
    APPLE --> ENTITY
    FIELD --> ENTITY