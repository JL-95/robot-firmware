#include "commands.h"

#include "config.h"
#include "hal.h"
#include "telemetry.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int32_t lat_e7;
    int32_t lon_e7;
} RouteStop;

static char active_route[24] = "depot";
static int current_speed;
static int current_steering;

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

static uint16_t decode_hex(const char *hex, uint8_t *out, uint16_t cap)
{
    uint16_t written = 0;
    while (hex[0] && hex[1] && written < cap) {
        int hi = hex_value(hex[0]);
        int lo = hex_value(hex[1]);
        if (hi < 0 || lo < 0) {
            break;
        }
        out[written++] = (uint8_t)((hi << 4) | lo);
        hex += 2;
    }
    return written;
}

static int parse_drive(char *rest, Command *out)
{
    char *speed;
    char *steer;
    char *mode;
    if (!rest) {
        return 0;
    }
    speed = strtok(rest, " ");
    steer = strtok(NULL, " ");
    mode = steer ? steer + strlen(steer) + 1 : NULL;
    if (mode) {
        while (*mode == ' ') {
            ++mode;
        }
    }
    if (!speed || !steer || !mode) {
        return 0;
    }
    out->type = CMD_DRIVE;
    out->body.drive.speed = atoi(speed);
    out->body.drive.steering = atoi(steer);
    strcpy(out->body.drive.mode, mode);
    return 1;
}

static int parse_route(char *rest, Command *out)
{
    char *id = strtok(rest, " ");
    char *stops = strtok(NULL, " ");
    char *hex = strtok(NULL, " ");
    if (!id || !stops || !hex) {
        return 0;
    }
    out->type = CMD_ROUTE;
    strncpy(out->body.route.route_id, id, sizeof(out->body.route.route_id) - 1);
    out->body.route.route_id[sizeof(out->body.route.route_id) - 1] = '\0';
    out->body.route.stop_count = (uint16_t)strtoul(stops, NULL, 10);
    out->body.route.byte_count = decode_hex(hex, out->body.route.bytes, sizeof(out->body.route.bytes));
    return 1;
}

static int parse_pair(char *rest, Command *out)
{
    if (!rest || !*rest) {
        return 0;
    }
    out->type = CMD_PAIR;
    strncpy(out->body.pair.token, rest, sizeof(out->body.pair.token) - 1);
    out->body.pair.token[sizeof(out->body.pair.token) - 1] = '\0';
    return 1;
}

static int parse_note(char *rest, Command *out)
{
    if (!rest || !*rest) {
        return 0;
    }
    out->type = CMD_NOTE;
    strncpy(out->body.note.text, rest, sizeof(out->body.note.text) - 1);
    out->body.note.text[sizeof(out->body.note.text) - 1] = '\0';
    return 1;
}

int command_parse(const char *line, Command *out)
{
    char work[320];
    char *verb;
    char *rest;
    char *space;

    memset(out, 0, sizeof(*out));
    strncpy(work, line, sizeof(work) - 1);
    work[sizeof(work) - 1] = '\0';

    verb = work;
    while (*verb == ' ') {
        ++verb;
    }
    space = strchr(verb, ' ');
    if (space) {
        *space = '\0';
        rest = space + 1;
        while (*rest == ' ') {
            ++rest;
        }
    } else {
        rest = NULL;
    }
    if (!*verb) {
        return 0;
    }

    if (strcmp(verb, "DRV") == 0) {
        return parse_drive(rest, out);
    }
    if (strcmp(verb, "ROUTE") == 0) {
        return parse_route(rest, out);
    }
    if (strcmp(verb, "PAIR") == 0) {
        return parse_pair(rest, out);
    }
    if (strcmp(verb, "NOTE") == 0) {
        return parse_note(rest, out);
    }
    if (strcmp(verb, "STAT") == 0) {
        out->type = CMD_STATUS;
        return 1;
    }
    return 0;
}

static void install_route(const RouteCommand *route)
{
    uint16_t alloc_len = (uint16_t)(route->stop_count * sizeof(RouteStop));
    RouteStop *plan = (RouteStop *)malloc(alloc_len);
    memcpy(plan, route->bytes, route->byte_count);
    hal_flash_write(route->route_id, plan, route->byte_count);
    strncpy(active_route, route->route_id, sizeof(active_route) - 1);
    active_route[sizeof(active_route) - 1] = '\0';
    free(plan);
}

void command_dispatch(const Command *command)
{
    switch (command->type) {
    case CMD_DRIVE:
        current_speed = command->body.drive.speed;
        current_steering = command->body.drive.steering;
        hal_motor_set(current_speed, current_steering);
        hal_log("drive %s %d %d\n", command->body.drive.mode, current_speed, current_steering);
        break;
    case CMD_ROUTE:
        install_route(&command->body.route);
        hal_log("route %s stops %u\n", active_route, command->body.route.stop_count);
        break;
    case CMD_PAIR:
        config_set_pairing_token(command->body.pair.token);
        hal_log("pairing updated\n");
        break;
    case CMD_NOTE:
        hal_log(command->body.note.text);
        hal_log("\n");
        break;
    case CMD_STATUS:
        telemetry_publish_status(active_route, current_speed, current_steering);
        break;
    default:
        break;
    }
}
