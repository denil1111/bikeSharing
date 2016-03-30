#include "commenhead.h" 

// used for . Min cost perfect matching convert to maximum matching.
int M=100000;
int STATION_CAPACITY=10;
int VEHICLE_CAPACITY=30;
int POINT_RANGE=100;

char* itoa(long num, char* s, int len)
{
        long n, m = 16;
        int i = 16+2;
        int shift = 'a'- ('9'+1);


        if(!s || len < 1)
                return 0;

        n = num < 0 ? -1 : 1;
        n = n * num;

        len = len > i ? i : len;
        i = len < i ? len : i;

        s[i-1] = 0;
        i--;

        if(!num)
        {
                if(len < 2)
                        return &s[i];

                s[i-1]='0';
                return &s[i-1];
        }

        while(i && n)
        {
                s[i-1] = n % m + '0';

                if (s[i-1] > '9')
                        s[i-1] += shift ;

                n = n/m;
                i--;
        }

        if(num < 0)
        {
                if(i)
                {
                        s[i-1] = '-';
                        i--;
                }
        }

        return &s[i];
}
