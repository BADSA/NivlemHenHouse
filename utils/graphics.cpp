#include "graphics.h"

int STATUS = 1;
bool done = false, render = false;
bool keyPressed = false;
bool keys[] = {false, false, false,false, false, false};
enum KEYS{LEFT, RIGHT, UP, DOWN, SPACE, ENTER};

int simulation_window(){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_TIMER *timer;
    al_init(); // I'm not checking the return value for simplicity.
    display = al_create_display(WIDTH, HEIGHT);

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
    ALLEGRO_DISPLAY_MODE   disp_data;

    ALLEGRO_COLOR color_start_s = GRAY_SELECTED;
    ALLEGRO_COLOR color_exit_s = GRAY;

    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT ev;
    int menu_selected = M_START;
    al_start_timer(timer);

    struct Chicken chk;
    init_chicken(chk);


    while(!done) {
        al_wait_for_event(event_queue, &ev);

        if (STATUS == 1) {

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
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
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

                            STATUS = 2;
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

        }else if (STATUS==2){

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        STATUS = 1;
                        break;
                    case ALLEGRO_KEY_BACKSPACE:
                        pthread_t t;
                        pthread_create (&t, NULL, &move_chick_eat, (void *)&chk );

                        break;
                }
                keys[ENTER] = false;
            }

            render = true;

            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;

                al_draw_text(font(25), GREEN, WIDTH/2 - 110, ALLEGRO_ALIGN_CENTER, 0, "Iniciando: ");

                //al_draw_bitmap(bg, 0, 0, 0);
                al_draw_bitmap(al_load_bitmap("img/c0dxhIv.png"), 200, 50, 0);
                al_draw_bitmap(al_load_bitmap("img/food.png"), 200,350, 0);
                al_draw_bitmap(chk.image, 210,chk.y, 0);


                al_draw_text(font_copy, GRAY, 30,450, 0, "BADSA Corp 2015");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

        }

    }
    return 0;
}


void *move_chick_eat(void *args){
    struct Chicken *chick = (Chicken *) args;

    printf("Moviendo gallina\n");
    double curr_pos = chick->y;
    while(chick->y > 50)
        chick->y -= chick->speed;
    while(chick->y < curr_pos)
        chick->y += chick->speed;
}

void move_chick_drink(Chicken &chick) {
    double curr_pos = chick.y;
    while(chick.y < 350)
        chick.y += chick.speed;
    while(chick.y > curr_pos)
        chick.y -= chick.speed;
}

void init_chicken(Chicken &chick){
    chick.x = WIDTH / 2;
    chick.y = 200;
    chick.speed = 5;
    chick.image = al_load_bitmap("img/chicken2.png");
    chick.w = al_get_bitmap_width(chick.image);
    chick.h = al_get_bitmap_height(chick.image);
}
