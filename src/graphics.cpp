#include <iostream>
#include "../include/main.h"
#include "../include/externs.h"
#include "../include/graphics.h"
#include <ctime>


int STATUS = MENU;
bool done = false, render = false, keyPressed = false;
bool keys[] = {false, false, false,false, false, false};
enum KEYS {LEFT, RIGHT, UP, DOWN, SPACE, ENTER};
sprite chk[3], nivlem, egg;
bool auxiliar_display = false;

pthread_mutex_t	mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex5 = PTHREAD_MUTEX_INITIALIZER;

bool show_egg = false;

int simulation_window(){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *bg = NULL;
    al_init(); // Initialize allegro.

    display = al_create_display(WIDTH, HEIGHT);
    al_set_window_position(display, 440, 120);
    al_set_window_title(display, "Nivlem Hen House");

    /* ADDON INSTALL */
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    // FONT DEL PROGRAMA.
    ALLEGRO_FONT *font = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",30,0 );
    ALLEGRO_FONT *font_copy = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",20,0 );

    ALLEGRO_FONT *font30 = al_load_ttf_font("resources/fonts/pirulen.ttf",30,0 );
    ALLEGRO_FONT *font20 = al_load_ttf_font("resources/fonts/pirulen.ttf",20,0 );

    ALLEGRO_COLOR color_start_s = GRAY_SELECTED;
    ALLEGRO_COLOR color_exit_s = GRAY;

    bg = al_load_bitmap("img/bg.jpg");

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;
    int menu_selected = M_START;
    al_start_timer(timer);


    sprite cesta, dollar_sign, mini_food, mini_water_bucket, egg_only;

    init_sprite(dollar_sign,-130,75,"img/dollarsign.png");
    init_sprite(mini_food,0,70,"img/minifood.png");
    init_sprite(mini_water_bucket,130,70,"img/miniwater.png");
    init_sprite(egg_only, 260, 70,"img/eggonly.png");
    init_sprite(nivlem, 200, 165,"img/man.png");
    init_sprite(cesta, -130, 175,"img/cesta.png");


    while( !done ) {
        al_wait_for_event(event_queue, &ev);

        if (STATUS == MENU) {

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        done = true;
                        break;
                    case ALLEGRO_KEY_UP:
                        keys[UP] = true;
                        keys[DOWN] = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        keys[DOWN] = true;
                        keys[UP] = false;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        keys[ENTER] = true;
                        break;
                }
                keyPressed = true;


            } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch (ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ENTER:
                        keys[ENTER] = false;
                        break;
                }

            } else if (ev.type == ALLEGRO_EVENT_TIMER) {
                if (keyPressed) {
                    if (keys[ENTER]) {
                        if (menu_selected == M_START) {
                            STATUS = SIMULATION;
                            printf("Inicio de la simulacion\n");
                            pthread_t sim, stats, nivlem;
                            auxiliar_display = true;
                            pthread_create(&sim, NULL, start_simulation, NULL);
                            //pthread_create(&stats, NULL, stats_window, NULL);
                            //pthread_create(&nivlem, NULL, nivlem_window,(void*) NULL);

                        } else if (menu_selected == M_EXIT) {
                            done = true;
                        }
                        keyPressed = false;
                    } else if (keys[UP]) {
                        if (menu_selected == M_EXIT) {
                            color_exit_s = GRAY;
                            color_start_s = GRAY_SELECTED;
                            menu_selected = M_START;
                        }
                        keyPressed = false;
                    } else if (keys[DOWN]) {
                        if (menu_selected == M_START) {
                            color_exit_s = GRAY_SELECTED;
                            color_start_s = GRAY;
                            menu_selected = M_EXIT;
                        }
                        keyPressed = false;
                    }
                }
            }
            render = true;
            if (render && al_is_event_queue_empty(event_queue)) {
                render = false;
                al_draw_text(font, GREEN, WIDTH / 2 - 140, HEIGHT / 2 - 100, 0, "Nivlem Hen House");
                al_draw_text(font30, GREEN, WIDTH / 2 - 120, HEIGHT / 2 - 70, 0, "simulator");
                al_draw_text(font30, color_start_s, WIDTH / 2 - 75, HEIGHT / 2 - 20, 0, "Start");
                al_draw_text(font30, color_exit_s, WIDTH / 2 - 55, HEIGHT / 2 + 10, 0, "Exit");
                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));

            }

        }else if (STATUS == SIMULATION){

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        STATUS = MENU;
                        auxiliar_display = false;
                        break;
                }
                keys[ENTER] = false;
            }


            al_draw_text(font, GREEN, ALLEGRO_ALIGN_CENTER, 20, 0, " Nivlem ");

            char vartext[15];
            char vartext1[15];
            sprintf(vartext,"%d",cost);
            al_draw_text(font20, GREEN,45,80, 0, vartext);

            if(!food_amount)
                strcpy(vartext1,"0");
            sprintf(vartext1,"%d",food_amount);
            al_draw_text(font20, GREEN,200,80, 0, vartext1);

            if(!water_amount)
                strcpy(vartext,"0");
            sprintf(vartext,"%d",water_amount);
            al_draw_text(font20, GREEN,330,80, 0, vartext);

            sprintf(vartext1,"%d",eggs_amount);
            al_draw_text(font20, GREEN,450,80, 0, vartext1);



            al_draw_bitmap(cesta.image, cesta.x, cesta.y, 0);
            al_draw_bitmap(nivlem.image, nivlem.x, nivlem.y, 0);
            al_draw_bitmap(mini_food.image, mini_food.x, mini_food.y, 0);
            al_draw_bitmap(mini_water_bucket.image, mini_water_bucket.x, mini_water_bucket.y, 0);
            al_draw_bitmap(dollar_sign.image, dollar_sign.x, dollar_sign.y, 0);
            al_draw_bitmap(egg_only.image, egg_only.x, egg_only.y, 0);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

        }

    }
    return 0;
}

int stats_window(){
    bool render = false;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    al_init(); // Initialize allegro.
    //al_set_new_window_position(50,50);
    display = al_create_display(400, 400);
    al_set_window_position(display, 20, 120);
    al_set_window_title(display, "Statistics");

    /* ADDON INSTALL */
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    // FONT DEL PROGRAMA.
    ALLEGRO_FONT *font = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",30,0 );
    ALLEGRO_FONT *font_copy = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",20,0 );

    ALLEGRO_FONT *font30 = al_load_ttf_font("resources/fonts/pirulen.ttf",30,0 );
    ALLEGRO_FONT *font20 = al_load_ttf_font("resources/fonts/pirulen.ttf",20,0 );

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;
    al_start_timer(timer);

    float chronometer = (START_TIME + SIMULATION_TIME*CLOCKS_PER_SEC);

    time_t current;
    while(simulation_active) {

        std::time_t current = std::time(nullptr);

        double total_seconds = difftime(current,START_TIME);
        double r_seconds = SIMULATION_TIME - total_seconds;
        al_wait_for_event(event_queue, &ev);

        if (al_is_event_queue_empty(event_queue)) {

            //float total_seconds = (chronometer - current)/CLOCKS_PER_SEC;
            //printf("%f\n",total_seconds);
            int hours = r_seconds / 3600;
            int minutes = r_seconds / 60;
            int seconds = (int)r_seconds % 60;

            if(r_seconds>=0) {
                char time[10];
                sprintf(time, "%d:%d:%d", hours, minutes, seconds);

                al_draw_text(font30, GREEN, 5, 260, 0, "Tiempo restante:");
                al_draw_text(font30, GREEN, 120, 300, 0, time);
            }else{

                al_draw_text(font30, GREEN, 20, 300, 0, "Simulacion" );
                al_draw_text(font30, GREEN, 20, 330, 0, "finalizada" );
            }


            al_draw_text(font, GREEN, ALLEGRO_ALIGN_CENTER, 10, 0, "Nivlem Hen House STATS");
            al_draw_text(font20, GREEN, 20, 50, 0, "Cantidad de huevos");

            char vartext[15];
            sprintf(vartext,"%d",total_eggs+eggs_amount);
            al_draw_text(font20, GREEN, 20, 70, 0, vartext);

            al_draw_text(font20, GREEN, 20, 100, 0, "Total de agua");
            sprintf(vartext,"%d",total_water);
            al_draw_text(font20, GREEN, 20, 120, 0, vartext);

            al_draw_text(font20, GREEN, 20, 150, 0, "Total de alimento");
            sprintf(vartext,"%d",total_food);
            al_draw_text(font20, GREEN, 20, 170, 0, vartext);

            al_draw_text(font20, GREEN, 20, 200, 0, "Costo total");
            sprintf(vartext,"%d",cost);
            al_draw_text(font20, GREEN, 20, 220, 0, vartext);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            //al_rest(0.9);
        }
    }
    return 0;
}


int henhouse_window(){
    bool render = false;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *bg = NULL;
    al_init(); // Initialize allegro.
    display = al_create_display(WIDTH, HEIGHT);
    al_set_window_position(display, 950, 120);
    al_set_window_title(display, "Nivlem");

    /* ADDON INSTALL */
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    // FONT DEL PROGRAMA.
    ALLEGRO_FONT *font = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",30,0 );
    ALLEGRO_FONT *font_copy = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",20,0 );

    ALLEGRO_FONT *font30 = al_load_ttf_font("resources/fonts/pirulen.ttf",30,0 );
    ALLEGRO_FONT *font20 = al_load_ttf_font("resources/fonts/pirulen.ttf",20,0 );

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;
    al_start_timer(timer);

    sprite cesta, dollar_sign, mini_food, mini_water_bucket, egg_only;

    init_sprite(dollar_sign,-130,75,"img/dollarsign.png");
    init_sprite(mini_food,0,70,"img/minifood.png");
    init_sprite(mini_water_bucket,130,70,"img/miniwater.png");
    init_sprite(egg_only, 260, 70,"img/eggonly.png");
    init_sprite(nivlem, 200, 165,"img/man.png");
    init_sprite(cesta, -130, 175,"img/cesta.png");

    /*
==================================
    Sprites for the Hen House
==================================
*/
    sprite food[3];
    sprite water[3];
    int width_diff = 0;
    for (int i=0; i<3;i++){
        init_sprite(chk[i], width_diff, 200, "img/chicken2.png");
        init_sprite(food[i], width_diff, 350, "img/food.png");
        init_sprite(water[i], width_diff, 50, "img/water.png");
        width_diff += 70;
    }
    init_sprite(egg, 155, 165,"img/egg.png");
    bg = al_load_bitmap("img/bg.jpg");

    while(true) {
        al_wait_for_event(event_queue, &ev);
        if (al_is_event_queue_empty(event_queue)) {
            al_draw_bitmap(bg, 0, 0, 0);

            al_draw_text(font20, GREEN, WIDTH/2 - 110, ALLEGRO_ALIGN_CENTER, 0, "Iniciando: ");

            if (show_egg)
                al_draw_bitmap(egg.image, egg.x, egg.y, 0);

            for (int i = 0; i<3; i++){
                al_draw_bitmap(food[i].image, food[i].x ,food[i].y, 0);
                al_draw_bitmap(water[i].image, water[i].x ,water[i].y, 0);
                al_draw_bitmap(chk[i].image, chk[i].x ,chk[i].y, 0);
            }


            al_draw_text(font_copy, GRAY, 30,450, 0, "BADSA Corp 2015");

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    return 0;
}


void *move_nivlem(void *args){
    struct sprite *nivlem = (sprite *) args;

    pthread_mutex_lock(&mutex2);
    double curr_pos = nivlem->x;
    while(nivlem->x > 25){
        int ran = rand();
        usleep(5000+ran%10000);
        nivlem->x -= nivlem->speed;
    }
    usleep(300000);
    while(nivlem->x < curr_pos){
        int ran = rand();
        usleep(5000+ran%10000);
        nivlem->x += nivlem->speed;
    }
    pthread_mutex_unlock(&mutex2);
}



void *move_chick_eat(void *args){
    struct sprite *chick = (sprite *) args;

    pthread_mutex_lock(&mutex2);
    double curr_pos = chick->y;
    while(chick->y > 50){
        int ran = rand();
        usleep(5000+ran%10000);
        chick->y -= chick->speed;
    }
    usleep(300000);
    while(chick->y < curr_pos){
        int ran = rand();
        usleep(5000+ran%10000);
        chick->y += chick->speed;
    }
    chick->busy = false;
    pthread_mutex_unlock(&mutex2);
}


void *move_chick_drink(void *args){
    struct sprite *chick = (sprite *) args;

    pthread_mutex_lock(&mutex3);
    double curr_pos = chick->y;
    while(chick->y < 350){
        int ran = rand();
        usleep(5000+ran%10000);
        chick->y += chick->speed;
    }
    usleep(300000);
    while(chick->y > curr_pos){
        int ran = rand();
        usleep(5000+ran%10000);
        chick->y -= chick->speed;
    }
    chick->busy = false;
    pthread_mutex_unlock(&mutex3);
}

void *chick_swot(void *args){
    struct sprite *chick = (sprite *) args;
    pthread_mutex_lock(&mutex4);
    egg.x = chick->x;
    show_egg = true;
    usleep(30000);
    show_egg = false;
    chick->busy = false;
    pthread_mutex_unlock(&mutex4);
}

void init_sprite(sprite &spt, int diff, int y, char const *img_path){
    spt.x = 150 + diff;
    spt.y = y;
    spt.speed = 10;
    spt.busy = false;
    spt.image = al_load_bitmap(img_path);
    spt.w = al_get_bitmap_width(spt.image);
    spt.h = al_get_bitmap_height(spt.image);
}


int get_available_chick( ){
    pthread_mutex_lock(&mutex5);
    int i=0;
    while(1) {
        if (!chk[i].busy) {
            chk[i].busy = true;
            pthread_mutex_unlock(&mutex5);
            return i;
        }
        i++;
        if (i % 3 == 0)
            i = 0;
    }
}