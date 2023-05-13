#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() 
{
	int i, j, k , l = 0;
	i = 1;		// dead store
	j = i + 1; 	// dead store
	k = j + i;
	l = l + j;
	return 2;
}


