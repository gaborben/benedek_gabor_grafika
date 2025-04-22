all:
	@cls
	gcc -Iinclude/ \
	    src/app.c src/camera.c src/main.c src/scene.c src/explosion.c src/texture.c src/utils.c \
	    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lm \
	    -Wall -Wextra -Wpedantic \
	    -o project.exe && project.exe
