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

    free(questions);
    free(selected_questions);

    return 0;
}

void loop_course(question_t* questions, unsigned int count, char* user)
{
    unsigned int total_questions = count;
    int i = 0;
    int errors = 0;
    unsigned int counting_questions = 1;
    while(count > 0)
    {
        if(i >= count) i = 0;
        if(counting_questions > total_questions)
            printf("Questão repetida\n");
        else
            printf("Contagem de questões: %d/%d\n", counting_questions, total_questions);
        char user_answer = 'F';
        int int_ans = 0;
        
        do
        {
            printf("%d: \n%s\n", questions[i].id, questions[i].description);
            printf("Resposta (F=Falso / V=Verdadeiro): ");
            scanf(" %c", &user_answer);  
        
        }while(user_answer != 'V' && user_answer != 'F'
                && user_answer != 'v' && user_answer != 'f');

        if(user_answer == 'F' || user_answer == 'f')
            int_ans = 0;
        else if(user_answer == 'V' || user_answer == 'v')
            int_ans = 1; 

        if(int_ans != questions[i].ans)
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
        counting_questions++; 
    }
    printf("Todas questoes respondidas\n");

    double performance = ((double)(total_questions) / (total_questions + errors)) * 10;
    printf("Seu desempenho: %d erros de %d questões.\n%.2f%%\n", 
            errors, total_questions, performance * 10); 

    register_session(user, performance);
    verify_historic(user, performance);
}
