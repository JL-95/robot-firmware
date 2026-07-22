#ifndef ROBOBITES_TELEMETRY_H
#define ROBOBITES_TELEMETRY_H

#include <stddef.h>
#include <stdint.h>

void telemetry_publish_status(const char *route_id, int speed, int steering);
uint8_t *telemetry_build_frame(const char *route_id, size_t *out_len);

#endif
