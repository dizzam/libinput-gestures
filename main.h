#ifndef MAIN_H
#define MAIN_H

#include <libinput.h>
#include <stdbool.h>

class Handler
{
public:
    struct libinput_event *current_event = NULL;
    virtual void handle(Handler* &handle) = 0;
};

typedef Handler* (*tester)(Handler *);

#endif
