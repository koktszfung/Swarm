#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "utils.h"
#include "swarm.h"
#include "hashgrid.h"
#include "render.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\ARIAL.TTF", FONT_SIZE);  // Load a font
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Swarm",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        // SDL_RENDERER_SOFTWARE
    );
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialize
    srand(0);
    bool show_debug = false;
    bool curr_keys[SDL_NUM_SCANCODES] = {0};
    bool prev_keys[SDL_NUM_SCANCODES] = {0};
    Camera* world_camera = camera_create(0.0f, 0.0f, CAMERA_SPEED, 1.0f);
    Camera* screen_camera = camera_create(0.0f, 0.0f, 0.0f, 1.0f);
    GlyphAtlas* glyphatlas = glyphatlas_create(renderer, font, (uint16_t)33, (uint16_t)126);
    CircleAtlas* circleatlas = circleatlas_create(renderer, 254);
    HashGrid *hashgrid = hashgrid_create(16384, 256, CELL_DIM);
    Swarm *swarm = swarm_create(4096);
    if (!world_camera || !screen_camera || !glyphatlas || !circleatlas || !hashgrid || !swarm) {
        fprintf(stderr, "Failed to create model");
        if (swarm) swarm_destroy(swarm);
        if (hashgrid) hashgrid_destroy(hashgrid);
        if (circleatlas) circleatlas_destroy(circleatlas);
        if (glyphatlas) glyphatlas_destroy(glyphatlas);
        if (screen_camera) camera_destroy(screen_camera);
        if (world_camera) camera_destroy(world_camera);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    for (size_t i = 0; i < swarm->capacity; ++i) {
        swarm_add(
            swarm,
            i < (int)(swarm->capacity * 0.9) ? randf_range(1.0f, 5.0f) : randf_range(5.0f, 15.0f),  // r
            randf_range(10, WIN_WIDTH-10),
            randf_range(10, WIN_HEIGHT-10),
            randf_range(-80.0f, 80.0f),  // vx
            randf_range(-80.0f, 80.0f),  // vy
            0,  // ax
            0   // ay
        );
    }

    // Helbing social force model (contact forces)
    const float cutoff = CELL_DIM;  // cutoff distance for efficiency
    const float k = 5.0f; // body force (N/m)
    const float c = 7.5f; // damping (N·s/m)
    const float mu = 0.1f; // sliding coefficient

    bool running = true;
    SDL_Event event;

    uint32_t last_tick = SDL_GetTicks();
    float accumulator = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;  // exit if the window is closed
                    break;
                case SDL_KEYDOWN:
                    curr_keys[event.key.keysym.scancode] = true;
                    break;
                case SDL_KEYUP:
                    curr_keys[event.key.keysym.scancode] = false;
                    break;
                default:
                    break;
            }
        }

        // Camera
        world_camera->x -= (world_camera->speed / world_camera->scale)*PHYSICS_DT * curr_keys[SDL_SCANCODE_A];
        world_camera->x += (world_camera->speed / world_camera->scale)*PHYSICS_DT * curr_keys[SDL_SCANCODE_D];
        world_camera->y -= (world_camera->speed / world_camera->scale)*PHYSICS_DT * curr_keys[SDL_SCANCODE_W];
        world_camera->y += (world_camera->speed / world_camera->scale)*PHYSICS_DT * curr_keys[SDL_SCANCODE_S];
        if (world_camera->scale < MAX_SCALE && curr_keys[SDL_SCANCODE_EQUALS] && !prev_keys[SDL_SCANCODE_EQUALS]) world_camera->scale *= ZOOM_SCALE;
        if (world_camera->scale > MIN_SCALE && curr_keys[SDL_SCANCODE_MINUS]  && !prev_keys[SDL_SCANCODE_MINUS])  world_camera->scale /= ZOOM_SCALE;
        if (curr_keys[SDL_SCANCODE_TAB] && !prev_keys[SDL_SCANCODE_TAB]) show_debug = !show_debug;
        if (curr_keys[SDL_SCANCODE_R]) {
            world_camera->x = world_camera->y = 0;
            world_camera->scale = 1;
        }
        
        // Hash grid
        hashgrid_clear(hashgrid);
        for (size_t i = 0; i < swarm->count; ++i) {
            hashgrid_add(hashgrid, swarm->x[i], swarm->y[i], i);
        }

        // Physics update
        uint32_t this_tick = SDL_GetTicks();
        int32_t frame_tick = this_tick - last_tick;
        float frame_second = fminf(0.05f, (frame_tick) / 1000.0f);
        last_tick = this_tick;
        accumulator += frame_second * PHYSICS_SPEEDUP;
        while (accumulator >= PHYSICS_DT) {            
            accumulator -= PHYSICS_DT;
            for (size_t a = 0; a < swarm->count; ++a) {
                int32_t cx = floorf(swarm->x[a] / hashgrid->cell_dim);
                int32_t cy = floorf(swarm->y[a] / hashgrid->cell_dim);
                for (int8_t i = -1; i <= 1; ++i) {
                    for (int8_t j = -1; j <= 1; ++j) {
                        int32_t adj_cx = cx + i;
                        int32_t adj_cy = cy + j;
                        uint64_t adj_key =  ((uint64_t)adj_cx << 32) | (uint32_t) adj_cy;
                        HashCell* adj_cell = NULL;
                        HASH_FIND(hh, hashgrid->hashtable, &adj_key, sizeof(uint64_t), adj_cell);                    
                        if (!adj_cell) continue;
                        for (size_t idx = 0; idx < adj_cell->count; idx++) {
                            size_t adj_a = hashgrid->indextable[adj_cell->row * adj_cell->capacity + idx];
                            repel(swarm, a, adj_a, k, c, mu, cutoff);
                        }
                    }
                }
            }

            for (size_t a = 0; a < swarm->count; ++a) {
                swarm->vx[a] += swarm->ax[a] * PHYSICS_DT;
                swarm->vy[a] += swarm->ay[a] * PHYSICS_DT;
                swarm->x[a]  += swarm->vx[a] * PHYSICS_DT;
                swarm->y[a]  += swarm->vy[a] * PHYSICS_DT;
                swarm->ax[a] = 0.0f;
                swarm->ay[a] = 0.0f;
                
                // Boundary condition
                if (swarm->x[a] - swarm->r[a] < 0 || swarm->x[a] + swarm->r[a] > WIN_WIDTH) {
                    swarm->x[a] -= swarm->vx[a] * PHYSICS_DT * 2;
                    swarm->vx[a] *= -1.0f;
                }
                if (swarm->y[a] - swarm->r[a] < 0 || swarm->y[a] + swarm->r[a] > WIN_HEIGHT) {
                    swarm->y[a] -= swarm->vy[a] * PHYSICS_DT * 2;
                    swarm->vy[a] *= -1.0f;
                }
            }
        }

        // Clear background
        SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
        SDL_RenderClear(renderer);

        // Render stuff
        for (size_t i = 0; i < swarm->count; ++i) {
            render_circle_float(renderer, world_camera, circleatlas, swarm->r[i], swarm->x[i], swarm->y[i]);
        }

        // Debug
        #if DEBUG
        if (show_debug) {
            #if DEBUG_DELTATIME
            char dt_text[32];
            snprintf(dt_text, sizeof(dt_text), "%d ms", frame_tick);
            render_text_colored(renderer, screen_camera, glyphatlas, dt_text, 10, 10, 0, 0, 0);
            #endif
            #if DEBUG_CAMERA_POS
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &(SDL_Rect){
                (float)WIN_WIDTH  * (1 - 1/ZOOM_SCALE)/2.0f,
                (float)WIN_HEIGHT * (1 - 1/ZOOM_SCALE)/2.0f,
                (float)WIN_WIDTH  * 1 / ZOOM_SCALE,
                (float)WIN_HEIGHT * 1 / ZOOM_SCALE
            });
            #endif
            #if DEBUG_TEXTURE
            SDL_Rect src = {0, 0, circleatlas->total_width, circleatlas->total_height};
            SDL_Rect dst = {10, 10, circleatlas->total_width, circleatlas->total_height};
            SDL_RenderCopy(renderer, circleatlas->texture, &src, &dst);
            #endif  // DEBUG_TEXTURE
            #if DEBUG_HASHCELL_RECT
            SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
            HashCell *cell, *tmp;
            HASH_ITER(hh, hashgrid->hashtable, cell, tmp) {
                int32_t cx = cell->key >> 32;
                int32_t cy = cell->key;
                SDL_Rect r;
                r.w = hashgrid->cell_dim;
                r.h = hashgrid->cell_dim;
                r.x = cx * hashgrid->cell_dim;
                r.y = cy * hashgrid->cell_dim;
                SDL_RenderDrawRect(renderer, &r);
            }
            #endif  // DEBUG_HASHCELL_RECT
            #if DEBUG_HASHCELL_COORD
            SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
            HashCell *cell, *tmp;
            HASH_ITER(hh, hashgrid->hashtable, cell, tmp) {
                int32_t cx = cell->key >> 32;
                int32_t cy = cell->key;
                int x = cx * hashgrid->cell_dim;
                int y = cy * hashgrid->cell_dim;
                char coord_text[32];
                snprintf(coord_text, sizeof(coord_text), "(%d, %d)", x, y);
                render_text_colored(renderer, screen_camera, glyphatlas, coord_text, x + 10, y + 10, 50, 255, 50);
            }
            #endif  // DEBUG_HASHCELL_COORD
            #if DEBUG_SWARM_COORD
            for (size_t i = 0; i < swarm->count; ++i) {
                char coord_text[32];
                snprintf(coord_text, sizeof(coord_text), "(%.0f, %.0f)", swarm->x[i], swarm->y[i]);
                render_text(renderer, screen_camera, glyphatlas, coord_text, swarm->x[i] + 10, swarm->y[i] - 10);
            }
            #endif  // DEBUG_SWARM_COORD
            #if DEBUG_SWARM_NEIGHBOUR
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            for (size_t a = 0; a < swarm->count; ++a) {            
                float x = swarm->x[a];
                float y = swarm->y[a];
                int32_t cx = floorf(x / hashgrid->cell_dim);
                int32_t cy = floorf(y / hashgrid->cell_dim);
                for (int8_t i = -1; i <= 1; ++i) {
                    for (int8_t j = -1; j <= 1; ++j) {
                        int32_t adj_cx = cx + i;
                        int32_t adj_cy = cy + j;
                        uint64_t adj_key =  ((uint64_t)adj_cx << 32) | (uint32_t) adj_cy;
                        HashCell* adj_cell = NULL;
                        HASH_FIND(hh, hashgrid->hashtable, &adj_key, sizeof(uint64_t), adj_cell);                    
                        if (!adj_cell) continue;
                        for (size_t k = 0; k < adj_cell->count; ++k) {
                            size_t adj_a = hashgrid->indextable[adj_cell->row * adj_cell->capacity + k];
                            if (a == adj_a) continue;
                            float adj_x = swarm->x[adj_a];
                            float adj_y = swarm->y[adj_a];
                            SDL_RenderDrawLine(renderer, x, y, adj_x, adj_y);
                        }
                    }
                }   
            }
            SDL_SetTextureColorMod(glyphatlas->texture, 255, 255, 255);
            #endif  // DEBUG_SWARM_NEIGHBOUR
        }
        #endif  // DEBUG
        
        memcpy(prev_keys, curr_keys, sizeof(curr_keys));
        SDL_RenderPresent(renderer);
    }

    swarm_destroy(swarm);
    hashgrid_destroy(hashgrid);
    glyphatlas_destroy(glyphatlas);
    camera_destroy(world_camera);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
