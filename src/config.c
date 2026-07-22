#include "config.h"

#include "hal.h"

#include <string.h>

typedef struct {
    char robot_id[16];
    char pairing_token[32];
} RobotConfig;

static RobotConfig cfg;

void config_init(void)
{
    strcpy(cfg.robot_id, "rb-porter-01");
    cfg.pairing_token[0] = '\0';
}

void config_set_pairing_token(const char *token)
{
    size_t len = strlen(token);
    size_t i;
    if (len > sizeof(cfg.pairing_token)) {
        len = sizeof(cfg.pairing_token);
    }
    for (i = 0; i <= len; ++i) {
        cfg.pairing_token[i] = token[i];
    }
    hal_flash_write("pairing", cfg.pairing_token, strlen(cfg.pairing_token) + 1);
}

const char *config_pairing_token(void)
{
    return cfg.pairing_token;
}
