#include "question.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_SIZE 20
#define SELECTED_QUESTIONS_SIZE 20
#define MAX_QUESTIONS 400

void load_questions(question_t* q, FILE* file);
void loop_course(question_t* questions, unsigned int size);
void remove_question(question_t* questions, int i, int size);
void select_questions(question_t* all_questions, 
        question_t* selected_questions, int size);

//Ler um txt para cada modulo
//Controle de questões repetidas
//Usar um txt para registrar:
//Data do questionario, nome do usuario e desempenho

int main()
{
    char user[MAX_NAME_SIZE];
    
    printf("Seu nome: ");
    scanf("%s", &user);

    question_t* questions = (question_t*) malloc(MAX_QUESTIONS * sizeof(question_t));

    FILE* fptr = fopen("questions_db.txt", "r");

    load_questions(questions, fptr);
    question_t* selected_questions = (question_t*) malloc(SELECTED_QUESTIONS_SIZE * sizeof(question_t));
    select_questions(questions, selected_questions, SELECTED_QUESTIONS_SIZE);

    loop_course(selected_questions, SELECTED_QUESTIONS_SIZE);

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

void select_questions(question_t* all_questions, 
    question_t* selected_questions, int size)
{
    srand(time(NULL));
    int k, l;
    for(int i = 0; i < size; i += 2)
    {
        k = rand() % 5;
        
        do {
            l = rand() % size;
        }while(l == k);

        selected_questions[i] = all_questions[k];
        if(i + 1 < size)
            selected_questions[i+1] = all_questions[l];
    }
}

void loop_course(question_t* questions, unsigned int count)
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
    printf("Seu desempenho: %d/%d\n %.2f%\n", 
            total_questions - errors, total_questions, performance); 
}

void remove_question(question_t* questions, int i, int size)
{
    for(i; i < size - 1; i++)
    {
        questions[i] = questions[i+1];
    }
}
