#include "advent.h"

struct Grid
{
    Grid(int w, int h, int val = 0) : w_(w), h_(h) 
    {
        v_.resize(w * h, val);
    }
    int& operator()(int x, int y)
    {
        return v_[x + w_ * y];
    }
    const int& operator()(int x, int y) const
    {
        return v_[x + w_ * y];
    }


    void fill(int val)
    {
        std::fill(begin(v_), end(v_), val);
    }
    int w_;
    int h_;
    IntVec v_;
};


int task1(int depth, int tx, int ty)
{
    auto w{tx + 1}, h{ty + 1};
    Grid g(w, h);
    
    auto MOD{20183};
    auto MY0{16807};
    auto MX0{48271};

    auto vel = [&](int value)
    {
        return (value + depth) % MOD; 
    };

    auto rl{vel(0) % 3};

    for (auto x = 0; x < w; ++x)
    {
        g(x, 0) = vel(x * MY0);
        cout << x << "," << 0 << ":" << g(x, 0) << endl;
        rl += g(x, 0) % 3;
    }
    for (auto y = 0; y < h; ++y)
    {
        g(0, y) = vel(y * MX0);
        cout << 0 << "," << y << ":" << g(0, y) << endl;
        rl += g(0, y) % 3;
    }
    for (auto y = 1; y < h; ++y)
    {
        for (auto x = 1; x < w; ++x)
        {
            auto gimod{0};
            if (x != tx || y != ty)
            {
                gimod = g(x - 1, y) * g(x, y - 1);
            }
            g(x, y) = vel(gimod);

            cout << x << "," << y << ":" << g(x, y) << ";" << g(x,y) % 3 << endl;
            rl += g(x, y) % 3;
        }
    }
    return rl;

}

int main()
{
    int ret{task1(10689, 11, 722)};
    //ret = task1(510, 10, 10);
    
    cout << "r=" << ret << endl;
    return 0;
}
