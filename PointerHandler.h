#ifndef POINTERHANDLER_H
#define POINTERHANDLER_H

#include "main.h"
#include "MainHandler.h"

enum update_status
{
    SUCCESS,
    AXIS_VERTICAL,
    AXIS_END
};

class PointerHandler: public MainHandler
{
public:
    PointerHandler() = default;
    PointerHandler(MainHandler &h): MainHandler(h) { }

    double x = 0;
    bool pressed = false;
    uint64_t start_time = 0;
    uint64_t time = 0;
    struct motion motion = { NONE, NONE };
    struct libinput_event_pointer *current_pevent = NULL;
    virtual void handle(Handler* &handler);

    static Handler* test(Handler* handler);

protected:
    enum update_status update(Handler* &handler);
};

class NavigationHandler: public PointerHandler
{
public:
    NavigationHandler() = default;
    NavigationHandler(PointerHandler &h): PointerHandler(h) { }

    bool trigged = false;
    void handle(Handler* &handler);
};

class SwitchTabHandler: public PointerHandler
{
public:
    SwitchTabHandler() = default;
    SwitchTabHandler(PointerHandler &h): PointerHandler(h) { }

    bool started = false;
    void handle(Handler* &handler);
};

#endif
