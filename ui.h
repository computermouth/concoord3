
#include "ww.h"

ww_polygon_t * bg = NULL;

ww_polygon_t * anim_panel = NULL;

ww_polygon_t * frame_panel = NULL;

ww_polygon_t * status_panel = NULL;

ww_polygon_t * canvas_panel = NULL;

void ui_init(){
	
	ww_rgba_t bg_color = { 0x30, 0x56, 0x6f , 0xff };
	short bg_x[4] = { 0, ww_window_get_width(), ww_window_get_width(), 0 };
	short bg_y[4] = { 0,  0, ww_window_get_height(), ww_window_get_height() };
	bg = ww_new_polygon(bg_color, bg_x, bg_y, 4);
	
	ww_rgba_t panel_color = { 0x2c, 0x2c, 0x2c , 0x2c };
	
	short anim_panel_x[4] = { 4, 104, 104, 4 };
	short anim_panel_y[4] = { 4,  4, 
		ww_window_get_height() - 5, 
		ww_window_get_height() - 5 };
	anim_panel = ww_new_polygon(panel_color, anim_panel_x, anim_panel_y, 4);
	
	short frame_panel_x[4] = { 109, 509, 509, 109 };
	short frame_panel_y[4] = { 4,  4, 
		ww_window_get_height() - 5, 
		ww_window_get_height() - 5 };
	frame_panel = ww_new_polygon(panel_color, frame_panel_x, frame_panel_y, 4);
	
	short status_panel_x[4] = { 513, ww_window_get_width() - 5, ww_window_get_width() - 5, 513 };
	short status_panel_y[4] = { 4,  4, 44, 44 };
	status_panel = ww_new_polygon(panel_color, status_panel_x, status_panel_y, 4);
	
	short canvas_panel_x[4] = { 513, ww_window_get_width() - 5, ww_window_get_width() - 5, 513 };
	short canvas_panel_y[4] = { 49,  49, 
		ww_window_get_height() - 5, 
		ww_window_get_height() - 5 };
	canvas_panel = ww_new_polygon(panel_color, canvas_panel_x, canvas_panel_y, 4);

}

void bg_update(){
	printf("bg_update\n");
	bg->x[1] = ww_window_get_width();
	bg->x[2] = ww_window_get_width();
	
	bg->y[2] = ww_window_get_height();
	bg->y[3] = ww_window_get_height();
	
}

void anim_panel_update(){
	
	anim_panel->y[2] = ww_window_get_height() - 5;
	anim_panel->y[3] = ww_window_get_height() - 5;
	
}

void frame_panel_update(){
	
	frame_panel->y[2] = ww_window_get_height() - 5;
	frame_panel->y[3] = ww_window_get_height() - 5;
	
}

void status_panel_update(){
	
	status_panel->x[1] = ww_window_get_width() - 5;
	status_panel->x[2] = ww_window_get_width() - 5;
	
}

void canvas_panel_update(){
	
	canvas_panel->x[1] = ww_window_get_width() - 5;
	canvas_panel->x[2] = ww_window_get_width() - 5;
	
	canvas_panel->y[2] = ww_window_get_height() - 5;
	canvas_panel->y[3] = ww_window_get_height() - 5;
	
}
