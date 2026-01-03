# Kręci! (Spirala)

A C++ clone of the classic "Achtung, die Kurve!" (Zatacka) game - a multiplayer arcade where players control continuously moving lines that leave trails, competing to survive the longest without colliding.

## Tech Stack

- **C++17** - Core language
- **SDL3** - Window management, rendering, input handling
- **SDL3_ttf** - Font rendering for UI elements
- **Visual Studio** - IDE and build system

## Game Overview

Players control colored lines that move continuously across the playfield. Each player can only turn left or right - there's no stopping or reversing. As players move, they leave behind a solid trail. The objective is to avoid colliding with:

- Your own trail
- Other players' trails  
- The border of the playfield

The last player standing wins the round. To prevent easy trapping, trails periodically create small gaps that players can pass through.

### Controls

| Player | Left | Right |
|--------|------|-------|
| 1 (Yellow) | ← | → |
| 2 (Magenta) | A | S |
| 3 (Cyan) | N | M |

- **Enter** - Restart game
- **Escape** - Exit
- **F1** - Toggle FPS counter

## Architecture

### Scene System

The game uses a layered scene architecture where multiple scenes can be active simultaneously:

```
┌─────────────────────────────────────┐
│         FPSCounterScene             │  ← Debug overlay (toggleable)
├─────────────────────────────────────┤
│          KillLogScene               │  ← Kill feed (top-right)
├─────────────────────────────────────┤
│         GameplayScene               │  ← Main game logic
├─────────────────────────────────────┤
│        BackgroundScene              │  ← Animated gradient
└─────────────────────────────────────┘
```

Each scene implements `IScene` interface:

```cpp
class IScene {
public:
    virtual void HandleEvent(const SDL_Event& e);
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Activate();
    virtual void Deactivate();
};
```

Scenes are rendered in order, allowing transparent overlays and independent update cycles.

### Task System

Asynchronous, delayed operations are handled through a task system. Tasks can:

- Execute after a delay
- Run callbacks on completion
- Handle input events
- Render UI elements (like countdown numbers)

```cpp
class ITask {
protected:
    float delayRemaining;
public:
    virtual void HandleEvent(const SDL_Event& event);
    virtual bool Update(float deltaTime);  // Returns true when complete
    virtual void Render();
};
```

For example the countdown timer uses `TaskCounter` which animates numbers scaling up and fading out:

```cpp
// Queue countdown: "3", "2", "1"
for (int i = 0; i < COUNTER_SIZE; i++) {
    auto task = make_unique<TaskCounter>(ctx, to_string(COUNTER_SIZE - i), COUNTER_TIME, i);
    taskManager->AddTask(move(task));
}
```

### Collision System

#### The Problem

The initial implementation stored every trail point in a `vector<SDL_FPoint>`:

```cpp
vector<SDL_FPoint> trail;

// Every frame, for every player:
for (size_t i = 0; i < trail.size(); i++) {
    float dist = distance(playerPos, trail[i]);
    if (dist < thickness) collision = true;
}
```

This O(n) per-frame check degraded rapidly as trails grew, causing noticeable slowdown after just few seconds of gameplay.

#### Solutions Explored

**Option 1: Grid Array**
- CPU-side `vector<int>` sized to playfield dimensions
- O(1) collision checks via direct array indexing
- Required separate rendering pass to draw grid to screen

**Option 2: Render Target Bitmap**  
- `SDL_Texture` with `SDL_TEXTUREACCESS_TARGET`
- Draw trails directly to texture, read pixels for collision
- `SDL_RenderReadPixels` proved too slow (GPU→CPU sync)

**Hybrid Solution (Implemented)**

Combines both approaches:

```cpp
class CollisionSystem {
    vector<int> grid;        // CPU collision data (stores player ID)
    SDL_Texture* texture;    // GPU render target
    
    bool CheckAndDrawLine(...) {
        // 1. Check collision on CPU grid - O(1)
        if (grid[y * width + x] != 0) {
            return true;  // Hit!
        }
        
        // 2. Write to both simultaneously
        grid[y * width + x] = playerId;
        SDL_RenderFillRect(renderer, &rect);  // To texture
    }
};
```

Benefits:
- Fast collision detection (CPU array lookup)
- Fast rendering (single texture blit)
- Stores player ID for kill attribution
- Constant memory regardless of game duration

### Key Implementation Details

#### Trail Gaps

Periodic gaps prevent players from being easily trapped:

```cpp
// Player.h
float trailTimer = 0.0f;
bool isDrawing = true;

void Update(float deltaTime) {
    trailTimer += deltaTime;
    if (isDrawing && trailTimer >= TRAIL_GAP_INTERVAL) {
        isDrawing = false;
        trailTimer = 0.0f;
    } else if (!isDrawing && trailTimer >= TRAIL_GAP_DURATION) {
        isDrawing = true;
        trailTimer = 0.0f;
    }
}
```

#### Pixel-Based Movement Tracking

Since movement per frame (~0.3px) is sub-pixel, collision checks only trigger on actual pixel transitions:

```cpp
int currPixelX = (int)position.x;
int currPixelY = (int)position.y;

if (currPixelX == lastPixelX && currPixelY == lastPixelY) {
    return false;  // Same pixel, skip collision check
}
```

#### Kill Attribution

The grid stores player IDs (not just occupied/empty), enabling proper kill logging:

```cpp
int GetPixelOwner(int x, int y) const {
    if (!IsInBounds(x, y)) return -1;  // Border
    return grid[y * width + x];
}

// On collision:
KillEvent event;
event.victimId = player->GetId();  // Who has crashed
```

## File Structure

```
├── Main.cpp           # Entry point, SDL initialization
├── Game.h             # Main game loop, scene management
├── GameContext.h      # Shared state (renderer, window, kill log)
├── Config.h           # Game constants and configuration
│
├── IScene.h           # Scene interface
├── BackgroundScene.h  # Animated gradient background
├── GameplayScene.h    # Core game logic
├── FPSCounterScene.h  # Debug FPS display
├── KillLogScene.h     # Kill feed UI
│
├── Player.h           # Player state, movement, input
├── CollisionSystem.h  # Hybrid grid/texture collision
│
├── ITask.h            # Task interface
├── Task.h             # Generic delayed callback
├── TaskCounter.h      # Animated countdown numbers
├── TaskEndGame.h      # End game message display
├── TaskManager.h      # Task queue management
│
├── StateAnimator.h    # Tweening/animation utility
└── Utils.h            # Color helpers, font loading
```

## Configuration

All game parameters are centralized in `Config.h`:

```cpp
// Player movement
const float PLAYER_SPEED = 0.35f;
const float PLAYER_TURN_SPEED = 0.02f;
const int PLAYER_THICKNESS = 1;

// Trail gaps
const float TRAIL_GAP_INTERVAL = 2.0f;   // Seconds between gaps
const float TRAIL_GAP_DURATION = 0.15f;  // Gap length in seconds

// Playfield
const float SCENE_GAMEPLAY_PADDING_L = 70;
const float SCENE_GAMEPLAY_PADDING_R = 70;
const float SCENE_GAMEPLAY_PADDING_T = 90;
const float SCENE_GAMEPLAY_PADDING_B = 50;
```

## Building

Requires:
- SDL3 development libraries
- SDL3_ttf development libraries
- C++17 compatible compiler

Visual Studio solution expects SDL3 headers and libraries in standard locations.

## Challenges & Lessons Learned

1. **Additive Scene Rendering** - Allowing multiple active scenes required careful ordering and transparency handling. Each scene renders independently, enabling overlays like FPS counter and kill log without coupling.

2. **Asynchronous UI** - The countdown timer needed to queue multiple delayed tasks that render independently. The task system evolved to support both logic callbacks and persistent render tasks.

3. **Collision Performance** - The naive trail vector approach was elegant but unscalable. The hybrid solution required understanding SDL's render target system and careful synchronization between CPU and GPU data structures.

4. **Sub-pixel Movement** - At 60 FPS with slow movement, players move less than 1 pixel per frame. This required tracking "last pixel position" separately from floating-point position to avoid redundant collision checks and self-collision on the same pixel.

5. **Self-Collision Avoidance** - Players must not collide with their own freshly-drawn trail. Solved by skipping collision checks for the first few pixel movements after spawn.