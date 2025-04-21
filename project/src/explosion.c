#include "explosion.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRAVITY_Z      -1.5
#define MAX_LIFETIME    3.0
#define PARTICLE_SCALE  (0.1f / 3.0f)

#define MAX_HEIGHT   0.2
#define BASE_RADIUS  0.2
#define UP_SPEED     0.5
#define SIDE_SPREAD  0.05

static void reset_particle(Particle* p)
{
    double u = (double)rand() / RAND_MAX;
    double h = u * MAX_HEIGHT;
    double r = BASE_RADIUS * (1.0 - u * u * u);

    double theta = ((double)rand() / RAND_MAX) * 2.0 * M_PI;
    p->position.x = r * cos(theta);
    p->position.y = r * sin(theta);
    p->position.z = h;

    p->velocity.x = (((double)rand() / RAND_MAX) * 2.0 - 1.0) * SIDE_SPREAD;
    p->velocity.y = (((double)rand() / RAND_MAX) * 2.0 - 1.0) * SIDE_SPREAD;
    p->velocity.z = UP_SPEED + ((double)rand() / RAND_MAX) * 0.5;

    p->lifetime   = ((double)rand() / RAND_MAX) * MAX_LIFETIME;
}

void init_explosion(Explosion* explosion)
{
    load_model(&explosion->model, "assets/models/cube.obj");
    explosion->texture_id = load_texture("assets/textures/cube.png");

    for (int i = 0; i < N_PARTICLES; ++i) {
        reset_particle(&explosion->particles[i]);
    }
}

void update_explosion(Explosion* explosion, double elapsed_time)
{
    for (int i = 0; i < N_PARTICLES; ++i) {
        Particle* p = &explosion->particles[i];

        p->lifetime += elapsed_time;
        if (p->lifetime >= MAX_LIFETIME || p->position.z <= 0.0) {
            reset_particle(p);
            continue;
        }

        p->velocity.z += GRAVITY_Z * elapsed_time;

        p->position.x += p->velocity.x * elapsed_time;
        p->position.y += p->velocity.y * elapsed_time;
        p->position.z += p->velocity.z * elapsed_time;
    }
}

const float fireX =  3.0f;
const float fireY =  0.0f;
const float fireZ =  0.0f;

void render_explosion(const Explosion* explosion)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPushMatrix();
      glTranslatef(fireX, fireY, fireZ);

      for (int i = 0; i < N_PARTICLES; ++i) {
          const Particle* p = &explosion->particles[i];

          glPushMatrix();
            glTranslated(p->position.x,
                        p->position.y,
                        p->position.z);
            glScaled(PARTICLE_SCALE,
                     PARTICLE_SCALE,
                     PARTICLE_SCALE);

            double lifetime = p->lifetime;
            int phase      = (int)lifetime;
            double t       = lifetime - phase;
            switch (phase) {
              case 0:
                glColor3f(1.0f, 1.0f, 1.0f - t);
                break;
              case 1:
                glColor3f(1.0f, 1.0f - t, 0.0f);
                break;
              case 2:
                glColor3f(1.0f - t, 0.0f, 0.0f);
                break;
              default:
                break;
            }

            draw_model(&explosion->model);
          glPopMatrix();
      }

    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void destroy_explosion(Explosion* explosion)
{
    free_model(&explosion->model);
    glDeleteTextures(1, &explosion->texture_id);
}
