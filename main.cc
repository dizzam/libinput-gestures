#include <poll.h>

#include "main.h"
#include "utils.h"
#include "MainHandler.h"

int main()
{
    struct libinput *li;
    struct libinput_event *event;
    Handler *handler = new MainHandler();

    li = libinput_path_create_context(&interface, NULL);
    struct libinput_device *dev = libinput_path_add_device(li, "/dev/input/by-path/platform-i8042-serio-1-event-mouse");
    libinput_device_config_tap_set_enabled(dev, LIBINPUT_CONFIG_TAP_ENABLED);

    struct pollfd fds;
    fds.fd = libinput_get_fd(li);
    fds.events = POLLIN;
    fds.revents = 0;
    while (poll(&fds, 1, -1) > -1)
    {
        do {
            libinput_dispatch(li);
            event = libinput_get_event(li);
            if(event == NULL) break;

            handler->current_event = event;
            handler->handle(handler);
            handler_gc();

            libinput_event_destroy(event);
        } while (1);
    }
    libinput_unref(li);
    return 0;
}
