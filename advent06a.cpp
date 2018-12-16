#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <set>

using IntPair = std::pair<int, int>;
using IPV = std::vector<IntPair>;
using std::begin;
using std::end;

auto r2 = [](const auto& s, int skip = 1)
{
    size_t i{0};
    return std::make_pair(std::stoi(s, &i), std::stoi(s.substr(i + skip), nullptr));
};

int task1()
{
    std::ifstream inf("input6.txt");
    std::string s;
    IPV v;

    while (std::getline(inf, s))
    {
        v.emplace_back(std::move(r2(s)));
    }

    auto minmax_x = std::minmax_element(begin(v), end(v), 
        [](auto p, auto q) { return p.first < q.first; });

    auto minmax_y = std::minmax_element(begin(v), end(v), 
        [](auto p, auto q) { return p.second < q.second; });

    auto xmin { minmax_x.first->first }, xmax { minmax_x.second->first };
    auto ymin { minmax_y.first->second }, ymax { minmax_y.second->second };
    auto w { xmax - xmin + 1 };
    auto h { ymax - ymin + 1 };
    auto border { std::max(w, h) };
    auto x0 { xmin - border }, x1 { xmax + border }; 
    auto y0 { ymin - border }, y1 { ymax + border };

    std::set<int> on_border;
    std::map<int, int> area;

    auto dist = [](auto x, auto y, auto p) {
        return std::abs(x - p.first) + std::abs(y - p.second);
    };

    for (auto y = y0; y <= y1; ++y)
    {
        for (auto x = x0; x <= x1; ++x)
        {
            int min_dist{ 3 * border };
            int cnt{ 0 };
            int closest{ -1 };

            for (auto i = 0; i < v.size(); ++i)
            {
                auto d = dist(x, y, v[i]); 
                if (d < min_dist) 
                {
                    cnt = 1;
                    min_dist = d;
                    closest = i;
                }
                else if (d == min_dist)
                {
                    cnt++;
                    closest = -1;
                }
            }
            if (closest >= 0 && cnt == 1)
            {
                if (x == x0 || x == x1 || y == y0 || y == y1)
                {
                    on_border.insert(closest);
                } 
                else
                {
                    ++area[closest];
                }
            }

        }
    }

    for (auto ob : on_border)
    {
        auto it = area.find(ob);
        if (it != area.end())
        {
            area.erase(it);
        }
    }

    auto itmax = std::max_element(begin(area), end(area), 
      [](auto p, auto q) { return p.second < q.second; });
    

    std::cout << xmin << " : " << xmax << "\n";
    std::cout << ymin << " : " << ymax << "\n";
    std::cout << itmax->first << " has area " << itmax->second << "\n";

    return itmax->second;
}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
