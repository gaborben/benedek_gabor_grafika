#include <stdio.h>
#include <math.h>
#include "app.h"

#include <SDL2/SDL_image.h>

static void draw_guide(const App* app);

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Project!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    app->brightness = 1.0f;

    app->show_guide = false;
    app->guide_texture = load_texture("assets/textures/guide.png");
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 10
    );
}

void handle_app_events(App* app)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_F1:
                app->show_guide = !app->show_guide;
                break;
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_LSHIFT:
                set_camera_run_speed(&(app->camera), true);
                break;
            case SDL_SCANCODE_C:
                set_camera_crouch(&(app->camera), true);
                break;
            case SDL_SCANCODE_SPACE:
                jump_camera(&(app->camera));
                break;
            case SDL_SCANCODE_N:
                app->brightness = fmaxf(0.0f, app->brightness - 0.1f);
                break;
            case SDL_SCANCODE_M:
                app->brightness = fminf(2.0f, app->brightness + 0.1f);
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_LSHIFT:
                set_camera_run_speed(&(app->camera), false);
                break;
            case SDL_SCANCODE_C:
                set_camera_crouch(&(app->camera), false);
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEMOTION:
            rotate_camera(&(app->camera), -event.motion.xrel, -event.motion.yrel);
            break;
        
        

        case SDL_QUIT:
            app->is_running = false;
            break;

        default:
            break;
        }
    }
}


void update_app(App* app)
{
    double current_time = SDL_GetTicks() / 1000.0;
    double delta        = current_time - app->uptime;
    app->uptime         = current_time;

    update_camera(&app->camera, delta);
    update_scene(&app->scene,  delta);
}


void render_app(App* app)
{
    //glClearColor(app->brightness, app->brightness, app->brightness, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glColor3f(app->brightness, app->brightness, app->brightness);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    if (app->show_guide) {
        draw_guide(app);
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

static void draw_guide(const App* app)
{
    int w, h;
    SDL_GetWindowSize(app->window, &w, &h);

    const float origW = 1024.0f;
    const float origH = 1024.0f;

    const float maxW = w * 0.8f;
    const float maxH = h * 0.8f;

    float scale = 1.0f;
    if (origW > maxW || origH > maxH) {
        float sx = maxW / origW;
        float sy = maxH / origH;
        scale = (sx < sy) ? sx : sy;
    }

    float drawW = origW * scale;
    float drawH = origH * scale;

    float x = (w - drawW) * 0.5f;
    float y = (h - drawH) * 0.5f;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();

      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, app->guide_texture);
      glColor3f(1,1,1);

      glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(x      , y      );
        glTexCoord2f(1, 1); glVertex2f(x+drawW, y      );
        glTexCoord2f(1, 0); glVertex2f(x+drawW, y+drawH);
        glTexCoord2f(0, 0); glVertex2f(x      , y+drawH);
      glEnd();

      glDisable(GL_TEXTURE_2D);
      glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


