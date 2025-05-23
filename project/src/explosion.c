#include "explosion.h"



#define GRAVITY_Z -1.5
#define MAX_LIFETIME 3.0
#define PARTICLE_SCALE (0.1f / 3.0f)

#define MAX_HEIGHT 0.2
#define BASE_RADIUS 0.2
#define UP_SPEED 0.5
#define SIDE_SPREAD 0.05

const float fireX =  3.0f;
const float fireY =  0.0f;
const float fireZ =  0.0f;

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

void render_explosion(const Explosion* e)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPushMatrix();
      glTranslatef(e->position.x, e->position.y, e->position.z);
      glScalef(e->size_scale, e->size_scale, e->size_scale);

      for (int i = 0; i < N_PARTICLES; ++i) {
          const Particle* p = &e->particles[i];
          glPushMatrix();
            glTranslatef(p->position.x, p->position.y, p->position.z);

            float particleScale = PARTICLE_SCALE;
            glScalef(particleScale, particleScale, particleScale);

            double lifetime = p->lifetime;
            int phase = (int)lifetime;
            double t = lifetime - phase;
            switch (phase) {
                case 0: glColor3f(1, 1, 1 - t); break;
                case 1: glColor3f(1, 1 - t, 0); break;
                case 2: glColor3f(1 - t, 0, 0); break;
                default: glColor3f(0,0,0); break;
            }

            draw_model(&e->model);
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

