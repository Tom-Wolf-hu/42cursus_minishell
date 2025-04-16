#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *remove_redirects(char *cmd)
{
    char *clean_cmd;
    int start;
    int end;
    int i;
    int j = 0;

    start = 0;
    while (cmd[start] && cmd[start] != '>' && cmd[start] != '<')
        start++;  // Находим первый символ редиректа

    if (!cmd[start])  // Если редиректов нет, просто копируем строку
        return strdup(cmd);

    end = start;
    while (cmd[end] && (cmd[end] == '>' || cmd[end] == '<' || isspace(cmd[end])))
        end++;  // Пропускаем пробелы и сам редирект

    while (cmd[end] && !isspace(cmd[end]))  // Пропускаем имя файла
        end++;

    // printf("start: %d; end: %d\n", start, end);

    i = 0;
    while (cmd[i] && i < start)  // Копируем всё до редиректа
        i++;

    while (cmd[end] && isspace(cmd[end]))  // Пропускаем лишние пробелы после имени файла
        end++;

    clean_cmd = malloc(strlen(cmd) - (end - start) + 1);
    if (!clean_cmd)
        return NULL;

    j = 0;
    i = 0;

    while (cmd[i] && i < start)  // Копируем всё до редиректа
        clean_cmd[j++] = cmd[i++];

    while (cmd[end])  // Копируем остаток команды после редиректа
        clean_cmd[j++] = cmd[end++];

    clean_cmd[j] = '\0';

    return clean_cmd;
}


int main()
{
	char *str = remove_redirects("<<file wc -l");
	char *str2 = remove_redirects("echo hello>file");

	if (str)
		printf("%s\n", str);
	if (str2)
		printf("%s\n", str2);
}