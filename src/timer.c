#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include "timer.h"
#include "ascii_digits.h"
#include "misc_symbols.h"

void add_seconds(Timer_t* timer) { 
    timer->time = timer->time + 1; 
}

void remove_seconds(Timer_t* timer) { 
    timer->time = timer->time - 1; 
}

 int get_hours (Timer_t* timer) {
    int hours = timer->time / 3600;
    return hours;
}

 int get_minutes (Timer_t* timer) {
    int minutes = timer->time % 3600 / 60;
    return minutes;
}

 int get_seconds (Timer_t* timer) {
    int seconds = timer->time % 3600 % 60;
    return seconds;
}

void pause_timer(Timer_t* timer) {
    timer->is_paused = true;
    timer->is_resumed = false;
}

void draw_pause(int maxX, int maxY) {

    const char * (*ascii_pause)[14] = ascii_art_pause;
    int middle_y = maxY / 2 - ((double)(0.5) * sizeof(ascii_pause));
    int middle_x = maxX / 2 - ((double)(0.5) * strlen(ascii_pause[1]));
    move(middle_y, middle_x); 
    
    int i;
    for (i=0; i<13; i++){
        printw("%s", ascii_art_pause[i]);
        refresh();

        printw("\n");
        middle_y++;
        move(middle_y, middle_x); 
    }
}

void resume_timer(Timer_t* timer) {
    timer->is_paused = false;
    timer->is_resumed = true;
}

 void get_time (Timer_t* timer) {
    int hours = get_hours(timer);
    int minutes = get_minutes(timer);
    int seconds = get_seconds(timer);

    if (timer->formatted_time != NULL) {
        free(timer->formatted_time); // Free previously allocated memory
        timer->formatted_time = NULL;

    } 
    if (timer->formatted_time == NULL) {
        timer->formatted_time = malloc(8 * sizeof(char));
    }
    // Set the correct format for leading 0s
    if (minutes < 10 && seconds < 10) {
        sprintf(timer->formatted_time, "%d:0%d:0%d", hours, minutes, seconds);
    } else if (minutes > 10 && seconds < 10){
        sprintf(timer->formatted_time, "%d:%02d:0%d", hours, minutes, seconds);
    } else if (minutes < 10 && seconds > 10) {
        sprintf(timer->formatted_time, "%d:0%d:%02d", hours, minutes, seconds);
    } else {
        sprintf(timer->formatted_time, "%d:%02d:%02d", hours, minutes, seconds);
    }

}

void timer_init(Timer_t* self, int time_sec) {
    self -> time =  time_sec;
    self -> start_time = time_sec;
    self -> is_paused = false;
    self -> is_resumed = false;
    self -> formatted_time = NULL;
}

Timer_t* create_timer(char time[3]) {
    int time_sec = (time[0] * 3600) + (time[1] * 60) + time[2];
    Timer_t* result = (Timer_t*) malloc(sizeof(Timer_t));
    timer_init(result, time_sec);
    return result;
}

void timer_destroy(Timer_t* self) {
    free(self->formatted_time);
    free(self);
}

void draw_time(Timer_t* self, int maxX, int maxY) {
    if (self == NULL) {
        return; 
    }
    // this works for the size of the digits i have but wont if the width of the chars increases
    get_time(self);
    const char* (*digits)[7] = ascii_art_digits;  // Reference to the array

    size_t size = sizeof(digits[0]) / sizeof(digits[0][0]);
    // Calculate the coordinates for the middle

    int middleY = maxY / 2 - (double)(0.5) * size;
    int middleX = maxX / 2 - (double)(0.5) * (7)*(strlen(digits[0][0])-1);
    int i, j = 0;
    // move(middleX, middleY);
    move(middleY, middleX);
    for (i=0; i < 6; i++){
        for (j=0; j < 7; j++) {
            char index = self->formatted_time[j];
            int int_index = index - '0';
            // printw("%d\n", rows);
            // refresh();
            printw("%s", ascii_art_digits[int_index][i]);

            refresh();
        }
            
          
        printw("\n");
        middleY++;
        move(middleY, middleX);
    }
}

