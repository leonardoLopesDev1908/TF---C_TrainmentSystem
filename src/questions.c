#include "question.h"
#include "questions.h"

#include <stdlib.h>

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
    question_t* selected_questions)
{
    srand(time(NULL));
    int out_index = 0;
    int k, l;

    for(int i = 0; i < 10; i++)
    {
        int base = i * 10;

        k = rand() % 10;

        do {
            l = rand() % 10;
        } while(l == k);

        selected_questions[out_index++] = all_questions[base + k];
        selected_questions[out_index++] = all_questions[base + l];
    }
}

void remove_question(question_t* questions, int i, int size)
{
    for(i; i < size; i++)
    {
        questions[i] = questions[i + 1];
    }
}
