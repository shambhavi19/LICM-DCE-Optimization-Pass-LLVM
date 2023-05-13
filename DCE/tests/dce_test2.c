#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 200
#define NUMBER '0'
#define MAX_ITER 100

int main() {
        char type = '0';
        double op2;
        int a, b, c, d, e, f, g, h;
        bool i, j, k = true;
        char s[MAX];
	static int loop_exit = 0;
        d = 0;
        b = 1;
        c = b - d;
        do 
        {
                switch (type) 
                {
                case NUMBER:
                        a = 1;
                        b = 2;
                        c = 25;
                        if (k) 
                        {
                                c = a + b;
                                e = c - a;
                        } 
                        else 
                        {
                                d = 8;
                                h = 6;
                                f = 24;
                                e = f + h;
                                if (e) 
                                {
                                        e -= 1;
                                }
                        }
                        break;
                case '+':
                        break;
                case '*':
                        f = c - d;
                        break;
                case '-':
                        break;
                case '/':
                        break;
                case '\n':
                        break;
                default:
                        break;
                }
		loop_exit++;
        } while (a && (loop_exit < MAX_ITER));

        e = d + f;

        return 0;
}

