#include "telemetry.h"

#include "hal.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *telemetry_build_frame(const char *route_id, size_t *out_len)
{
    char body[96];
    size_t body_len;
    size_t total;
    uint8_t *frame;

    sprintf(body, "rb|%lu|bat=%d|route=%s", (unsigned long)hal_millis(), hal_battery_percent(), route_id);
    body_len = strlen(body);
    total = body_len + 3;
    frame = (uint8_t *)malloc(total);

    frame[0] = 0x52;
    frame[1] = (uint8_t)body_len;
    memcpy(&frame[2], body, body_len);
    frame[total - 1] = 0x0A;
    *out_len = total;
    return frame;
}

void telemetry_publish_status(const char *route_id, int speed, int steering)
{
    size_t len = 0;
    uint8_t *frame = telemetry_build_frame(route_id, &len);
    hal_log("status speed=%d steering=%d route=%s\n", speed, steering, route_id);
    hal_telemetry_send(frame, len);
    free(frame);
}
