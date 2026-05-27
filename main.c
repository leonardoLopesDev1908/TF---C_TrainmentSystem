#include "question.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_SIZE 20
#define SELECTED_QUESTIONS_SIZE 20
#define MAX_QUESTIONS 400

void load_questions(question_t* q);
void loop_course(question_t* questions, unsigned int size, char* user);
void remove_question(question_t* questions, int i, int size);
void select_questions(question_t* all_questions, 
        question_t* selected_questions, int size);
void register_session(char* user, double performance);
void verify_historic();

/*A aplicação deverá ser capaz de calcular:
• quantidade total de utilizações da ferramenta;
• pontuação total acumulada;
• média de pontuação;
• quantidade de dias consecutivos recentes de utilização*/

int main()
{
    char user[MAX_NAME_SIZE];
    
    printf("Seu nome: ");
    scanf("%s", &user);

    question_t* questions = (question_t*) malloc(MAX_QUESTIONS * sizeof(question_t));

    load_questions(questions);
    question_t* selected_questions = (question_t*) malloc(SELECTED_QUESTIONS_SIZE * sizeof(question_t));
    select_questions(questions, selected_questions, SELECTED_QUESTIONS_SIZE);

    loop_course(selected_questions, SELECTED_QUESTIONS_SIZE, user);

    return 0;
}

void load_questions(question_t* q_array)
{
    int count = 0;
    for(int i = 0; i < 10; i++)
    {
        char path[100];

        snprintf(path, sizeof(path), "questions/questions_db%d.txt", i);

        FILE* file = fopen(path, "r");
    
        if(file == NULL)
        {
            printf("Arquivo indisponivel.\n");
            return;
        }
    
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

        fclose(file);
    }
}

void select_questions(question_t* all_questions, 
    question_t* selected_questions, int size)
{
    srand(time(NULL));
    int out_index = 0;
    int k, l;

    for(int i = 0; i < 10; i++)
    {
        int base = i * 5;

        k = rand() % 5;

        do {
            l = rand() % 5;
        } while(l == k);

        selected_questions[out_index++] = all_questions[base + k];
        selected_questions[out_index++] = all_questions[base + l];
    }
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
    verify_historic();
}

void register_session(char* user, double performance)
{
    FILE *file = fopen("log_sessions.txt", "a");
    
    if(file == NULL)
    {
        printf("Log de sessoes indisponivel\n");
        return;
    }

    time_t currentTime;
    time(&currentTime);

    char* timeStr = ctime(&currentTime);
    timeStr[strcspn(timeStr, "\n")] = '\0'; 
    
    fprintf(file, "%s (%s): %.2f\n", user, ctime(&currentTime), performance);
    fclose(file);
}

void verify_historic()
{
    FILE *file = fopen("users_register.txt", "r+");
    if(file == NULL)
    {
        printf("Registro de usuarios indisponivel\n");
        return;
    }

    time_t t;
    time(&t);

    bool user_found = false;
    char line[512];
    
    while(fgets(line, sizeof(line), file) == NULL)
    {
        if(line != *user) 
            continue;
        else 
        {
            user_found = true;
            break;
        }
    } 
    
    long last_date;
    long accumulated_sessions;
    
    if(user_found)
    {
        
        if(fgets(line, sizeof(line), file) == NULL)
        {
            fclose(file);
            return;
        }

        accumulated_sessions = atoi(line);
    
        if(fgets(line, sizeof(line), file) == NULL)
        {
            fclose(file);
            return;
        }
            last_date = atoi(line);
    }

    struct tm tm = *localtime(&t);

    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    time_t normalized = mktime(&tm);

    long days = normalized / 86400;

    if((days - last_date) == 1)
        accumulated_sessions++;
    else if(days != last_date)
        accumulated_sessions = 1;

    rewind(file);
    
    printf("Accumulated sessions: %ld\n", accumulated_sessions);
    printf("Last day: %ld\n", last_date);

    fprintf(file, "%ld\n %ld", accumulated_sessions, days);
    
    fclose(file);
}

void remove_question(question_t* questions, int i, int size)
{
    for(i; i < size - 1; i++)
    {
        questions[i] = questions[i+1];
    }
}
