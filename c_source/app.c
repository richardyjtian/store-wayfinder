#include <pthread.h>
#include <stdlib.h>

#include "Graphics/gui.h"
#include "Security/tracker.h"
#include "io/bridge.h"

void app_start() {
    // start tracking of the device with refresh rate of 1 second
    tracker_start(1);
    gui_start();
}