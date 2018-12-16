#include "advent.h"
#include <array>



string task1(int from, int len)
{

    IntVec v{3, 7};

    v.reserve(from + len);

    auto p{0u};
    auto q{1u};

    while (v.size() < from + len)
    {
        auto n{v[p] + v[q]};
        auto d1{n / 10};
        auto d0{n % 10};
        if (d1 > 0)
        {
            v.emplace_back(d1);
        }
        v.emplace_back(d0);
        p = (p + 1 + v[p]) % v.size();
        q = (q + 1 + v[q]) % v.size();

    }


    std::ostringstream ss;
    for (auto i = 0; i < len; ++i)
    {

        ss << std::to_string(v[from + i]);
    }
    return ss.str();
};

int main()
{
    auto ret = task1(74501, 10);
    //auto ret = task1(18, 10);
    
    //auto ret = task1("example13.txt");
    cout << ret << endl;
    return 0;
}
