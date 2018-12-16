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

string task1()
{
    std::ifstream inf("input7.txt");
    string s;
    std::vector<string> v;

    CharToCharSet pred;
    CharSet used;

    while (std::getline(inf, s))
    {
        string pr { extract(s, "Step "), extract(s, "step ") };
        used.insert(pr[0]);
        pred[pr[0]];
        pred[pr[1]].insert(pr[0]);

        cout << pr << endl;
        v.emplace_back(pr);
    }

    string ret;

    while (pred.size() > 0)
    {
        auto it = std::min_element(begin(pred), end(pred),
            [](const auto& a, const auto& b){
               if (a.second.size() == b.second.size())
               {
                   return a.first < b.first;
               }
               return (a.second.size() < b.second.size());
                
                });
        auto ch { it->first };
        ret += ch;
        cout << ch << it->second.size();
        pred.erase(ch);
        for (auto& p : pred)
        {
            p.second.erase(ch);
        }
    }

    cout << endl;

    return ret;
}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
