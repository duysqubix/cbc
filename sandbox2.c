#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ROWS 10
#define BYTES_PER_ROW 16

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Buffer Display",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        240,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load font (replace with a valid path to a .ttf file on your system)
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 16);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Get font height for positioning
    int font_height = TTF_FontHeight(font);

    // Initialize buffer with sample data
    uint8_t buffer[1024];
    for (int i = 0; i < 1024; i++) {
        buffer[i] = rand() % 256;
    }

    // Array to store textures for each row
    SDL_Texture* textures[NUM_ROWS];
    for (int row = 0; row < NUM_ROWS; row++) {
        char str[256];
        int offset = row * BYTES_PER_ROW;
        sprintf(str, "$%04X: ", offset);
        for (int i = 0; i < BYTES_PER_ROW; i++) {
            char byte_str[5];
            sprintf(byte_str, "$%02X ", buffer[offset + i]);
            strcat(str, byte_str);
        }

        // Render text to surface
        SDL_Color color = {255, 255, 255, 255}; // White text
        SDL_Surface* surface = TTF_RenderText_Solid(font, str, color);
        if (surface == NULL) {
            fprintf(stderr, "TTF_RenderText_Solid Error: %s\n", TTF_GetError());
            for (int i = 0; i < row; i++) {
                SDL_DestroyTexture(textures[i]);
            }
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        // Create texture from surface
        textures[row] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (textures[row] == NULL) {
            fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
            for (int i = 0; i < row; i++) {
                SDL_DestroyTexture(textures[i]);
            }
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
    }

    // Main event loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear renderer with black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render each row
        for (int row = 0; row < NUM_ROWS; row++) {
            int w, h;
            SDL_QueryTexture(textures[row], NULL, NULL, &w, &h);
            SDL_Rect dstrect = {10, 10 + row * font_height, w, h};
            SDL_RenderCopy(renderer, textures[row], NULL, &dstrect);
        }

        // Update display
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Reduce CPU usage
    }

    // Cleanup
    for (int i = 0; i < NUM_ROWS; i++) {
        SDL_DestroyTexture(textures[i]);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}