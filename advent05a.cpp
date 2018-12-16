#include <iostream>
#include <fstream>
#include <list>
#include <cctype>

int task1()
{
    std::ifstream inf("input5.txt");
    std::string s;
    inf >> s;
    std::cout << "'" << s << "'" << std::endl;

    std::list<char> lst;

    lst.emplace_back('<');
    for (auto&& ch : s)
    {
        lst.emplace_back(ch);
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

int main()
{
    std::cout << task1() << std::endl;
}
