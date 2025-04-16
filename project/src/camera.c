#include "camera.h"

#include <GL/gl.h>

#include <math.h>

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;

    camera->is_preview_visible = false;

    camera->run_multiplier = 1.0f;
    camera->is_crouching = false;
    camera->vertical_speed = 0.0f;


}

void update_camera(Camera* camera, double delta)
{
    double angle;
    double side_angle;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * camera->run_multiplier * delta;
    camera->position.y += sin(angle) * camera->speed.y * camera->run_multiplier * delta;
    camera->position.x += cos(side_angle) * camera->speed.x * camera->run_multiplier * delta;
    camera->position.y += sin(side_angle) * camera->speed.x * camera->run_multiplier * delta;

    camera->position.z += camera->vertical_speed * delta;

    camera->vertical_speed -= GRAVITY * delta;

    float ground = camera->is_crouching ? 0.5f : 1.0f;
    if (camera->position.z < ground) {
        camera->position.z = ground;
        camera->vertical_speed = 0.0f;
    }
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    float sensitivity = 0.2f;

    camera->rotation.z += horizontal * sensitivity;
    camera->rotation.x += vertical * sensitivity;

    if (camera->rotation.x > 89.0) {
        camera->rotation.x = 89.0;
    }
    if (camera->rotation.x < -89.0) {
        camera->rotation.x = -89.0;
    }

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }
}


void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}

void set_camera_run_speed(Camera* camera, bool is_running)
{
    camera->run_multiplier = is_running ? 4.0 : 1.0;
}

void set_camera_crouch(Camera* camera, bool is_crouching)
{
    camera->is_crouching = is_crouching;

    if (is_crouching) {
        camera->position.z = 0.5f;
    }
    else {
        camera->position.z = 1.0f;
    }
}

void jump_camera(Camera* camera)
{
    if (!camera->is_crouching && camera->position.z <= 1.0f + 1e-3f) {
        camera->vertical_speed = JUMP_IMPULSE;
    }
}



void show_texture_preview()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
