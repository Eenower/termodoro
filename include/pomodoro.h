#ifndef POMODORO_H
#define POMODORO_H

#include "timer.h"
#include "preferences.h"

typedef struct {
    bool is_running;
    bool is_work_period;
    bool is_rest_period;
    bool is_long_rest_period;
    int total_work;
    int total_rest;
    Timer_t* timer;
    Preferences_t* preferences;


} Pomodoro_t;




#endif