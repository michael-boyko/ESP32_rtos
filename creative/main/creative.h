#ifndef CREATIVE_H
#define CREATIVE_H

#include "../components/uart_console/readuart.h"

void digital_clock();
void task_get_data_from_dht11();
void commands_registration(t_pars_tree **commands);
void make_noise();

#endif