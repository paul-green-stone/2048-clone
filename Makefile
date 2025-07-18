a.out: main.c 2048.c
	gcc main.c 2048.c `pkg-config --libs --cflags sdl2 SDL2_image libconfig SDL2_ttf lstart`