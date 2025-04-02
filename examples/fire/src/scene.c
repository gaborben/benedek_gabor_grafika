#include "scene.h"

#include "explosion.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    // load_model(&(scene->cube), "assets/models/cube.obj");
    // scene->texture_id = load_texture("assets/textures/cube.png");
    load_model(&(scene->cube), "assets/models/cube.obj");

    //glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    init_explosion(&(scene->explosion));

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.5;

    scene->material.diffuse.red = 0.0;
    scene->material.diffuse.green = 0.3;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.2;
    scene->material.specular.green = 0.2;
    scene->material.specular.blue = 1.0;
    scene->material.shininess = 80.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.3f, 1.0f };
    float diffuse_light[] = { 0.0f, 0.3f, 1.0f, 1.0f };
    float specular_light[] = { 0.3f, 0.3f, 1.0f, 1.0f };
    float position[] = { 0.0f, -2.0f, 3.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    // Enable color material mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Debugging output
    printf("Material Colors:\n");
    printf("Ambient: R=%.2f, G=%.2f, B=%.2f\n", 
           material->ambient.red, material->ambient.green, material->ambient.blue);
    printf("Diffuse: R=%.2f, G=%.2f, B=%.2f\n", 
           material->diffuse.red, material->diffuse.green, material->diffuse.blue);
    printf("Specular: R=%.2f, G=%.2f, B=%.2f\n", 
           material->specular.red, material->specular.green, material->specular.blue);
    printf("Shininess: %.2f\n", material->shininess);

    // Convert struct values to OpenGL format
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue,
        1.0f // Alpha value
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue,
        1.0f // Alpha value
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue,
        1.0f // Alpha value
    };

    // Apply material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
}


void update_scene(Scene* scene, double elapsed_time)
{
    update_explosion(&(scene->explosion), elapsed_time);
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
    render_explosion(&(scene->explosion));
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
