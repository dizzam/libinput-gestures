#ifndef SWIPEHANDLER_H
#define SWIPEHANDLER_H

#include "MainHandler.h"

class SwipeHandler: public MainHandler
{
public:
    SwipeHandler() = default;
    SwipeHandler(MainHandler &h): MainHandler(h) { }

    double x = 0;
    double y = 0;
    struct motion motion = {NONE, NONE};
    int count = 0;
    int finger = 0;
    bool flag = false;
    uint64_t start_time = 0;
    struct libinput_event_gesture *current_gevent = NULL;
    virtual void handle(Handler* &handler);

    static MainHandler* test(MainHandler *handle);

protected:
    void update();

private:
    static tester<SwipeHandler> handlers[2];
};

class SwitchWindowHandler: public SwipeHandler
{
public:
    SwitchWindowHandler(SwipeHandler &h): SwipeHandler(h) { flag = true; }

    bool started = false;
    void handle(Handler* &handler);

    static SwipeHandler* test(SwipeHandler* handle);
};

class OneHitHandler: public SwipeHandler
{
public:
    OneHitHandler(SwipeHandler &h): SwipeHandler(h) { flag = true; }

    void handle(Handler* &handle);

    static SwipeHandler* test(SwipeHandler* handle);
};

#endif
