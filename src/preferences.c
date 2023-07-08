#include "preferences.h"
#include <string.h>

void preference_init(Preferences_t* self) {
    char long_rest_period[] = {0,0,20};
    char work_period[] = {0,0,10};
    char rest_period[] = {0,0,5};

    int i;
    for (i=0; i < 3; i++) {
        self->long_rest_period[i] = long_rest_period[i];
        self->rest_period[i] = rest_period[i];
        self->work_period[i] = work_period[i]; 
    }

    self-> current_session_length = 4;
    self-> session_length = 4;
}