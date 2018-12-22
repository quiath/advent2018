#include "advent.h"

using Int = long long;

auto print = [](const auto& v, bool nl = true)
{
    for (const auto& x : v)
    {
        cout << x << " ";
    }
    if (nl)
    {
        cout << endl;
    }
};



Int sim(Int r0, Int maxs)
{
    Int r[6]{r0, 0, 0, 0, 0, 0};

    for (Int i = 0; i < maxs; ++i)
    {
        r[2] = r[1] | (1 << 16);
        r[1] = 10605201;
        cout << "for ";
        print(r);
        while (true)
        {
            cout << "while " << endl;
            //r[1] = (((r[1] + r[2] & 0b11111111) & 16777215 ) * 65899) & 16777215;
            
            r[5] = r[2] & 255;
            print(r); 
            r[1] = r[1] + r[5];
            print(r); 
            r[1] = r[1] & 16777215;        

            print(r); 

            r[1] = r[1] * 65899; 
            print(r); 
            r[1] = r[1] & 16777215; 
            print(r); 
            
            if (r[2] < 256)
            {
                break;
            }
            r[2] = r[2] / 256;
        }
        cout << "break" << endl;
        if (r[1] == r[0])
        {
            return i;
        }
    }
    return maxs;
}

Int simmax(Int goal)
{
    std::vector<Int> nxt(16777216LL, -1LL);

    Int r[6]{goal, 0, 0, 0, 0, 0};

    Int i{0};
    Int cnt{0};
    Int prev{-1};
    
    while (true)
    {
        r[2] = r[1] | (1 << 16);
        r[1] = 10605201;
        //cout << "for ";
        //print(r);
        while (true)
        {
            //cout << "while " << endl;
            //r[1] = (((r[1] + r[2] & 0b11111111) & 16777215 ) * 65899) & 16777215;
            
            r[5] = r[2] & 255;
            //print(r); 
            r[1] = r[1] + r[5];
            //print(r); 
            r[1] = r[1] & 16777215;        

            //print(r); 

            r[1] = r[1] * 65899; 
            //print(r); 
            r[1] = r[1] & 16777215; 
            //print(r); 
            
            if (r[2] < 256)
            {
                break;
            }
            r[2] = r[2] / 256;
        }
        //cout << "break" << endl;
        //cout << r[1] << endl;

        if (i % 100000 == 0)
        {
            //cout << i << ": " << prev << " -> " << r[1] << endl;
        }

        if (prev != -1LL)
        {
            if (nxt[prev] != -1LL)
            {
                cout << "Was " << prev << " -> " << nxt[prev] << " is " <<
                    r[1] << " in step " << i << endl;
            }
            nxt[prev] = r[1];
        }

        /*if (r[1] == r[0])
        {
            ++cnt;
            //return i;
            if (cnt == 2)
            {
                cout << prev << " cnt=" << cnt << endl;
                return prev;
            }
        }*/
        prev = r[1];
        ++i;
    }
    return 0;
}

/*
Was 10598657 -> 7379282 is 7379282 in step 15601
Was 7379282 -> 13218917 is 13218917 in step 15602
Was 13218917 -> 4642009 is 4642009 in step 15603
Was 4642009 -> 16271679 is 16271679 in step 15604
Was 16271679 -> 7684420 is 7684420 in step 15605
Was 7684420 -> 4767215 is 4767215 in step 15606


Was 6628526 -> 12817064 is 12817064 in step 18178
   2579 Was 12817064 -> 14031230 is 14031230 in step 18179
   2580 Was 14031230 -> 313035 is 313035 in step 18180
-->2581 Was 313035 -> 10598657 is 10598657 in step 18181
   2582 Was 10598657 -> 7379282 is 7379282 in step 18182
   2583 Was 7379282 -> 13218917 is 13218917 in step 18183
   2584 Was 13218917 -> 4642009 is 4642009 in step 18184
*/


int main()
{
    //auto ret{sim(11592302, 2)};
    //auto ret{sim(0, 10)};
    //auto ret{simmax(11592302)};

    auto ret{sim(6628526, 40000)};
    cout << "s=" << ret << endl;


#if 0
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
#endif
    return 0;
}
