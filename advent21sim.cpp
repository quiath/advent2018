#include "advent.h"

using Int = long long;

Int sim(Int r0, Int maxs)
{
    Int r[6]{r0, 0, 0, 0, 0, 0};

    for (Int i = 0; i < maxs; ++i)
    {
        r[2] = r[1] | (1 << 16);
        r[1] = 10605201;
        while (true)
        {
            //r[1] = (((r[1] + r[2] & 0b11111111) & 16777215 ) * 65899) & 16777215;
            
            r[5] = r[2] & 255;
            r[1] = r[1] + r[5];
            r[1] = r[1] & 16777215;        
            r[1] = r[1] * 65899; 
            r[1] = r[1] & 16777215; 
            
            if (r[2] < 256)
            {
                break;
            }
            r[2] = r[2] / 256;
        }
        if (r[1] == r[0])
        {
            return i;
        }
    }
    return maxs;
}

int main()
{
    Int N{50};
    Int bests{20000000LL}, bestv{0};
    for (Int j = 0; j < 20000000LL; ++j)
    {
        auto steps{sim(j, N)};
        if (steps < N)
        {
            cout << j << " s=" << steps << " " << 
               bestv << " bs=" << bests << endl;
            if (steps < bests)
            {
                bests = steps;
                bestv = j;
            }
        }
    }
    return 0;
}
