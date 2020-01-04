#ifndef TAPHANDLER_H
#define TAPHANDLER_H

#include "main.h"
#include "MainHandler.h"

class TapHandler: public MainHandler
{
public:
    TapHandler() = default;
    TapHandler(MainHandler &h): MainHandler(h) { }

    void handle(Handler* &);

    static MainHandler* test(MainHandler *handler);
};

#endif
