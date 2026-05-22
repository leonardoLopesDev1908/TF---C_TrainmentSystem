#include "question.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_SIZE 20
#define SELECTED_QUESTIONS_SIZE 20
#define MAX_QUESTIONS 400

void load_questions(question_t* q, FILE* file);
void loop_course(question_t* questions);
void remove_question(question_t* questions, int i);
void selected_questions(question_t* all_questions, int size_1 
    question_t* selected_questions, int size_2);

int main()
{
    char user[MAX_NAME_SIZE];
    
    printf("Seu nome: ");
    scanf("%s", &user);

    question_t* questions = (question_t*) malloc(MAX_QUESTIONS * sizeof(question_t));

    FILE* fptr = fopen("questions_db.txt", "r");

    load_questions(questions, fptr);

    question_t* selected_questions = (question_t*) malloc(SELECTED_QUESTIONS_SIZE * sizeof(question_t));
    select_questions(&selected_question, SELECTED_QUESTIONS_SIZE,
            &questions, MAX_QUESTIONS);

    loop_course(selected_questions, SELECTED_QUESTIONS_SIZE);

    //testando
    // for(int i = 0; i < MAX_QUESTIONS; i++)
    // {
    //     question_t q = questions[i];
        
    //     if(q.id == 0) break;

    //     printf("Questao %d: \n%s \n%d \n%s \n%s\n",
    //         q.id, q.description, q.ans, q.correct_msg, q.incorrect_msg
    //     );
    // }

    return 0;
}

void load_questions(question_t* q_array, FILE* file)
{
    if(file == NULL)
    {
        printf("Arquivo indisponivel.\n");
        return;
    }

    int count = 0;
    char line[512];

    while(fgets(line, sizeof(line), file) != NULL)
    {
        if(strcmp(line, "\n") == 0)
            continue;

        question_t q;

        q.id = atoi(line);

        fgets(q.description, sizeof(q.description), file);
        q.description[strcspn(q.description, "\n")] = '\0';

        fgets(line, sizeof(line), file);
        q.ans = atoi(line);

        fgets(q.correct_msg, sizeof(q.correct_msg), file);
        q.correct_msg[strcspn(q.correct_msg, "\n")] = '\0';

        fgets(q.incorrect_msg, sizeof(q.incorrect_msg), file);
        q.incorrect_msg[strcspn(q.incorrect_msg, "\n")] = '\0';
        q_array[count++] = q;
    }
}

void selected_questions(question_t* all_questions, int size_1 
    question_t* selected_questions, int size_2)
{
    srand(time(NULL));
    for(int i = 0; i < size_1, i++)
    {
    
    }
}

void loop_course(question_t* questions, size_t count)
{
    int i = 0;
    while(count > 0)
    {
        if(i == count - 1) i = 0;

        int user_answer = 0;

        printf("%d: \n%s\n", questions[i].id, questions[i].description);
        scanf("%d", &user_answer);

        if(user_answer != questions[i].ans)
        {
            printf("%s\n", questions[i].incorrect_msg);
            ++i;
        }
        else
        {
            --count;
            printf("%s\n", questions[i].correct_msg);
            remove_question(questions, i, count);
        }
    }

    printf("Todas questoes respondidas\n");
}

void remove_question(question_t* questions, int index, int size)
{
    for(i; i < count - 1; i++)
        questions[i] = questions[i+1];
}
