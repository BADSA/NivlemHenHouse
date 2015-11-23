#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <pthread.h>

#define MENU 1
#define SIMULATION 2

#define GREEN al_map_rgb(10,200,10)
#define GRAY_SELECTED al_map_rgb(200,200,200)
#define GRAY al_map_rgb(70,70,70)

#define WIDTH 500//350
#define HEIGHT 500//300
#define M_START 1
#define M_EXIT 2

#ifndef NIVLEMHENHOUSE_GRAPHICS_H
#define NIVLEMHENHOUSE_GRAPHICS_H

struct sprite {
    float x;
    float y;
    int w;
    int h;
    int speed;
    bool busy;
    ALLEGRO_BITMAP *image;
};

int simulation_window();
void *stats_window(void *args);
void *nivlem_window(void *args);
void init_sprite(sprite &spt, int diff, int y, char const *img_path);
void *move_chick_eat(void *args);
void *move_chick_drink(void *args);
void *move_nivlem(void *args);
void *chick_swot(void *args);
int get_available_chick();

#endif //NIVLEMHENHOUSE_GRAPHICS_H
