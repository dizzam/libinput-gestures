#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <typeinfo>
#include <queue>

#include "utils.h"

using namespace std;

static int open_restricted(const char *path, int flags, void *user_data)
{
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data)
{
    close(fd);
}

const struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
};

void run(const char *command)
{
    printf("run: %s\n", command);
    system(command);
}

const char* motions_map[5] = {"NONE", "UP", "DOWN", "LEFT", "RIGHT"};

struct motion test_motion(double x, double y, double a, double threshold)
{
    struct motion motion = {NONE, NONE};
    if (a*x*x + y*y < threshold*threshold) return motion;

    double abx = abs(x);
    double aby = abs(y);
    enum motions mx;
    enum motions my;

    if (x > 0) mx = RIGHT;
    else mx = LEFT;
    if (y > 0) my = DOWN;
    else my = UP;

    if (abx > aby)
    {
        motion.primary = mx;
        motion.seconary = my;
    }
    else
    {
        motion.primary = my;
        motion.seconary = mx;
    }

    return motion;
}

queue<Handler*> trash;

void handler_gc()
{
    while (!trash.empty())
    {
        Handler* handler = trash.front();
        delete handler;
        trash.pop();
    }
}

void update_handler(Handler* &handler, Handler *new_handler)
{
    printf("update handler to %s\n", typeid(*new_handler).name());
    trash.push(handler);
    handler = new_handler;
    handler->handle(handler);
}
