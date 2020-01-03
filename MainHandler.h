#ifndef MAINHANDLER_H
#define MAINHANDLER_H

#include<libinput.h>

#include "main.h"
#include "utils.h"

class MainHandler: public Handler
{
public:
    MainHandler() = default;
    MainHandler(Handler &h): Handler(h) { }

    enum libinput_event_type type = LIBINPUT_EVENT_NONE;
    virtual void handle(Handler* &handler);
private:
    static tester handlers[4];
};

#endif
