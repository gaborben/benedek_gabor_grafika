#include "utils.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

static float   hs_value;
static Uint32  hs_start;

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

void draw_text_2d(const char* text, float x, float y, float scale)
{
    int w, h;
    SDL_Window* win = SDL_GL_GetCurrentWindow();
    SDL_GetWindowSize(win, &w, &h);

    char buffer[99999];
    int quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer));

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      glOrtho(0, w, h, 0, -1, 1);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
        glLoadIdentity();

        glPushAttrib(
              GL_ENABLE_BIT
            | GL_CURRENT_BIT
            | GL_DEPTH_BUFFER_BIT
            | GL_LIGHTING_BIT
            | GL_TEXTURE_BIT
        );

          glDisable(GL_DEPTH_TEST);
          glDisable(GL_LIGHTING);
          glDisable(GL_TEXTURE_2D);

          glColor3f(1.0f, 1.0f, 1.0f);

          glTranslatef(x, y, 0);
          glScalef(scale, scale, 1);

          glEnableClientState(GL_VERTEX_ARRAY);
          glVertexPointer(2, GL_FLOAT, 16, buffer);
          glDrawArrays(GL_QUADS, 0, quads * 4);
          glDisableClientState(GL_VERTEX_ARRAY);

        glPopAttrib();

      glPopMatrix();

      glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void highscore_init(void) {
  hs_start = SDL_GetTicks();
  FILE* f = fopen("highscore.txt","r");
  if(f){ fscanf(f,"%f",&hs_value); fclose(f);}
  else  hs_value = 0;
}

void highscore_update_and_draw(void) {
    Uint32 now = SDL_GetTicks();
    unsigned int elapsed_sec = (now - hs_start) / 1000;
    unsigned int min = elapsed_sec / 60;
    unsigned int sec = elapsed_sec % 60;

    if (elapsed_sec > (unsigned int)hs_value) {
        hs_value = elapsed_sec;
        FILE* f = fopen("highscore.txt", "w");
        if (f) {
            fprintf(f, "%u\n", elapsed_sec);
            fclose(f);
        }
    }
    unsigned int hs_min = ((unsigned int)hs_value) / 60;
    unsigned int hs_sec = ((unsigned int)hs_value) % 60;

    char buf1[32], buf2[32];
    snprintf(buf1, sizeof(buf1), "Your score: %02u:%02u", min, sec);
    snprintf(buf2, sizeof(buf2), "Highscore: %02u:%02u", hs_min, hs_sec);

    glColor3f(1.0f, 1.0f, 0.0f);

    const float scale = 2.0f;

    draw_text_2d(buf1, 10.0f, 10.0f, scale);
    draw_text_2d(buf2, 10.0f, 30.0f, scale);
}


