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

    std::map<std::pair<int, int>, int> g_m_asleep;

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

            if (wakes - falls <= 0)
            {
                return -g;
            }
            for (auto i = falls; i < wakes; ++i)
            {
                auto pr = std::make_pair(g, i);
                ++g_m_asleep[pr];
            }
        }
        else
        {

            std::cout << "falls " << m << std::endl;
            falls = m;
        }
        std::cout << s << std::endl;

    }

    for (auto&& pr : g_m_asleep)
    {
        std::cout << pr.first.first << " " << pr.first.second << ":" << pr.second << std::endl;
    }

    auto g_m = std::max_element(std::begin(g_m_asleep), std::end(g_m_asleep), 
    [](auto&& a, auto&& b){ return a.second < b.second; })->first;

    std::cout << "Max " << g_m.first << " " << g_m.second << std::endl;
    return g_m.first * g_m.second ;
    //return g;

}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
