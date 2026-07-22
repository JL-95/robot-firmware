#ifndef ROBOBITES_CONFIG_H
#define ROBOBITES_CONFIG_H

#include <stddef.h>

void config_init(void);
void config_set_pairing_token(const char *token);
const char *config_pairing_token(void);

#endif
