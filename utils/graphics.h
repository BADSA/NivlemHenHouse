#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <pthread.h>

#define GREEN al_map_rgb(10,200,10)
#define GRAY_SELECTED al_map_rgb(200,200,200)
#define GRAY al_map_rgb(70,70,70)
#define GRAY2 al_map_rgb(150,150,150)

// Just to test
#define font(tam) al_load_ttf_font("resources/fonts/pirulen.ttf",tam,0 )
#define WIDTH 500//350
#define HEIGHT 500//300
#define M_START 1
#define M_EXIT 2

struct Chicken {
    float x;
    float y;
    int w;
    int h;
    int speed;
    ALLEGRO_BITMAP *image;
};

#ifndef NIVLEMHENHOUSE_GRAPHICS_H
#define NIVLEMHENHOUSE_GRAPHICS_H


int simulation_window(void);
void init_chicken(Chicken &chick);
void move_chick_eat(Chicken &chick);
void move_chick_drink(Chicken &chick);

#endif //NIVLEMHENHOUSE_GRAPHICS_H
