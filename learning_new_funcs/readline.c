#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

// readline

int main()
{
    char *line;

    while (1)
	{
        line = readline("> ");
        if (strcmp(line, "replace") == 0)
		{
            rl_on_new_line();
            rl_replace_line("This is a replaced line", 0);
            rl_redisplay();
            free(line);
            continue;  // Пропускаем добавление строки в историю
        }
        if (strcmp(line, "clear") == 0)
            rl_clear_history();  // Очищаем историю команд
        add_history(line);  // Добавляем введенную команду в историю
        printf("Your command: %s\n", line);  // Выводим введенную команду
        free(line);  // Освобождаем память
    }

    return 0;
}
// -lreadline