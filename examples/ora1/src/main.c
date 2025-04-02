#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} Point;

Point rotatePoint(Point p, double angle) {
    Point rotated;
    double radians = angle * M_PI / 180.0;
    printf("Radians: %f\n", radians);

    rotated.x = (int)round(p.x * cos(radians) - p.y * sin(radians));
    rotated.y = (int)round(p.x * sin(radians) + p.y * cos(radians));

    printf("Intermediate x: %f\n", p.x * cos(radians) - p.y * sin(radians));
    printf("Intermediate y: %f\n", p.x * sin(radians) + p.y * cos(radians));

    return rotated;
}

int SDL_main(int argc, char *argv[]) {
    Point p;
    double angle;

    printf("Enter the coordinates of the point (x y): ");
    scanf("%d %d", &p.x, &p.y);

    printf("Enter the angle of rotation (in degrees): ");
    scanf("%lf", &angle);

    Point rotated = rotatePoint(p, angle);

    printf("Original point: (%d, %d)\n", p.x, p.y);
    printf("Rotated point: (%d, %d)\n", rotated.x, rotated.y);

    return 0;
}