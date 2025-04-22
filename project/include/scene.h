#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "explosion.h"

#include <GL/gl.h>
#include <obj/model.h>
#include "utils.h"
#include <stdbool.h>

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
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

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
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void draw_ground();

void generate_trees(Scene* scene);

#endif /* SCENE_H */
