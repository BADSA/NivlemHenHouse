//
// Created by melalonso on 11/11/15.
//

#include "mathfunc.h"

#ifndef NIVLEMHENHOUSE_UTILS_H
#define NIVLEMHENHOUSE_UTILS_H

void read_file();
void reset_values();
void error(const char *msg);
void *parse_bootsinfo_msg(void *arg);
void *parse_henhouse_msg(void *arg);
void *parse_nivlem_msg(void *arg);
void simulation_status(char *data);
#endif //NIVLEMHENHOUSE_UTILS_H
