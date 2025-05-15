CFLAGS = -Iinclude/ \
         -Wall -Wextra -Wpedantic \
         -Wno-unused-function \
         -Wno-unused-parameter \
         -Wno-missing-braces

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lm

all:
	@cls
	@if not exist build mkdir build
	gcc $(CFLAGS) \
	    src/app.c src/camera.c src/main.c src/scene.c src/explosion.c src/texture.c src/utils.c \
	    $(LDFLAGS) \
	    -o build/project.exe
	build\project.exe
