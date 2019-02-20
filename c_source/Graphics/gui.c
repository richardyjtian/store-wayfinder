#include <stdio.h>
#include "Globalvars.h"
#include "Touchscreen.h"
#include "item/item.h"
#include "screens/screen.h"

#include "gui.h"

/*******************************/
//GLOBAL VARIABLES
char *cur_category;

int item_list_size = 0;
Item * item_list[20];
int pagenum = 0;
const int pagesize = 7;
/*******************************/

int gui_run() {
    screen_t curr_screen = HOME;

    while (1) {
        screen_draw(curr_screen);
        curr_screen = screen_listen(curr_screen);
    } 
}

int gui_start() {
    printf("GUI starting...\n");
	Init_Touch();
    screen_init();

    gui_run();
	return 0;
}
