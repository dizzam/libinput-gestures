#include <stdio.h>

#include "PinchHandler.h"
#include "utils.h"

MainHandler* PinchHandler::test(MainHandler* handler)
{
    if (handler->type == LIBINPUT_EVENT_GESTURE_PINCH_BEGIN)
        return new PinchHandler(*handler);

    return NULL;
}

void PinchHandler::handle(Handler* &handler)
{
    type = libinput_event_get_type(current_event);
    switch (type)
    {
    case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
    case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE:
    {
        struct libinput_event_gesture *current_gevent = libinput_event_get_gesture_event(current_event);
        double scale = libinput_event_gesture_get_scale(current_gevent);
        uint64_t time = libinput_event_gesture_get_time_usec(current_gevent);
        printf("[%lu] scale: %6.2f\n", time, scale);

        double delta = last_scale - scale;
        if (abs(delta) > 0.1)
        {
            if(delta < 0) run("xdotool key ctrl+plus");
            else if(delta > 0) run("xdotool key ctrl+minus");
            last_scale = scale;
        }
        break;
    }
    default:
        update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
        break;
    }
}
