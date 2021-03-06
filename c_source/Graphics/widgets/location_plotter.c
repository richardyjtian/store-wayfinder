#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../io/bridge.h"
#include "../../Bluetooth/query.h"
#include "../Touchscreen.h"
#include "../draw/DrawImages.h"
#include "../draw/DrawMap.h"

static _update_rate;
static volatile int _running;
static pthread_t _lp_thread;
static int _colour;
static int _xoffset, _yoffset;
static void *update_location(void * args);
static void plot_location(Point * p);
static void refill_location(Point * p);
static int approx_equal(Point * p1, Point * p2, int tolerance);
static int is_in_map(Point * p);

int location_plotter_start(int update_rate, int x_offset, int y_offset, int colour) {
    int result;
    _running = 1;
    _update_rate = update_rate;
    _xoffset = x_offset;
    _yoffset = y_offset;
    _colour = colour;

    result = pthread_create(&_lp_thread, NULL, update_location, NULL);
    if (result) {
        return 0;
    }
    return 1;
}

void location_plotter_end() {
    *LEDs = *LEDs & ~(0x4); // turn LED3 off
    _running = 0;
    pthread_join(_lp_thread, NULL);
}

static void *update_location(void * args) {
    *LEDs = *LEDs | 0x4; // turn on LED2 to indicate its running 
    Point *p = malloc(sizeof(Point));
    Point *curr_p, *prev_p;
    // initalize the point so the first iteration won't complain
    p->x = _xoffset + 50;
    p->y = _yoffset + 50;
    prev_p = p;
    while (_running) {
        curr_p = query_map_position();
        if (curr_p != NULL && !approx_equal(curr_p, prev_p, 5) && is_in_map(curr_p)) {
            refill_location(prev_p);
            plot_location(curr_p);
            free(prev_p);
            prev_p = curr_p;
        }
        if (!_running) break; // so we don't have to sleep for another bit before killing the thread
        sleep(_update_rate);
    }
    refill_location(prev_p);
    pthread_exit(NULL);
}

static void plot_location(Point * p) {
    Person(_xoffset, _yoffset, p->x, p->y, _colour);
}

static void refill_location(Point * p) {
    DrawMapSection(_xoffset, _yoffset, p->x, p->y);
}

/**
 * @returns 1 : if p would be located in the map domain
 */ 
static int is_in_map(Point * p) {
    return (p->x > PERSON_HALF_WIDTH) 
        && (p->y > PERSON_HALF_HEIGHT) 
        && (p->x < MAP_WIDTH - PERSON_HALF_WIDTH) 
        && (p->y < MAP_HEIGHT - PERSON_HALF_HEIGHT);
}

/** 
 * checks if 2 points are approximately equal
 * @returns 1 : if p1 and p2's difference in x and y are within tolerance 
 */
static int approx_equal(Point * p1, Point * p2, int tolerance) {
    return abs(p1->x - p2->x) < tolerance && abs(p1->y - p2->y) < tolerance;
}
