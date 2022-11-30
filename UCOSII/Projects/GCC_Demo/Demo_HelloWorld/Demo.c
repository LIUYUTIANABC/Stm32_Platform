#include <stdio.h>
#include <stdlib.h>
#include "Test.h"
#include "Test_1.h"

int main(void)
{
    int a = N;
    int b = 2;
    int c = 0;
    int d= 0;

    printf("System Start!! \n");

    c = a + b;
    printf("%d\n", c);

    CODE

    #ifdef DEBUG_D
        printf("System Debug!! \n");
    #endif

    DoNothing();
    DoNothing_1();
    system("pause");
    return 0;
}
