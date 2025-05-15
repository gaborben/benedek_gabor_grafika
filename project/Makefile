# Makefile

# Include- és warning-zászlók
CFLAGS = -Iinclude/ \
         -Wall -Wextra -Wpedantic \
         -Wno-unused-function \
         -Wno-unused-parameter \
         -Wno-missing-braces

# Linkeléshez
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lm

all:
	@cls
	gcc $(CFLAGS) \
	    src/app.c src/camera.c src/main.c src/scene.c src/explosion.c src/texture.c src/utils.c \
	    $(LDFLAGS) \
	    -o project.exe \
	&& project.exe
