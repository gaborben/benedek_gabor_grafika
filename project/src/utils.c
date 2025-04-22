#include "utils.h"

#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <math.h>

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

void draw_text_2d(const char* text, float x, float y, float scale)
{
    // 1) Lekérjük az ablakméretet, hogy top‑based Y‑koordinátákat használjunk:
    int w, h;
    SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &w, &h);

    // 2) Előállítjuk a vertex‑buffer‑t
    char buffer[99999];
    int quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer));

    // 3) Átváltunk egy ortho projekcióra, Y‑t felül kezdjük:
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1, +1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();

            // 4) Elmozgatjuk az (x,y) pozícióba, és nagyítjuk
            glTranslatef(x, y, 0);
            glScalef(scale, scale, 1);

            // 5) Fehérre állítjuk, hogy biztosan ne zöld legyen
            glColor3f(1.0f, 1.0f, 1.0f);

            // 6) Kirajzoljuk a STB‑bufferből
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 16, buffer);
            glDrawArrays(GL_QUADS, 0, quads * 4);
            glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

