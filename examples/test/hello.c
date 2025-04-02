#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>

FILE *file = fopen("model.obj", "r");

// Kamera pozíciója
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;
float lightIntensity = 1.0f;
int showHelp = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
    
    GLfloat light_ambient[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    // Egyszerű kocka model
    glutSolidTeapot(1.0);
    
    // Ha az F1 lenyomva, írja ki a használati utasítást
    if (showHelp) {
        printf("Használati utasítás:\n");
        printf("W/S/A/D: Kamera mozgatása\n");
        printf("+/-: Fényerő állítása\n");
        printf("F1: Használati utasítás megjelenítése/elrejtése\n");
        printf("ESC: Kilépés\n");
    }
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': cameraZ -= 0.1f; break;
        case 's': cameraZ += 0.1f; break;
        case 'a': cameraX -= 0.1f; break;
        case 'd': cameraX += 0.1f; break;
        case '+': lightIntensity += 0.1f; if (lightIntensity > 1.0f) lightIntensity = 1.0f; break;
        case '-': lightIntensity -= 0.1f; if (lightIntensity < 0.0f) lightIntensity = 0.0f; break;
        case 27: exit(0); // Escape kilépéshez
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        showHelp = !showHelp;
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Semester Project");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}
