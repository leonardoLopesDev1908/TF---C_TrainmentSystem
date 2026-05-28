#ifndef QUESTION_H
#define QUESTION_H

#define MAX_STR 512

typedef struct{
    int id;
    char description[MAX_STR];
    int ans;
    char correct_msg[MAX_STR];
    char incorrect_msg[MAX_STR];
} question_t;

#endif