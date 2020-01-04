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

template<class H>
using tester = H* (*)(H*);

template<class H>
void update_handler(Handler* &handler, const tester<H> handlers[], int n)
{
    Handler *new_handler = NULL;
    for (int i = 0; i < n; i++)
    {
        if (!handlers[i]) continue;

        new_handler = handlers[i](static_cast<H*>(handler));
        if (new_handler)
        {
            update_handler(handler, new_handler);
            return;
        }
    }
}

#endif
