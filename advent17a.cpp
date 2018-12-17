#include "advent.h"
#include <iomanip>

IntVec parseUInts(std::string s)
{
    IntVec v;
    auto k{s.find_first_of("0123456789")};
    while (k != string::npos)
    {
        s = s.substr(k);
        auto n{std::stoi(s, &k)};
        v.emplace_back(n);
        k = s.find_first_of("0123456789", k);
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


StrVec readMap(const std::string& fn)
{
    StrVec mp{" "};
    std::ifstream inf(fn);
    std::string s;

    auto w{mp[0].length()};
    auto h{mp.size()};

    while (std::getline(inf, s))
    {
        auto v{parseUInts(s)};
        if (v.size() < 3)
        {
            throw string("!!!");
        }
        cout << s << ": ";
        print(v);

        auto ww{v[0] + 1};
        auto hh{std::max(v[1] + 1, v[2] + 1)};

        if (s[0] == 'y')
        {
            std::swap(ww, hh);
        }
        if (hh > h)
        {
            h = hh;
            mp.resize(hh);
            w = 0;
        }
        if (ww > w)
        {
            w = ww;
            for (auto &z : mp)
            {
                z.resize(w, ' ');
            }
        }
        if (s[0] == 'y')
        {
            for (auto x = v[1]; x <= v[2]; ++x)
            {
                mp[v[0]][x] = '#';
            }
        }
        else
        {
            for (auto y = v[1]; y <= v[2]; ++y)
            {
                mp[y][v[0]] = '#';
            }
        }

    }
    return mp;
}


int main()
{
    auto v(parseUInts("123, 456..789"));
    cout << v[0] << v[1] << v[2] << endl;

    string s{"abc"};
    s.resize(60, '-');
    cout << s << endl;

    auto mp{readMap("input17.txt")};
    cout << mp[0].length() << "," << mp.size() << endl;

    //print(mp);

    for (auto i = 0; i < mp.size(); ++i)
    {
        cout << std::setw(5) << i << ":" << mp[i] << endl;
    }

    return 0;
}


