#include <stdio.h>
#include <stdlib.h>

// continue используется для того, чтобы пропустить оставшеюсю часть условий цикла и перейти к след циклу
// то есть, если код встречает continue, он немедленно перескакивает на начало след итерации цикла

// пропуск чисел, пример с continue 
void	pass_num(void)
{
	int i = 1;

	while (i < 6)
	{
		if (i == 3)
		{
			i++; // увеличиваем индекс для след итерации
			continue; // возвращаемся в начало след итерации
		}
		printf("i == %d\n", i);
		i++;
	}
}

// пропуск чисел, пример без continue 
void	pass_num2(void)
{
	int i = 1;
	
	while (i < 6)
	{
		if (i != 3)
		{
			printf("i == %d\n", i);
		}
		i++;
	}
}

int main()
{
	pass_num();
	printf("pass num2\n");
	pass_num2();
}