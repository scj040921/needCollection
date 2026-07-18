#include<stdio.h>
#include<stdbool.h>
bool judge(int a)
{
     if(a==2)
        {
            return true;
        }
        if(a==1)
        {
            return false;
        }
    for(int i=2;i*i<=a;i++)
    {
        if(a%i==0)
        {
            return false;
        }
    }
    return true;
}
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