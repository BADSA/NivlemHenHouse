#include "../include/chicken.h"
#include "../include/client.h"

/*
    Process of each chicken.
    Every chicken has a thread that runs this function.
*/
void *chicken_process(void * args){
    int chick_num = *((int*) args);
    printf("Creando gallina #%d\n",chick_num);
    pthread_t eat_thread, drink_thread, swot_thread;

    pthread_create(&eat_thread, NULL, &eat, args);
    pthread_create(&drink_thread, NULL, &drink, args);
    pthread_create(&swot_thread, NULL, &swot, args);

    return NULL;
}


/*
    Eat function of the chickens.
    Randomly eats an amount beetween
    FOOD_INTAKE[0] (min) and
    FOOD_INTAKE[1] (max).
*/
void *eat(void *args){
    int chick_num = *((int*) args);
    int time, amount, chk_index;
    while(simulation_active){
        time = get_wait_time(food_dist);
        sleep(time);
        if(!simulation_active)
            break;
        amount = ( rand() % (FOOD_INTAKE[1] - FOOD_INTAKE[0]))+ FOOD_INTAKE[0]; // Random amount of food.
        pthread_mutex_lock(&mutex);
        total_food += amount;
        food_amount -= amount;
        char message[256];
        sprintf(message,"c-1-%d",amount);
        send_message(nivlen_csock,message);
        printf("Pegado en el condicional.................................. eat\n");
        if(food_amount <= FOOD_MIN){
            send_message(nivlen_csock, "c-4");
            food_fill = true;
            while(food_fill)pthread_cond_wait(&food_cond, &mutex);
            //pthread_cond_signal(&water_cond); // For the bot to refill it
        }

        printf("La gallina %d comió %d de alimento, la cantidad total de comida es: %d \n", chick_num, amount, food_amount);
        pthread_mutex_unlock(&mutex);

        chk_index = get_available_chick();

        pthread_t chicken;
        pthread_create(&chicken, NULL, move_chick_eat, (void*) &chk[chk_index]);
    }
    return NULL;
}


/*
    Drink function of the chickens.
    Randomly drinks an amount beetween
    WATER_INTAKE[0] (min) and
    WATER_INTAKE[1] (max).
*/
void *drink(void *args){
    int chick_num = *((int*) args);
    int time, amount, chk_index;
    while(simulation_active){
        time = get_wait_time(water_dist);
        sleep(time);
        if(!simulation_active)
            break;
        amount = ( rand() % (WATER_INTAKE[1] - WATER_INTAKE[0]))+ WATER_INTAKE[0]; // Random amount of food.
        pthread_mutex_lock(&mutex);
        total_water+= amount;
        water_amount -= amount;
        char message[256];
        sprintf(message,"c-2-%d",amount);
        send_message(nivlen_csock,message);
        printf("Pegado en el condicional.................................. drink\n");
        if(water_amount <= WATER_MIN){
            send_message(nivlen_csock, "c-5");
            water_fill = true;
            while(water_fill)pthread_cond_wait(&water_cond, &mutex);
            //pthread_cond_signal(&water_cond); // For the bot to refill it
        }

        printf("La gallina %d tomó %d mililitros agua, la cantidad de agua total es: %d \n", chick_num, amount, water_amount);
        pthread_mutex_unlock(&mutex);

        chk_index = get_available_chick();

        pthread_t chicken;
        pthread_create(&chicken, NULL, move_chick_drink, (void*) &chk[chk_index]);

    }
    return NULL;
}


/*
    Swot function of the chickens.
    Randomly puts an egg.
*/
void *swot(void *args){
    int chick_num = *((int*) args);
    int time, chk_index;
    while(simulation_active){
        time = get_wait_time(egg_dist);
        sleep(time);
        if(!simulation_active)
            break;
        pthread_mutex_lock(&mutex);
        eggs_amount++;
        printf("La gallina %d puso un huevo, cantidad de huevos: %d\n",chick_num,eggs_amount);

        send_message(nivlen_csock,"c-3");

        if(eggs_amount >= EGGS_MAX){
            printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            printf("+ La canasta se ha llenado, Nivlem recogerá los huevos +\n");
            printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            send_message(nivlen_csock, "c-6");
            pick_eggs = true;
            while(pick_eggs)pthread_cond_wait(&eggs_cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        chk_index = get_available_chick();

        pthread_t chicken;
        pthread_create(&chicken, NULL, chick_swot, &chk[chk_index]);
    }
    return NULL;
}

void update_chicken_resources(char *data){
    pthread_mutex_lock(&mutex);
    int action = data[2]-48;
    int value = atoi(data+4);
    printf("UPDATING RESOURCES\n");
    switch (action){
        case 1:
            food_amount = value;
            food_fill = false;
            pthread_cond_signal(&food_cond);
            pthread_mutex_unlock(&mutex);
            break;
        case 2:
            water_amount = value;
            water_fill = false;
            pthread_cond_signal(&water_cond);
            pthread_mutex_unlock(&mutex);
            break;
        case 3:
            eggs_amount=0;
            pick_eggs = false;
            pthread_cond_signal(&eggs_cond);
            pthread_mutex_unlock(&mutex);
            break;
        default:
            pthread_mutex_unlock(&mutex);
            break;
    }
}
