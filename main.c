/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gl3.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#define MAX_STR_LEN 128
#define MAX_POLYGONS 128
#define X_PAD 4
#define Y_PAD 5

typedef struct{
	int show;
	char name[MAX_STR_LEN];
	int name_len;
	struct nk_color color;
} polygon;

int active_polygons;
int total_created;
polygon polygon_list[MAX_POLYGONS];
polygon * sorted_polygon_list[MAX_POLYGONS];

int resized_height = WINDOW_HEIGHT;
int resized_width = WINDOW_WIDTH;

void init_polygons(){
	
	int i;
	for(i = 0; i < MAX_POLYGONS; i++){
	
		memset(polygon_list[i].name, 0, MAX_STR_LEN);
		sprintf(polygon_list[i].name, "unnamed_%03d", i);
		
		polygon_list[i].show = 1;
		polygon_list[i].name_len = strlen(polygon_list[i].name);
		polygon_list[i].color = nk_rgba(0, 255, 255, 255);
		
		sorted_polygon_list[i] = &polygon_list[i];
	}
	
	active_polygons = 0;
	total_created = 0;
	
}

int
main(int argc, char* argv[])
{
    /* Platform */
    SDL_Window *win;
    SDL_GLContext glContext;
    struct nk_color background;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    win = SDL_CreateWindow("Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);

    /* OpenGL setup */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    ctx = nk_sdl_init(win);
    {
		struct nk_font_atlas *atlas;
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
    }
    
    init_polygons();	
	
    background = nk_rgb(48,86,111);
        
    while (running)
    {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);
        
        SDL_GetWindowSize(win, &resized_width, &resized_height);


        /* Left panel GUI */
        if (nk_begin(ctx, "New", nk_rect(4, 5, 296, resized_height - (5 * 2)),
            NK_WINDOW_TITLE|NK_WINDOW_BORDER))
        {
            nk_menubar_begin(ctx);
            nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
            nk_layout_row_push(ctx, 45);
            if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))) {
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
                nk_menu_item_label(ctx, "SAVE", NK_TEXT_LEFT);
                if (nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT))
					goto cleanup;					
                if (nk_menu_item_label(ctx, "RESET", NK_TEXT_LEFT))
					init_polygons();
                nk_menu_end(ctx);
            }
            nk_layout_row_end(ctx);
            nk_menubar_end(ctx);
            
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "add path"))
                if (active_polygons < MAX_POLYGONS - 1){
					active_polygons++;
					total_created++;
					
					memset(sorted_polygon_list[active_polygons]->name, 0, MAX_STR_LEN);
					sprintf(sorted_polygon_list[active_polygons]->name, "unnamed_%03d", total_created);
					
					sorted_polygon_list[active_polygons]->show = 1;
					sorted_polygon_list[active_polygons]->name_len = strlen(sorted_polygon_list[active_polygons]->name);
					sorted_polygon_list[active_polygons]->color = nk_rgba(0, 255, 255, 255);
				}
            
            
            // separator hack
			nk_layout_row_dynamic(ctx, 2, 1);
			nk_spacing(ctx, 1);
			nk_layout_row_dynamic(ctx, 5, 1);
			nk_size cur = 1;
			nk_progress(ctx, &cur, 1, 0);
			nk_layout_row_dynamic(ctx, 2, 1);
			nk_spacing(ctx, 1);
            
            int i;
            for(i = active_polygons; i >=0 ; i--) {
				
				nk_layout_row_dynamic(ctx, 30, 2);
				nk_checkbox_label(ctx, "show", &sorted_polygon_list[i]->show);
				nk_edit_string(ctx, NK_EDIT_FIELD, sorted_polygon_list[i]->name, &sorted_polygon_list[i]->name_len, MAX_STR_LEN, nk_filter_ascii);
				
				nk_layout_row_dynamic(ctx, 30, 4);
				sorted_polygon_list[i]->color.r = (nk_byte)nk_propertyi(ctx, "#", 0, sorted_polygon_list[i]->color.r, 255, 1,1);
				sorted_polygon_list[i]->color.g = (nk_byte)nk_propertyi(ctx, "#", 0, sorted_polygon_list[i]->color.g, 255, 1,1);
				sorted_polygon_list[i]->color.b = (nk_byte)nk_propertyi(ctx, "#", 0, sorted_polygon_list[i]->color.b, 255, 1,1);
				nk_button_color(ctx, sorted_polygon_list[i]->color);
				
				nk_layout_row_dynamic(ctx, 30, 4);
				if (nk_button_label(ctx, "up") && i != active_polygons){					
					polygon * tmp = sorted_polygon_list[i];
					sorted_polygon_list[i] = sorted_polygon_list[i+1];
					sorted_polygon_list[i+1] = tmp;
				}
				if (nk_button_label(ctx, "down") && i != 0){					
					polygon * tmp = sorted_polygon_list[i];
					sorted_polygon_list[i] = sorted_polygon_list[i-1];
					sorted_polygon_list[i-1] = tmp;
				}
				nk_spacing(ctx, 1);
				if (nk_button_label(ctx, "delete")){
					
					int j;
					polygon * tmp = sorted_polygon_list[i];
					// I call it "One inch... to the left!"
					for(j = i; j < MAX_POLYGONS - 1; j++)
						sorted_polygon_list[j] = sorted_polygon_list[j + 1];
					
					// re-cap with defaults
					int end = MAX_POLYGONS - 1;
					sorted_polygon_list[end] = tmp;
					
					memset(sorted_polygon_list[end]->name, 0, MAX_STR_LEN);
					sprintf(sorted_polygon_list[end]->name, "unnamed_%03d", total_created);
					
					sorted_polygon_list[end]->show = 1;
					sorted_polygon_list[end]->name_len = strlen(sorted_polygon_list[end]->name);
					sorted_polygon_list[end]->color = nk_rgba(0, 255, 255, 255);
					
					active_polygons--;
					
				}
				
				// separator hack
				nk_layout_row_dynamic(ctx, 2, 1);
				nk_spacing(ctx, 1);
				nk_layout_row_dynamic(ctx, 5, 1);
				nk_progress(ctx, &cur, 1, 0);
				nk_layout_row_dynamic(ctx, 2, 1);
				nk_spacing(ctx, 1);
			}

            
        }
        nk_end(ctx);
        
        if (nk_begin(ctx, "top panel", nk_rect(300 + 4, 5, resized_width - 300 - 9, 30),
			NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
        {
			char tmp_str[MAX_STR_LEN];
			sprintf(tmp_str, "polys: %03d / %03d", active_polygons, MAX_POLYGONS);
			
            nk_layout_row_dynamic(ctx, 20, 3);
            nk_label(ctx, tmp_str, NK_TEXT_LEFT);
			nk_spacing(ctx, 1);
			
			memset(tmp_str, 0, MAX_STR_LEN);
			sprintf(tmp_str, "points: %03d / %03d", active_polygons, MAX_POLYGONS);
			
            nk_label(ctx, tmp_str, NK_TEXT_RIGHT);
        }
        nk_end(ctx);
        
        if (nk_begin(ctx, "canvas", nk_rect(300 + 4, 30 + 11, resized_width - 300 - 9, resized_height - 46),
			NK_WINDOW_BORDER))
        {
            
        }
        nk_end(ctx);


        /* Draw */
        {float bg[4];
        nk_color_fv(bg, background);
        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
        SDL_GL_SwapWindow(win);}
    }

cleanup:
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

