#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif


GLuint load_texture(char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);
    GLuint texture_name;
    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        surface->w, surface->h, 0,
        GL_RGB, GL_UNSIGNED_BYTE,
        surface->pixels
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    SDL_FreeSurface(surface);
    return texture_name;
}

GLuint load_transparent_texture(char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);
    GLuint texture_name;
    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        surface->w, surface->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        surface->pixels
    );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_FreeSurface(surface);
    return texture_name;
}
