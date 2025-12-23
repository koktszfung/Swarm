#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "common.h"

typedef struct {
    float x, y;
    float speed;
    float scale;
} Camera;

Camera* camera_create(float x, float y, float speed, float scale);
void camera_destroy(Camera* camera);

typedef struct {
    SDL_Texture* texture;
    SDL_Rect* rects;
    int first, last;
    int glyph_height, glyph_width;
    int total_height, total_width;
} GlyphAtlas;

GlyphAtlas* glyphatlas_create(SDL_Renderer* renderer, TTF_Font* font, uint16_t first, uint16_t last);
void glyphatlas_destroy(GlyphAtlas* glyphatlas);

void render_text(SDL_Renderer* renderer, Camera* camera, GlyphAtlas* glyphatlas, const char* text, int x, int y);
void render_text_colored(SDL_Renderer* renderer, Camera* camera, GlyphAtlas* glyphatlas, const char* text, int x, int y, uint8_t r, uint8_t g, uint8_t b);

typedef struct {
    SDL_Texture* texture;
    SDL_Rect* rects;
    int total_height, total_width;
} CircleAtlas;

CircleAtlas* circleatlas_create(SDL_Renderer* renderer, size_t idxmax);
void circleatlas_destroy(CircleAtlas* circleatlas);

void render_circle(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, int x, int y);
void render_circle_colored(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void render_circle_float(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, float x, float y);
void render_circle_float_colored(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, float x, float y, uint8_t r, uint8_t g, uint8_t b);