#include <stdio.h>

int test(int x)
{
	int p = 0;
	int y = 2;
	for (int i = 0; i < 100; i++)
	{
		p = p * 2;
		y = x + 3;
	}
	return p;
}

int main()
{
	int r = test(3);
	printf("result is: %d\n",r);
	return 0;
}
