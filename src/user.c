#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void verify_historic(char* user, double performance)
{
    char path[128];
    snprintf(path, sizeof(path), "user_registers/%s.txt", user);

    FILE *file = fopen(path, "r+");
    if(file == NULL)
    {
        file = fopen(path, "w+");
        if(file == NULL)
        {
            printf("Erro ao criar registro de usuario\n");
            return;
        }
    }
    time_t t;
    time(&t);

    char* current_date = ctime(&t);
    current_date[strcspn(current_date,  "\n")] = '\0';
    
    struct tm tm = *localtime(&t);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    time_t normalized = mktime(&tm);
    long days = normalized / 86400;
    
    char line[512];
    long last_date = days;
    long accumulated_sessions = 1;
    long total_sessions = 0;
    long current_total = 0;
    double current_media = 0;

    int line_n = 0;

    rewind(file);

    while(fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        switch(line_n)
        {
            case 0: 
            {
                break;
            }
            case 1: 
            {
                last_date = atoi(line);
                break;
            }
            case 2:
            {
                accumulated_sessions = atoi(line);
                break;
            }
            case 3:
            {
                total_sessions = atoi(line);
                break;
            }
            case 4:
            {
                current_total = atoi(line);
                break;
            }
            case 5:
            {   
                current_media = atoi(line);
                break;
            }
        }
        line_n++;
    }

    if((days - last_date) == 1)
        accumulated_sessions++;
    else if(days != last_date)
        accumulated_sessions = 1;
    
    total_sessions++;
    current_total += performance;
    current_media = current_total / total_sessions; 

    fprintf(file, 
        "%s\n%ld\n%ld\n%ld\n%ld\n%.2f", 
        current_date, last_date, total_sessions,
        accumulated_sessions, current_total, current_media
    );
    
    fclose(file);
}