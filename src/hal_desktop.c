#include "hal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void hal_init(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Robobites rover link ready\n");
}

int hal_uart_read_line(char *buffer, size_t capacity)
{
    if (!fgets(buffer, (int)capacity, stdin)) {
        return 0;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}

void hal_motor_set(int speed, int steering)
{
    printf("motor speed=%d steering=%d\n", speed, steering);
}

void hal_flash_write(const char *key, const void *data, size_t len)
{
    printf("flash key=%s bytes=%u\n", key, (unsigned)len);
}

int hal_battery_percent(void)
{
    return 87;
}

uint32_t hal_millis(void)
{
    return (uint32_t)((clock() * 1000u) / CLOCKS_PER_SEC);
}

void hal_log(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void hal_telemetry_send(const uint8_t *data, size_t len)
{
    size_t i;
    printf("telemetry");
    for (i = 0; i < len; ++i) {
        printf(" %02X", data[i]);
    }
    printf("\n");
}
