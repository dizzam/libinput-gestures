#include <stdio.h>
#include <libinput.h>

#include "PointerHandler.h"

MainHandler* PointerHandler::test(MainHandler* handler)
{
    if (handler->type == LIBINPUT_EVENT_POINTER_AXIS)
    {
        return new PointerHandler(*handler);
    }

    return NULL;
}

enum update_status PointerHandler::update(Handler* &handler)
{
    current_pevent = libinput_event_get_pointer_event(current_event);

    if (libinput_event_pointer_has_axis(current_pevent, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL))
    {
        if (libinput_event_pointer_has_axis(current_pevent, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL)
                && libinput_event_pointer_get_axis_value(current_pevent, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL) == 0
                && libinput_event_pointer_get_axis_value(current_pevent, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL) == 0)
        {
            current_event = NULL;
            update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
            return AXIS_END;
        }
        return AXIS_VERTICAL;
    }

    x += libinput_event_pointer_get_axis_value(current_pevent, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL);
    time = libinput_event_pointer_get_time_usec(current_pevent);
    printf("[%lu] horiz: %6.2f\n", time, x);
    motion = test_motion(x, 0, 1, 70);

    return SUCCESS;
}

void PointerHandler::handle(Handler* &handler)
{
    type = libinput_event_get_type(current_event);
    switch (type)
    {
    case LIBINPUT_EVENT_POINTER_AXIS:
        if(update(handler) != SUCCESS) break;

        if (!pressed)
        {
            start_time = time;
            pressed = true;
        }

        if (motion.primary != NONE)
        {
            if ((time - start_time) < 50*1000)
                update_handler(handler, new NavigationHandler(*static_cast<PointerHandler*>(handler)));
            else
                update_handler(handler, new SwitchTabHandler(*static_cast<PointerHandler*>(handler)));
        }
        break;

    default:
        update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
        break;
    }
}

void NavigationHandler::handle(Handler* &handler)
{
    type = libinput_event_get_type(current_event);
    switch (type)
    {
    case LIBINPUT_EVENT_POINTER_AXIS:
        if (!update(handler)) break;

        if(!trigged)
        {
            motion = test_motion(x, 0, 1, 40);
            if (motion.primary == LEFT) run("xdotool key alt+Right");
            else if (motion.primary == RIGHT) run("xdotool key alt+Left");
            trigged = true;
        }

        break;

    default:
        update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
        break;
    }
}

void SwitchTabHandler::handle(Handler* & handler)
{
    type = libinput_event_get_type(current_event);
    switch (type)
    {
    case LIBINPUT_EVENT_POINTER_AXIS:
    {
        int status = update(handler);
        if (status == AXIS_VERTICAL) break;
        if (status == AXIS_END)
        {
            run("xdotool keyup ctrl");
            break;
        }

        if (!started)
        {
            run("xdotool keydown ctrl");
            started = true;
        }

        if (motion.primary != NONE) x = 0;
        if (motion.primary == RIGHT) run("xdotool key Tab");
        else if (motion.primary == LEFT) run("xdotool key shift+Tab");

        break;
    }
    default:
        update_handler(handler, new MainHandler(*static_cast<MainHandler*>(handler)));
        break;
    }
}
