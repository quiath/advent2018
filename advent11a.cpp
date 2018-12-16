#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::get;
using IntVec = std::vector<int>;

struct Grid
{
    Grid(int n) : n_(n) 
    {
        v_.resize(n * n, 0);
    }
    int& operator()(int x, int y)
    {
        return v_[(x - 1) + n_ * (y - 1)];
    }
    int n_;
    IntVec v_;
};




auto task2(int n, int serial)
{
    auto rackID = [](auto x) { return x + 10; };
    auto powerLevel = [serial, &rackID](auto x, auto y) {
        auto p = rackID(x) * y;
        p += serial;
        p *= rackID(x);
        p = (p / 100) % 10;
        return p - 5;
    };

    auto g = Grid(n);
    auto sum = Grid(n);

    for (auto y = 1; y <= n; ++y)
    {
        for (auto x = 1; x <= n; ++x)
        {
            g(x, y) = powerLevel(x, y);
            
            sum(x, y) = g(x, y) + 
                        (x > 1 ? sum(x - 1, y) : 0) + 
                        (y > 1 ? sum(x, y - 1) : 0) + 
                        (x > 1 && y > 1 ? -sum(x - 1, y - 1) : 0);


            if (x < 5 && y < 5)
            {
                cout << x << "," << y << "=" << g(x, y) << " s=" << sum(x, y) << endl; 
            }

            
        }
    }
    

    auto max_p{ -10000 }, mx{0}, my{0}, mr{0};
    for (auto y = 1; y <= n; ++y)
    {
        for (auto x = 1; x <= n; ++x)
        {
            auto bound {std::min(n - x + 1, n - y + 1)};
            for (auto r = 1; r <= bound; ++r)
            {
                auto r0{r - 1};
                auto sqv = sum(x + r0, y + r0);
                sqv += (y > 1) ? -sum(x + r0, y - 1) : 0; 
                sqv += (x > 1) ? -sum(x - 1, y + r0) : 0; 
                sqv += (x > 1 && y > 1) ? sum(x - 1, y - 1) : 0; 

                //cout << x << "," << y << "," << r << "," << sqv << endl; 

                if (sqv > max_p)
                {
                    mx = x;
                    my = y;
                    mr = r;
                    max_p = sqv;
                }

            }
        }
    }

    return std::make_tuple(mx, my, mr);

}

int main()
{
    auto t = task2(300, 3628);
    std::cout << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << std::endl;
    return 0;
}
