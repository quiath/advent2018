#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> read()
{
    std::ifstream inf("input2.txt");
    std::string x;
    std::vector<std::string> v;
    while (inf >> x)
    {
        v.emplace_back(x);
        //std::cout << ">" << x << "<" << std::endl;
    }
    return v;
}

std::string task()
{
    auto v = read();
    for (auto i = 0L; i < v.size(); ++i)
    {
        for (auto j = i + 1; j < v.size(); ++j)
        {
            std::vector<int> diff;
            std::transform(std::begin(v[i]), std::end(v[i]), std::begin(v[j]), std::back_inserter(diff), 
                [](auto a, auto b){ return int(a != b); });
            auto cnt = std::count(std::begin(diff), std::end(diff), 1);
            if (cnt == 1)
            {
                auto it = std::find(std::begin(diff), std::end(diff), 1);
                auto k = std::distance(std::begin(diff), it);
                std::string s;
                s.reserve(v[i].length() - 1);
                std::remove_copy_if(std::begin(v[i]), std::end(v[i]), std::back_inserter(s), 
                    [&k](auto x){ bool ret = (k == 0); --k; return ret; });
                return s;

            }
        }
    }
    return "";
}


int main()
{
    std::cout << task() << std::endl;
}
