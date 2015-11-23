//
// Created by melalonso on 11/10/15.
//
#include <iostream>
#include "mathfunc.h"
#include "graphics.h"

#ifndef NIVLEMHENHOUSE_CHICKEN_H
#define NIVLEMHENHOUSE_CHICKEN_H

extern sprite chk[3];
extern bool show_egg;

void *eat(void *args);
void *drink(void *args);
void *swot(void *args);
void *chicken_process(void * args);
void update_chicken_resources(char *data);
#endif //NIVLEMHENHOUSE_CHICKEN_H
