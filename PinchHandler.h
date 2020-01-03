#ifndef PINCHHANDLER_H
#define PINCHHANDLER_H

#include "MainHandler.h"

class PinchHandler: public MainHandler
{
public:
    PinchHandler() = default;
    PinchHandler(MainHandler &h): MainHandler(h) { }

    double last_scale = 1;
    void handle(Handler* &handler);

    static Handler* test(Handler* handler); 
};

#endif
