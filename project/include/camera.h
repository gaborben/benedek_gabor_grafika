#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include <stdbool.h>

#define JUMP_IMPULSE    3.0f   //unit/sec
#define GRAVITY         9.81f

/**
 * Camera, as a moving point with direction
 */
typedef struct Camera
{
    vec3 position;
    vec3 rotation;
    vec3 speed;
    bool is_preview_visible;

    float run_multiplier;
    bool is_crouching;
    float vertical_speed;

} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera* camera);

/**
 * Update the position of the camera.
 */
void update_camera(Camera* camera, double delta);

/**
 * Apply the camera settings to the view transformation.
 */
void set_view(const Camera* camera);

/**
 * Set the horizontal and vertical rotation of the view angle.
 */
void rotate_camera(Camera* camera, double horizontal, double vertical);

/**
 * Set the speed of forward and backward motion.
 */
void set_camera_speed(Camera* camera, double speed);

/**
 * Set the speed of left and right side steps.
 */
void set_camera_side_speed(Camera* camera, double speed);

void set_camera_run_speed(Camera* camera, bool is_running);

void set_camera_crouch(Camera* camera, bool is_crouching);

void jump_camera(Camera* camera);

/**
 * Draw a fullscreen quad with the current texture (preview/debug).
 */
void show_texture_preview(void);

#endif /* CAMERA_H */
