#include <stdio.h>
#include <time.h>

#include "MainHandler.h"
#include "SwipeHandler.h"

tester SwipeHandler::handlers[2] = {
    &SwitchWindowHandler::test,
    &OneHitHandler::test,
};

Handler* SwipeHandler::test(Handler* handler)
{
    MainHandler *handler1 = static_cast<MainHandler *>(handler);
    if (handler1->type == LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN)
    {
        return new SwipeHandler(*handler1);
    }

    return NULL;
}

void SwipeHandler::update()
{
    current_gevent = libinput_event_get_gesture_event(current_event);
    type = libinput_event_get_type(current_event);
    x += libinput_event_gesture_get_dx_unaccelerated(current_gevent);
    y += libinput_event_gesture_get_dy_unaccelerated(current_gevent);
    count += 1;
    motion = test_motion(x, y);
    uint64_t time = libinput_event_gesture_get_time_usec(current_gevent);
    printf("[%lu] pos: %6.2f\t%6.2f\t[%s, %s]\n", time, x, y, motions_map[motion.primary], motions_map[motion.seconary]);
}

void SwipeHandler::handle(Handler* &handler)
{
    update();
    switch (type)
    {
    case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
        finger = libinput_event_gesture_get_finger_count(current_gevent);
        start_time = libinput_event_gesture_get_time_usec(current_gevent);
    case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
        update_handler(handler, handlers, sizeof(handlers)/sizeof(*handlers));
        break;
    case LIBINPUT_EVENT_GESTURE_SWIPE_END:
        motion = test_motion(x, y, 1, 17);
        if (!flag && motion.primary == NONE && (libinput_event_gesture_get_time_usec(current_gevent) - start_time) < 150*1000)
            run("xdotool key ctrl+w");
    default:
        update_handler(handler, new MainHandler(*static_cast<SwipeHandler*>(handler)));
        break;
    }
}

Handler* SwitchWindowHandler::test(Handler* handler)
{
    SwipeHandler *handler1 = static_cast<SwipeHandler *>(handler);
    if (!handler1->flag && handler1->finger == 3)
    {
        if (handler1->motion.primary == LEFT || handler1->motion.primary == RIGHT) return new SwitchWindowHandler(*handler1);
    }

    return NULL;
}

void SwitchWindowHandler::handle(Handler* &handler)
{
    update(); // FIXME: When called from SwipeHandler, this will be executed twice
    switch (type)
    {
    case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
    case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
        if (started && count < 10) return;

        if(!started)
        {
            printf("swipe begin\n");
            run("xdotool keydown alt");
            started = true;
        }

        if (motion.primary == LEFT) run("xdotool key shift+Tab");
        else if (motion.primary == RIGHT) run("xdotool key Tab");

        if (motion.primary == LEFT || motion.primary == RIGHT)
        {
            printf("reset\n");
            x = 0; y = 0; count = 0;
        }
        break;
    case LIBINPUT_EVENT_GESTURE_SWIPE_END:
    default:
        if (started)
        {
            printf("swipe stop\n");
            const struct timespec req = { 0, 20*1000*1000 };
            nanosleep(&req, NULL);
            run("xdotool keyup alt");
        }
        x = 0; y = 0;
        update_handler(handler, new SwipeHandler(*static_cast<SwitchWindowHandler*>(handler)));
        break;
    }
}

Handler* OneHitHandler::test(Handler* handler)
{
    SwipeHandler *handler1 = static_cast<SwipeHandler *>(handler);
    if (!handler1->flag && handler1->motion.primary != NONE)
    {
        return new OneHitHandler(*handler1);
    }

    return NULL;
}

void OneHitHandler::handle(Handler* &handler)
{
    if (finger == 3)
    {
        if (motion.primary == UP) run("xdotool key super+Tab");
        else if (motion.primary == DOWN) run("xdotool key super+d");
    }
    else
    {
        if (motion.primary == LEFT) run("xdotool key ctrl+super+Right");
        else if (motion.primary == RIGHT) run("xdotool key ctrl+super+Left");
    }

    x = 0; y = 0;
    update_handler(handler, new SwipeHandler(*static_cast<OneHitHandler*>(handler)));
}
