#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "texture.h"
#include "utils.h"

#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_PARTICLES 500

typedef struct Particle {
  vec3 position;
  vec3 velocity;
  double lifetime;
} Particle;

typedef struct Explosion {
  Model model;
  GLuint texture_id;
  Particle particles[N_PARTICLES];
  vec3 position;
  float size_scale;
} Explosion;

void update_explosion(Explosion* explosion, double elapsed_time);
void render_explosion(const Explosion* explosion);
void destroy_explosion(Explosion* explosion);

/**
 * Initialize the particles of the explosion.
 */
void init_explosion(Explosion* explosion);

/**
 * Update the position of the particles.
 */
void update_explosion(Explosion* explosion, double elapsed_time);

/**
 * Render the particles.
 */
//void render_explosion(Explosion* explosion);
void render_explosion(const Explosion* explosion);

/**
 * Deallocate the resources.
 */
void destroy_explosion(Explosion* explosion);

#endif /* EXPLOSION_H */

