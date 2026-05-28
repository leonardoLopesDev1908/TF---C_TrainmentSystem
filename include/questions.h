#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "question.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

void load_questions(question_t* q);
void remove_question(question_t* questions, int i, int size);
void select_questions(question_t* all_questions, 
        question_t* selected_questions);

#endif
