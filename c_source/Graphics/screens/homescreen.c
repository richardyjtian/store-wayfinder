#include "../Colours.h"
#include "../Globalvars.h"
#include "../Text.h"
#include "../components/textbox.h"
#include "../Touchscreen.h"
#include <stdio.h>

#include "screen.h"

// every shape that should be plotted should go here
static struct {
    TextBox * new_list_button;
    TextBox * add_to_list_button;
    TextBox * map_button;
    TextBox * help_button;
    // TODO: add all the component of homescreen here
} _HomeScreen;

void homescreen_init() {
    // declare and set all shapes and components on the screen here
    TextBox * new_list_button = textbox_create(30, 60, 760, 95);
	TextBox * add_to_list_button = textbox_create(30, 165, 760, 95);
	TextBox * map_button = textbox_create(30, 270, 760, 95);
	TextBox * help_button = textbox_create(30, 375, 760, 95);

	textbox_set_box_colour(new_list_button, BLUE, FOREST_GREEN);
	textbox_set_box_colour(add_to_list_button, BLUE, FOREST_GREEN);
	textbox_set_box_colour(map_button, BLUE, FOREST_GREEN);
	textbox_set_box_colour(help_button, BLUE, FOREST_GREEN);

	textbox_set_text(new_list_button, "Create New Shopping List", FONT2, WHITE);
	textbox_set_text(add_to_list_button, "Add To Shopping List", FONT2, WHITE);
	textbox_set_text(map_button, "Go To Map", FONT2, WHITE);
	textbox_set_text(help_button, "Help", FONT2, WHITE);

    _HomeScreen.new_list_button = new_list_button;
    _HomeScreen.add_to_list_button = add_to_list_button;
    _HomeScreen.map_button = map_button;
    _HomeScreen.help_button = help_button;
}

void homescreen_draw() {
	CenteredSentence(FONT2, 30, 789, 0, 55, FOREST_GREEN, 0, "Hardware Store Wayfinder", DONT_ERASE);
    textbox_draw(_HomeScreen.new_list_button);
	textbox_draw(_HomeScreen.add_to_list_button);
	textbox_draw(_HomeScreen.map_button);
	textbox_draw(_HomeScreen.help_button);
}

screen_t homescreen_listen() {
	Point pp, pr;
    while (1) {
        pp = GetPress();
		pr = GetRelease();
		printf("Press %d, %d", pp.x, pp.y);
        if (textbox_within(_HomeScreen.new_list_button, pr)) {
			//Free item_list memory
			for(int i = 0; i < item_list_size; i++) {
				destroy_item(item_list[i]);
			}
			//Set the global item_list_size to 0
            item_list_size = 0;
			return CATEGORIES;
        } else if (textbox_within(_HomeScreen.add_to_list_button, pr)) {
			//Keep using the current item_list
            return CATEGORIES;
        } else if (textbox_within(_HomeScreen.map_button, pr)) {
            return MAP;
        } else if (textbox_within(_HomeScreen.help_button, pr)) {
            return HELP;
        }
	}
}
