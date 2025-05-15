#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "explosion.h"
#include "utils.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>

typedef enum {
    PLAYING,
    GAME_OVER
} GameState;


typedef struct Tree {
    Model  trunk_model;
    GLuint trunk_tex;
    Model  leaves_model;
    GLuint leaves_tex;
    vec3   position;
    bool   collected;
} Tree;

typedef struct Rock {
    Model rock_model;
    GLuint rock_texture;

    vec3 rock_center;
    float rock_radius;
    float rock_top;
} Rock;

typedef struct Scene
{
    GameState game_state;

    Model cube;
    Material material;
    GLuint texture_id;

    Explosion explosion;
    float fire_strength;

    Tree* trees;
    int number_of_trees;
    Tree tree_prefab;

    Model campfire_model;
    GLuint campfire_tex;

    Rock rock;

} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(const vec3 light_pos, float light_intensity);


/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double delta);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene, float brightness);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void draw_ground();

void generate_trees(Scene* scene);

#endif /* SCENE_H */
