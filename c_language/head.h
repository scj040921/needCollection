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