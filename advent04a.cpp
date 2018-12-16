#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

auto s2 = [](const auto& s, auto& x, auto& y)
{
    size_t i{0};
    x = std::stoi(s, &i);  
    y = std::stoi(s.substr(i + 1), nullptr);
};

int task1()
{
    std::ifstream inf("input4.txt");
    std::string s;
    std::vector<std::string> v;

    while (std::getline(inf, s))
    {
        v.emplace_back(s);
    }

    std::sort(v.begin(), v.end());

    auto g{0}, falls{0}, wakes{0};

    std::map<int, int> sleep;
    std::vector<int> minutes(60, 0);

    const int G = 727;
    const int M = 43;

    for (auto& s : v)
    {
        auto p = s.find("#");
        if (p != std::string::npos)
        {
            g = std::stoi(s.substr(p + 1), nullptr);
            std::cout << g << std::endl;
            continue;
        } 
        p = s.find(":"); 
        auto m = std::stoi(s.substr(p + 1), nullptr);
        p = s.find("wakes"); 
        if ( p !=std::string::npos) 
        {
            std::cout << "wakes " << m << std::endl;
            wakes = m;

            sleep[g] += (wakes - falls);
            if (wakes - falls <= 0)
            {
                return -g;
            }
            if (g == G)
            {
                for (auto i = falls; i < wakes; ++i)
                {
                    ++minutes[i];
                }
            }
        }
        else
        {

            std::cout << "falls " << m << std::endl;
            falls = m;
        }
        std::cout << s << std::endl;

    }

    for (auto&& pr : sleep)
    {
        std::cout << pr.first << " " << pr.second << std::endl;
    }

    g = std::max_element(std::begin(sleep), std::end(sleep), 
    [](auto&& a, auto&& b){ return a.second < b.second; })->first;

    auto maxit = std::max_element(std::begin(minutes), std::end(minutes));

    return std::distance(std::begin(minutes), maxit) * G;
    //return g;

}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
