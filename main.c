#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "console.h"
#include "cpu_temp.h"
#include <math.h>
#include <string.h>
#include "colors.h"

uint16_t* cpu_temp_history = NULL;
uint16_t cpu_temp_hist_size = 0;
int cpu_hist_index = 0;

char* graph_buf = NULL;
int graph_buf_w = 0,
	graph_buf_h = 0;

void history_add(int cpu_temp) {
	int array_length = cpu_temp_hist_size / sizeof(uint16_t);
	cpu_temp_history[cpu_hist_index] = (uint16_t)cpu_temp;

	if (cpu_hist_index == array_length) {
		memcpy(&cpu_temp_history[0], &cpu_temp_history[1], (array_length - 1) * sizeof(uint16_t));
	}

	if (cpu_hist_index < array_length)
		cpu_hist_index++;
}

void clear_graph(consize_t csize) {

	int buf_size = csize.w * (csize.h-1)+1;

	if (csize.w != graph_buf_w ||
		(csize.h - 1) != graph_buf_h) {

		if (graph_buf == NULL) {
			graph_buf = malloc(buf_size);
		} else {
			graph_buf = realloc(graph_buf, buf_size);
		}

		graph_buf_w = csize.w;
		graph_buf_h = csize.h-1;
	}

	// Clear the buffer with spaces
	memset(graph_buf, ' ', buf_size);
	graph_buf[buf_size-1] = 0x00;
}

void graph_draw_at(uint16_t x, uint16_t y, char c) {
	int index = x + (y * graph_buf_w);
	graph_buf[index] = c;
}

void draw_graph() {
	for(int i = 0; i < cpu_hist_index; i++) {
		uint16_t cpu_temp = cpu_temp_history[i];
		
		int draw_h = (int)((double)(((double)(cpu_temp - 20) / 80.00) * (double)(graph_buf_h)));
		// sprintf(graph_buf, "%d ", draw_h);
		for (int y = 0; y < draw_h; y++) {
			int ypos = graph_buf_h - y - 1;
			if (y == draw_h-1) {
				graph_draw_at(i, ypos, '.');
			} else {
				graph_draw_at(i, ypos, ':');
			}
		}
	}
}

void generate_graph() {
	consize_t csize = console_get_size();
	clear_graph(csize);
	draw_graph();
	printf(FORERED BOLD("%s"), graph_buf);
	fflush(stdout);
}

void display_temp(int cpu_temp) {

	consize_t csize = console_get_size();

	int array_size = csize.w * sizeof(uint16_t);
	if (array_size != cpu_temp_hist_size) {
		if (cpu_temp_history == NULL) {
			cpu_temp_history = malloc(array_size);
		}
		else {
			cpu_temp_history = realloc(cpu_temp_history, array_size);
		}
		cpu_temp_hist_size = array_size; 
	}

	int temp_str_len = 0;
	while(pow(10, temp_str_len) < cpu_temp)
		temp_str_len++;

	int draw_pos_x = csize.w / 2 - (temp_str_len + 27) / 2;

	console_set_cursor_pos(draw_pos_x, 0);
	if (cpu_temp < 50)
		printf(FOREGRN);
	else if (cpu_temp >= 50 && cpu_temp <= 70)
		printf(FOREYEL);
	else
		printf(FORERED);

	printf(cpu_temp <= 90 ? "[ Package temperature: %d°C ]\n" : BOLD("[ Package temperature: %d°C ]\n"), cpu_temp);
	history_add(cpu_temp);
}

int main( int argc, char *argv[] )
{

    system("clear");
    console_hide_cursor();

    while(1) {
        console_set_cursor_pos(0, 0);
        int temp = get_cpu_temp();
        display_temp(temp);
		generate_graph();
        sleep(1);
    }

    return 0;
}