#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

// fork исп для создания нового процесса (нужно для выполнения внешних команд)

int main()
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("Ошибка при создании дочернего процесса");
	}
	else if (pid == 0)
	{
		printf("Дочерний процесс: PID: %d, PPID: %d\n", getpid(), getppid());
	}
	else
		printf("Родительский процесс: PID = %d, дочерний PID = %d\n", getpid(), pid);
}