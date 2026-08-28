```mermaid

classDiagram
    %% ====== ENTITY HIERARCHY ======
    class Entity {
        <<abstract>>
        +update(dt: double) void
        +draw() void
    }
    
    class Field {
        -bmp_: ALLEGRO_BITMAP*
        +draw() void
    }
    
    class Apple {
        -bmp_: ALLEGRO_BITMAP*
        -pos_: Vector2D
        -radius_: float
        +spawnRandom() void
        +draw() void
        +position() Vector2D
    }
    
    class Snake {
        -body_: vector~Segment~
        -headBmp_: ALLEGRO_BITMAP*
        -bodyBmp_: ALLEGRO_BITMAP*
        -tailBmp_: ALLEGRO_BITMAP*
        -score_: int
        +turnLeft() void
        +turnRight() void
        +grow() void
        +checkSelfCollision() bool
        +draw() void
    }
    
    class Segment {
        +pos: Vector2D
        +size: double
        +angleDeg: double
    }
    
    %% ====== GAME MANAGEMENT ======
    class Game {
        -field_: unique_ptr~Field~
        -apple_: unique_ptr~Apple~
        -snake_: unique_ptr~Snake~
        -hud_: unique_ptr~ConsoleHUD~
        -sound_: SoundBank*
        -state_: GameState
        +run() void
        -handleEvent(ev: ALLEGRO_EVENT) void
        -updateLogic() void
        -render() void
    }
    
    class ConsoleHUD {
        -console_: ALLEGRO_BITMAP*
        -kbNone_: ALLEGRO_BITMAP*
        -kbLeft_: ALLEGRO_BITMAP*
        -kbRight_: ALLEGRO_BITMAP*
        -score_: int
        -record_: int
        +draw() void
        +setScore(s: int) void
        +setRecord(r: int) void
    }
    
    class SoundBank {
        -eat_: ALLEGRO_SAMPLE*
        -gameOver_: ALLEGRO_SAMPLE*
        -music_: ALLEGRO_AUDIO_STREAM*
        -musicMuted_: bool
        -fxMuted_: bool
        +playEat(vol: float) void
        +playGameOver(vol: float) void
        +startMusic(gain: float) void
        +stopMusic() void
    }
    
    %% ====== RESOURCE MANAGEMENT ======
    class ResourceManager~Key, Resource, Loader, Unloader~ {
        -cache_: unordered_map~Key, Resource~
        +get(key: Key) Resource
        +clear() void
    }
    
    class BitmapLoader {
        +operator()(path: string) ALLEGRO_BITMAP*
    }
    
    class FontSpecLoader {
        +operator()(spec: string) ALLEGRO_FONT*
    }
    
    %% ====== UTILITIES ======
    class Vector2D {
        +x: double
        +y: double
        +operator+(o: Vector2D) Vector2D
        +operator*(s: double) Vector2D
        +dot(o: Vector2D) double
        +norm() double
    }
    
    class Types {
        <<namespace>>
    }
    
    %% ====== RELATIONSHIPS ======
    Entity <|-- Field
    Entity <|-- Apple
    Entity <|-- Snake
    
    Game *-- Field
    Game *-- Apple
    Game *-- Snake
    Game *-- ConsoleHUD
    Game *-- SoundBank
    
    Snake *-- Segment
    Snake o-- ResourceManager
    
    ResourceManager *-- BitmapLoader
    ResourceManager *-- FontSpecLoader
    
    Apple --> Vector2D
    Snake --> Vector2D
    Segment --> Vector2D
