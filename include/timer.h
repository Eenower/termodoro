#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    bool is_paused;
    bool is_resumed;
    int time;
    int start_time;
    char* formatted_time;
} Timer_t;

void add_seconds(Timer_t* timer);
void remove_seconds(Timer_t* timer);
int get_hours(Timer_t* timer);
int get_minutes(Timer_t* timer);
int get_seconds(Timer_t* timer);
void get_time(Timer_t* timer);
void timer_init(Timer_t* self, int time);
Timer_t* create_timer(char time[3]);
void timer_destroy(Timer_t* self);
void draw_time(Timer_t* self, int maxX, int maxY);
void pause_timer(Timer_t* timer);
void resume_timer(Timer_t* timer);
void draw_pause(int maxX, int maxY);

#endif  // TIMER_H
