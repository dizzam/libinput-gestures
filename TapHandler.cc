#include <linux/input.h>

#include "TapHandler.h"

MainHandler* TapHandler::test(MainHandler* handler)
{
    if (handler->type == LIBINPUT_EVENT_POINTER_BUTTON)
    {
        return new TapHandler(*handler);
    }

    return NULL;
}

void TapHandler::handle(Handler* &handler)
{
    type = libinput_event_get_type(current_event);
    switch (type)
    {
    case LIBINPUT_EVENT_POINTER_BUTTON:
    {
        struct libinput_event_pointer *pevent = libinput_event_get_pointer_event(current_event);
        uint32_t btn = libinput_event_pointer_get_button(pevent);
        int state = libinput_event_pointer_get_button_state(pevent);
        if (btn == BTN_MIDDLE && state == LIBINPUT_BUTTON_STATE_RELEASED)
        {
            run("xdotool key ctrl+w");
        }
        break;
    }
    default:
        update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
        break;
    }
}
