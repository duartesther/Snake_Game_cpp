```mermaid

graph TB
    %% ====== ENTRY POINT ======
    MAIN[main.cpp] --> GAME[Game]
    
    %% ====== CORE GAME LOOP ======
    GAME --> GAMELOOP[Game Loop]
    GAME --> EVENTMGMT[Event Management]
    GAME --> STATEMGMT[State Management]
    
    %% ====== ENTITY SYSTEM ======
    GAME --> ENTITY[Entity]
    ENTITY --> FIELD[Field]
    ENTITY --> SNAKE[Snake]
    ENTITY --> APPLE[Apple]
    
    %% ====== SNAKE COMPONENTS ======
    SNAKE --> SEGMENT[Segment]
    SNAKE --> MOVEMENT[Movement System]
    SNAKE --> COLLISION[Collision Detection]
    SNAKE --> GROWTH[Growth System]
    
    MOVEMENT --> WRAPPING[Screen Wrapping]
    MOVEMENT --> ROTATION[Rotation Control]
    COLLISION --> SELFCOLL[Self-Collision]
    COLLISION --> APPLE_COLL[Apple Collection]
    
    %% ====== RENDERING SYSTEM ======
    GAME --> RENDER[Renderer]
    RENDER --> GFX[AllegroGfx]
    RENDER --> PRIMITIVES[Primitives]
    RENDER --> BITMAPS[Bitmap Rendering]
    
    FIELD --> FIELD_RENDER[Field Rendering]
    SNAKE --> SNAKE_RENDER[Snake Rendering]
    APPLE --> APPLE_RENDER[Apple Rendering]
    
    %% ====== UI SYSTEM ======
    GAME --> HUD[ConsoleHUD]
    HUD --> SCORE_DISPLAY[Score Display]
    HUD --> KEYBOARD_VIS[Keyboard Visual]
    HUD --> TITLE_DISPLAY[Title Display]
    
    %% ====== AUDIO SYSTEM ======
    GAME --> SOUND[SoundBank]
    SOUND --> MUSIC[Music System]
    SOUND --> SFX[Sound Effects]
    SOUND --> MUTE[Mute Controls]
    
    MUSIC --> MUSIC_STREAM[Audio Stream]
    SFX --> EAT_SFX[Eat Sound]
    SFX --> GAMEOVER_SFX[GameOver Sound]
    
    %% ====== RESOURCE MANAGEMENT ======
    GAME --> RESMGR[ResourceManager]
    RESMGR --> BITMAP_MGR[Bitmap Manager]
    RESMGR --> FONT_MGR[Font Manager]
    
    BITMAP_MGR --> BITMAP_LOADER[BitmapLoader]
    BITMAP_MGR --> BITMAP_UNLOADER[BitmapUnloader]
    FONT_MGR --> FONT_LOADER[FontSpecLoader]
    FONT_MGR --> FONT_UNLOADER[FontUnloader]
    
    %% ====== UTILITIES ======
    GAME --> TYPES[Types]
    TYPES --> CONFIG[Configuration]
    TYPES --> COLOR_PRESETS[Color Presets]
    
    GAME --> VECTOR2D[Vector2D]
    VECTOR2D --> MATH_OPS[Math Operations]
    
    %% ====== DEPENDENCIES ======
    GFX --> ALLEGRO[Allegro 5]
    PRIMITIVES --> ALLEGRO_PRIM[Allegro Primitives]
    BITMAPS --> ALLEGRO_IMG[Allegro Image]
    FONT_MGR --> ALLEGRO_FONT[Allegro Font]
    SOUND --> ALLEGRO_AUDIO[Allegro Audio]
    
    %% ====== DATA FLOW ======
    EVENTMGMT --> KEYBOARD[Keyboard Input]
    KEYBOARD --> SNAKE_CONTROL[Snake Control]
    
    STATEMGMT --> MENU_STATE[Menu State]
    STATEMGMT --> PLAYING_STATE[Playing State]
    STATEMGMT --> GAMEOVER_STATE[GameOver State]
    
    SCORE_DISPLAY --> SCORE_DATA[Score Data]
    KEYBOARD_VIS --> INPUT_STATE[Input State]
    
    %% ====== STYLING ======
    classDef core fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    classDef entity fill:#f3e5f5,stroke:#4a148c,stroke-width:2px
    classDef render fill:#e8f5e8,stroke:#1b5e20,stroke-width:2px
    classDef audio fill:#fff3e0,stroke:#e65100,stroke-width:2px
    classDef ui fill:#fce4ec,stroke:#880e4f,stroke-width:2px
    classDef resource fill:#e0f2f1,stroke:#004d40,stroke-width:2px
    classDef util fill:#fafafa,stroke:#424242,stroke-width:2px
    
    class MAIN,GAME,GAMELOOP,EVENTMGMT,STATEMGMT core
    class ENTITY,SNAKE,APPLE,FIELD,SEGMENT entity
    class RENDER,GFX,PRIMITIVES,BITMAPS,FIELD_RENDER,SNAKE_RENDER,APPLE_RENDER render
    class SOUND,MUSIC,SFX,MUSIC_STREAM,EAT_SFX,GAMEOVER_SFX,MUTE audio
    class HUD,SCORE_DISPLAY,KEYBOARD_VIS,TITLE_DISPLAY,SCORE_DATA,INPUT_STATE ui
    class RESMGR,BITMAP_MGR,FONT_MGR,BITMAP_LOADER,BITMAP_UNLOADER,FONT_LOADER,FONT_UNLOADER resource
    class TYPES,VECTOR2D,CONFIG,COLOR_PRESETS,MATH_OPS util