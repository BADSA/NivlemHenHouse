#include "../include/graphics.h"

int STATUS = 1;
bool done = false, render = false;
bool keyPressed = false;
bool keys[] = {false, false, false,false, false, false};
enum KEYS{LEFT, RIGHT, UP, DOWN, SPACE, ENTER};

pthread_mutex_t	mutex2 = PTHREAD_MUTEX_INITIALIZER;

int simulation_window(){
    ALLEGRO_DISPLAY *display, *display2, *display3 = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_TIMER *timer;
    al_init(); // I'm not checking the return value for simplicity.
    display = al_create_display(WIDTH, HEIGHT);
    //display2 = al_create_display(WIDTH, HEIGHT);
    //display3 = al_create_display(WIDTH, HEIGHT);

    //==============================================
    //ADDON INSTALL
    //==============================================
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    // FONT DEL PROGRAMA.
    ALLEGRO_FONT *font = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",30,0 );
    ALLEGRO_FONT *font_copy = al_load_ttf_font("resources/fonts/Sarpanch-SemiBold.ttf",20,0 );
    // 50 30 20 15 10
    ALLEGRO_FONT *font50 = al_load_ttf_font("resources/fonts/pirulen.ttf",50,0 );
    ALLEGRO_FONT *font30 = al_load_ttf_font("resources/fonts/pirulen.ttf",30,0 );
    ALLEGRO_FONT *font20 = al_load_ttf_font("resources/fonts/pirulen.ttf",20,0 );
    ALLEGRO_FONT *font15 = al_load_ttf_font("resources/fonts/pirulen.ttf",15,0 );
    ALLEGRO_FONT *font10 = al_load_ttf_font("resources/fonts/pirulen.ttf",10,0 );
    ALLEGRO_DISPLAY_MODE disp_data;

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
    sprite chk[3];
    sprite food[3];
    sprite water[3];
    int width_diff = 0;
    for (int i=0; i<3;i++){
        init_sprite(chk[i], width_diff, 200, "img/chicken2.png");
        init_sprite(food[i], width_diff, 350, "img/food.png");
        init_sprite(water[i], width_diff, 50, "img/water.png");
        width_diff += 70;
    }


    while(!done) {
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

                //al_draw_bitmap(bg, 0, 0, 0);
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
    return 0;
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

void init_sprite(sprite &spt, int diff, int y, char const *img_path){
    spt.x = 150 + diff;
    spt.y = y;
    spt.speed = 5;
    spt.image = al_load_bitmap(img_path);
    spt.w = al_get_bitmap_width(spt.image);
    spt.h = al_get_bitmap_height(spt.image);
}
