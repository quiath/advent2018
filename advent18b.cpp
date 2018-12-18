#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::begin;
using std::end;

string E = R"(.#.#...|#.
.....#|##|
.|..|...#.
..|#.....#
#.#|||#|#|
...#.||...
.|....|...
||...#|.#|
|.||||..|.
...#.|..|.)";

string T = R"(#.##.....||||#|.....|..#.|.||..||.#|....##..#|..|.
|#....||..|..#.#..|.....##.|..|..#.....#|..|..|#..
.#.....#.........#..|.....|.#.#|#||....|...||#..##
....#|...#...#||..#.#..|||#|.||.......#|..#|#..#.#
....|..|......|....|..#.................#..||||.#|
.....||..|##...#.||.....||......##....|...|.|.##..
#.|||#.|##.|...#|.|..|###....||.#.#....#.|.##...||
.||.|.|..#|....|.#.|.....##..##..#.#.||..#.#.|.|..
#..|#...|.#.##..##||.|#...||....#...#..|...##.....
..........|||.........||#.....#...#.#.....|.....||
...#|#.##.#.|#.....||..|#..|.#...|.......##....|#.
..||.|..##..#.#.##|....|..#|..##|..|...#.#....#.|.
..#...|.##||.|.#|....#|#...#..|......#..|...#..#..
.#|.#.....|.....||..|...||...|.#...#...#....|....|
|........##....|.#.|.#.||||#||.#..#.|..||||..|..#.
.##.|.|#||#||....|.#..#.#|.|...|.|...||....#..|.#.
|..||#.||.#.||.#...||#.||...|#|||..###|...##|...|.
...#.|##.....|.#..|....|.##.#..||..|....#.#.|#|...
.|#..||.|..#.|.|......#...#.|#.|...|.||.#....#||..
.##......##....|..#|...#.#.#.#......#||...#|..##..
#..#..#.#.|#.#|#..|#|#|..##..|.|.|.#.#.|...|#...#.
..#.##|.|........|##.#...|###..|..|.....|...|.#|#.
#.#....|.|#..#.||..##......#.......|#|...|.#.#||||
...|#.............|.......#.#.|#..#..||.#||.#.|#.#
#.|..|.#..|..#|.....|.#.....#..|#.#......|#.|.....
......#|........#||..||...|#.###|.....#.|..#|.|...
..#....#...#...#.|.||###...#.|..|||.....|...|.|...
..#.|..|.#.##|......#..#|.#.|.####.||.|..|.|.##||.
........##....#....###....#|...|..|...#||#|#.##.##
....#|||##.#.##.|.##..|#.|..#.|....#..#||##|.#...#
#..........|....|.....||.....#..|#...##.||.....||.
..#........#....#....#.|#.|.|.....##.|#...#.||||#|
..#.....|###.#...#.|.||...|...|||......#.|...|....
|..|#|..#....#..|....#..||.||.||.......#..#..|..#.
.##.#..#....|#..#......#...#.|..|....|......##.#..
.....||#.........|.....|.|||..|.|....|#..|....|..#
..|..|||#..#|.#.|#...#|.#..###..#|#|.|##...##||..#
.....#...###.....#..#|#...##|.#|#|##||.#...#.|....
##.#......|#......#..|#.|#.#...#.#.|....###|.||...
.#....#.....##.#.....|..#|........#.##|...#.#.#...
|##|#|....|..#.#||#.....#|#.....#.|.|#|.|.|.#|...#
||#|#..||........#.#.......##..#...||.||...#|.....
|.|.##|.....#.....|....###....|..|##.##|.#..|##.|#
#|.|.||.......|..#.#.#|...|....|##.#.##.|.#.||#...
.||..|#..#|.#|.|##..|#|#....#..#.#...#.#.......#.|
#|...|..#|.|#|.......|#.##|.##.............#.#..|.
...|...|.#.....|#..#.|...#|.#..#.....##|.....#.|..
##|.#.....#...##|.##|....#.##|#.||...#..|.#.#.||#.
.|###|.........|...#.#..|||.....##|...|#.||||..#..
..#.|..|##.||.#||..#..|.|....|||.#....|#.....|....)";

int iter(std::vector<string>& v)
{
    int w{static_cast<int>(v[0].length())};
    int h{static_cast<int>(v.size())};

    //cout << w << "," << h << endl;

    std::vector<string> d{v};
    std::vector<int> c(256, 0);
    std::vector<int> ec(256, 0);

    for (auto y = 0; y < h; ++y)
    {
        for (auto x = 0; x < w; ++x)
        {
            for (auto yy = std::max(0, y - 1); yy < std::min(h, y + 2); ++yy )
            {
                for (auto xx = std::max(0, x - 1); xx < std::min(w, x + 2); ++xx )
                {
                    //cout << x << "," << y << "," << xx << "," << yy << endl;
                    ++c[v[yy][xx]];
                }
            }
            --c[v[y][x]];
            switch (v[y][x])
            {
                case '.':
                    if (c['|'] >= 3)
                    {
                        d[y][x] = '|';
                    }
                    break;
                case '|':
                    if (c['#'] >= 3)
                    {
                        d[y][x] = '#';
                    }
                    break;
                case '#':
                    if (c['#'] >= 1 && c['|'] >= 1)
                    {
                      // pass

                    }
                    else
                    {
                        d[y][x] = '.';
                    }
            }
            ++ec[d[y][x]];
            
            c['#'] = c['|'] = c['.'] = 0;
            //std::fill(begin(c), end(c), 0);
        }
    }
    v = std::move(d);
    return ec['|'] * ec['#'] ;
}

int main()
{
    // std::istringstream iss(E);
    std::istringstream iss(T);
    string s;

    std::vector<string> v;

    while (iss >> s)
    {
        v.emplace_back(s);
        cout << ">>" << s << endl;
    }

    std::map<int, int> last;

    for (long long i = 1; i <= 1000000000LL; ++i)
    {
        auto ret{iter(v)};
        if (false) //(i % 100000 < 20)
        {
            cout << i << ": " << ret << endl;
        }
        auto prev{last[ret]};
        if (prev > 0)
        {
            cout << (i - prev) << " for " << ret << " i=" << i << endl;

            auto n = 1000000000LL % (i - prev);
            if (i % (i - prev) == n)
            {
                cout << "Solution " << ret << endl;
            }
        }
        last[ret] = i;
    }

    std::vector<int> c(256, 0);
    for (const auto& s : v)
    {
        cout << s << endl;
        for (auto ch : string("#|."))
        {
            c[ch] += std::count(begin(s), end(s), ch);
        }
    }

    //cout << c['|'] << "*" << c['#'] << "=" << c['|'] * c['#'] << endl;


    //cout << E << endl;
    return 0;
}
