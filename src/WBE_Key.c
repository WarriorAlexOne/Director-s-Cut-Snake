#include <stdio.h>
#include <stdlib.h>

#include "WBE_Key.h"

WBE_KeyCheck* WBE_CreateKeyChecker () {
    WBE_KeyCheck* keyChecker = (WBE_KeyCheck*)malloc(sizeof(WBE_KeyCheck));
    if (keyChecker) {
        WBE_InitKeyCheck(keyChecker);
    }
    return keyChecker;
}

void WBE_InitKeyCheck (WBE_KeyCheck* keyCheckObject) {
    keyCheckObject->lastKeyDown = 0;
    keyCheckObject->lastKeyUp = 0;
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        keyCheckObject->keyDown[i] = 0;
        keyCheckObject->keyUp[i] = 0;
        keyCheckObject->keyPressed[i] = 0;
        keyCheckObject->keyReleased[i] = 0;
        keyCheckObject->wasPressed[i] = 0;
        keyCheckObject->wasReleased[i] = 0;
    }
}

void WBE_UpdateKeyCheck (WBE_KeyCheck* keyCheckObject, SDL_Event event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        keyCheckObject->lastKeyDown = event.key.scancode;
        keyCheckObject->lastKeyUp = event.key.scancode;
        keyCheckObject->keyDown[keyCheckObject->lastKeyDown] = true;
        keyCheckObject->keyUp[keyCheckObject->lastKeyUp] = false;
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        keyCheckObject->lastKeyDown = event.key.scancode;
        keyCheckObject->lastKeyUp = event.key.scancode;
        keyCheckObject->keyUp[keyCheckObject->lastKeyUp] = true;
        keyCheckObject->keyDown[keyCheckObject->lastKeyDown] = false;
    }
    
    keyCheckObject->keyPressed[keyCheckObject->lastKeyDown] = false;
    keyCheckObject->keyReleased[keyCheckObject->lastKeyUp] = false;
    if (event.type == SDL_EVENT_KEY_DOWN && !keyCheckObject->wasPressed[keyCheckObject->lastKeyDown]) {
        keyCheckObject->keyPressed[keyCheckObject->lastKeyDown] = true;
        keyCheckObject->wasPressed[keyCheckObject->lastKeyDown] = true;
        keyCheckObject->wasReleased[keyCheckObject->lastKeyUp] = false;
    }
    if (event.type == SDL_EVENT_KEY_UP && !keyCheckObject->wasReleased[keyCheckObject->lastKeyUp]) {
        keyCheckObject->keyReleased[keyCheckObject->lastKeyUp] = true;
        keyCheckObject->wasReleased[keyCheckObject->lastKeyUp] = true;
        keyCheckObject->wasPressed[keyCheckObject->lastKeyDown] = false;
    }
}

bool WBE_IsKeyDown (WBE_KeyCheck* keyCheckObject, int sdl_scancode) {
    if (keyCheckObject->keyDown[sdl_scancode]) return true;
    return false;
}
bool WBE_IsKeyUp (WBE_KeyCheck* keyCheckObject, int sdl_scancode) {
    if (keyCheckObject->keyUp[sdl_scancode]) return true;
    return false;
}
bool WBE_IsKeyPressed (WBE_KeyCheck* keyCheckObject, int sdl_scancode) {
    if (keyCheckObject->keyPressed[sdl_scancode]) return true;
    return false;
}
bool WBE_IsKeyReleased (WBE_KeyCheck* keyCheckObject, int sdl_scancode) {
    if (keyCheckObject->keyReleased[sdl_scancode]) return true;
    return false;
}
