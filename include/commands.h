#ifndef ROBOBITES_COMMANDS_H
#define ROBOBITES_COMMANDS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    CMD_NONE = 0,
    CMD_DRIVE,
    CMD_ROUTE,
    CMD_PAIR,
    CMD_NOTE,
    CMD_STATUS
} CommandType;

typedef struct {
    int speed;
    int steering;
    char mode[16];
} DriveCommand;

typedef struct {
    char route_id[24];
    uint16_t stop_count;
    uint16_t byte_count;
    uint8_t bytes[256];
} RouteCommand;

typedef struct {
    char token[64];
} PairCommand;

typedef struct {
    char text[128];
} NoteCommand;

typedef struct {
    CommandType type;
    union {
        DriveCommand drive;
        RouteCommand route;
        PairCommand pair;
        NoteCommand note;
    } body;
} Command;

int command_parse(const char *line, Command *out);
void command_dispatch(const Command *command);

#endif
