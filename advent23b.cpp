#include "advent.h"
#include <iomanip>
#include <cstdlib>
#include <tuple>

using std::abs;
using std::get;
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

LInt dist3(LInt x, LInt y, LInt z, LInt x1, LInt y1, LInt z1)
{
    return abs(x1 - x) + abs(y1 -y) + abs(z1 - z);
}

LInt count(LInt sx, LInt sy, LInt sz, const LIntVec& v)
{
    auto n{v.size() / 4};
    auto cnt{0};
    for (auto i = 0; i < n; ++i)
    {
        auto d{dist3(sx, sy, sz, v[i * 4], v[i * 4 + 1], v[i * 4 + 2])};
        cnt += int(d <= v[i * 4 + 3]);
    }
    return cnt;

}

struct Finder
{
    Finder(const LIntVec& v/*, LInt sx, LInt sy, LInt sz*/)
    : v_(v)
    , n_(v.size() / 4)
    {
    
    }

    const LIntVec& v_;
    LIntVec n_;

    std::pair<LInt, LInt> ray(LInt x0, LInt y0, LInt z0, LInt dx, LInt dy, LInt dz, LInt cnt0) const
    {
        auto i{0LL};
        auto cnt{0LL};
        LInt x{x0}, y{y0}, z{z0};
        do 
        {
            ++i;
            x += dx; y += dy; z += dz;
            cnt = count(x, y, z, v_);
        }
        while (cnt == cnt0);
        return std::make_pair(i, cnt);
    }

    int search(LInt sx, LInt sy, LInt sz)
    {
        while (true) 
        {
            auto cnt0{count(sx, sy, sz, v_)};
            LInt bx{sx}, by{sy}, bz{sz};

            std::vector<std::tuple<LInt, LInt, LInt, LInt, LInt>> found;

            for (auto dx = -1LL; dx <= 1LL; ++dx)
            {
                for (auto dy = -1LL; dy <= 1LL; ++dy)
                {
                    for (auto dz = -1LL; dz <= 1LL; ++dz)
                    {
                        if (dz != 0 || dz != 0 || dx != 0)
                        {
                            auto len_cnt{ray(sx, sy, sz, dx, dy, dz, cnt0)};
                            if (len_cnt.second > cnt0)
                            {
                                cout << "cnt=" << len_cnt.first << " steps=" << len_cnt.first << " dxdyzz:" <<
                                  dx << "," << dy << "," << dz << endl;
                                found.emplace_back(std::make_tuple(-len_cnt.second,
                                    len_cnt.first, 
                                    sx + dx * len_cnt.first,
                                    sy + dy * len_cnt.first,
                                    sz + dz * len_cnt.first
                                    ));

                            }
                        }
                    }
                }

            }
            if (found.size() == 0)
            {
                cout << abs(sx) + abs(sy) + abs(sz) << endl;
                return abs(sx) + abs(sy) + abs(sz);
            }
            auto it = std::min_element(begin(found), end(found));
            
            cout << "next";
            
            cout << " " << get<0>(*it); 
            cout << " " << get<1>(*it); 
            cout << " " << get<2>(*it); 
            cout << " " << get<3>(*it); 
            cout << " " << get<4>(*it); 
            cout << endl;

            sx = get<2>(*it);
            sy = get<3>(*it);
            sz = get<4>(*it);

        }
        return 0;
    }
};


int task2(string fn)
{
    std::ifstream ifs(fn);
    //std::ifstream ifs("i20.txt");
    string s((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());


    auto v{parseLInt(s)};
    auto n{v.size() / 4};
    cout << v.size() << ":" << n<< endl;

    auto maxs{0};
    auto maxid{0};

    

    for (auto i = 0; i < n; ++i)
    {
        if (v[i * 4 + 3] > maxs)
        {
            maxs = v[i * 4 + 3];
            maxid = i;
        }
    }

    auto get_dist = [&v](int i, int j)
    {
        i = i * 4;
        j = j * 4;
        return abs(v[i] - v[j]) + abs(v[i + 1] - v[j + 1]) + abs(v[i + 2] - v[j + 2]);
    };
    auto get_dist3 = [&v](auto x, auto y, auto z, int i)
    {
        i = i * 4;
        return abs(v[i] - x) + abs(v[i + 1] - y) + abs(v[i + 2] - z);
    };


    LInt maxc[4]{-1, -1, -1, -1};
    LInt minc[4]{-1, -1, -1, -1};

    for (auto c = 0; c < 4; ++c)
    {
        maxc[c] = v[c];
        minc[c] = v[c];
    }
   

    auto cnt{0};
    for (auto i = 0; i < n; ++i)
    {
        for (auto c = 0; c < 4; ++c)
        {
            maxc[c] = std::max(maxc[c], v[i * 4 + c]);
            minc[c] = std::min(minc[c], v[i * 4 + c]);
        }
    }
 
    for (auto c = 0; c < 4; ++c)
    {
        cout << c << ": " << minc[c] << " " << maxc[c] << endl;
    }

    LIntVec inrng(n, 0);
 
    for (auto i = 0; i < n; ++i)
    {
        for (auto j = 0; j < n; ++j)
        {
            auto d{get_dist(i, j)};
            if (d <= v[i * 4 + 3])
            {
                inrng[j]++; // other in range
            }
        }
    }
    /*
    for (auto i = 0; i < n; ++i)
    {
        cout << inrng[i] << " " << i << " xx" << endl;
    }
    */
    auto startit{std::max_element(begin(inrng), end(inrng))};
    auto starti{std::distance(begin(inrng), startit)};

    cout << starti << ":";
    for (auto i = 0; i < 4; ++i)
    {
        cout << "," << v[starti * 4 + i];
    }
    cout << endl;

    // cube0:32961998,33788773,52352148:880

    //auto sx{32961998LL}, sy{33788773LL}, sz{52352148LL}; 847
    //cube0:40311998,32608773,57122148:847
    
    //auto sx{33011998LL}, sy{33638773LL}, sz{52402148LL};
    //auto sx{33011998LL}, sy{33638773LL}, sz{52402148LL}; // good starting point derived by grid scanning
    auto sx{v[starti * 4 ]}, sy{v[starti * 4 + 1]}, sz{v[starti * 4 + 2]};
    

    Finder finder(v);
    auto ret{finder.search(sx, sy, sz)};
    //cnt = count(sx, sy, sz, v);
    
    return ret;
#if 0

    auto maxinrng{0LL};
    for (auto i = 0; i < n; ++i)
    {
        auto d{dist3(sx, sy, sz, v[i * 4], v[i * 4 + 1], v[i * 4 + 2])};
        cout << "_ " << v[i * 4] << "," <<  v[i * 4 + 1] << "," << v[i * 4 + 2] << "," << v[i * 4 + 3] << ": d=" << d << " res=" << (d <= v[i * 4 + 3] ) << endl;
    }


    const auto N{100};
    const auto M{10000LL};
    const auto Q{0LL * M / 2};
    LInt mx{0}, my{0}, mz{0};


    for (LInt dx = -N; dx <= N; ++dx)
    {
        for (LInt dy = -N; dy <= N; ++dy)
        {
            for (LInt dz = -N; dz <= N; ++dz)
            {
                auto x{Q + dx * M + v[starti * 4]};            
                auto y{Q + dy * M + v[starti * 4 + 1]};            
                auto z{Q + dz * M + v[starti * 4 + 2]};

                auto cnt{0};
                for (auto i = 0; i < n; ++i)
                {
                    auto d{get_dist3(x, y, z, i)};
                    if (d <= v[i * 4 + 3])
                    {
                        ++cnt;
                    }
                }
                if (cnt > maxinrng)
                {
                    mx = x; my = y; mz = z;
                    maxinrng = cnt;
                }
                //cout << "=" << x << "," << y << "," << z << ":" << cnt << endl;
            }
        }
    }
    cout << "cube0:" << mx << "," << my << "," << mz << ":" << maxinrng << endl;
    



    //LIntVec signal(n, 0);

    

    LIntVec inrng(n, 0);
    //auto maxinrng{0};

    for (auto i = 0; i < n; ++i)
    {
        for (auto j = i + 1; i < n; ++i)
        {
                        if 
        }

    }
#endif
    

}

int main()
{
    auto ret{task2("input23.txt")};
    cout << "r=" << ret << endl;
    return 0;
}


