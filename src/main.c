#include <SDL3/SDL.h>
#include <Raylib/raylib.h>
#include <stdio.h>
#include "WBE_Defines.h"
#include "WBE_Clock.h"
#include "WBE_Key.h"
#define SNAKE "Director's Cut: Snake"

// Note, to account for the snake's segments values reaching 0, when
// it reaches 0, use a for loop that goes through each segment, and
// add (segment value - integer limit) to each individual segment, to
// reset the values of the snake when needed

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

WBE_Clock* clock;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Event event;
WBE_KeyCheck* keyChecker;

Player player;
Map map;

int zoom = 16;

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
    if (WBE_IsKeyDown(keyChecker, SDL_SCANCODE_W)) {
        player->up = true;
        player->left = false;
        player->down = false;
        player->right = false;
    }
    else {
        player->up = false;
    }

    if (WBE_IsKeyDown(keyChecker, SDL_SCANCODE_A)) {
        player->up = false;
        player->left = true;
        player->down = false;
        player->right = false;
    }
    else {
        player->left = false;
    }

    if (WBE_IsKeyDown(keyChecker, SDL_SCANCODE_S)) {
        player->up = false;
        player->left = false;
        player->down = true;
        player->right = false;
    }
    else {
        player->down = false;
    }

    if (WBE_IsKeyDown(keyChecker, SDL_SCANCODE_D)) {
        player->up = false;
        player->left = false;
        player->down = false;
        player->right = true;
    }
    else {
        player->right = false;
    }
}

void MoveTail (Player* player, Map* map) {
    if (
        map->map[player->lastX][player->lastY-1] == map->map[player->lastX][player->lastY]-1
    ) {
        map->map[player->lastX][player->lastY] = 0;
        player->lastY--;
    }
    if (
        map->map[player->lastX-1][player->lastY] == map->map[player->lastX][player->lastY]-1
    ) {
        map->map[player->lastX][player->lastY] = 0;
        player->lastX--;
    }
    if (
        map->map[player->lastX][player->lastY+1] == map->map[player->lastX][player->lastY]-1
    ) {
        map->map[player->lastX][player->lastY] = 0;
        player->lastY++;
    }
    if (
        map->map[player->lastX+1][player->lastY] == map->map[player->lastX][player->lastY]-1
    ) {
        map->map[player->lastX][player->lastY] = 0;
        player->lastX++;
    }
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
    MoveTail(player, map);
}

Player InitSnake () {
    Player newPlayer = {0};
    newPlayer.size = 1;
    newPlayer.segmentValue = 1;

    return newPlayer;
}

char fpsText[32];

void SecUpdate () {
    SDL_snprintf(fpsText, 32, "%i", WBE_GetFPS(clock));
}

void FrameUpdate () {
    Controls(&player);
    UpdateSnake(&player, &map);
}

int main () {
    SDL_Init(SDL_INIT_VIDEO);

    clock = WBE_CreateClock();
    WBE_AddFrameFunction(clock, FrameUpdate);
    WBE_AddSecFunction(clock, SecUpdate);

    SDL_CreateWindowAndRenderer(SNAKE, 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer);

    keyChecker = WBE_CreateKeyChecker();

    map = CreateMap(32, 32);
    player = InitSnake();
    player.posX = 10;
    player.posY = 10;
    player.lastX = 10;
    player.lastY = 9;

    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            WBE_UpdateKeyCheck(keyChecker, event);
        }

        WBE_UpdateClock(clock);

        SDL_SetRenderDrawColor(renderer, 25, 51, 153, 255);
        SDL_RenderClear(renderer);

        char text[32];
        for (int y = 0; y < map.h; y++) {
            for (int x = 0; x < map.w; x++) {
                if (map.map[x][y] == 0) {
                    SDL_SetRenderDrawColor(renderer, 10, 127, 255, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 10, 255, 10, 255);
                }
                SDL_RenderFillRect(renderer, &(SDL_FRect){x*zoom, y*zoom, zoom, zoom});

                SDL_SetRenderDrawColor(renderer, 255, 10, 10, 255);
                SDL_RenderFillRect(renderer, &(SDL_FRect){player.lastX*zoom, player.lastY*zoom, zoom, zoom});

                SDL_SetRenderDrawColor(renderer, 0, 0, 00, 255);
                SDL_RenderRect(renderer, &(SDL_FRect){x*zoom, y*zoom, zoom, zoom});

                SDL_snprintf(text, 32, "%i", map.map[x][y]);
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderDebugText(renderer, x*zoom, y*zoom, text);
            }
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDebugText(renderer, 10, 10, fpsText);

        SDL_RenderPresent(renderer);
    }

    return 0;
}