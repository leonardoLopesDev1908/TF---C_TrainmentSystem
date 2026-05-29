#ifndef AUTH_H
#define AUTH_H

#include <openssl/sha.h>
#include <stdbool.h>

bool is_safe(char* password);
bool auth(char* user);

#endif

