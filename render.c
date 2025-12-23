#include "render.h"

Camera* camera_create(float x, float y, float speed, float scale) {
    Camera* camera = malloc(sizeof(*camera));
    if (!camera) {
        fprintf(stderr, "Failed to allocate memory for Camera");
        return NULL;
    }

    camera->x = x;
    camera->y = y;
    camera->speed = speed;
    camera->scale = scale;

    return camera;
}

void camera_destroy(Camera* camera) {
    if (!camera) return;
    
    free(camera);
}

GlyphAtlas* glyphatlas_create(SDL_Renderer* renderer, TTF_Font* font, uint16_t first, uint16_t last) {
    GlyphAtlas* glyphatlas = malloc(sizeof(*glyphatlas));
    if (!glyphatlas) {
        fprintf(stderr, "Failed to allocate memory for GlyphAtlas");
        return NULL;
    }

    glyphatlas->rects = malloc((last - first + 1) * sizeof(*glyphatlas->rects));
    if (!glyphatlas->rects) {
        fprintf(stderr, "Failed to allocate memory for GlyphAtlas->rects");
        return NULL;
    }

    glyphatlas->first = first;
    glyphatlas->last = last;
    glyphatlas->glyph_height = TTF_FontHeight(font);          // max glyph height
    glyphatlas->glyph_width  = glyphatlas->glyph_height / 3;  // monospace
    
    // Simple packing: one row, glyphs side-by-side
    int total_width = 0;
    for (uint16_t c = first; c <= last; ++c) {
        if (!TTF_GlyphIsProvided(font, c)) continue;
        int xmin, xmax, ymin, ymax, advance;
        TTF_GlyphMetrics(font, c, &xmin, &xmax, &ymin, &ymax, &advance);
        total_width += advance;
    }
    glyphatlas->total_height = glyphatlas->glyph_height;
    glyphatlas->total_width = total_width;

    // Create target surface for the whole texture
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0, total_width, glyphatlas->glyph_height,
        32, SDL_PIXELFORMAT_RGBA32
    );
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_NONE);
    SDL_FillRect(surf, NULL, 0x00000000);  // transparent black

    int x = 0;
    SDL_Color white = {255,255,255,255};
    for (uint16_t c = first; c <= last; ++c) {
        if (!TTF_GlyphIsProvided(font, c)) {
            glyphatlas->rects[c - first].w = glyphatlas->glyph_width;  // space for missing chars
            glyphatlas->rects[c - first].h = 0;
            continue;
        }

        SDL_Surface* glyph_surf = TTF_RenderGlyph_Solid(font, c, white);
        if (glyph_surf) {
            SDL_Rect dst = {x, 0, glyph_surf->w, glyph_surf->h};
            SDL_BlitSurface(glyph_surf, NULL, surf, &dst);

            glyphatlas->rects[c - first] = (SDL_Rect){x, 0, glyph_surf->w, glyph_surf->h};
            x += glyph_surf->w;

            SDL_FreeSurface(glyph_surf);
        }
    }

    // Upload to GPU once
    glyphatlas->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureBlendMode(glyphatlas->texture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(surf);

    return glyphatlas;
}

void glyphatlas_destroy(GlyphAtlas* glyphatlas) {
    if (!glyphatlas) return;
    
    SDL_DestroyTexture(glyphatlas->texture);
    free(glyphatlas->rects);
    
    free(glyphatlas);
}

void render_text(SDL_Renderer* renderer, Camera* camera, GlyphAtlas* glyphatlas, const char* text, int x, int y) {
    int pen_x = x;

    while (*text) {
        unsigned char c = *text++;

        if (c == ' ') {
            pen_x += glyphatlas->glyph_width;
            continue;
        }

        SDL_Rect src = glyphatlas->rects[c - glyphatlas->first];
        if (src.h == 0) continue;

        SDL_Rect dst = {pen_x - camera->x, y - camera->y, src.w, src.h};
        dst.x = (dst.x - WIN_WIDTH/2) *camera->scale + WIN_WIDTH/2;
        dst.y = (dst.y - WIN_HEIGHT/2)*camera->scale + WIN_HEIGHT/2;
        dst.w *= camera->scale;
        dst.h *= camera->scale;
        SDL_RenderCopy(renderer, glyphatlas->texture, &src, &dst);

        pen_x += src.w;  // monospace
    }
}

void render_text_colored(SDL_Renderer* renderer, Camera* camera, GlyphAtlas* glyphatlas, const char* text, int x, int y,
                         uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetTextureColorMod(glyphatlas->texture, r, g, b);
    render_text(renderer, camera, glyphatlas, text, x, y);
    SDL_SetTextureColorMod(glyphatlas->texture, 255, 255, 255);
}

void set_pixel(SDL_Surface *surface, int x, int y, uint32_t color) {
    *(uint32_t *)((uint8_t *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel) = color;
}

void set_circle(SDL_Surface *surface, int diameter, int i, int j, uint32_t color) {
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    
    float radius = ((float)diameter - 1)/2;
    float x = (diameter % 2 == 0)? 0.5 : 0;
    float y = radius;
    float cx = j + radius;
    float cy = i + radius;
    float p = (diameter % 2 == 0)? 1.5 - radius : 1.25 - radius;

    while (x <= y) {
        set_pixel(surface, cx + x, cy + y, color);
        set_pixel(surface, cx + x, cy - y, color);
        set_pixel(surface, cx - x, cy + y, color);
        set_pixel(surface, cx - x, cy - y, color);
        set_pixel(surface, cx + y, cy + x, color);
        set_pixel(surface, cx + y, cy - x, color);
        set_pixel(surface, cx - y, cy + x, color);
        set_pixel(surface, cx - y, cy - x, color);
        if (p < 0) {
            p += 2*x + 3;
        } else {
            p += 2*(x - y) + 5;
            y--;
        }
        x++;
    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

CircleAtlas* circleatlas_create(SDL_Renderer* renderer, size_t idxmax) {
    CircleAtlas* circleatlas = malloc(sizeof(*circleatlas));
    if (!circleatlas) {
        fprintf(stderr, "Failed to allocate memory for CircleAtlas");
        return NULL;
    }
    circleatlas->rects = malloc(idxmax * sizeof(*circleatlas->rects));
    if (!circleatlas->rects) {
        fprintf(stderr, "Failed to allocate memory for CircleAtlas->rects");
        free(circleatlas);
        return NULL;
    }

    // Two-row packing: first row small to big, second row big to small
    size_t idxmin = 3;
    size_t idxswitch = ceil((-1 + sqrt((1+idxmin)*(1+idxmin)/4 + 2*idxmax*(idxmax + 1)))/2);  // when top width = bottom width
    circleatlas->total_height = 2*idxswitch + 1;
    circleatlas->total_width = (idxmin + idxswitch)*(idxswitch - idxmin + 1)/2;

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    if (circleatlas->total_width > info.max_texture_width || circleatlas->total_height > info.max_texture_height) {
        fprintf(stderr, "CircleAtlas texture size (%d x %d) exceeds maximum allowed (%d x %d)\n",
            circleatlas->total_width, circleatlas->total_height,
            info.max_texture_width, info.max_texture_height);
        free(circleatlas->rects);
        free(circleatlas);
        return NULL;
    }

    // Create target surface for the whole texture
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0, circleatlas->total_width, circleatlas->total_height,
        32, SDL_PIXELFORMAT_RGBA32
    );
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_NONE);
    SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 0, 0, 0, 0));  // transparent black

    // Render circles
    uint32_t white = SDL_MapRGBA(surf->format, 255, 255, 255, 255);
    //   First row
    for (size_t idx = idxmin - 1; idx < idxswitch; idx++) {
        size_t imin = 0;
        size_t jmin = (idxmin + idx)*(idx - idxmin + 1)/2;
        circleatlas->rects[idx] = (SDL_Rect){jmin, imin, idx+1, idx+1};
        set_circle(surf, idx-1, imin+1, jmin+1, white);
    }
    //   Second row
    for (size_t idx = idxswitch; idx < idxmax; idx++) {
        size_t imin = 2*idxswitch - idx;
        size_t jmin = circleatlas->total_width-1 - (idxswitch+1 + idx)*(idx - idxswitch)/2 - idx;
        circleatlas->rects[idx] = (SDL_Rect){jmin, imin, idx+1, idx+1};
        set_circle(surf, idx-1, imin+1, jmin+1, white);
    }

    // Upload to GPU once
    circleatlas->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_SetTextureBlendMode(circleatlas->texture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(surf);

    return circleatlas;
}

void circleatlas_destroy(CircleAtlas* circleatlas) {
    if (!circleatlas) return;

    SDL_DestroyTexture(circleatlas->texture);
    free(circleatlas->rects);
    
    free(circleatlas);
}

void render_circle(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, int x, int y) {
    size_t circle_idx = (int)roundf(radius * 2 * camera->scale) - 1;
    SDL_Rect src = circleatlas->rects[circle_idx];
    SDL_Rect dst = {
        (x - camera->x - WIN_WIDTH/2)  * camera->scale + WIN_WIDTH/2  - src.w/2,
        (y - camera->y - WIN_HEIGHT/2) * camera->scale + WIN_HEIGHT/2 - src.h/2,
        src.w,
        src.h
    };
    SDL_RenderCopy(renderer, circleatlas->texture, &src, &dst);
}

void render_circle_colored(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, int x, int y,
                           uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetTextureColorMod(circleatlas->texture, r, g, b);
    render_circle(renderer, camera, circleatlas, radius, x, y);
    SDL_SetTextureColorMod(circleatlas->texture, 255, 255, 255);
}

void render_circle_float(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, float x, float y) {
    size_t circle_idx = (int)roundf(radius * 2.0f * camera->scale) - 1;
    SDL_Rect src = circleatlas->rects[circle_idx];
    SDL_FRect dst = {
        (x - camera->x - (float)(WIN_WIDTH)/2.0f)  * camera->scale + (float)(WIN_WIDTH)/2.0f  - (float)(src.w)/2.0f,
        (y - camera->y - (float)(WIN_HEIGHT)/2.0f) * camera->scale + (float)(WIN_HEIGHT)/2.0f - (float)(src.h)/2.0f,
        src.w,
        src.h
    };
    SDL_RenderCopyF(renderer, circleatlas->texture, &src, &dst);
}

void render_circle_float_colored(SDL_Renderer* renderer, Camera* camera, CircleAtlas* circleatlas, float radius, float x, float y,
                           uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetTextureColorMod(circleatlas->texture, r, g, b);
    render_circle_float(renderer, camera, circleatlas, radius, x, y);
    SDL_SetTextureColorMod(circleatlas->texture, 255, 255, 255);
}
