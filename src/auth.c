#include "auth.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_safe(char* password);

bool auth(char* user)
{
    char path[64];
    snprintf(path, sizeof(path), "user_registers/%s.txt", user);
    
    FILE *file = fopen(path, "r");
    if(file == NULL)
    {
        file = fopen(path, "w");
        
        if(file == NULL)
        {
            printf("Erro inicializando sessão. Tente novamente\n");
            return false;
        }

        char password[64];
        printf("Cadastre uma senha: ");    
        scanf("%127s", &password);
            
        while(!is_safe(password))
        {
            printf(
                "Senha muito fraca. A senha deve:\n"
                "Ter pelo menos 8 caracteres\n"
                "Ter digitos, letras e caracteres especiais\n"
            );
            printf("Cadastre uma senha: ");    
            scanf("%127s", &password);
        }

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
    
    char senha[32];

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

    char line[65];
    if(fgets(line, sizeof(line), file) == NULL)
    {
        printf("Erro lendo arquivo\n");
        fclose(file);
        return false;
    }

    fclose(file);
    return strcmp(hex, line) == 0;
}

bool is_safe(char* password)
{   
    if(strlen(password) < 8) return false;

    bool has_digit = false;
    bool has_alpha = false;
    bool has_special = false;

    for(int i = 0; password[i] != '\0'; i++)
    {
        if(isdigit(password[i])) has_digit = true;
        else if(isalpha(password[i])) has_alpha = true;
        else if(ispunct(password[i])) has_special = true;
    }

    return has_digit && has_alpha && has_special;
}
    
