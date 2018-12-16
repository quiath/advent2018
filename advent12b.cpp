#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::get;
using std::vector;
using IntVec = std::vector<int>;

string STATE0{"#...##.#...#..#.#####.##.#..###.#.#.###....#...#...####.#....##..##..#..#..#..#.#..##.####.#.#.###"};

string RULES {R"(..... => .
..#.. => #
..##. => #
#..## => .
..#.# => #
####. => .
##.## => .
#.... => .
###.. => #
##### => #
##..# => #
#.### => #
#..#. => #
.#### => #
#.#.. => #
.###. => #
.##.. => #
.#... => #
.#.## => #
##... => #
..### => .
##.#. => .
...## => .
....# => .
###.# => .
#.##. => #
.##.# => .
.#..# => #
#.#.# => #
.#.#. => #
...#. => #
#...# => #)" };


auto value(const std::string& s)
{
    int v{0};
    for (auto ch : s)
    {
        v = v * 2 + (ch == '#');
    }
    return v;
};



auto task1(string state, string rules0, long long steps)
{

    std::istringstream iss(rules0);
    string s, dummy, res;

    vector<int> rules(32, 0);

    while (iss >> s)
    {
        iss >> dummy;
        iss >> res;
        if (res[0] == '#')
        {
            rules[value(s)] = 1;
        }
           //cout << s << endl;
    }

    auto prev{STATE0 + "...."};
    auto prev_computed{0}, computed{0};
    //auto prev{STATE0};
    //prev += "....";
    auto pos{0};
    char token[2] = { '.', '#' };

    auto compute = [](auto s, auto pos)
    {
        long long result{0};
        for (auto ch : s)
        {
            result += (ch == '#') * pos;
            ++pos;
        }
        return result;
    };

    cout << endl;
    cout << prev << ":" << pos << ";" << prev.length() << endl;
    auto j = 1;
    for (; j <= steps; ++j)
    {
        auto val{0u};
        string s;
        int plen {static_cast<int>(prev.length())};
        //s.reserve(prev.length() + 4);
        s.reserve(plen + 4);
       
        for (auto i = -4; i < plen - 4; ++i)
        {
            val = (val * 2 + (prev[i + 4] == '#')) & 31; 
            s += token[rules[val]];
        }

        auto k{s.find('#')};
        s = s.substr(k); 

        auto lst{s.rfind('#')};
        s = s.substr(0, lst + 1);

        s += "....";
        pos += k - 2;
        //pos -= 2;
        computed = compute(s, pos);
        cout << s << " , " << j << ", " << pos << " v=" << computed << endl;

        if (prev == s)
        {
            break;
        }

        prev = s;
        prev_computed = computed;
    }


    return (steps - j) * (computed - prev_computed) + computed; 
}

int main()
{
    auto t = task1(STATE0, RULES, 50000000000LL);
    cout << t << endl;
    return 0;
}
