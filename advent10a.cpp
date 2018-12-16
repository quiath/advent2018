#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using IntVec = std::vector<int>;

auto get_pair_from = [](const auto& s, auto& x, auto& y)
{
    size_t i{0};
    x = std::stoi(s, &i);  
    y = std::stoi(s.substr(i + 1), nullptr);
};

int task1()
{
    std::ifstream inf("input10.txt");
    string s;
    IntVec vx, vy, vdx, vdy;

    auto emp = [](auto& v1, auto& v2, auto a1, auto a2)
    {
        v1.emplace_back(a1);
        v2.emplace_back(a2);
    };

    while (std::getline(inf, s))
    {
        auto x{0}, y{0}, dx{0}, dy{0};
        auto i = s.find('<');
        auto ss = s.substr(i + 1);
        get_pair_from(ss, x, y);
        i = ss.find('<');
        get_pair_from(ss.substr(i + 1), dx, dy);
        emp(vx, vy, x, y);
        emp(vdx, vdy, dx, dy);

        //cout << vx.back() << "," << vy.back() << " " << vdx.back() << "," << vdy.back() << endl;
    }

    auto n = vx.size();

    auto step = [](auto& p, auto d) { p += d; };

   
    auto compute_size = [&vx, &vy]()
    {
        auto hor = std::minmax_element(begin(vx), end(vx));
        auto ver = std::minmax_element(begin(vy), end(vy));
        auto sizex {*hor.second - *hor.first};
        auto sizey {*ver.second - *ver.first};
        return std::make_pair(sizex, sizey);
    };

    auto update = [&vx, &vy, &vdx, &vdy, n, &step](auto sign)
    {
        for (auto j = 0; j < n; ++j)
        {
            step(vx[j], sign * vdx[j]);
            step(vy[j], sign * vdy[j]);
        }
    };

    auto size = compute_size();

    std::cout << "start " <<  size.first << "," << size.second << endl;
    
    auto i{0};

    while (true)
    {
        
        update(1);

        auto new_size = compute_size();

        //10417

        if (new_size.first > size.first || new_size.second > size.second)
        {
            update(-1);
            std::cout <<  "break " << i << " " <<  size.first << "," << size.second << endl;

            //size = new_size;
            //std::cout <<  "break " << i << " " <<  size.first << "," << size.second << endl;

            std::vector<string> w(size.second + 1);
            for (auto& s : w)
            {
                s = std::move(string(size.first + 1, ' '));
            }

            auto xmin { *std::min_element(begin(vx), end(vx)) };
            auto ymin { *std::min_element(begin(vy), end(vy)) };

            cout << xmin << endl;
            cout << ymin << endl;


            for (auto j = 0; j < n; ++j)
            {
                auto x{vx[j] - xmin};
                auto y{vy[j] - ymin};
                cout << x << "," << y << endl;
                w[y][x] = '*';
            }

            for (const auto& s : w)
            {
                std::cout << s << endl;
            }

            return i;
        }

        size = new_size;
        ++i;

        //std::cout << i << " " <<  size.first << "," << size.second << endl;
        //cout << sizex << "," << sizey << endl;
    }

    return n;

}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
