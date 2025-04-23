#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

#include "defs.h"
#include "log.h"
bool STEP_MODE = false;


// Game Boy screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE 4
#define WINDOW_WIDTH (SCREEN_WIDTH * SCALE)
#define WINDOW_HEIGHT (SCREEN_HEIGHT * SCALE)

static struct option long_options[] ={
    {"debug", required_argument, 0, 'd'},
    {"step", no_argument, 0, 's'},
    {0, 0, 0, 0}
};


// Emulator state structure
typedef struct {
    uint32_t frame_buffer[SCREEN_WIDTH * SCREEN_HEIGHT]; // Frame buffer for Game Boy screen
    uint8_t buttons; // Bitfield for button states (placeholder)
} EmulatorState;

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
void update_emulator_state(EmulatorState* state) {
    // Fill frame buffer with a gradient pattern
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t r = x % 256; // Red varies with x
            uint8_t g = y % 256; // Green varies with y
            uint8_t b = 0;       // Blue fixed at 0
            uint8_t a = 255;     // Fully opaque
            state->frame_buffer[y * SCREEN_WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    // Check keyboard state for Game Boy input (placeholder)
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    state->buttons = 0;
    if (keystate[SDL_SCANCODE_UP])    state->buttons |= 1 << 0; // D-pad up
    if (keystate[SDL_SCANCODE_DOWN])  state->buttons |= 1 << 1; // D-pad down
    if (keystate[SDL_SCANCODE_LEFT])  state->buttons |= 1 << 2; // D-pad left
    if (keystate[SDL_SCANCODE_RIGHT]) state->buttons |= 1 << 3; // D-pad right
    if (keystate[SDL_SCANCODE_Z])     state->buttons |= 1 << 4; // A button
    if (keystate[SDL_SCANCODE_X])     state->buttons |= 1 << 5; // B button
    if (keystate[SDL_SCANCODE_RETURN])state->buttons |= 1 << 6; // Start
    if (keystate[SDL_SCANCODE_LSHIFT])state->buttons |= 1 << 7; // Select
}

// Render frame buffer to screen
void render(SDL_Renderer* renderer, SDL_Texture* texture, EmulatorState* state) {
    SDL_UpdateTexture(texture, NULL, state->frame_buffer, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL); // Scales to window size
    SDL_RenderPresent(renderer);
}

void parse_flags(int *argc, char ***argv){
    int c;
    int option_index = 0;
    while((c = getopt_long(*argc, *argv, "d:", long_options, &option_index)) != -1){
        switch (c){
            case 'd':

                if(strcmp(optarg, "trace") == 0){ log_set_level(LOG_TRACE); }
                if(strcmp(optarg, "debug") == 0){ log_set_level(LOG_DEBUG); }
                if(strcmp(optarg, "info") == 0){ log_set_level(LOG_INFO); }
                if(strcmp(optarg, "warn") == 0){ log_set_level(LOG_WARN); }
                if(strcmp(optarg, "error") == 0){ log_set_level(LOG_ERROR); }
                if(strcmp(optarg, "fatal") == 0){ log_set_level(LOG_FATAL); }

                break;
            case 's':
                STEP_MODE = true;
                break;
        }
    }
    *argc -= optind;
    *argv += optind;
}


int main(int argc, char* argv[]) {
    log_set_level(LOG_WARN);    

    parse_flags(&argc, &argv);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    EmulatorState state = {0};
    int running = 1;

    // Setup SDL
    initialize_sdl(&window, &renderer, &texture);

    // Setup Emulator 
    gameboy_init(argv[0]);

    // Main game loop
    while (running) {
        uint32_t start_time = SDL_GetTicks();

        handle_input(&running);
        update_emulator_state(&state);
        render(renderer, texture, &state);

        printf("Button state: %08b\n", state.buttons);
        // Cap frame rate to ~60 FPS
        uint32_t end_time = SDL_GetTicks();
        uint32_t frame_time = end_time - start_time;
        if (frame_time < 16) { // 16 ms ≈ 60 FPS
            SDL_Delay(16 - frame_time);
        }
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}