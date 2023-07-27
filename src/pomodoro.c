#include "timer.h"
#include <stdbool.h>
#include "preferences.h"
#include "pomodoro.h"
#include <time.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

int maxY, maxX;

Preferences_t* get_preferences() {
    Preferences_t* preferences = (Preferences_t*) malloc(sizeof(Preferences_t));
    preference_init(preferences);
    return preferences;
}

void set_preferences(Preferences_t*) {}



void pomodoro_init(Pomodoro_t* pomodoro) {
    pomodoro-> is_running = false;
    pomodoro-> is_rest_period = false;
    pomodoro-> is_work_period = true;
    pomodoro-> is_long_rest_period = false;
    pomodoro-> total_work = 0;
    pomodoro-> total_rest = 0;
    pomodoro-> timer = NULL;
    pomodoro-> preferences = get_preferences();
}

void new_timer(Pomodoro_t* pomodoro) {
    if (pomodoro->timer != NULL) {
        timer_destroy(pomodoro->timer);
        pomodoro->timer = NULL;
    }

    if (pomodoro->timer == NULL) {
        if (pomodoro-> is_work_period) {

            // char workp_period[] = {0,25,0}; 
            pomodoro->timer = create_timer(pomodoro->preferences->work_period);        
        } else if (pomodoro-> is_rest_period) {
            pomodoro->timer = create_timer(pomodoro->preferences->rest_period);        
        } else if (pomodoro-> is_long_rest_period) {
            pomodoro->timer = create_timer(pomodoro->preferences->long_rest_period);        
        }
    }   
        
}

Pomodoro_t* start() {
    Pomodoro_t* pomodoro = (Pomodoro_t*) malloc(sizeof(Pomodoro_t));
    pomodoro_init(pomodoro);
    return pomodoro;
}

void pomodoro_destroy(Pomodoro_t* pomodoro) {
    timer_destroy(pomodoro->timer);
    free(pomodoro->preferences);
    free(pomodoro);
}

void check_time(Pomodoro_t* pomodoro){
    if (pomodoro->timer->time < 0) {
        printf("\a");
        if (pomodoro->preferences->current_session_length <= 0) {
            pomodoro->is_long_rest_period = true;
            pomodoro->is_work_period = false;
            pomodoro->is_rest_period = false;
            pomodoro->preferences->current_session_length = pomodoro->preferences->session_length;
            new_timer(pomodoro);
        
        } else if (pomodoro->is_long_rest_period) {
            pomodoro-> is_long_rest_period = false;
            pomodoro-> is_rest_period = false;
            pomodoro-> is_work_period = true;
            new_timer(pomodoro);

        } else if (pomodoro->is_work_period) {
            pomodoro-> is_work_period = false;
            pomodoro->is_rest_period = true;
            new_timer(pomodoro);
        } else {
            pomodoro->preferences->current_session_length --;
            pomodoro -> is_work_period = true;
            pomodoro-> is_rest_period = false;
            new_timer(pomodoro);
        }
    }
}


void finish(int sig) {
}

void handle_key_press(Pomodoro_t* pomodoro, int ch) {
    if (ch == ' ' && pomodoro->timer->is_paused) {
        resume_timer(pomodoro->timer);
    } else if (ch == ' ' && !pomodoro->timer->is_paused) {
        pause_timer(pomodoro->timer);
        draw_pause(maxX, maxY);
    } 
}


int main() {
    Pomodoro_t* pomodoro_p = start();  
    new_timer(pomodoro_p);
    
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
    getmaxyx(stdscr, maxY, maxX);
    int maxY_new;
    int maxX_new;
    // raw();


    int x = 3;
    

    // pthread_mutex_init(&mutex, NULL);  // Initialize the mutex
    // pthread_t key_thread; 

    // if (pthread_create(&key_thread, NULL, keyHandlerThread, pomodoro_p) != 0) {
    //     fprintf(stderr, "Failed to create key handler thread\n");
    //     return 1;
    // }

    int ch; 
    time_t last_time = time(NULL);  // Get the current time
    // timeout(1);  // Set a timeout of 100 milliseconds
    while (true) {

    
        // clear();
        getmaxyx(stdscr, maxY_new, maxX_new);
        if (maxX_new != maxX || maxY_new != maxY) {
            clear();
            maxX = maxX_new;
            maxY = maxY_new;
        }
                ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        } else {
            handle_key_press(pomodoro_p, ch);
        }

        time_t current_time = time(NULL);
        // clear();

        // sleep(1);

        if (current_time - last_time >= 1 && !pomodoro_p->timer->is_paused) {
            // Perform necessary updates here
            // ...
            if (pomodoro_p->timer->is_resumed) {
                pomodoro_p->timer->is_resumed = false;
            }

            check_time(pomodoro_p);
            draw_time(pomodoro_p->timer, maxX, maxY);
            remove_seconds(pomodoro_p->timer);

        
            last_time = current_time;  // Update lastTime
        }

        refresh();


    }

    // if (pthread_join(key_thread, NULL) != 0) {
    //     fprintf(stderr, "Failed to join key handler thread\n");
    //     return 1;
    // }
    // pomodoro_destroy(pomodoro_p);
    endwin();
    return 0;  
}