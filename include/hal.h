#ifndef ROBOBITES_HAL_H
#define ROBOBITES_HAL_H

#include <stddef.h>
#include <stdint.h>

void hal_init(void);
int hal_uart_read_line(char *buffer, size_t capacity);
void hal_motor_set(int speed, int steering);
void hal_flash_write(const char *key, const void *data, size_t len);
int hal_battery_percent(void);
uint32_t hal_millis(void);
void hal_log(const char *fmt, ...);
void hal_telemetry_send(const uint8_t *data, size_t len);

#endif
