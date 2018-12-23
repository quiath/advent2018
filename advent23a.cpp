#include "advent.h"
#include <iomanip>
#include <cstdlib>

using std::abs;
using LInt = long long int;
using LIntVec = std::vector<LInt>;

LIntVec parseLInt(std::string s)
{
    LIntVec v;
    auto k{s.find_first_of("0123456789-")};
    while (k != string::npos)
    {
        s = s.substr(k);
        auto n{std::stoll(s, &k)};
        v.emplace_back(n);
        k = s.find_first_of("0123456789-", k);
    }
    return v;
}

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

int task1(string fn)
{
    std::ifstream ifs(fn);
    //std::ifstream ifs("i20.txt");
    string s((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());


    auto v{parseLInt(s)};
    auto n{v.size() / 4};
    cout << v.size() << ":" << n<< endl;

    auto maxs{0};
    auto maxid{0};

    

    for (auto i = 0; i < n; ++i)
    {
        if (v[i * 4 + 3] > maxs)
        {
            maxs = v[i * 4 + 3];
            maxid = i;
        }
    }

    auto get_dist = [&v](int i, int j)
    {
        return abs(v[i] - v[j]) + abs(v[i + 1] - v[j + 1]) + abs(v[i + 2] - v[j + 2]);
    };

    auto cnt{0};
    for (auto i = 0; i < n; ++i)
    {
        auto d{get_dist(i * 4, maxid * 4)};
        if (d <= maxs)
        {
            ++cnt;
        }

    }
 
    return cnt;

}

int main()
{
    auto ret{task1("input23.txt")};
    cout << "r=" << ret << endl;
    return 0;
}


