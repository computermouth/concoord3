
#include <stdio.h>

#include "ww.h"
#include "ww.h"
#include "ui.h"

void update(){
	
	bg_update();
	anim_panel_update();
	frame_panel_update();
	status_panel_update();
	canvas_panel_update();
	
}

void draw(){
	
	ww_draw_polygon(bg);
	ww_draw_polygon(anim_panel);
	ww_draw_polygon(frame_panel);
	ww_draw_polygon(status_panel);
	ww_draw_polygon(canvas_panel);

}

void cleanup(){
	
	ww_free_polygon(bg);
	ww_free_polygon(anim_panel);
	ww_free_polygon(frame_panel);
	ww_free_polygon(status_panel);
	ww_free_polygon(canvas_panel);
	
}

int main( void ) {
	
	if(ww_window_create("Testing", 1600, 900)) {
		printf("Failure\n");
		return 1;
	}
	
	ui_init();
	
	while(!ww_window_received_quit_event()) {
		
		update();
		
		ww_window_update_events();
		
		draw();
		
		ww_window_update_buffer();
		
		//~ SDL_Delay(1000 * 1);
	}
	
	cleanup();
	
	ww_window_destroy();
	return 0;
}
