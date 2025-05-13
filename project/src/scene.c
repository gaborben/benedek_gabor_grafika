#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <GL/glu.h>

#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <math.h>

int grass_texture;

void init_scene(Scene* scene)
{
    load_model(&(scene->cube), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    grass_texture = load_texture("assets/textures/grass.jpg");

    init_explosion(&scene->explosion);
    scene->explosion.position.x = 3.0f;
    scene->explosion.position.y = 0.0f;
    scene->explosion.position.z = 0.0f;

    load_model(&scene->tree_prefab.trunk_model, "assets/models/tree.obj");
    scene->tree_prefab.trunk_tex = load_transparent_texture("assets/textures/tree.png");
    load_model(&scene->tree_prefab.leaves_model, "assets/models/leaves.obj");
    scene->tree_prefab.leaves_tex  = load_transparent_texture("assets/textures/leaves.png");

    scene->tree_prefab.position.x = 0;
    scene->tree_prefab.position.y = 0;
    scene->tree_prefab.position.z = 0;

    load_model(&scene->campfire_model, "assets/models/bonfire.obj");
    scene->campfire_tex = load_transparent_texture("assets/textures/tree.png");
    //load_model(&scene->campfire_model, "assets/models/campfire2/campfire2.obj");
    //printf("campfire_tex = %u\n", scene->campfire_tex);

    load_model(&scene->rock_model, "assets/models/low-poly-rock-tri.obj");

    scene->game_state = PLAYING;
    scene->fire_strength = 1.0f;

    generate_trees(scene);

    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting(const vec3 light_pos, float light_intensity)
{
    // Ambient
    GLfloat amb[]  = { 0.2f * light_intensity,
                       0.1f * light_intensity,
                       0.05f* light_intensity,
                       1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

    // Diffuse
    GLfloat dif[]  = { light_intensity,
                       light_intensity * 0.8f,
                       light_intensity * 0.4f,
                       1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

    // Specular
    GLfloat spec[] = { light_intensity,
                       light_intensity,
                       light_intensity,
                       1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    // Position
    GLfloat pos[]  = { light_pos.x, light_pos.y, light_pos.z, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, double delta)
{
    update_explosion(&scene->explosion, delta);
}

void render_scene(const Scene* scene, float brightness)
{
    //flicker
    double t = SDL_GetTicks() / 1000.0;
    float base_strength = scene->fire_strength;
    float flicker_intensity =
          0.10f * sinf(t * 10.3f)
        + 0.06f * sinf(t * 17.7f)
        + 0.04f * (((rand() % 100) / 100.0f) - 0.5f);
    float light_strength = fminf(1.0f, fmaxf(0.0f, base_strength + flicker_intensity));

    vec3 fire_pos = scene->explosion.position;
    vec3 light_pos = {
        fire_pos.x + 0.1f  * sinf(t * 5.0f),
        fire_pos.y + 0.05f * sinf(t * 7.0f),
        fire_pos.z
    };

    //light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightCol[4] = {
        1.0f,
        0.3f + 0.7f * light_strength,
        0.1f * (1.0f - light_strength),
        1.0f
    };
    GLfloat lightPosArr[4] = {
        light_pos.x,
        light_pos.y,
        light_pos.z,
        1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosArr);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);

    //ground
    set_material(&scene->material);
    draw_ground();

    //trees
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < scene->number_of_trees; ++i) {
        const Tree* T = &scene->trees[i];
        glPushMatrix();
            glTranslatef(T->position.x, T->position.y, T->position.z);
            glScalef(0.2f, 0.2f, 0.2f);
            glBindTexture(GL_TEXTURE_2D, T->trunk_tex);
            draw_model(&T->trunk_model);
            glBindTexture(GL_TEXTURE_2D, T->leaves_tex);
            draw_model(&T->leaves_model);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    //fire
    render_explosion(&scene->explosion);

    //campfire
    glPushMatrix();
    glPushAttrib(
          GL_ENABLE_BIT
        | GL_CURRENT_BIT
        | GL_TEXTURE_BIT
        | GL_DEPTH_BUFFER_BIT
    );
        glTranslatef(fire_pos.x, fire_pos.y, fire_pos.z + 0.2f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.15f, 0.15f, 0.15f);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_COLOR_MATERIAL);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, scene->campfire_tex);

        glColor3f(brightness, brightness, brightness);
        draw_model(&scene->campfire_model);
    glPopAttrib();
    glPopMatrix();

    // rock
    glPushMatrix();

    vec3 fp = scene->explosion.position;
    glTranslatef(fp.x + 2.0f, fp.y, fp.z);
    glRotatef(90, 1,0,0);
    glScalef(0.005f, 0.005f, 0.005f);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glDisable(GL_TEXTURE_2D);
    
    GLfloat mat_diffuse [] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //?
    GLfloat mat_shininess[] = { 10.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glColor3f(brightness, brightness, brightness);
    glDisable(GL_COLOR_MATERIAL);
    draw_model(&scene->rock_model);
    glEnable(GL_COLOR_MATERIAL);

    glPopMatrix();
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void draw_ground()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grass_texture);
    //glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f( 0,  0); glVertex3f(-100, -100, 0);
        glTexCoord2f(50,  0); glVertex3f( 100, -100, 0);
        glTexCoord2f(50, 50); glVertex3f( 100,  100, 0);
        glTexCoord2f( 0, 50); glVertex3f(-100,  100, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void generate_trees(Scene* scene)
{
    scene->number_of_trees = 20;
    scene->trees = malloc(sizeof(Tree) * scene->number_of_trees);
    if (!scene->trees) {
        fprintf(stderr, "Invalid trees allocation\n");
        scene->number_of_trees = 0;
        return;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < scene->number_of_trees; ++i) {

        scene->trees[i] = scene->tree_prefab;

        float rx = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;
        float ry = (rand() / (float)RAND_MAX) * 50.0f - 25.0f;

        scene->trees[i].position.x = rx;
        scene->trees[i].position.y = ry;
        scene->trees[i].position.z = 0.0f;

        scene->trees[i].collected = false;
    }
}

