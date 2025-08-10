#include <SDL3/SDL.h>
#include <Raylib/raylib.h>
#include <stdio.h>
#define SNAKE "Director's Cut: Snake"

#ifndef byte
#define byte char
#endif

// Note, to account for the snake's segments values reaching 0, when
// it reaches 0, use a for loop that goes through each segment, and
// add (segment value - integer limit) to each individual segment, to
// reset the values of the snake when needed

int zoom = 16;

typedef struct {
    byte** map;

    int w;
    int h;
} Map;

typedef struct {
    bool isAlive;

    int size;
    int segmentValue;

    int posX;
    int posY;

    int lastX;
    int lastY;

    bool up;
    bool left;
    bool down;
    bool right;
} Player;

Map CreateMap (int width, int height) {
    Map newMap = {0};
    if (width < 0 || height < 0) {
        SDL_Log("%s: Map Width or Height is too small!", SNAKE);
        return newMap;
    }
    if  (width > 4096 || height > 4096) {
        SDL_Log("%s: Map Width or Height is too large!", SNAKE);
        return newMap;
    }

    newMap.w = width;
    newMap.h = height;

    newMap.map = (byte**)SDL_malloc(sizeof(byte*) * width);
    for (int i = 0; i < width; i++) {
        newMap.map[i] = (byte*)SDL_calloc(height, sizeof(byte));
    }

    return newMap;
}

void Controls (Player* player) {
    if (IsKeyDown(KEY_W)) {
        player->up = true;
        player->left = false;
        player->down = false;
        player->right = false;
    }
    else {
        player->up = false;
    }

    if (IsKeyDown(KEY_A)) {
        player->up = false;
        player->left = true;
        player->down = false;
        player->right = false;
    }
    else {
        player->left = false;
    }

    if (IsKeyDown(KEY_S)) {
        player->up = false;
        player->left = false;
        player->down = true;
        player->right = false;
    }
    else {
        player->down = false;
    }

    if (IsKeyDown(KEY_D)) {
        player->up = false;
        player->left = false;
        player->down = false;
        player->right = true;
    }
    else {
        player->right = false;
    }
}

void MoveSnake (Player* player) {
    
}

void UpdateSnake (Player* player, Map* map) {
    if (player->up) {
        map->map[player->posX][player->posY-1] = player->segmentValue++;
        player->posY--;
    }
    else if (player->down) {
        map->map[player->posX][player->posY+1] = player->segmentValue++;
        player->posY++;
    }

    if (player->right) {
        map->map[player->posX+1][player->posY] = player->segmentValue++;
        player->posX++;
    }
    else if (player->left) {
        map->map[player->posX-1][player->posY] = player->segmentValue++;
        player->posX--;
    }
}

void DrawSnake (Player* player) {
    // for (int i = 0; i < player->size; i++) {
    //     DrawRectangle(player->segmentsX[i], player->segmentsY[i], zoom, zoom, WHITE);
    // }
}

Player InitSnake () {
    Player newPlayer = {0};
    newPlayer.size = 1;

    return newPlayer;
}

int main () {
    InitWindow(800, 600, SNAKE);
    SetTargetFPS(10);

    Map map = CreateMap(32, 32);
    Player player = InitSnake();
    player.posX = 10;
    player.posY = 10;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PURPLE);

        Controls(&player);
        UpdateSnake(&player, &map);

        for (int y = 0; y < map.h; y++) {
            for (int x = 0; x < map.w; x++) {
                DrawRectangle(x*zoom, y*zoom, zoom, zoom, map.map[x][y] == 0 ? DARKBLUE : DARKGREEN);
                DrawRectangleLines(x*zoom, y*zoom, zoom, zoom, BLACK);
                char num[32];
                SDL_snprintf(num, 32, "%i", map.map[x][y]);
                DrawText(num, x*zoom, y*zoom, 10, LIGHTGRAY);
            }
        }

        DrawSnake(&player);

        EndDrawing();
    }

    return 0;
}