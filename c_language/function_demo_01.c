#include<stdio.h>
#include"head.h"
int main()
{
    int a;
    printf("请输入一个整数：");
    scanf("%d",&a);
    if(judge(a))
    {
        printf("%d是素数\n",a);
    }else
    {
        printf("%d不是素数\n",a);
    }

    return 0;
}