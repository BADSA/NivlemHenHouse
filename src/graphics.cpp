#include "../include/main.h"
#include "../include/graphics.h"
#include "../include/externs.h"

int STATUS = MENU;
bool done = false, render = false, keyPressed = false;
bool keys[] = {false, false, false,false, false, false};
enum KEYS {LEFT, RIGHT, UP, DOWN, SPACE, ENTER};
sprite chk[3];

pthread_mutex_t	mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	mutex4 = PTHREAD_MUTEX_INITIALIZER;

bool show_egg = false;

int simulation_window(){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    al_init(); // Initialize allegro.
    display = al_create_display(WIDTH, HEIGHT);
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

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;
    int menu_selected = M_START;
    al_start_timer(timer);

    /*
    ==================================
        Sprites for the Hen House
    ==================================
    */
    sprite food[3];
    sprite water[3];
    sprite egg;
    int width_diff = 0;
    for (int i=0; i<3;i++){
        init_sprite(chk[i], width_diff, 200, "img/chicken2.png");
        init_sprite(food[i], width_diff, 350, "img/food.png");
        init_sprite(water[i], width_diff, 50, "img/water.png");
        width_diff += 70;
    }
    init_sprite(egg, 155, 165,"img/egg.png");


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
                render = true;
                if (keyPressed) {
                    if (keys[ENTER]) {
                        if (menu_selected == M_START) {
                            STATUS = SIMULATION;
                            printf("Inicio de la simulacion\n");
                            pthread_t sim, stats;
                            pthread_create(&sim, NULL, start_simulation, NULL);
                            pthread_create(&stats, NULL, stats_window, NULL);

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
                        break;
                }
                keys[ENTER] = false;
            }

            render = true;

            //==============================================
            //                  RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue)) {
                render = false;

                al_draw_text(font20, GREEN, WIDTH/2 - 110, ALLEGRO_ALIGN_CENTER, 0, "Iniciando: ");

                if (show_egg)
                    al_draw_bitmap(egg.image, egg.x, egg.y, 0);

                for (int i = 0; i<3; i++){
                    al_draw_bitmap(chk[i].image, chk[i].x ,chk[i].y, 0);
                    al_draw_bitmap(food[i].image, food[i].x ,food[i].y, 0);
                    al_draw_bitmap(water[i].image, water[i].x ,water[i].y, 0);
                }


                al_draw_text(font_copy, GRAY, 30,450, 0, "BADSA Corp 2015");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

        }

    }
    return NULL;
}



void *stats_window(void *args){
    bool render = false;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    al_init(); // Initialize allegro.
    //al_set_new_window_position(50,50);
    display = al_create_display(400, 500);
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

    while( !done ) {
        al_wait_for_event(event_queue, &ev);
        render = true;
        if (render && al_is_event_queue_empty(event_queue)) {
            render = false;
            al_draw_text(font, GREEN, ALLEGRO_ALIGN_CENTER, 10, 0, "Nivlem Hen House STATS");
            al_draw_text(font20, GREEN, 20, 50, 0, "Cantidad de huevos");
            char vartext[10];
            sprintf(vartext,"%d",total_eggs+eggs_amount);
            al_draw_text(font20, GREEN, 20, 70, 0, vartext);

            al_draw_text(font20, GREEN, 20, 100, 0, "Costo total");
            sprintf(vartext,"%d",cost);
            al_draw_text(font20, GREEN, 20, 120, 0, vartext);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    return NULL;
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
    pthread_mutex_unlock(&mutex3);
}

void *chick_swot(void *args){
    pthread_mutex_lock(&mutex4);
    show_egg = true;
    usleep(30000);
    show_egg = false;
    pthread_mutex_unlock(&mutex4);
}

void init_sprite(sprite &spt, int diff, int y, char const *img_path){
    spt.x = 150 + diff;
    spt.y = y;
    spt.speed = 10;
    spt.image = al_load_bitmap(img_path);
    spt.w = al_get_bitmap_width(spt.image);
    spt.h = al_get_bitmap_height(spt.image);
}
