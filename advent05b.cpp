#include <iostream>
#include <fstream>
#include <list>
#include <cctype>
#include <algorithm>

int task1(char remove_ch = ' ')
{
    std::ifstream inf("input5.txt");
    std::string s;
    inf >> s;
    std::cout << "'" << s << "'" << std::endl;

    std::list<char> lst;

    lst.emplace_back('<');
    for (auto&& ch : s)
    {
        if (std::tolower(ch) != remove_ch)
        {
            lst.emplace_back(ch);
        }
    }
    lst.emplace_back('>');

    auto p = std::begin(lst);
    auto q{ p };
    ++q;
    while (q != lst.end())
    {
        if ((std::islower(*p) && std::isupper(*q) && std::toupper(*p) == *q) ||
            (std::isupper(*p) && std::islower(*q) && std::tolower(*p) == *q))
        {
            auto z{p};
            --p;
            ++q;
            lst.erase(z, q);
        }
        else
        {
            ++p;
            ++q;
        }
    }

    return lst.size() - 2;
}

int task2()
{
    auto min_res { 50000 };
    for (auto ch = 'a'; ch <= 'z'; ++ch)
    {
       min_res = std::min(min_res, task1(ch)); 
    }
    return min_res;
}

int main()
{
    std::cout << task2() << std::endl;
}
