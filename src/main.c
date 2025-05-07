#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "argparse.h"
#include "gameboy.h"

gflags_t gflags;

static void render(SDL_Renderer* renderer, SDL_Texture* texture, Gameboy *gb);
static void handle_input(int* running);
static GameboyState update_emulator_state(Gameboy *gb);
static void initialize_sdl(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture);


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
        } 
        else if (strcmp(log_level, "warn") == 0) {
            log_set_level(LOG_WARN);
        }
        else if (strcmp(log_level, "error") == 0) {
            log_set_level(LOG_ERROR);
        }
        else {
            fprintf(stderr, "Invalid log level: %s", log_level);
            exit(1);
        }
    }
    log_warn("Log level: %s", log_level);
}


void parse_flags(int argc, const char **argv){
    const char *const usages[] = {
        "basic [options] [[--] args]", 
        "basic [options]",
        NULL
    };

    const char *step_at_str = NULL;


    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('a', "step-at", &step_at_str, "Step at specific address", NULL, 0, 0),
        OPT_BOOLEAN('t', "trace", &gflags.trace_state, "Print each instruction executed", NULL, 0, 0),
        OPT_STRING('f', "trace-file", &gflags.trace_file, "File to write trace to", NULL, 0, 0),
        OPT_END()
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "CBC - Gameboy Emulator", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    if(step_at_str){
        gflags.step_at.address = (uint16_t)strtol(step_at_str, NULL, 16);
    }
}

int main(int argc, const char* argv[]) {
    parse_flags(argc, argv);
    
    logging_init();

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    int running = 1;

    // Setup SDL
    // initialize_sdl(&window, &renderer, &texture);

    // Setup Emulator 
    Gameboy *gb = gameboy_new(argv[0]);

    if(!gb){
        log_error("Failed to create Gameboy instance");
        goto cleanup;
    }

    log_info("Gameboy created successfully");

    // Main game loop
    while (running) {
        const uint64_t start_time = SDL_GetTicks64();

        // handle_input(&running);

        GameboyState state = update_emulator_state(gb);

        if (state == GAMEBOY_ERROR){
            log_error("Gameboy state is error");
            running = 0;
        }

        // render(renderer, texture, gb);

        // Cap frame rate to ~60 FPS
        const uint64_t frame_time = SDL_GetTicks64() - start_time;

        const uint64_t delay_start = SDL_GetTicks64() - frame_time;
        while (SDL_GetTicks64() < delay_start + 16) {}

        // log_trace("Tick");
    }

cleanup:
    // Cleanup
    // gameboy_free(gb);
    gb->free(gb);
    // SDL_DestroyTexture(texture);
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return 0;
}


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
GameboyState update_emulator_state(Gameboy *gb) {
    // update internal gameboy state
    // one frame is 70224 cycles
    static gbcycles_t total_cycles = 0;
    static gbcycles_t cycles = 0;
    while(1){
        cycles += gb->tick(gb);

        if (cycles == 0xFFFFFFFF){
            log_error("Gameboy state is error");
            return GAMEBOY_ERROR;
        }

        if (gb->is_stuck){
            log_error("Gameboy is stuck");
            return GAMEBOY_ERROR;
        }

        if (cycles >= 70224){
            break;
        }
    }
    total_cycles += cycles;
    // create offset for next frame 
    cycles -= 70224;
    // log_warn("Total cycles: %zu", total_cycles);
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

    return GAMEBOY_RUNNING;
}

// Render frame buffer to screen
void render(SDL_Renderer* renderer, SDL_Texture* texture, Gameboy *gb) {
    SDL_UpdateTexture(texture, NULL, gb->frame_buffer, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL); // Scales to window size
    SDL_RenderPresent(renderer);
}
