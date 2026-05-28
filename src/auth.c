#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool auth(char* user)
{
    char path[64];
    snprintf(path, sizeof(path), "user_register/%s.txt", user);
    
    FILE *file = fopen(path, "r");
    if(file == NULL)
    {
        file = fopen(path, "w");
        
        if(file == NULL)
        {
            printf("Erro inicializando sessão. Tente novamente\n");
            return false;
        }

        char password[128];
        printf("Cadastre uma senha: ");
        scanf("%127s", &password);
        
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)password, strlen(password), hash);

        char hex[SHA256_DIGEST_LENGTH * 2 + 1];

        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            sprintf(hex + (i * 2), "%02x", hash[i]);
        }

        hex[64] = '\0';

        fprintf(file, "%s\n", hex);

        fclose(file);

        return true;
    }
    
    char senha[256];

    printf("Senha: ");
    scanf("%127s", &senha);
    printf("\n");

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)senha, strlen(senha), hash);
    char hex[SHA256_DIGEST_LENGTH * 2 + 1];

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hex + (i * 2), "%02x", hash[i]);
    }
    hex[64] = '\0';

    char line[512];
    char last_line[512] = "";
    while(fgets(line, sizeof(line), file) != NULL)
    {
        strcpy(last_line, line);
    }

    last_line[strcspn(last_line, "\n")] = '\0';
    fclose(file);
    
    return strcmp(hex, last_line) == 0;
}
