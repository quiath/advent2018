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

int task2(const std::string& filename, int MAX)
{
    std::ifstream inf(filename);
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

    auto cx { (xmin + xmax) / 2 }, cy { (ymin + ymax) / 2};

    auto cnt{0};
    auto prevcnt{-1};

    auto dist = [](auto x, auto y, auto p) {
        return std::abs(x - p.first) + std::abs(y - p.second);
    };

    auto bounded_dist_sum = [&v, MAX, &dist](auto x, auto y)
    {
        auto sum{0};
        for (auto p : v)
        {
            sum += dist(x, y, p);
            if (sum >= MAX)
            {
                //std::cout << x << "," << y << "=" << sum << std::endl;
                break;
            }
        }
        //std::cout << x << "," << y << "=" << sum << std::endl;
        return sum;
    };

    auto rad{1};

    while (prevcnt != cnt)
    {
        prevcnt = cnt;
        auto x{cx - rad + 1};
        auto y{cy - rad + 1};
        auto b{2 * rad - 1};

        int DIR[] {  0, 1, 0, -1, 0 };

        for (auto j = 0; j < 4; ++j)
        {
            auto dx{ DIR[j]}, dy{DIR[j + 1]};
            for (auto i = 0; i < b; ++i)
            {
                if (bounded_dist_sum(x, y) < MAX)
                {
                    ++cnt;
                }

                x += dx;
                y += dy;

            }
        }
        ++rad;

    }
    std::cout << xmin << " : " << xmax << "\n";
    std::cout << ymin << " : " << ymax << "\n";
    std::cout << cx << ", " << cy << "\n";
    std::cout << "count:" << cnt << "\n";

    return cnt;
}

int main()
{
    //std::cout << task2("example6.txt", 32) << std::endl;
    std::cout << task2("input6.txt", 10000) << std::endl;
    // "input6.txt"
    return 0;
}
