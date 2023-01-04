#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TCB 的定义
typedef struct tcb
{
    char *code_name;
    int p;
    int v_num;
    void (*fun)(void);
} TCB;

//----------------------------------------------------------------------------------
// Function name:
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void fun_1(void)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("function_1 run \n");
    }
}

//----------------------------------------------------------------------------------
// Function name:
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void fun_2(void)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("function_2 run \n");
    }
}
//----------------------------------------------------------------------------------
// Function name:
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
void fun_3(void)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("function_3 run \n");
    }
}

//----------------------------------------------------------------------------------
// Function name:
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
TCB CreateTCB(char *name, int pp, int num, void (*f)())
{
    TCB tcb;
    tcb.code_name = name;
    tcb.p = pp;
    tcb.v_num = num;
    tcb.fun = f;
    return tcb;
}

//----------------------------------------------------------------------------------
// Function name:
// Input:
// Output:
// Comment:
//----------------------------------------------------------------------------------
int main(void)
{
    char code_name[10];
    int t, i;
    TCB tabTb1[3];

    tabTb1[0] = CreateTCB("F1", 2, 1, fun_1);
    tabTb1[1] = CreateTCB("F2", 2, 4, fun_2);
    tabTb1[2] = CreateTCB("F3", 2, 5, fun_3);

    printf("Input CodeName:  \n");
    scanf("%s", code_name);
    t = 0;

    for (i = 0; i < 3; i++)
    {
        if (strcmp(tabTb1[i].code_name, code_name) == 0)
        {
            tabTb1[i].fun();
            t = 1;
        }
        if (i == 2 && t == 0)
        {
            printf("No %s function! \n", code_name);
        }
    }

    return 1;
}
