#ifndef USER_H
#define USER_H

#include <string.h>
#include <time.h>

void register_session(char* user, double performance);
void verify_historic(char* user, double performance);

#endif