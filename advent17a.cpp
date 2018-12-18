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

auto printnbr = [](const auto& mp)
{
    for (auto i = 0; i < mp.size(); ++i)
    {
        cout << std::setw(5) << i << ":" << mp[i] << endl;
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
        ++ww; // fix for right boundary
        bool hor_res{ww > w};
        if (hh > h)
        {
            h = hh;
            mp.resize(hh);
            hor_res = true;
        }
        if (hor_res)
        {
            w = std::max(ww, int(w));
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

int simulate(StrVec& mp, int x0, int y0)
{
    mp[y0][x0] = '+';

    auto w{mp[0].length()};
    auto h{mp.size()};

    int ymax = h - 1;

    auto M = [&mp](auto x, auto y) -> char& { return mp[y][x]; };
    
    int changed(0);
    do {
        changed = 0;

        for (auto y = ymax; y > 0 ; --y)
        {
            auto x{0};
            while (x < w)
            {
                //if (x == x0) cout << x << "," << y << " ";
                if ((mp[y][x] == ' ' || mp[y][x] == '.') &&
                    (mp[y - 1][x] == '|' || mp[y - 1][x] == '+' ))
                {
                    mp[y][x] = '|';
                    changed++;
                }
                    
                if (M(x, y) == '|')
                {
                    // cout << x << "," << y << endl;
                    if (y < h - 1 && (mp[y + 1][x] == '#' || mp[y + 1][x] == '~') )
                    {
                        
                        auto findBound = [&M,w,&changed](auto x, auto y, auto d)
                        {
                            auto b{-1};
                            while (x >= 0 && x < w)
                            {

                                if (M(x, y) == ' ' || M(x, y) == '|' )
                                {
                                    if (M(x, y + 1) == '#' || M(x, y + 1) == '~' )
                                    {
                                        changed += M(x, y) != '|';
                                        M(x, y) = '|'; 
                                        x += d;
                                    }
                                    else
                                    {
                                        changed += M(x, y) != '|';
                                        M(x, y) = '|'; 
                                        break;
                                    }
                                }
                                else
                                {
                                    b = x;
                                    break;
                                }
                            }

                            return b;
                        };

                        auto lb{findBound(x - 1, y, -1)};
                        auto rb{findBound(x + 1, y,  1)};

                        //cout << lb << " " << rb << endl;
                        if (lb >= 0 && rb >= 0)
                        {
                            for (auto xx = lb + 1; xx < rb; ++xx)
                            {
                                changed += M(x, y) != '~';
                                M(xx, y) = '~';
                            }
                        }

                    }


                }



                ++x;
            }
        }
        
        //for (auto i = 0; i < mp.size(); ++i)
        //{
        //    cout << std::setw(5) << i << ":" << mp[i] << endl;
        //}



    } while (changed > 0);

    return 0;
}

auto count = [](const auto& mp, const auto& what) 
{
    auto cnt{0};
    auto clay{0};
    for (const auto& s : mp)
    {
        clay += std::count(begin(s), end(s), '#');
        if (clay > 0)
        {
            for (auto ch : what)
            { 
                cnt += std::count(begin(s), end(s), ch);
            }
        }
    }
    return cnt;
};


int main()
{
    auto v(parseUInts("123, 456..789"));
    cout << v[0] << v[1] << v[2] << endl;

    string s{"abc"};
    s.resize(60, '-');
    cout << s << endl;

#ifdef EXAMPLE
    auto mp{readMap("example17.txt")};
#else
    auto mp{readMap("input17.txt")};
#endif
    
    cout << mp[0].length() << "," << mp.size() << endl;
    printnbr(mp);

#ifdef EXAMPLE
    simulate(mp, 50, 0);
#else
    simulate(mp, 500, 0);
#endif

    printnbr(mp);

    cout << "part1=" << count(mp, "~|") << endl;
    cout << "part2=" << count(mp, "~") << endl;

    return 0;
}


