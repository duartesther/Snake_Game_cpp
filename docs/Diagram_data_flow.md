```mermaid

flowchart TD
    %% ====== INITIALIZATION ======
    START[Program Start] --> INIT[Initialize Systems]
    INIT --> ALLEGRO_INIT[Allegro Initialization]
    INIT --> RESOURCE_LOAD[Load Resources]
    INIT --> AUDIO_INIT[Audio Initialization]
    
    %% ====== MAIN LOOP ======
    INIT --> MAIN_LOOP[Main Game Loop]
    
    MAIN_LOOP --> EVENT_PROCESS[Process Events]
    EVENT_PROCESS --> KEY_INPUT[Keyboard Input]
    EVENT_PROCESS --> WINDOW_EVENTS[Window Events]
    
    MAIN_LOOP --> UPDATE_LOGIC[Update Game Logic]
    UPDATE_LOGIC --> SNAKE_UPDATE[Update Snake]
    UPDATE_LOGIC --> COLLISION_CHECK[Check Collisions]
    UPDATE_LOGIC --> APPLE_CHECK[Check Apple Collection]
    UPDATE_LOGIC --> SCORE_UPDATE[Update Score]
    
    MAIN_LOOP --> RENDER_FRAME[Render Frame]
    RENDER_FRAME --> RENDER_FIELD[Render Field]
    RENDER_FRAME --> RENDER_ENTITIES[Render Entities]
    RENDER_FRAME --> RENDER_HUD[Render HUD]
    RENDER_FRAME --> RENDER_OVERLAYS[Render Overlays]
    
    %% ====== INPUT PROCESSING ======
    KEY_INPUT --> LEFT_RIGHT[Left/Right Keys]
    KEY_INPUT --> SPECIAL_KEYS[Special Keys]
    
    LEFT_RIGHT --> SNAKE_ROTATION[Snake Rotation]
    SPECIAL_KEYS --> PAUSE[Pause Game]
    SPECIAL_KEYS --> COLOR_CHANGE[Change Colors]
    SPECIAL_KEYS --> MUTE_TOGGLE[Toggle Mute]
    
    %% ====== GAME LOGIC ======
    SNAKE_UPDATE --> MOVE_SNAKE[Move Snake]
    MOVE_SNAKE --> WRAP_BORDERS[Wrap Borders]
    MOVE_SNAKE --> UPDATE_SEGMENTS[Update Segments]
    
    COLLISION_CHECK --> SELF_COLLISION[Self-Collision]
    COLLISION_CHECK --> BORDER_COLLISION[Border Collision]
    
    APPLE_CHECK --> COLLISION_DETECT[Collision Detection]
    COLLISION_DETECT --> EAT_APPLE[Eat Apple]
    EAT_APPLE --> PLAY_SOUND[Play Eat Sound]
    EAT_APPLE --> GROW_SNAKE[Grow Snake]
    EAT_APPLE --> SPAWN_APPLE[Spawn New Apple]
    
    %% ====== STATE MANAGEMENT ======
    SELF_COLLISION --> GAME_OVER[Game Over]
    BORDER_COLLISION --> GAME_OVER
    
    GAME_OVER --> STOP_MUSIC[Stop Music]
    GAME_OVER --> PLAY_GAMEOVER_SFX[Play GameOver Sound]
    GAME_OVER --> SHOW_GAMEOVER[Show GameOver Screen]
    
    %% ====== RENDERING PIPELINE ======
    RENDER_FIELD --> DRAW_BACKGROUND[Draw Background]
    RENDER_FIELD --> DRAW_BORDERS[Draw Borders]
    
    RENDER_ENTITIES --> DRAW_SNAKE[Draw Snake]
    RENDER_ENTITIES --> DRAW_APPLE[Draw Apple]
    
    DRAW_SNAKE --> APPLY_COLORS[Apply Color Preset]
    DRAW_SNAKE --> DRAW_SEGMENTS[Draw Segments]
    DRAW_SNAKE --> DRAW_EYES[Draw Eyes]
    
    RENDER_HUD --> DRAW_SCORE[Draw Score]
    RENDER_HUD --> DRAW_RECORD[Draw Record]
    RENDER_HUD --> DRAW_KEYBOARD[Draw Keyboard]
    RENDER_HUD --> DRAW_INSTRUCTIONS[Draw Instructions]
    
    RENDER_OVERLAYS --> MENU_OVERLAY[Menu Overlay]
    RENDER_OVERLAYS --> GAMEOVER_OVERLAY[GameOver Overlay]
    
    %% ====== STYLING ======
    classDef init fill:#e3f2fd,stroke:#1565c0
    classDef input fill:#f3e5f5,stroke:#7b1fa2
    classDef logic fill:#e8f5e8,stroke:#2e7d32
    classDef render fill:#fff3e0,stroke:#ef6c00
    classDef audio fill:#ffebee,stroke:#c62828
    
    class START,INIT,ALLEGRO_INIT,RESOURCE_LOAD,AUDIO_INIT init
    class EVENT_PROCESS,KEY_INPUT,WINDOW_EVENTS,LEFT_RIGHT,SPECIAL_KEYS,SNAKE_ROTATION,PAUSE,COLOR_CHANGE,MUTE_TOGGLE input
    class UPDATE_LOGIC,SNAKE_UPDATE,COLLISION_CHECK,APPLE_CHECK,SCORE_UPDATE,MOVE_SNAKE,WRAP_BORDERS,UPDATE_SEGMENTS,SELF_COLLISION,BORDER_COLLISION,COLLISION_DETECT,EAT_APPLE,GROW_SNAKE,SPAWN_APPLE,GAME_OVER logic
    class RENDER_FRAME,RENDER_FIELD,RENDER_ENTITIES,RENDER_HUD,RENDER_OVERLAYS,DRAW_BACKGROUND,DRAW_BORDERS,DRAW_SNAKE,DRAW_APPLE,APPLY_COLORS,DRAW_SEGMENTS,DRAW_EYES,DRAW_SCORE,DRAW_RECORD,DRAW_KEYBOARD,DRAW_INSTRUCTIONS,MENU_OVERLAY,GAMEOVER_OVERLAY render
    class PLAY_SOUND,STOP_MUSIC,PLAY_GAMEOVER_SFX audio

