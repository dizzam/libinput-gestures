#ifndef UTIL_H
#define UTIL_H

#include "main.h"

extern const struct libinput_interface interface;

void run(const char *command);

enum motions
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};
extern const char* motions_map[5];

struct motion
{
    enum motions primary;
    enum motions seconary;
};

struct motion test_motion(double x, double y, double a = 0.2, double threshold = 75);

void handler_gc();

void update_handler(Handler* &handler, Handler *new_handler);

void update_handler(Handler* &handler, const tester handlers[], int n);

#endif
