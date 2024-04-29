#ifndef KAKURO_SLNOSLAV_H
#define KAKURO_SLNOSLAV_H

#include <ds/argument.h>
#include <ds/state.h>

void run(Settings settings);

#ifdef EXPOSE_PRIVATE_FUNCTIONS

void _print_state(SArray s);
bool _is_valid_file(char * filepath);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

#endif