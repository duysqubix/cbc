#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "gameboy.h"

void logging_init(){
    const char *log_level = getenv("LOG_LEVEL");
    log_set_level(LOG_WARN);
    if (log_level){
        if (strcmp(log_level, "trace") == 0) {
            log_set_level(LOG_TRACE);
        } else if (strcmp(log_level, "debug") == 0) {
            log_set_level(LOG_DEBUG);
        } else if (strcmp(log_level, "info") == 0) {
            log_set_level(LOG_INFO);
        } else {
            fprintf(stderr, "Invalid log level: %s", log_level);
            exit(1);
        }
    }
    log_warn("Log level: %s", log_level);
}


static struct option long_options[] ={
    {"debug", required_argument, 0, 'd'},
    {0, 0, 0, 0}
};


// Emulator state structure

// Initialize SDL, window, renderer, and texture
void initialize_sdl(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *window = SDL_CreateWindow("Game Boy Emulator",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (*window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }

    *texture = SDL_CreateTexture(*renderer,
                                 SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 SCREEN_WIDTH, SCREEN_HEIGHT);
    if (*texture == NULL) {
        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }

    // Use nearest neighbor scaling for pixelated retro look
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

// Handle input events (e.g., quitting)
void handle_input(int* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // if (event.type == SDL_QUIT) {
        //     *running = 0;
        // }
        switch (event.type){
            case SDL_QUIT:
                *running = 0;
                break;
            default:
                break;
        }
    }
}

// Update emulator state (placeholder with gradient for testing)
void update_emulator_state(Gameboy *gb, int *running) {
    // update internal gameboy state
    // one frame is 70224 cycles

    static size_t total_cycles = 0;
    static gbcycles_t cycles = 0;
    gbcycles_t cur_cycles = 0;
    while(1){
        cur_cycles = gb->tick(gb);
        if (cur_cycles == 0xFFFFFFFF){
            log_error("Gameboy state is error");
            *running = 0;
            break;
        }

        if (gb->is_stuck){
            *running = 0;
            break;
        }

        cycles += cur_cycles;
        if (cycles >= 70224){
            break;
        }
    }
    total_cycles += cycles;
    cycles -= 70224;

    // Fill frame buffer with a gradient pattern
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t r = x % 256; // Red varies with x
            uint8_t g = y % 256; // Green varies with y
            uint8_t b = 0;       // Blue fixed at 0
            uint8_t a = 255;     // Fully opaque
            gb->frame_buffer[y * SCREEN_WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    // Check keyboard state for Game Boy input (placeholder)
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    gb->buttons = 0;
    if (keystate[SDL_SCANCODE_UP])    gb->buttons |= 1 << 0; // D-pad up
    if (keystate[SDL_SCANCODE_DOWN])  gb->buttons |= 1 << 1; // D-pad down
    if (keystate[SDL_SCANCODE_LEFT])  gb->buttons |= 1 << 2; // D-pad left
    if (keystate[SDL_SCANCODE_RIGHT]) gb->buttons |= 1 << 3; // D-pad right
    if (keystate[SDL_SCANCODE_Z])     gb->buttons |= 1 << 4; // A button
    if (keystate[SDL_SCANCODE_X])     gb->buttons |= 1 << 5; // B button
    if (keystate[SDL_SCANCODE_RETURN])gb->buttons |= 1 << 6; // Start
    if (keystate[SDL_SCANCODE_LSHIFT])gb->buttons |= 1 << 7; // Select
}

// Render frame buffer to screen
void render(SDL_Renderer* renderer, SDL_Texture* texture, Gameboy *gb) {
    SDL_UpdateTexture(texture, NULL, gb->frame_buffer, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL); // Scales to window size
    SDL_RenderPresent(renderer);
}

void parse_flags(int *argc, char ***argv){
    int c;
    int option_index = 0;
    while((c = getopt_long(*argc, *argv, "d:", long_options, &option_index)) != -1){
        switch (c){
            default:
                break;
        }
    }
    *argc -= optind;
    *argv += optind;
}


int main(int argc, char* argv[]) {

    if(argc < 2){
        log_error("Usage: <rom_file> <optional flags>");
        return 1;
    }

    logging_init();

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    int running = 1;

    // Setup SDL
    initialize_sdl(&window, &renderer, &texture);

    // Setup Emulator 
    Gameboy *gb = gameboy_new(argv[1]);

    if(!gb){
        log_error("Failed to create Gameboy instance");
        goto cleanup;
    }

    log_info("Gameboy created successfully");

    // Main game loop
    while (running) {
        uint32_t start_time = SDL_GetTicks();

        handle_input(&running);
        update_emulator_state(gb, &running);
        render(renderer, texture, gb);

        // Cap frame rate to ~60 FPS
        uint32_t end_time = SDL_GetTicks();
        uint32_t frame_time = end_time - start_time;
        if (frame_time < 16) { // 16 ms ≈ 60 FPS
            SDL_Delay(16 - frame_time);
        }
    }

cleanup:
    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
