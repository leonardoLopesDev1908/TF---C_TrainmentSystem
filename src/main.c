#include "auth.h"
#include "question.h"
#include "questions.h"
#include "session.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SIZE 20
#define SELECTED_QUESTIONS_SIZE 20
#define MAX_QUESTIONS 400

void loop_course(question_t* questions, unsigned int size, char* user);

int main()
{
    char user[MAX_NAME_SIZE];
    
    printf("Seu nome: ");
    scanf("%s", &user);
    
    if(!auth(user))
    {
        printf("Senha incorreta!\n");
        return 1;
    }

    question_t* questions = (question_t*) malloc(MAX_QUESTIONS * sizeof(question_t));

    load_questions(questions);
    question_t* selected_questions = (question_t*) malloc(SELECTED_QUESTIONS_SIZE * sizeof(question_t));
    select_questions(questions, selected_questions);

    loop_course(selected_questions, SELECTED_QUESTIONS_SIZE, user);

    return 0;
}

void loop_course(question_t* questions, unsigned int count, char* user)
{
    unsigned int total_questions = count;
    int i = 0;
    int errors = 0;
    while(count > 0)
    {
        if(i >= count) i = 0;

        int user_answer = 0;

        printf("%d: \n%s\n", questions[i].id, questions[i].description);
        printf("1 para Verdadeiro\n0 para Falso: ");
        scanf("%d", &user_answer);  

        if(user_answer != questions[i].ans)
        {
            printf("%s\n", questions[i].incorrect_msg);
            ++i;
            errors++;
        }
        else
        {
            --count;
            printf("%s\n", questions[i].correct_msg);
            remove_question(questions, i, count);
        }
        printf("\n");
    }
    printf("Todas questoes respondidas\n");

    double performance = ((double)(total_questions - errors) / total_questions) * 100;
    printf("Seu desempenho: %d/%d\n%.2f%%\n", 
            total_questions - errors, total_questions, performance); 

    register_session(user, performance);
    verify_historic(user, performance);
}
