#include "commands.h"
#include "config.h"
#include "hal.h"

#include <stdio.h>

int main(void)
{
    char line[320];
    Command command;

    hal_init();
    config_init();

    while (hal_uart_read_line(line, sizeof(line))) {
        if (command_parse(line, &command)) {
            command_dispatch(&command);
        } else {
            hal_log("packet skipped\n");
        }
    }

    return 0;
}
