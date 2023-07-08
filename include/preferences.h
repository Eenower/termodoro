#ifndef PREFERENCES_H
#define PREFERENCES_H


typedef struct {
    char rest_period[3];
    char work_period[3];
    char long_rest_period[3];
    int session_length;
    int current_session_length;

} Preferences_t;

void preference_init(Preferences_t* preferences);

#endif