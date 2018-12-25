#include "advent.h"
#include <iomanip>
#include <cstdlib>
#include <set>

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


    auto get_dist = [&v](int i, int j)
    {
        return abs(v[i] - v[j]) + abs(v[i + 1] - v[j + 1]) + abs(v[i + 2] - v[j + 2]) + abs(v[i + 3] - v[j + 3]);
    };
    auto get_disti = [&v, &get_dist](int i, int j)
    {
        return get_dist(i * 4, j * 4);
    };

    std::vector<int> con(n);
    for (auto i = 0; i < n; ++i)
    {
        con[i] = i;
    }

#ifdef TOTAL_BRUTE_FORCE_WITH_A_SUBTLE_BUG
    // produced the correct solution but the algorithm has a bug
    // -O3 is your friend
    for (auto z = 0; z < n; ++z)
    {
        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < n; ++j)
            {
                if (get_disti(i, j) <= 3 && con[i] != con[j])
                {
                    for (auto k = 0; k < n; ++k)
                    {
                        if (con[k] == con[j])
                        {
                            con[k] = con[i];
                        }
                    }
                }
            }
        }
    }

    std::set<int> uniq;
    for (auto i = 0; i < n; ++i)
    {
        uniq.insert(con[i]);
    }
#else
    // slightly smarter brute force
    std::set<int> uniq;
    for (auto i = 0; i < n; ++i)
    {
        uniq.insert(i);
    }
   
    // I guess it can be proven that the loop needs to be executed only once
    auto before{0};
    do
    {
        before = uniq.size();

        for (auto i = 0; i < n; ++i)
        {
            for (auto j = 0; j < n; ++j)
            {
                if (get_disti(i, j) <= 3 && con[i] != con[j])
                {
                    auto rem{con[j]};
                    replace(begin(con), end(con), rem, con[i]);
                    uniq.erase(rem);
                }
            }
        }
        cout << before << " " << uniq.size() << endl;
    }
    while (uniq.size() != before);
    
#endif

    return uniq.size();
}

int main()
{
    auto ret{task1("input25.txt")};
    cout << "r=" << ret << endl;
    return 0;
}


