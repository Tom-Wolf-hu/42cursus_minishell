#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

void	check_access_file(char *filename)
{
	if (access(filename, F_OK) != -1)
		printf("Файл существует\n");
	if (access(filename, R_OK) != -1)
		printf("Файл доступен для чтения\n");
	if (access(filename, W_OK) != -1)
		printf("Файл доступен для записи\n");
	if (access(filename, X_OK) != -1)
		printf("Файл доступен для выполнения\n");
}

void	my_getcwd(void)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("Ошибка при получении тек рабочего каталога\n");
}

void	my_chdir(void)
{
	if(chdir("/tmp") == 0)
		printf("Success changed to /tmp\n");
	else
		perror("Error while changing the dir\n");
}

void	stat_operations(void)
{
	struct stat file_info;
	char *filepath = "/dev/sda5";
	if (stat(filepath, &file_info) == -1)
	{
		perror("Ошибка");
		return ;
	}
	printf("PATH: %s\n", filepath);

	// Проверяем тип файла
	if (S_ISREG(file_info.st_mode))
		printf("Тип файла: обычный файл\n");
	else if (S_ISDIR(file_info. st_mode))
		printf("Тип файла: каталог\n");
	else if (S_ISLNK(file_info. st_mode))
		printf("Тип файла: символ ссылка\n");
	else if (S_ISCHR(file_info. st_mode))
		printf("Тип файла: символьное устройство\n");
	else if (S_ISBLK(file_info. st_mode))
		printf("Тип файла: блочное устройство\n");
	else if (S_ISFIFO(file_info. st_mode))
		printf("Тип файла: канал (FILO)\n");
	else if (S_ISSOCK(file_info. st_mode))
		printf("Тип файла: сокет\n");
	else
		printf("Тип файла: неизвестный тип\n");
}

void	lstat_operations(void) // работает аналогичено stat, но если указанный путь - символ ссылка, то lstat
// возвращает инфо о самой символ ссылке, а не о файле, на который она указывает
{
	// остальная работа аналогична stat
}

void	fstat_operations(void)
{
	struct stat file_info;

	int fd = open("/home/alex/42_learning/minishell/learning_new_funcs/posix.c", O_RDONLY);
	if (fd == -1)
	{
		perror("Ошибка1");
		return;
	}
	// получаем инфо о файле через fstat
	if (fstat(fd, &file_info) == -1)
	{
		perror("Ошибка");
		close(fd);
		return;
	}
	// выводим инфо о файле (эту же информацию можно получить с помощью fstat and lstat)
	printf("Инфо о файле (дескриптор %d):\n", fd);
	printf("The size of the file: %ld bytes\n", file_info.st_size);
	printf("Права доступа: %o\n", file_info.st_mode & 0777);
	printf("К-во ссылок: %ld\n", file_info.st_nlink);
	printf("Владелец (UID): %d\n", file_info.st_uid);
	printf("Группа (GID): %d\n", file_info.st_gid);

	if (S_ISREG(file_info.st_mode))
		printf("Тип файла: обычный файл\n");
	else if (S_ISDIR(file_info. st_mode))
		printf("Тип файла: каталог\n");
	else if (S_ISLNK(file_info. st_mode))
		printf("Тип файла: символ ссылка\n");
	else if (S_ISCHR(file_info. st_mode))
		printf("Тип файла: символьное устройство\n");
	else if (S_ISBLK(file_info. st_mode))
		printf("Тип файла: блочное устройство\n");
	else if (S_ISFIFO(file_info. st_mode))
		printf("Тип файла: канал (FILO)\n");
	else if (S_ISSOCK(file_info. st_mode))
		printf("Тип файла: сокет\n");
	else
		printf("Тип файла: неизвестный тип\n");

	close(fd);
}

void	unlink_actions(void)
{
	const char *filepath = "/home/alex/42_learning/minishell/learning_new_funcs/cat";

	if (unlink(filepath) == -1)
	{
		perror("Error to delete the file");
		return ;
	}
	printf("The file '%s' has succed removed\n");
}

void	dir_actions(void)
{
	// opendir исп для открытия каталога для дальнейшего чтения, она возвращает указатель на объект типа DIR, который будет исп для получения содерж
	// readdir исп для получения содерж в этой директории. Каждый вызов readdir возвращает структуру типа struct dirent, которая содержит инфо о файле 
//и подкаталоге, найденном в данном каталоге
	// closedir исп для закрытия каталога, который был открыт с помощью opendir

	const char *dir_path = "."; // тек каталог
	DIR *dir = opendir(dir_path);
	if (dir == NULL)
	{
		perror("Error");
		return ;
	}
	struct dirent *entry;

	// читаем содерж каталога
	printf("Содержимое каталога %s\n", dir_path);
	while ((entry = readdir(dir)) != NULL)
	{
		printf("%s\n", entry->d_name); // выводим имя файла или подкаталога
	}
	if (closedir(dir) == -1)
	{
		perror("Error while closing the dir");
		return ;
	}
}

int main()
{
	check_access_file("continue.c");
	my_getcwd(); // получение тек рабочего каталога
	// my_chdir(); // смена каталога на /tmp (программа меняет каталог, но, при завершении возвращается обратно)
	stat_operations(); // получить данные о файле по его пути
	fstat_operations(); // получить данные о файле по его fd
	unlink_actions(); // исп для удаления файлов в файловой системе
	dir_actions();
}

// правило для всех ф-й:
/* 
 0 - в случае успеха
-1 - в случае ошибки 
*/

/* access
F_OK — проверяет, существует ли файл.
R_OK — проверяет, можно ли читать файл.
W_OK — проверяет, можно ли записывать в файл.
X_OK — проверяет, можно ли выполнить файл.
*/