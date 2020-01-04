#include "MainHandler.h"
#include "SwipeHandler.h"
#include "PointerHandler.h"
#include "TapHandler.h"
#include "PinchHandler.h"

tester<MainHandler> MainHandler::handlers[4] = {
    &SwipeHandler::test,
    &PinchHandler::test,
    &PointerHandler::test,
    &TapHandler::test
};

void MainHandler::handle(Handler* &handler)
{
    if (!current_event) return;

    type = libinput_event_get_type(current_event);
    update_handler(handler, handlers, sizeof(handlers)/sizeof(*handlers));
}
