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

using CharSet = std::set<char>;
using CharToCharSet = std::map<char, std::set<char>>;

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;

auto extract = [](const std::string& where, const std::string& before)
{
    auto i { where.find(before) };
    if (i == string::npos)
    {
        return ' ';
    }
    return where[i + before.size()];
};

const char EMPTY{'.'};

struct Task
{
    char id{EMPTY};
    int rem{0};
};

int task2(const string& fn, int t0, int n)
{
    std::ifstream inf(fn);
    string s;
    std::vector<string> v;

    CharToCharSet pred;
    CharSet used;
    auto cnt_used{ 0 };

    auto get_time = [t0](char ch){ return ch - 'A' + t0; };


    while (std::getline(inf, s))
    {
        string pr { extract(s, "Step "), extract(s, "step ") };
        used.insert(pr[0]);
        pred[pr[0]];
        pred[pr[1]].insert(pr[0]);

        cout << pr << endl;
        v.emplace_back(pr);
    }

    auto seconds{0};
    std::vector<Task> tasks(n);

    while (pred.size() > 0 || cnt_used > 0)
    {
        string ended;

        std::cout << seconds << ": ";

        for (auto& task : tasks)
        {
            if (task.id == EMPTY)
            {
                auto it = std::min_element(begin(pred), end(pred),
                    [](const auto& a, const auto& b){
                       if (a.second.size() == b.second.size())
                       {
                           return a.first < b.first;
                       }
                       return (a.second.size() < b.second.size());
                        
                        });
                if (it->second.size() > 0)
                {
                    continue;
                }
                auto ch { it->first };
                task.id = ch;
                task.rem = get_time(ch);
                ++cnt_used;
                pred.erase(ch);
            }
            else
            {
                --task.rem;
                if (task.rem == 0)
                {
                    ended += task.id;
                    task.id = EMPTY;
                    --cnt_used;
                }
            }
            cout << task.id << task.rem << " ";
        }

        cout << "\n";

        if (ended.length() > 0)
        {
            for (auto ch : ended)
            {
                for (auto& p : pred)
                {
                    p.second.erase(ch);
                }
            }
        }

        ++seconds;

    }

    cout << endl;

    return seconds;
}

int main()
{
    std::cout << task2("input7.txt", 60, 5) << std::endl;
    return 0;
}
